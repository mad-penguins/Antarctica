#ifndef ANTARCTICA_FILE_H
#define ANTARCTICA_FILE_H


#include <QtCore/QString>
#include <QtCore/QDateTime>
#include <utility>

class File {
public:
    int id;
    QString name;
    QString path;
    QByteArray content;
    QDateTime created;
    QDateTime modified;
    int package_id;
};


#endif //ANTARCTICA_FILE_H
