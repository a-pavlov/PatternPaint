#include "patterneditordelegate.h"
#include <QDebug>
#include <QPainter>

#include "slideshowitem.h"

PatternEditorDelegate::PatternEditorDelegate(QObject* parent) : QItemDelegate(parent) {
}

PatternEditorDelegate::~PatternEditorDelegate() {
}

void PatternEditorDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex & index ) const {
    QImage img = qvariant_cast<QImage>(index.data(SlideShowItem::PreviewImage));
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
    QSize size = qvariant_cast<QSize>(index.data(SlideShowItem::PatternSize));
    float aspect = 200.0/size.width();
    return QSize(200, size.height()*aspect + 5);
}

