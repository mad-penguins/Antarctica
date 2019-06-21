/*!
 * \file
 * \author Nikita Mironov <nickfrom22nd@gmail.com>
 * \brief The file tree item implementation
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


#include <QtCore/QCollator>
#include "FileTreeItem.h"

FileTreeItem::FileTreeItem(const QVector<QVariant> &data, FileTreeItem *parent) : TreeItem(data, parent) {
    file = nullptr;
}

FileTreeItem::~FileTreeItem() {
    qDeleteAll(childItems);
    delete file;
}

File *FileTreeItem::getFile() const {
    return file;
}

void FileTreeItem::setFile(File *f) {
    file = f;
}

bool FileTreeItem::insertChildren(int position, int count, int columns) {
    if (position < 0 || position > childItems.size()) {
        return false;
    }

    for (int row = 0; row < count; ++row) {
        QVector<QVariant> data(columns);
        auto *item = new FileTreeItem(data, this);
        childItems.insert(position, item);
    }

    return true;
}

FileTreeItem *FileTreeItem::findName(const QString &name) {
    QCollator collator;
    auto iter = std::lower_bound(
            childItems.begin(),
            childItems.end(),
            FileTreeItem(QVector<QVariant>() << name),
            [&collator](TreeItem *f, const TreeItem &n) {
                return collator.compare(f->data(0).toString(), n.data(0).toString()) < 0;
            });
    return reinterpret_cast<FileTreeItem *>(iter == childItems.end() ? nullptr : *iter);
}
