#include "slideshowitem.h"

SlideShowItem::SlideShowItem(QListWidget* parent) :
    QListWidgetItem(parent, QListWidgetItem::UserType + 1), modified(false) {
        ustack.setUndoLimit(50);
}

SlideShowItem::SlideShowItem(const QString& text) :
    QListWidgetItem(text, 0, QListWidgetItem::UserType + 1), modified(false) {

}

QVariant SlideShowItem::data(int role) const {
    switch(role) {
        case PreviewImage: return img;
        case Modified: return modified;
        case PatternSize: return psize;
    };

    return QListWidgetItem::data(role);
}

void SlideShowItem::setData(int role, const QVariant& value) {
    switch(role)
    {
    case PreviewImage:
        img = qvariant_cast<QImage>(value);
        psize = img.size();
        break;
    case Modified:
        modified = qvariant_cast<bool>(value);
        break;
    case PatternSize:
        Q_ASSERT(false);    // never set size separated from image!
        break;
    default:
        break;
    }

    QListWidgetItem::setData(role, value);
}

void SlideShowItem::setImage(const QImage& image) {
    img = image;
    psize= img.size();
}
