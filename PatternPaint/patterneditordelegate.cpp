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
    QItemDelegate::paint(painter, option, index);
    painter->fillRect(QRect(option.rect.x(), option.rect.y(), img.width(), img.height()), QColor(0,0,0));
    painter->drawImage(option.rect.x(), option.rect.y(), img);
}

QSize PatternEditorDelegate::sizeHint(const QStyleOptionViewItem & option, const QModelIndex & index ) const {
    Q_UNUSED(option);
    QImage img = qvariant_cast<QImage>(index.data(Qt::UserRole));
    return QSize(img.size().width(), img.size().height() + 5);
}

