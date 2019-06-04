#include <QtCore/QVector>
#include <QtCore/QCollator>

#include "PackageTreeItem.h"

PackageTreeItem::PackageTreeItem(const QVector<QVariant> &data, PackageTreeItem *parent) : TreeItem(data, parent) {
    pkg = nullptr;
}

PackageTreeItem::~PackageTreeItem() {
    delete pkg;
    qDeleteAll(childItems);
}
