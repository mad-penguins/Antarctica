#include <QtCore/QVector>
#include <QtCore/QCollator>

#include "PackageTreeItem.h"

PackageTreeItem::PackageTreeItem(const QVector<QVariant> &data, PackageTreeItem *parent) : TreeItem(data, parent) {
    parentItem = parent;
    package = nullptr;
}

PackageTreeItem::~PackageTreeItem() {
    qDeleteAll(childItems);
    delete package;
}
