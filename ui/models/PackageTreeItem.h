#ifndef ANTARCTICA_PACKAGETREEITEM_H
#define ANTARCTICA_PACKAGETREEITEM_H


#include <QtCore/QVariant>
#include <api/models/Package.h>

class PackageTreeItem {
public:
    explicit PackageTreeItem(const QVector<QVariant> &data, PackageTreeItem *parent = nullptr);

    ~PackageTreeItem();

    inline PackageTreeItem *child(int number) {
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

    inline PackageTreeItem *parent() {
        return parentItem;
    }

    bool removeChildren(int position, int count);

    bool removeColumns(int position, int columns);

    int childNumber() const;

    bool setData(int column, const QVariant &value);

    PackageTreeItem *findName(const QString &name);

private:
    QList<PackageTreeItem *> childItems;
    QVector<QVariant> itemData;
    PackageTreeItem *parentItem;
    Package *package;
};


#endif //ANTARCTICA_PACKAGETREEITEM_H
