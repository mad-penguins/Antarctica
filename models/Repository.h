#ifndef ANTARCTICA_REPOSITORY_H
#define ANTARCTICA_REPOSITORY_H


#include <QtCore/QString>

using namespace std;

class Repository {
public:
    unsigned id;
    QString name;
    QString url;
    QString manager;

    Repository(int id, QString name, QString url, QString manager)
        : id(id), name(move(name)), url(move(url)), manager(move(manager)) {}
};


#endif //ANTARCTICA_REPOSITORY_H
