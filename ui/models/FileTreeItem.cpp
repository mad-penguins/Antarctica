#include <QtCore/QCollator>
#include "FileTreeItem.h"

#include <QDebug>

FileTreeItem::FileTreeItem(const QVector<QVariant> &data, FileTreeItem *parent) {
    parentItem = parent;
    itemData = data;
    file = nullptr;
}

FileTreeItem::~FileTreeItem() {
    qDeleteAll(childItems);
}

int FileTreeItem::childNumber() const {
    if (parentItem) {
        return parentItem->childItems.indexOf(const_cast<FileTreeItem *>(this));
    }
    return 0;
}

bool FileTreeItem::insertChildren(int position, int count, int columns) {
    if (position < 0 || position > childItems.size()) {
        return false;
    }

    for (int row = 0; row < count; ++row) {
        QVector<QVariant> data(columns);
        auto *item = new FileTreeItem(data, this);
        childItems.insert(position, item);
    }

    return true;
}

bool FileTreeItem::insertColumns(int position, int columns) {
    if (position < 0 || position > itemData.size()) {
        return false;
    }

    for (int column = 0; column < columns; ++column) {
        itemData.insert(position, QVariant());
    }

    for (auto &&child : childItems) {
        child->insertColumns(position, columns);
    }

    return true;
}

bool FileTreeItem::removeChildren(int position, int count) {
    if (position < 0 || position + count > childItems.size()) {
        return false;
    }

    for (int row = 0; row < count; ++row) {
        delete childItems.takeAt(position);
    }

    return true;
}

bool FileTreeItem::removeColumns(int position, int columns) {
    if (position < 0 || position + columns > itemData.size()) {
        return false;
    }

    for (int column = 0; column < columns; ++column) {
        itemData.remove(position);
    }

    for (auto &&child : childItems) {
        child->removeColumns(position, columns);
    }

    return true;
}

bool FileTreeItem::setData(int column, const QVariant &value) {
    if (column < 0 || column >= itemData.size()) {
        return false;
    }

    itemData[column] = value;
    return true;
}

FileTreeItem *FileTreeItem::findName(const QString &name) {
    QCollator collator;
    auto iter = std::lower_bound(
            childItems.begin(),
            childItems.end(),
            FileTreeItem(QVector<QVariant>() << name),
            [&collator] (FileTreeItem *f, const FileTreeItem &n) {
                return collator.compare(f->data(0).toString(), n.data(0).toString()) < 0;
            });
    return iter == childItems.end() ? nullptr : *iter;
}

bool FileTreeItem::appendChild(const QVector<QVariant> &data) {
    if (insertChildren(childCount(), 1, data.size())) {
        for (int i = 0; i < data.size(); ++i) {
            if (!child(childCount()-1)->setData(i, data.at(i))) {
                return false;
            }
        }
        return true;
    }
    return false;
}
