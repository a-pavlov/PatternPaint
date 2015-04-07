#include "letterscrollarea.h"
#include "ui_letterscrollarea.h"

LetterScrollArea::LetterScrollArea(QWidget *parent) : QScrollArea(parent) {
    setupUi(this);
}

LetterScrollArea::~LetterScrollArea()
{
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
