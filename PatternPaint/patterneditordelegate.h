#ifndef PATTERNEDITORDELEGATE_H
#define PATTERNEDITORDELEGATE_H

#include <QItemDelegate>

class PatternEditorDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    PatternEditorDelegate(QObject* parent = 0);
    virtual ~PatternEditorDelegate();
    virtual void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex & index ) const;
    QSize sizeHint(const QStyleOptionViewItem & option, const QModelIndex & index ) const;
};

#endif // PATTERNEDITORDELEGATE_H
