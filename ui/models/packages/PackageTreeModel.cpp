#include <api/models/Package.h>
#include <QtCore/QDebug>
#include <QtCore/QCollator>

#include "PackageTreeModel.h"

PackageTreeModel::PackageTreeModel(const QStringList &headers, const QList<Package *> &data, QObject *parent)
        : TreeModel(parent) {
    QVector<QVariant> rootData;
    for (auto &&header : headers) {
        rootData << header;
    }

    rootItem = new TreeItem(rootData);
    QList<Entity *> pkgs;
    for (auto &&pkg : data) {
        pkgs.append(pkg);
    }
    setupModelData(pkgs, rootItem);
}

PackageTreeModel::~PackageTreeModel() {
    delete rootItem;
}

void PackageTreeModel::setupModelData(const QList<Entity *> &packages, TreeItem *parent) {
    if (packages.size() <= 1) { // there's a special package for files that aren't a package config
        parent->appendChild(QVector<QVariant>() << "No packages yet :(");
        return;
    }

    QCollator collator; // TODO: implement server-side sorting?
    QList<Package *> sortedPackages;
    for (auto &&pkg : packages) {
        auto casted = const_cast<Package *>(reinterpret_cast<const Package *>(pkg));
        if (casted->id != 1) {
            sortedPackages.append(casted);
        }
    }
    std::sort(sortedPackages.begin(), sortedPackages.end(), [&collator](Package *pkg1, Package *pkg2) {
        return collator.compare(
                pkg1->repository->name + "@" + pkg1->name,
                pkg2->repository->name + "@" + pkg2->name
        ) < 0;
    });

    QMap<QString, PackageTreeItem *> repos;
    for (auto &&pkg : sortedPackages) {
        if (!repos.keys().contains(pkg->repository->name)) {
            parent->appendChild(QVector<QVariant>() << pkg->repository->name);
            repos.insert(pkg->repository->name, (PackageTreeItem *) parent->child(parent->childCount() - 1));
        }
        repos[pkg->repository->name]->appendChild(QVector<QVariant>() << pkg->name);
        reinterpret_cast<PackageTreeItem*>(
                repos[pkg->repository->name]->child(repos[pkg->repository->name]->childCount()-1)
                )->setPackage(pkg);
    }
}
