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

#ifndef FILLINSTRUMENT_H
#define FILLINSTRUMENT_H

#include "abstractinstrument.h"

#include <QtCore/QObject>

/**
 * @brief Fill instrument class.
 *
 */
class FillInstrument : public CustomCursorInstrument
{
    Q_OBJECT
public:
    explicit FillInstrument(QObject *parent = 0);
    
    void mousePressEvent(QMouseEvent *event, PatternEditor&);
    void mouseMoveEvent(QMouseEvent *event, PatternEditor&);
    void mouseReleaseEvent(QMouseEvent *event, PatternEditor&);
    bool showPreview() const { return false; }
protected:
    void paint(PatternEditor&, bool additionalFlag = false);

private:
    void fillRecurs(int x, int y, QRgb switchColor, QRgb oldColor, QImage& tempImage);
    
};

#endif // FILLINSTRUMENT_H