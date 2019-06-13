/*!
 * \file
 * \author Nikita Mironov <nickfrom22nd@gmail.com>
 * \brief The file tree model implementation
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


#include <api/models/File.h>
#include <QtCore/QStack>
#include <QtCore/QDebug>
#include <QtCore/QCollator>
#include <utils/Files.hpp>

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

    if (const auto searchResult = parent->findName(dir); searchResult) {
        lastDir = reinterpret_cast<FileTreeItem *>(searchResult);
    } else {
        parent->appendChild(QVector<QVariant>() << dir);
        lastDir = reinterpret_cast<FileTreeItem *>(parent->child(parent->childCount() - 1));
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
        fileData << file->name << file->created << file->modified << QVariant(Utils::Files::isFileDownloaded(file));
        lastDir->appendChild(fileData);
        reinterpret_cast<FileTreeItem *>(lastDir->child(lastDir->childCount() - 1))->setFile(file);
    }
}
