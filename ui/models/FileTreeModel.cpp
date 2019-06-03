#include <api/models/File.h>
#include <QtCore/QStack>
#include <QtCore/QDebug>
#include <QtCore/QCollator>

#include "FileTreeModel.h"

FileTreeModel::FileTreeModel(const QStringList &headers, const QList<File *> &data, QObject *parent)
        : QAbstractItemModel(parent) {
    QVector<QVariant> rootData;
    for (auto &&header : headers) {
        rootData << header;
    }

    rootItem = new FileTreeItem(rootData);
    lastDir = rootItem;
    setupModelData(data, rootItem);
}

FileTreeModel::~FileTreeModel() {
    delete rootItem;
}

QVariant FileTreeModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid()) {
        return QVariant();
    }

    if (role != Qt::DisplayRole && role != Qt::EditRole) {
        return QVariant();
    }

    FileTreeItem *item = getItem(index);
    return item->data(index.column());
}

Qt::ItemFlags FileTreeModel::flags(const QModelIndex &index) const {
    if (!index.isValid()) {
        return 0;
    }

    return Qt::ItemIsEditable | QAbstractItemModel::flags(index);
}

FileTreeItem *FileTreeModel::getItem(const QModelIndex &index) const {
    if (index.isValid()) {
        auto *item = static_cast<FileTreeItem *>(index.internalPointer());
        if (item) {
            return item;
        }
    }
    return rootItem;
}

QVariant FileTreeModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        return rootItem->data(section);
    }

    return QVariant();
}

QModelIndex FileTreeModel::index(int row, int column, const QModelIndex &parent) const {
    if (parent.isValid() && parent.column() != 0)
        return {};

    FileTreeItem *parentItem = getItem(parent);

    FileTreeItem *childItem = parentItem->child(row);
    if (childItem) {
        return createIndex(row, column, childItem);
    } else {
        return {};
    }
}

bool FileTreeModel::insertColumns(int position, int columns, const QModelIndex &parent) {
    bool success;

    beginInsertColumns(parent, position, position + columns - 1);
    success = rootItem->insertColumns(position, columns);
    endInsertColumns();

    return success;
}

bool FileTreeModel::insertRows(int position, int rows, const QModelIndex &parent) {
    FileTreeItem *parentItem = getItem(parent);
    bool success;

    beginInsertRows(parent, position, position + rows - 1);
    success = parentItem->insertChildren(position, rows, rootItem->columnCount());
    endInsertRows();

    return success;
}

QModelIndex FileTreeModel::parent(const QModelIndex &index) const {
    if (!index.isValid()) {
        return {};
    }

    FileTreeItem *childItem = getItem(index);
    FileTreeItem *parentItem = childItem->parent();

    if (parentItem == rootItem) {
        return {};
    }

    return createIndex(parentItem->childNumber(), 0, parentItem);
}

bool FileTreeModel::removeColumns(int position, int columns, const QModelIndex &parent) {
    bool success;

    beginRemoveColumns(parent, position, position + columns - 1);
    success = rootItem->removeColumns(position, columns);
    endRemoveColumns();

    if (rootItem->columnCount() == 0) {
        removeRows(0, rowCount());
    }

    return success;
}

bool FileTreeModel::removeRows(int position, int rows, const QModelIndex &parent) {
    FileTreeItem *parentItem = getItem(parent);
    bool success = true;

    beginRemoveRows(parent, position, position + rows - 1);
    success = parentItem->removeChildren(position, rows);
    endRemoveRows();

    return success;
}

int FileTreeModel::rowCount(const QModelIndex &parent) const {
    FileTreeItem *parentItem = getItem(parent);
    return parentItem->childCount();
}

bool FileTreeModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    if (role != Qt::EditRole) {
        return false;
    }

    FileTreeItem *item = getItem(index);
    bool result = item->setData(index.column(), value);

    if (result) {
        emit dataChanged(index, index, {role});
    }

    return result;
}

bool FileTreeModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role) {
    if (role != Qt::EditRole || orientation != Qt::Horizontal) {
        return false;
    }

    bool result = rootItem->setData(section, value);

    if (result) {
        emit headerDataChanged(orientation, section, section);
    }

    return result;
}

void FileTreeModel::createDirs(QStringList dirsList, FileTreeItem *parent) {
    if (dirsList.isEmpty()) {
        return;
    }

    auto dir = dirsList.front();
    dirsList.pop_front();

    if (auto searchResult = parent->findName(dir)) {
        lastDir = searchResult;
    } else {
        parent->insertChildren(parent->childCount(), 1, rootItem->columnCount());
        parent->child(parent->childCount() - 1)->setData(0, dir);

        lastDir = parent->child(parent->childCount() - 1);
    }

    createDirs(dirsList, lastDir);
}

void FileTreeModel::setupModelData(const QList<File *> &files, FileTreeItem *parent) {
    QCollator collator; // TODO: implement server-side sorting?
    QList<File *> sortedFiles(files);
    std::sort(sortedFiles.begin(), sortedFiles.end(), [&collator] (File *file1, File *file2) {
        return collator.compare(
                file1->path + "/" + file1->name,
                file2->path + "/" + file2->name
                ) < 0;
    });

    parent->insertChildren(parent->childCount(), 1, 1);
    auto root = parent->child(parent->childCount() - 1);
    root->setData(0, "/");

    for (auto &&file : sortedFiles) {
        QStringList dirsList = file->path.split('/');
        createDirs(dirsList, root);

        auto dir = lastDir;
        dir->insertChildren(dir->childCount(), 1, 1);
        dir->child(dir->childCount() - 1)->setData(0, file->name);
    }
}
