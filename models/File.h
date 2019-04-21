#include <utility>

#include <utility>

#include <utility>

#include <utility>

#include <utility>

#include <utility>

#ifndef ANTARCTICA_FILE_H
#define ANTARCTICA_FILE_H


#include <QtCore/QString>
#include <QtCore/QDateTime>

class File {
public:
    uint id;
    QString name;
    QString path;
    QByteArray content;
    QDateTime created;
    QDateTime modified;
    uint package_id;

    File(int id, QString name, QString path, QByteArray content, QDateTime created, QDateTime modified, uint packageId)
    : id(id), name(std::move(name)), path(std::move(path)), content(std::move(content)),
      created(std::move(created)), modified(std::move(modified)), package_id(packageId) {}
};


#endif //ANTARCTICA_FILE_H
