#ifndef LETTERSCROLLAREA_H
#define LETTERSCROLLAREA_H

#include <QScrollArea>
#include "ui_letterscrollarea.h"

class AbstractInstrument;

class LetterScrollArea : public QScrollArea, public Ui::LetterScrollArea
{
    Q_OBJECT

public:
    explicit LetterScrollArea(QWidget *parent = 0);
    ~LetterScrollArea();
    void setInstrument(AbstractInstrument*);
    AbstractInstrument* instrument();
protected:
    void resizeEvent(QResizeEvent *event);
};

#endif // LETTERSCROLLAREA_H
