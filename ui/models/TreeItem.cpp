#include <QtCore/QCollator>
#include "TreeItem.h"

TreeItem::TreeItem(const QVector<QVariant> &data, TreeItem *parent) {
    parentItem = parent;
    itemData = data;
}

TreeItem::~TreeItem() {
    qDeleteAll(childItems);
}

int TreeItem::childNumber() const {
    if (parentItem) {
        return parentItem->childItems.indexOf(const_cast<TreeItem *>(this));
    }
    return 0;
}

bool TreeItem::insertChildren(int position, int count, int columns) {
    if (position < 0 || position > childItems.size()) {
        return false;
    }

    for (int row = 0; row < count; ++row) {
        QVector<QVariant> data(columns);
        auto *item = new TreeItem(data, this);
        childItems.insert(position, item);
    }

    return true;
}

bool TreeItem::insertColumns(int position, int columns) {
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

bool TreeItem::removeChildren(int position, int count) {
    if (position < 0 || position + count > childItems.size()) {
        return false;
    }

    for (int row = 0; row < count; ++row) {
        delete childItems.takeAt(position);
    }

    return true;
}

bool TreeItem::removeColumns(int position, int columns) {
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

bool TreeItem::setData(int column, const QVariant &value) {
    if (column < 0 || column >= itemData.size()) {
        return false;
    }

    itemData[column] = value;
    return true;
}

TreeItem *TreeItem::findName(const QString &name) {
    QCollator collator;
    auto iter = std::lower_bound(
            childItems.begin(),
            childItems.end(),
            TreeItem(QVector<QVariant>() << name),
            [&collator](TreeItem *f, const TreeItem &n) {
                return collator.compare(f->data(0).toString(), n.data(0).toString()) < 0;
            });
    return iter == childItems.end() ? nullptr : *iter;
}

bool TreeItem::appendChild(const QVector<QVariant> &data) {
    if (insertChildren(childCount(), 1, data.size())) {
        for (int i = 0; i < data.size(); ++i) {
            if (!child(childCount() - 1)->setData(i, data.at(i))) {
                return false;
            }
        }
        return true;
    }
    return false;
}
