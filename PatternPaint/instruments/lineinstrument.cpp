/*
 * This source file is part of EasyPaint.
 *
 * Copyright (c) 2012 EasyPaint <https://github.com/Gr1N/EasyPaint>
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "lineinstrument.h"
#include "patterneditor.h"

#include <QPen>
#include <QPainter>
#include <QImage>

LineInstrument::LineInstrument(QObject *parent) :
    AbstractInstrument(parent)
{
}

void LineInstrument::mousePressEvent(QMouseEvent *event, PatternEditor& pe)
{
    if(event->button() == Qt::LeftButton || event->button() == Qt::RightButton)
    {
        mStartPoint = mEndPoint = event->pos();
        pe.setPaint(true);
        mImageCopy = pe.getPatternAsImage();
        makeUndoCommand(pe);
    }
}

void LineInstrument::mouseMoveEvent(QMouseEvent *event, PatternEditor& pe)
{
    if(pe.isPaint())
    {
        mEndPoint = event->pos();
        pe.setImage(mImageCopy);
        if(event->buttons() & Qt::LeftButton)
        {
            paint(pe, false);
        }
        else if(event->buttons() & Qt::RightButton)
        {
            paint(pe, true);
        }
    }
}

void LineInstrument::mouseReleaseEvent(QMouseEvent *event, PatternEditor& pe)
{
    if(pe.isPaint())
    {
        pe.setImage(mImageCopy);
        if(event->button() == Qt::LeftButton)
        {
            paint(pe, false);
        }
        else if(event->button() == Qt::RightButton)
        {
            paint(pe, true);
        }
        pe.setPaint(false);
    }
}

void LineInstrument::paint(PatternEditor& pe, bool isSecondaryColor, bool)
{
    QPainter painter(pe.getDevice());
    if(isSecondaryColor)
    {
#if 0
        painter.setPen(QPen(DataSingleton::Instance()->getSecondaryColor(),
                            DataSingleton::Instance()->getPenSize() * imageArea.getZoomFactor(),
                            Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
#endif
    }
    else
    {
        painter.setPen(QPen(pe.getPrimaryColor(),
                            pe.getPenSize() * pe.getZoomFactor(),
                            Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    }

    if(mStartPoint != mEndPoint)
    {
        painter.drawLine(mStartPoint, mEndPoint);
    }

    if(mStartPoint == mEndPoint)
    {
        painter.drawPoint(mStartPoint);
    }

    //    int rad(DataSingleton::Instance()->getPenSize() + round(sqrt((mStartPoint.x() - mEndPoint.x()) *
    //                                                                 (mStartPoint.x() - mEndPoint.x()) +
    //                                                                 (mStartPoint.y() - mEndPoint.y()) *
    //                                                                 (mStartPoint.y() - mEndPoint.y()))));
    //    mPImageArea->update(QRect(mStartPoint, mEndPoint).normalized().adjusted(-rad, -rad, +rad, +rad));
    painter.end();
    pe.update();
}