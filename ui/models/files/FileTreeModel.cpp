#include <api/models/File.h>
#include <QtCore/QStack>
#include <QtCore/QDebug>
#include <QtCore/QCollator>

#include "FileTreeModel.h"
#include "FileTreeItem.h"

FileTreeModel::FileTreeModel(const QStringList &headers, const QList<File *> &data, QObject *parent) : TreeModel(
        parent) {
    QVector<QVariant> rootData;
    for (auto &&header : headers) {
        rootData << header;
    }

    rootItem = new TreeItem(rootData);
    QList<Entity *> files;
    for (auto &&file : data) {
        files.append(file);
    }
    setupModelData(files, rootItem);
}

FileTreeModel::~FileTreeModel() {
    delete rootItem;
}

void FileTreeModel::createDirs(QStringList dirsList, TreeItem *parent) {
    if (dirsList.isEmpty()) {
        return;
    }

    auto dir = dirsList.front();
    dirsList.pop_front();

    if (auto searchResult = parent->findName(dir)) {
        lastDir = searchResult;
    } else {
        parent->appendChild(QVector<QVariant>() << dir);
        lastDir = parent->child(parent->childCount() - 1);
    }

    createDirs(dirsList, lastDir);
}

void FileTreeModel::setupModelData(const QList<Entity *> &files, TreeItem *parent) {
    if (files.isEmpty()) {
        parent->appendChild(QVector<QVariant>() << "No files yet :(");
        return;
    }

    QCollator collator; // TODO: implement server-side sorting?
    QList<File *> sortedFiles;
    for (auto &&file : files) {
        sortedFiles.append(const_cast<File *>(reinterpret_cast<const File *>(file)));
    }
    std::sort(sortedFiles.begin(), sortedFiles.end(), [&collator](File *file1, File *file2) {
        return collator.compare(
                file1->path + "/" + file1->name,
                file2->path + "/" + file2->name
        ) < 0;
    });

    parent->appendChild(QVector<QVariant>() << "/");
    auto root = parent->child(parent->childCount() - 1);

    for (auto &&file : sortedFiles) {
        QStringList dirsList = file->path.split('/');
        createDirs(dirsList, (FileTreeItem *) root);

        QVector<QVariant> fileData;
        fileData << file->name << file->created << file->modified;
        lastDir->appendChild(fileData);
    }
}
