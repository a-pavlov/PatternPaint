#include "patterneditordelegate.h"
#include <QDebug>
#include <QPainter>
#include "patterneditor.h"

PatternEditorDelegate::PatternEditorDelegate(QObject* parent) : QItemDelegate(parent) {
}

PatternEditorDelegate::~PatternEditorDelegate() {
}

void PatternEditorDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex & index ) const {
    QImage img = qvariant_cast<QImage>(index.data(Qt::UserRole));
    //Q_ASSERT(pe != NULL);

    QItemDelegate::paint(painter, option, index);
    painter->fillRect(QRect(option.rect.x(), option.rect.y(), img.width(), img.height()), QColor(0,0,0));
    painter->drawImage(option.rect.x(), option.rect.y(), img);
}

QSize PatternEditorDelegate::sizeHint(const QStyleOptionViewItem & option, const QModelIndex & index ) const {
    Q_UNUSED(option);
    //PatternEditor* pe = qvariant_cast<PatternEditor*>(index.data(Qt::UserRole));
    QImage img = qvariant_cast<QImage>(index.data(Qt::UserRole));
    return img.size();
}

