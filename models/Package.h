#ifndef ANTARCTICA_PACKAGE_H
#define ANTARCTICA_PACKAGE_H


#include <QtCore/QString>
#include "Repository.h"

using namespace std;

class Package {
public:
    uint id;
    QString name;
    uint repo_id;
    Repository *repository;

    Package(int id, QString name, uint repoId) : id(id), name(move(name)), repo_id(repoId) {}
};


#endif //ANTARCTICA_PACKAGE_H
