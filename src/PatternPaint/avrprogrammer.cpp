#include "avrprogrammer.h"
#include <QDebug>

#define COMMAND_TIMEOUT_TIME 1000
#define PAGE_SIZE_BYTES 128

AvrProgrammer::AvrProgrammer(QObject *parent) :
    QObject(parent)
{
    commandTimeoutTimer = new QTimer(this);
    commandTimeoutTimer->setSingleShot(true);

    serial = new QSerialPort(this);
    connect(serial, SIGNAL(error(QSerialPort::SerialPortError)),
            this, SLOT(handleSerialError(QSerialPort::SerialPortError)));
    connect(serial,SIGNAL(readyRead()),this,SLOT(handleReadData()));
}

bool AvrProgrammer::openSerial(QSerialPortInfo info) {
    if(isConnected()) {
        qCritical("Already connected to a programmer");
        return false;
    }

    qDebug() << "connecting to " << info.portName();
    serial->setPortName(info.portName());
    serial->setBaudRate(QSerialPort::Baud19200);
    serial->open(QIODevice::ReadWrite);

    if(!isConnected()) {
        return false;
    }

    // TODO: Create a new serial port object, instead of clearing the current one?
    serial->clear(QSerialPort::AllDirections);
    serial->clearError();

    return true;
}

void AvrProgrammer::closeSerial() {
    serial->close();
}

bool AvrProgrammer::isConnected() {
    return serial->isOpen();
}

void AvrProgrammer::queueCommand(QString name,
                                 QByteArray data,
                                 QByteArray expectedRespone) {

    commandQueue.push_back(Command(name, data, expectedRespone));

    // If this is the only command, start processing commands
    // TODO: Merge this with the implementation in handleReadData()
    if(commandQueue.length() == 1) {
        processCommandQueue();
    }
}

void AvrProgrammer::processCommandQueue() {
    // Note: don't call this if there is a command already running; perhaps add some state?
    responseData.clear();

//    qDebug() << "Command started:" << commandQueue.front().name;
    if(!isConnected()) {
        qCritical() << "Device disappeared, cannot run command";
        return;
    }
    if(serial->write(commandQueue.front().data) != commandQueue.front().data.length()) {
        qCritical() << "Error writing to device";
        return;
    }

    // Start the timer; the command must complete before it fires, or it
    // is considered an error. This is to prevent a misbehaving device from hanging
    // the programmer code.
    commandTimeoutTimer->start(COMMAND_TIMEOUT_TIME);
}

void AvrProgrammer::handleReadData() {
    if(commandQueue.length() == 0) {
        // TODO: error, we got unexpected data.
        qCritical() << "Got data when we didn't expect it!";
        return;
    }

    responseData.append(serial->readAll());

    if(responseData.length() > commandQueue.front().expectedResponse.length()) {
        // TODO: error, we got unexpected data.
        qCritical() << "Got more data than we expected";
        return;
    }

    if(responseData.length() < commandQueue.front().expectedResponse.length()) {
        qDebug() << "Didn't get enough data yet, so just waiting";
        return;
    }

    // If the command was to read from flash, short-circuit the response data check.
    if(commandQueue.front().name == "readFlash") {
        // TODO: Test me?
        if(responseData.at(responseData.length()-1) != '\r') {
            qCritical() << "readFlash response didn't end with a \\r";
            return;
        }
    }
    else if(responseData != commandQueue.front().expectedResponse) {
        qCritical() << "Got unexpected data back";
        return;
    }

    // At this point, we've gotten all of the data that we expected.
    commandTimeoutTimer->stop();

    // If the command was reset, disconnect from the programmer
    if(commandQueue.front().name == "reset") {
        qDebug() << "Disconnecting from programmer";
        closeSerial();
    }

//    qDebug() << "Command completed successfully: " << commandQueue.front().name;
    emit(commandFinished(commandQueue.front().name,responseData));
    commandQueue.pop_front();

    // Start another command, if there is one.
    if(commandQueue.length() > 0) {
        processCommandQueue();
    }
}

void AvrProgrammer::handleSerialError(QSerialPort::SerialPortError serialError)
{
    if(serialError == QSerialPort::NoError) {
        // The serial library appears to emit an extraneous SerialPortError
        // when open() is called. Just ignore it.
        return;
    }

    qCritical() << serial->errorString();

    // TODO: handle other error types?
    if (serialError == QSerialPort::ResourceError) {
        emit(error(serial->errorString()));
        closeSerial();
        commandQueue.clear();
    }
}

void AvrProgrammer::handleCommandTimeout()
{
    qCritical() << "Command timed out, disconnecting from programmer";
    emit(error("Command timed out, disconnecting from programmer"));
    closeSerial();
    commandQueue.clear();
}

// Send the command to probe for the device signature, and register the expected response
void AvrProgrammer::checkDeviceSignature() {
    queueCommand("checkDeviceSignature",QByteArray("s"), QByteArray("\x87\x95\x1E"));
}

void AvrProgrammer::reset()
{
    queueCommand("reset",QByteArray("E"), QByteArray("\r"));
}

void AvrProgrammer::setAddress(int address) {
    // TODO: Test if address > supported
    if(address & 0x01) {
        // TODO: report error
        qCritical() << "Address out of bounds";
    }

    // Note that the address her is word aligned.
    QByteArray command;
    command.append('A');
    command.append((address >> 9) & 0xFF);
    command.append((address >> 1) & 0xFF);

    queueCommand("setAddress", command, QByteArray("\r"));
}

void AvrProgrammer::readFlash(int startAddress, int lengthBytes) {
    // Set the address to read from
    // TODO: avoid the bootloader?
    setAddress(startAddress);

    // Read the flash contents
    // Baller that we can just read the whole thing at once!
    QByteArray command;
    command.append('g'); // command: read memory
    command.append((lengthBytes >> 8) & 0xFF); // read size (high)
    command.append((lengthBytes)      & 0xFF); // read size (low)
    command.append('F'); // memory type: flash

    // Note: only the length matters for the response, the response data will be overwritten in handleReadData)_
    queueCommand("readFlash",command,QByteArray(lengthBytes + 1, '\r'));
}

void AvrProgrammer::writeFlash(QByteArray& data, int startAddress) {
    if(startAddress%PAGE_SIZE_BYTES) {
        qCritical() << "Bad start address, must align with page boundary";
        return;
    }

    if(data.length() == 0) {
        qCritical() << "No data to write";
        return;
    }

    // Pad the data length to an even number, since we can only write word-sized chunks
    if(data.length() % 2 == 1) {
        qDebug() << "Data length falls on a byte boundary, padding to a word boundary";
        data.append(0xff);
    }

    // TODO: Check that we fit into available memory.

    setAddress(startAddress);

    // Write the data in page-sized chunks
    // TODO: Do we actually need to do chunks here?
    for(int currentChunkPosition = 0;
        currentChunkPosition < data.length();
        currentChunkPosition += PAGE_SIZE_BYTES) {

        int currentChunkSize = std::min(PAGE_SIZE_BYTES, data.length() - currentChunkPosition);

        QByteArray command;
        command.append('B'); // command: write memory
        command.append((currentChunkSize >> 8) & 0xFF); // read size (high)
        command.append((currentChunkSize)      & 0xFF); // read size (low)
        command.append('F'); // memory type: flash
        command.append(data.mid(currentChunkPosition,currentChunkSize));

        queueCommand("writeFlash", command, QByteArray("\r"));
    }
}
