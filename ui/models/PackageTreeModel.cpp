#include <api/models/Package.h>
#include <QtCore/QDebug>
#include <QtCore/QCollator>

#include "PackageTreeModel.h"

PackageTreeModel::PackageTreeModel(const QStringList &headers, const QList<Package *> &data, QObject *parent)
        : QAbstractItemModel(parent) {
    QVector<QVariant> rootData;
    for (auto &&header : headers) {
        rootData << header;
    }

    rootItem = new PackageTreeItem(rootData);
    setupModelData(data, rootItem);
}

PackageTreeModel::~PackageTreeModel() {
    delete rootItem;
}

QVariant PackageTreeModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid()) {
        return QVariant();
    }

    if (role != Qt::DisplayRole && role != Qt::EditRole) {
        return QVariant();
    }

    PackageTreeItem *item = getItem(index);
    return item->data(index.column());
}

Qt::ItemFlags PackageTreeModel::flags(const QModelIndex &index) const {
    if (!index.isValid()) {
        return 0;
    }

    return Qt::ItemIsEditable | QAbstractItemModel::flags(index);
}

PackageTreeItem *PackageTreeModel::getItem(const QModelIndex &index) const {
    if (index.isValid()) {
        auto *item = static_cast<PackageTreeItem *>(index.internalPointer());
        if (item) {
            return item;
        }
    }
    return rootItem;
}

QVariant PackageTreeModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        return rootItem->data(section);
    }

    return QVariant();
}

QModelIndex PackageTreeModel::index(int row, int column, const QModelIndex &parent) const {
    if (parent.isValid() && parent.column() != 0)
        return {};

    PackageTreeItem *parentItem = getItem(parent);

    PackageTreeItem *childItem = parentItem->child(row);
    if (childItem) {
        return createIndex(row, column, childItem);
    } else {
        return {};
    }
}

bool PackageTreeModel::insertColumns(int position, int columns, const QModelIndex &parent) {
    bool success;

    beginInsertColumns(parent, position, position + columns - 1);
    success = rootItem->insertColumns(position, columns);
    endInsertColumns();

    return success;
}

bool PackageTreeModel::insertRows(int position, int rows, const QModelIndex &parent) {
    PackageTreeItem *parentItem = getItem(parent);
    bool success;

    beginInsertRows(parent, position, position + rows - 1);
    success = parentItem->insertChildren(position, rows, rootItem->columnCount());
    endInsertRows();

    return success;
}

QModelIndex PackageTreeModel::parent(const QModelIndex &index) const {
    if (!index.isValid()) {
        return {};
    }

    PackageTreeItem *childItem = getItem(index);
    PackageTreeItem *parentItem = childItem->parent();

    if (parentItem == rootItem) {
        return {};
    }

    return createIndex(parentItem->childNumber(), 0, parentItem);
}

bool PackageTreeModel::removeColumns(int position, int columns, const QModelIndex &parent) {
    bool success;

    beginRemoveColumns(parent, position, position + columns - 1);
    success = rootItem->removeColumns(position, columns);
    endRemoveColumns();

    if (rootItem->columnCount() == 0) {
        removeRows(0, rowCount());
    }

    return success;
}

bool PackageTreeModel::removeRows(int position, int rows, const QModelIndex &parent) {
    PackageTreeItem *parentItem = getItem(parent);
    bool success = true;

    beginRemoveRows(parent, position, position + rows - 1);
    success = parentItem->removeChildren(position, rows);
    endRemoveRows();

    return success;
}

int PackageTreeModel::rowCount(const QModelIndex &parent) const {
    PackageTreeItem *parentItem = getItem(parent);
    return parentItem->childCount();
}

bool PackageTreeModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    if (role != Qt::EditRole) {
        return false;
    }

    PackageTreeItem *item = getItem(index);
    bool result = item->setData(index.column(), value);

    if (result) {
        emit dataChanged(index, index, {role});
    }

    return result;
}

bool PackageTreeModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role) {
    if (role != Qt::EditRole || orientation != Qt::Horizontal) {
        return false;
    }

    bool result = rootItem->setData(section, value);

    if (result) {
        emit headerDataChanged(orientation, section, section);
    }

    return result;
}

void PackageTreeModel::setupModelData(const QList<Package *> &packages, PackageTreeItem *parent) {
    QCollator collator; // TODO: implement server-side sorting?
    QList<Package *> sortedPackages(packages);
    std::sort(sortedPackages.begin(), sortedPackages.end(), [&collator](Package *pkg1, Package *pkg2) {
        return collator.compare(
                pkg1->repository->name + "@" + pkg1->name,
                pkg2->repository->name + "@" + pkg2->name
        ) < 0;
    });

    QMap<QString, PackageTreeItem *> repos;
    for (auto &&pkg : sortedPackages) {
        if (!repos.keys().contains(pkg->repository->name)) {
            parent->appendChild(QVector<QVariant>() << pkg->repository->name);
            repos.insert(pkg->repository->name, parent->child(parent->childCount() - 1));
        }
        repos[pkg->repository->name]->appendChild(QVector<QVariant>() << pkg->name);
    }
}
