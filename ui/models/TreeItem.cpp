/*!
 * \file
 * \author Nikita Mironov <nickfrom22nd@gmail.com>
 * \brief The polymorphic tree item implementation
 *
 * \section LICENSE
 *
 * Copyright (c) 2019 Penguins of Madagascar

 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */


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
