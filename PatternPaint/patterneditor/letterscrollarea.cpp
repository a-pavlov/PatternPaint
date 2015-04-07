#include "letterscrollarea.h"
#include "abstractinstrument.h"

LetterScrollArea::LetterScrollArea(QWidget *parent) : QScrollArea(parent) {
    setupUi(this);
    patternEditor->setToolSize(1);
    patternEditor->setToolColor(QColor(255,255,255));
    patternEditor->init(DEFAULT_PATTERN_LENGTH, DEFAULT_PATTERN_HEIGHT);
}

LetterScrollArea::~LetterScrollArea()
{
}

void LetterScrollArea::setInstrument(AbstractInstrument* instr) {
    patternEditor->setInstrument(instr);
}

AbstractInstrument* LetterScrollArea::instrument() {
    return patternEditor->instrument();
}

void LetterScrollArea::resizeEvent(QResizeEvent *event)
{
    Q_ASSERT(widget() != NULL);
    float widgetAspect = float(widget()->baseSize().width())
            /widget()->baseSize().height();
    float widgetWidth = height()*widgetAspect;
    widget()->setMinimumWidth(widgetWidth);
    widget()->setMaximumWidth(widgetWidth);
    QScrollArea::resizeEvent(event);
}
