#include "TreeModel.h"


TreeModel::TreeModel(QObject *parent) : QAbstractItemModel(parent) {

}


TreeModel::~TreeModel() {
    delete rootItem;
}


QVariant TreeModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid()) {
        return QVariant();
    }

    if (role != Qt::DisplayRole && role != Qt::EditRole) {
        return QVariant();
    }

    TreeItem *item = getItem(index);
    return item->data(index.column());
}


Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const {
    if (!index.isValid()) {
        return nullptr;
    }

    return Qt::ItemIsEditable | QAbstractItemModel::flags(index);
}


TreeItem *TreeModel::getItem(const QModelIndex &index) const {
    if (index.isValid()) {
        auto *item = static_cast<TreeItem *>(index.internalPointer());
        if (item) {
            return item;
        }
    }
    return rootItem;
}


QVariant TreeModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        return rootItem->data(section);
    }

    return QVariant();
}


QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent) const {
    if (parent.isValid() && parent.column() != 0)
        return {};

    TreeItem *parentItem = getItem(parent);

    TreeItem *childItem = parentItem->child(row);
    if (childItem) {
        return createIndex(row, column, childItem);
    } else {
        return {};
    }
}


bool TreeModel::insertColumns(int position, int columns, const QModelIndex &parent) {
    bool success;

    beginInsertColumns(parent, position, position + columns - 1);
    success = rootItem->insertColumns(position, columns);
    endInsertColumns();

    return success;
}


bool TreeModel::insertRows(int position, int rows, const QModelIndex &parent) {
    TreeItem *parentItem = getItem(parent);
    bool success;

    beginInsertRows(parent, position, position + rows - 1);
    success = parentItem->insertChildren(position, rows, rootItem->columnCount());
    endInsertRows();

    return success;
}


QModelIndex TreeModel::parent(const QModelIndex &index) const {
    if (!index.isValid()) {
        return {};
    }

    TreeItem *childItem = getItem(index);
    TreeItem *parentItem = childItem->parent();

    if (parentItem == rootItem) {
        return {};
    }

    return createIndex(parentItem->childNumber(), 0, parentItem);
}


bool TreeModel::removeColumns(int position, int columns, const QModelIndex &parent) {
    bool success;

    beginRemoveColumns(parent, position, position + columns - 1);
    success = rootItem->removeColumns(position, columns);
    endRemoveColumns();

    if (rootItem->columnCount() == 0) {
        removeRows(0, rowCount());
    }

    return success;
}


bool TreeModel::removeRows(int position, int rows, const QModelIndex &parent) {
    TreeItem *parentItem = getItem(parent);

    beginRemoveRows(parent, position, position + rows - 1);
    bool success = parentItem->removeChildren(position, rows);
    endRemoveRows();

    return success;
}


int TreeModel::rowCount(const QModelIndex &parent) const {
    TreeItem *parentItem = getItem(parent);
    return parentItem->childCount();
}


bool TreeModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    if (role != Qt::EditRole) {
        return false;
    }

    TreeItem *item = getItem(index);
    bool result = item->setData(index.column(), value);

    if (result) {
        emit dataChanged(index, index, {role});
    }

    return result;
}


bool TreeModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role) {
    if (role != Qt::EditRole || orientation != Qt::Horizontal) {
        return false;
    }

    bool result = rootItem->setData(section, value);

    if (result) {
        emit headerDataChanged(orientation, section, section);
    }

    return result;
}