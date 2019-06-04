#ifndef ANTARCTICA_PACKAGETREEITEM_H
#define ANTARCTICA_PACKAGETREEITEM_H


#include <QtCore/QVariant>
#include <api/models/Package.h>

#include "TreeItem.h"

class PackageTreeItem : public TreeItem {
public:
    explicit PackageTreeItem(const QVector<QVariant> &data, PackageTreeItem *parent = nullptr);

    ~PackageTreeItem() override;

private:
    Package *pkg;
};


#endif //ANTARCTICA_PACKAGETREEITEM_H
