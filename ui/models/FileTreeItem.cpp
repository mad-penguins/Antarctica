#include <QtCore/QCollator>
#include "FileTreeItem.h"

#include <QDebug>

FileTreeItem::FileTreeItem(const QVector<QVariant> &data, FileTreeItem *parent) : TreeItem(data, parent) {
    parentItem = parent;
    file = nullptr;
}

FileTreeItem::~FileTreeItem() {
    qDeleteAll(childItems);
    delete file;
}