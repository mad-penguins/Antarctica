#ifndef ANTARCTICA_TREEITEM_H
#define ANTARCTICA_TREEITEM_H


#include <QtCore/QVector>
#include <QtCore/QVariant>

class TreeItem {
public:
    explicit TreeItem(const QVector<QVariant> &data, TreeItem *parent = nullptr);

    virtual ~TreeItem();

    inline TreeItem *child(int number) {
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

    inline TreeItem *parent() {
        return parentItem;
    }

    bool removeChildren(int position, int count);

    bool removeColumns(int position, int columns);

    int childNumber() const;

    bool setData(int column, const QVariant &value);

    TreeItem *findName(const QString &name);

protected:
    QList<TreeItem *> childItems;
    QVector<QVariant> itemData;
    TreeItem *parentItem;

};


#endif //ANTARCTICA_TREEITEM_H
