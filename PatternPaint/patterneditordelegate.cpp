#include "patterneditordelegate.h"
#include <QDebug>
#include <QPainter>

PatternEditorDelegate::PatternEditorDelegate(QObject* parent) : QItemDelegate(parent) {
}

PatternEditorDelegate::~PatternEditorDelegate() {
}

void PatternEditorDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex & index ) const {
    QImage img = qvariant_cast<QImage>(index.data(Qt::UserRole + 1));
    QImage scaled = img.scaledToWidth(200);
    QItemDelegate::paint(painter, option, index);
    painter->fillRect(QRect(option.rect.x(), option.rect.y(), scaled.width(), scaled.height()), QColor(0,0,0));
    painter->drawImage(option.rect.x(), option.rect.y(), scaled);
    if (index.data(Qt::UserRole + 2).toBool()) {
        painter->drawImage(QRect(option.rect.x() + 2, option.rect.y() + 2, 16, 16), QImage(":/resources/images/modified.png"));
    }
}

QSize PatternEditorDelegate::sizeHint(const QStyleOptionViewItem & option, const QModelIndex & index ) const {
    Q_UNUSED(option);
    QImage img = qvariant_cast<QImage>(index.data(Qt::UserRole + 1));
    QImage scaled = img.scaledToWidth(200);
    return QSize(scaled.size().width(), scaled.size().height() + 5);
}

