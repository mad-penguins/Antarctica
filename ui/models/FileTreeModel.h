#ifndef ANTARCTICA_FILETREEMODEL_H
#define ANTARCTICA_FILETREEMODEL_H


#include <QtCore/QAbstractItemModel>
#include <QtCore/QStack>
#include <api/models/File.h>
#include "FileTreeItem.h"

class FileTreeModel : public QAbstractItemModel {
Q_OBJECT

public:
    FileTreeModel(const QStringList &headers, const QList<File *> &data, QObject *parent = nullptr);

    ~FileTreeModel();

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
    void setupModelData(const QList<File *> &files, FileTreeItem *parent);

    FileTreeItem *getItem(const QModelIndex &index) const;

    FileTreeItem *rootItem;

    void createDirs(QStringList dirsList, FileTreeItem *parent);

    FileTreeItem *lastDir{};
};


#endif //ANTARCTICA_FILETREEMODEL_H
