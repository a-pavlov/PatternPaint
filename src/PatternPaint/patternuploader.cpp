#include "patternuploader.h"
#include "uploaddata.h"

#include <QDebug>

/// Interval between polling the serial port list for new devices
#define WAIT_FOR_BOOTLOADER_POLL_INTERVAL 200

/// Maximum time to wait before giving up on finding a bootloader
#define WAIT_FOR_BOOTLOADER_TIMEOUT 10000


/// How long to wait between receiving notification that the programmer has been
/// reset, and notifying the caller that we are finished
#define PROGRAMMER_RESET_DELAY 1000

/// Length of character buffer for debug messages
#define BUFF_LENGTH 100

PatternUploader::PatternUploader(QObject *parent) :
    QObject(parent)
{
    processTimer = new QTimer(this);

    state = State_Ready;

    connect(&programmer,SIGNAL(error(QString)),
            this,SLOT(handleProgrammerError(QString)));
    connect(&programmer,SIGNAL(commandFinished(QString,QByteArray)),
            this,SLOT(handleProgrammerCommandFinished(QString,QByteArray)));
}

void PatternUploader::handleProgrammerError(QString error) {
    qCritical() << error;

    // TODO: not sure if we should do this, or let the programmer handle it?
    if(programmer.isConnected()) {
        programmer.closeSerial();
    }

    emit(finished(false));
}

void PatternUploader::handleProgrammerCommandFinished(QString command, QByteArray __attribute__((unused))returnData) {
    // TODO: Update our progress somehow? But how to tell how far we've gotten?
//    qDebug() << "Command finished:" << command;
    updateProgress(progress + 1);

    // we know reset is the last command, so the BlinkyTape should be ready soon.
    // Schedule a timer to emit the message shortly.
    // TODO: Let the receiver handle this instead.
    if(command == "reset") {
        QTimer::singleShot(PROGRAMMER_RESET_DELAY, this,SLOT(handleResetTimer()));
    }
}

void PatternUploader::handleResetTimer()
{
    emit(finished(true));
    programmer.closeSerial();  // TODO: Is this the correct place?
}


void PatternUploader::updateProgress(int newProgress) {
    progress = newProgress;
    emit(progressChanged(progress));
}




bool PatternUploader::startUpload(BlinkyTape& tape, std::vector<Pattern> patterns) {
    updateProgress(0);

    // We can't reset if we weren't already connected...
    if(!tape.isConnected()) {
        errorString = "Not connected to a tape, cannot upload again";
        return false;
    }

    /// Create the compressed image and check if it will fit into the device memory
    uploadData data;
    if(!data.init(patterns)) {
        errorString = data.errorString;
        return false;
    }

    // TODO: Only write the sketch portion if we absolutely have to, to lower the risk
    // that we trash the firmware if there is a problem later

    // The entire sketch must fit into the available memory, minus a single page
    // at the end of flash for the configuration header
    if(data.sketch.length() + data.patternData.length() + data.patternTable.length() > FLASH_MEMORY_AVAILABLE) {
        qCritical() << "sketch can't fit into memory!";

        errorString = QString("Sorry! The Pattern is a bit too big to fit in BlinkyTape memory! Avaiable space=%1, Pattern size=%2")
                              .arg(FLASH_MEMORY_AVAILABLE)
                              .arg(data.sketch.length() + data.patternData.length() + data.patternTable.length());
        return false;
    }

    // Put the sketch, pattern, and metadata into the programming queue.
    qDebug() << "Sketch address: " << data.sketchAddress << ", size: " << data.sketch.length();
    qDebug() << "pattern data address: " << data.patternDataAddress << ", size: " << data.patternData.length();
    qDebug() << "pattern table address: " << data.patternTableAddress << ", size: " << data.patternTable.length();

    flashData.push_back(FlashSection(data.sketchAddress,        data.sketch));
    flashData.push_back(FlashSection(data.patternDataAddress,  data.patternData));
    flashData.push_back(FlashSection(data.patternTableAddress, data.patternTable));

    // Next, tell the tape to reset.
    tape.reset();

    // Now, start the polling processes to detect a new bootloader
    stateStartTime = QDateTime::currentDateTime();
    state = State_WaitForBootloaderPort;
    waitOneMore = true;
    processTimer->singleShot(WAIT_FOR_BOOTLOADER_POLL_INTERVAL,this,SLOT(doWork()));
    return true;
}


bool PatternUploader::startUpload(BlinkyTape& tape, QByteArray sketch) {
    // TODO: Reconcile this with the other startUpload function.
    updateProgress(0);

    // We can't reset if we weren't already connected...
    if(!tape.isConnected()) {
        errorString = "Not connected to a tape, cannot upload again";
        return false;
    }

    char buff[BUFF_LENGTH];
    snprintf(buff, BUFF_LENGTH, "Sketch size: %iB",
             sketch.length()),
    qDebug() << buff;

    // The entire sketch must fit into the available memory, minus a single page
    // at the end of flash for the configuration header
    // TODO: Could save ~100 bytes if we let the sketch spill into the unused portion
    // of the header.
    if(sketch.length() > FLASH_MEMORY_AVAILABLE) {
        qDebug() << "sketch can't fit into memory!";

        errorString = QString("Sorry! The Pattern is a bit too big to fit in BlinkyTape memory right now. We're working on improving this! Avaiable space=%1, Pattern size=%2")
                .arg(FLASH_MEMORY_AVAILABLE)
                .arg(sketch.length());
        return false;
    }

    // Put the sketch, pattern, and metadata into the programming queue.
    flashData.push_back(FlashSection(0, sketch));

/// Attempt to reset the strip using the 1200 baud rate method, and identify the newly connected bootloader
    // Next, tell the tape to reset.
    tape.reset();

    // Now, start the polling processes to detect a new bootloader
    stateStartTime = QDateTime::currentDateTime();
    state = State_WaitForBootloaderPort;
    waitOneMore = true;
    processTimer->singleShot(WAIT_FOR_BOOTLOADER_POLL_INTERVAL,this,SLOT(doWork()));
    return true;
}

QString PatternUploader::getErrorString() const
{
    return errorString;
}


void PatternUploader::doWork() {
    // TODO: This flow is really ungainly

//    qDebug() << "In doWork state=" << state;

    // Continue the current state
    switch(state) {
    case State_WaitForBootloaderPort:
        {
            // Scan to see if there is a bootloader present
            QList<QSerialPortInfo> postResetTapes;
            postResetTapes = BlinkyTape::findBlinkyTapeBootloaders();

            // If we didn't detect a bootloader and still have time, then queue the timer and
            // wait. Otherwise, we timed out, so fail.
            if(postResetTapes.length() == 0) {
                if(stateStartTime.msecsTo(QDateTime::currentDateTime())
                        > WAIT_FOR_BOOTLOADER_TIMEOUT) {
                    handleProgrammerError("Didn't detect a bootloader, something went wrong!");
                    return;
                }

                processTimer->singleShot(WAIT_FOR_BOOTLOADER_POLL_INTERVAL,this,SLOT(doWork()));
                return;
            }

            if(postResetTapes.length() > 1) {
                qWarning() << "Too many bootloaders, something is amiss.";
            }

            // Wait one extra timer cycle, to give the bootloader some time to come on board
            if(waitOneMore) {
                waitOneMore = false;
                processTimer->singleShot(WAIT_FOR_BOOTLOADER_POLL_INTERVAL,this,SLOT(doWork()));
                return;
            }

            qDebug() << "Bootloader waiting on: " << postResetTapes.at(0).portName();

            // Try to create a new programmer by connecting to the port
            if(!programmer.openSerial(postResetTapes.at(0))) {
                handleProgrammerError("could not connect to programmer!");
                return;
            }
            else {
                qDebug() << "Connected to programmer!";
            }

            // Send Check Device Signature command
            programmer.checkDeviceSignature();

            // Queue all of the flash sections to memory
            while(!flashData.empty()) {
                FlashSection f = flashData.front();
                programmer.writeFlash(f.data, f.address);
                flashData.pop_front();
            }

            programmer.reset();

            // TODO: Read back
            state = State_WaitForDeviceSignature;
        }
        break;
    case State_WaitForDeviceSignature:
        // TODO

        break;

    default:
        break;
    }

}
