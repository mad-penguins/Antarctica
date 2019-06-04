#ifndef ANTARCTICA_TREEMODEL_H
#define ANTARCTICA_TREEMODEL_H


#include <QtCore/QAbstractItemModel>
#include <api/models/Entity.h>

#include "TreeItem.h"

class TreeModel : public QAbstractItemModel {
public:
    explicit TreeModel(QObject *parent = nullptr);

    ~TreeModel() override;

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

protected:
    virtual void setupModelData(const QList<Entity *> &objects, TreeItem *parent) = 0;

    TreeItem *getItem(const QModelIndex &index) const;

    TreeItem *rootItem{};

};


#endif //ANTARCTICA_TREEMODEL_H
