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

    rootItem = new FileTreeItem(rootData);
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
        lastDir = dynamic_cast<FileTreeItem *>(searchResult);
    } else {
        parent->appendChild(QVector<QVariant>() << dir);
        lastDir = dynamic_cast<FileTreeItem *>(parent->child(parent->childCount() - 1));
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
        sortedFiles.append(dynamic_cast<File *>(file));
    }
    std::sort(sortedFiles.begin(), sortedFiles.end(), [&collator](File *file1, File *file2) {
        return collator.compare(
                file1->getRelativeName(),
                file2->getRelativeName()
        ) < 0;
    });

    parent->appendChild(QVector<QVariant>() << "/");
    auto root = parent->child(parent->childCount() - 1);

    for (auto &&file : sortedFiles) {
        QStringList dirsList = file->path.split('/');
        createDirs(dirsList, dynamic_cast<FileTreeItem *>(root));

        QVector<QVariant> fileData;
        fileData << file->name << file->created << file->modified
                 << Utils::Files::downloaded(file) << Utils::Files::actual(file);
        lastDir->appendChild(fileData);
        lastDir->child(lastDir->childCount() - 1)->setFile(file);

        items.insert(file, lastDir->child(lastDir->childCount() - 1));
    }

    mergeDirs(dynamic_cast<FileTreeItem *>(rootItem->child(0)->findName("~")));
}

void FileTreeModel::mergeDirs(FileTreeItem *parent) {
    if (parent->isDir()) {
        if (parent->childCount() == 1) {
            auto child = parent->child(0);
            if (child->isDir()) {
                parent->insertChildren(0, child->childCount(), rootItem->columnCount());
                for (int j = 0; j < child->childCount(); ++j) {
                    parent->setChild(j, child->child(j));
                }
                parent->setData(0, parent->data(0).toString() + "/" + child->data(0).toString());
                parent->removeChild(parent->childCount() - 1);
                mergeDirs(parent);
            }
        } else {
            for (int i = 0; i < parent->childCount(); ++i) {
                auto child = parent->child(i);
                mergeDirs(child);
            }
        }
    }
}

void FileTreeModel::handleChanges(const QList<File *> &changes) {
    if (!changes.isEmpty()) {
        qDebug() << "These files were changed:";
    }
    for (auto &&file : changes) {
        qDebug() << "\t" << file->getRelativeName();
        items[file]->setData(4, Utils::Files::actual(file));
    }
}
