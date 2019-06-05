#ifndef ANTARCTICA_FILETREEITEM_H
#define ANTARCTICA_FILETREEITEM_H


#include <api/models/File.h>
#include "ui/models/TreeItem.h"

class FileTreeItem : public TreeItem {
public:
    explicit FileTreeItem(const QVector<QVariant> &data, FileTreeItem *parent = nullptr);

    ~FileTreeItem() override;

    void setFile(File *f);

    File *getFile() const;

private:
    File *file;
};


#endif //ANTARCTICA_FILETREEITEM_H
