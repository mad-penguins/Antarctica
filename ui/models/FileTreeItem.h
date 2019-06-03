#ifndef ANTARCTICA_FILETREEITEM_H
#define ANTARCTICA_FILETREEITEM_H


#include <QtCore/QVector>
#include <QtCore/QVariant>
#include <api/models/File.h>

class FileTreeItem {
public:
    explicit FileTreeItem(const QVector<QVariant> &data, FileTreeItem *parent = nullptr);

    ~FileTreeItem();

    inline FileTreeItem *child(int number) {
        return childItems.value(number);
    }

    inline int childCount() const {
        return childItems.count();
    }

    inline int columnCount() const {
        return itemData.count();
    }

    inline QVariant data(int column) const {
        return itemData.value(column);
    }

    bool insertChildren(int position, int count, int columns);

    bool appendChild(const QVector<QVariant> &data);

    bool insertColumns(int position, int columns);

    inline FileTreeItem *parent() {
        return parentItem;
    }

    bool removeChildren(int position, int count);

    bool removeColumns(int position, int columns);

    int childNumber() const;

    bool setData(int column, const QVariant &value);

    FileTreeItem *findName(const QString &name);

private:
    QList<FileTreeItem *> childItems;
    QVector<QVariant> itemData;
    FileTreeItem *parentItem;
    File *file;
};


#endif //ANTARCTICA_FILETREEITEM_H
