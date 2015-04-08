#include "slideshowitem.h"

SlideShowItem::SlideShowItem(QListWidget* parent) : QListWidgetItem(parent, QListWidgetItem::UserType + 1) {
        ustack.setUndoLimit(50);
}

SlideShowItem::SlideShowItem(const QString& text) : QListWidgetItem(text, 0, QListWidgetItem::UserType + 1) {}

QVariant SlideShowItem::data(int role) const {
    if (role == PreviewImage) {
        return img;
    }

    return QListWidgetItem::data(role);
}

void SlideShowItem::setData(int role, const QVariant& value) {
    if (role == PreviewImage) {
        img = qvariant_cast<QImage>(value);
    }

    QListWidgetItem::setData(role, value);
}

void SlideShowItem::setImage(const QImage& image) {
    img = image;
}
