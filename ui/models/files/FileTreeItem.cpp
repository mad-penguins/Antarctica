#include "FileTreeItem.h"

FileTreeItem::FileTreeItem(const QVector<QVariant> &data, FileTreeItem *parent) : TreeItem(data, parent) {
    file = nullptr;
}

FileTreeItem::~FileTreeItem() {
    delete file;
    qDeleteAll(childItems);
}

File *FileTreeItem::getFile() const {
    return file;
}

void FileTreeItem::setFile(File *f) {
    file = f;
}
