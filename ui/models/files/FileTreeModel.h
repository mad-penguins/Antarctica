#ifndef ANTARCTICA_FILETREEMODEL_H
#define ANTARCTICA_FILETREEMODEL_H


#include <QtCore/QAbstractItemModel>
#include <api/models/File.h>

#include "ui/models/TreeModel.h"
#include "FileTreeItem.h"

class FileTreeModel : public TreeModel {
Q_OBJECT

public:
    FileTreeModel(const QStringList &headers, const QList<File *> &data, QObject *parent = nullptr);

    ~FileTreeModel() override;

private:
    void setupModelData(const QList<Entity *> &files, TreeItem *parent) override;

    void createDirs(QStringList dirsList, TreeItem *parent);

    FileTreeItem *lastDir{};
};


#endif //ANTARCTICA_FILETREEMODEL_H
