#ifndef ANTARCTICA_PACKAGETREEMODEL_H
#define ANTARCTICA_PACKAGETREEMODEL_H


#include <QtCore/QAbstractItemModel>

#include "ui/models/TreeModel.h"
#include "PackageTreeItem.h"

class PackageTreeModel : public TreeModel {
Q_OBJECT

public:
    PackageTreeModel(const QStringList &headers, const QList<Package *> &data, QObject *parent = nullptr);

    ~PackageTreeModel() override;

private:
    void setupModelData(const QList<Entity *> &packages, TreeItem *parent) override;

};


#endif //ANTARCTICA_PACKAGETREEMODEL_H
