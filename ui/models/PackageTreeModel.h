#ifndef ANTARCTICA_PACKAGETREEMODEL_H
#define ANTARCTICA_PACKAGETREEMODEL_H


#include <QtCore/QAbstractItemModel>

#include "PackageTreeItem.h"

class PackageTreeModel : public QAbstractItemModel {
Q_OBJECT

public:
    PackageTreeModel(const QStringList &headers, const QList<Package *> &data, QObject *parent = nullptr);

    ~PackageTreeModel() override;

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

private:
    void setupModelData(const QList<Package *> &packages, PackageTreeItem *parent);

    PackageTreeItem *getItem(const QModelIndex &index) const;

    PackageTreeItem *rootItem;

};


#endif //ANTARCTICA_PACKAGETREEMODEL_H
