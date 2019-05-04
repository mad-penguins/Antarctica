#ifndef ANTARCTICA_FILE_H
#define ANTARCTICA_FILE_H


#include <QtCore/QString>
#include <QtCore/QDateTime>
#include <utility>

#include "Package.h"

using namespace std;

class File {
public:
    unsigned id;
    QString name;
    QString path;
    QByteArray content;
    QDateTime created;
    QDateTime modified;
    uint package_id;
    Package *package;

    File(int id, QString name, QString path, QDateTime created, QDateTime modified, uint packageId)
            : id(id), name(move(name)), path(move(path)),
              created(move(created)), modified(move(modified)), package_id(packageId) {}

    File(int id, QString name, QString path, QByteArray content, QDateTime created, QDateTime modified, uint packageId)
            : id(id), name(move(name)), path(move(path)), content(move(content)),
              created(move(created)), modified(move(modified)), package_id(packageId) {}
};


#endif //ANTARCTICA_FILE_H
