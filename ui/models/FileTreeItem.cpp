#include "FileTreeItem.h"

FileTreeItem::FileTreeItem(const QVector<QVariant> &data, FileTreeItem *parent) : TreeItem(data, parent) {
    file = nullptr;
}

FileTreeItem::~FileTreeItem() {
    delete file;
    qDeleteAll(childItems);
}
