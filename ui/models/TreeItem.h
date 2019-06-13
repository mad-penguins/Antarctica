/*!
 * \file
 * \author Nikita Mironov <nickfrom22nd@gmail.com>
 * \brief The polymorphic tree item class
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

#ifndef ANTARCTICA_TREEITEM_H
#define ANTARCTICA_TREEITEM_H


#include <QtCore/QVector>
#include <QtCore/QVariant>

class TreeItem {
public:
    explicit TreeItem(const QVector<QVariant> &data, TreeItem *parent = nullptr);

    virtual ~TreeItem();

    inline TreeItem *child(int number) {
        return childItems.value(number);
    }

    inline int childCount() const {
        return childItems.count();
    }

    inline int columnCount() const {
        return itemData.count();
    }

    inline QVariant data(int column) const {
        return itemData.value(column);
    }

    bool insertChildren(int position, int count, int columns);

    bool appendChild(const QVector<QVariant> &data);

    bool insertColumns(int position, int columns);

    inline TreeItem *parent() {
        return parentItem;
    }

    bool removeChildren(int position, int count);

    bool removeColumns(int position, int columns);

    int childNumber() const;

    bool setData(int column, const QVariant &value);

    TreeItem *findName(const QString &name);

protected:
    QList<TreeItem *> childItems;
    QVector<QVariant> itemData;
    TreeItem *parentItem;

};


#endif //ANTARCTICA_TREEITEM_H
