#include "patterneditordelegate.h"
#include <QDebug>
#include <QPainter>

#include "slideshowitem.h"

PatternEditorDelegate::PatternEditorDelegate(QObject* parent) : QItemDelegate(parent) {
}

PatternEditorDelegate::~PatternEditorDelegate() {
}

void PatternEditorDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex & index ) const {
    painter->save();
    QImage img = qvariant_cast<QImage>(index.data(SlideShowItem::PreviewImage));
    QImage scaled = img.scaledToWidth(200);
    QItemDelegate::paint(painter, option, index);
    painter->fillRect(QRect(option.rect.x(), option.rect.y(), scaled.width(), scaled.height()), QColor(0,0,0));
    painter->drawImage(option.rect.x(), option.rect.y(), scaled);
    if (index.data(SlideShowItem::Modified).toBool()) {
        painter->drawImage(QRect(option.rect.x() + 2, option.rect.y() + 2, 16, 16), QImage(":/resources/images/modified.png"));
    }

    if (index.data(Qt::DisplayRole).toInt() != -1) {
        painter->setPen(QColor::fromRgb(255,255,0));
        painter->drawText(option.rect.x() + scaled.width() - 16, option.rect.y() + scaled.height() - 10,
                          QString::number(index.data(Qt::DisplayRole).toInt()));
    }

    painter->restore();
}

QSize PatternEditorDelegate::sizeHint(const QStyleOptionViewItem & option, const QModelIndex & index ) const {
    Q_UNUSED(option);
    QSize size = qvariant_cast<QSize>(index.data(SlideShowItem::PatternSize));
    float aspect = 200.0/size.width();
    return QSize(200, size.height()*aspect + 5);
}

