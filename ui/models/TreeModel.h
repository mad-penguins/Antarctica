/*!
 * \file
 * \author Nikita Mironov <nickfrom22nd@gmail.com>
 * \brief The polymorphic tree model class
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

#ifndef ANTARCTICA_TREEMODEL_H
#define ANTARCTICA_TREEMODEL_H


#include <QtCore/QAbstractItemModel>
#include <api/models/Entity.h>

#include "TreeItem.h"

class TreeModel : public QAbstractItemModel {
public:
    explicit TreeModel(QObject *parent = nullptr);

    ~TreeModel() override = 0;

    QVariant data(const QModelIndex &index, int role) const override;

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;

    QModelIndex parent(const QModelIndex &index) const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    inline int columnCount(const QModelIndex &parent = QModelIndex()) const override {
        return rootItem->columnCount();
    }

    Qt::ItemFlags flags(const QModelIndex &index) const override;

    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::DisplayRole) override;

    bool
    setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role = Qt::DisplayRole) override;

    bool insertColumns(int position, int columns, const QModelIndex &parent = QModelIndex()) override;

    bool removeColumns(int position, int columns, const QModelIndex &parent = QModelIndex()) override;

    bool insertRows(int position, int rows, const QModelIndex &parent = QModelIndex()) override;

    bool removeRows(int position, int rows, const QModelIndex &parent = QModelIndex()) override;

    TreeItem *getItem(const QModelIndex &index) const;

protected:
    virtual void setupModelData(const QList<Entity *> &objects, TreeItem *parent) = 0;

    TreeItem *rootItem{};

};

inline TreeModel::~TreeModel() = default;


#endif //ANTARCTICA_TREEMODEL_H
