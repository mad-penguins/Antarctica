#ifndef ANTARCTICA_USER_H
#define ANTARCTICA_USER_H


#include <utility>
#include <QtCore/QString>

class User {
public:
    int id;
    QString login;
    QString displayName;
    QString password;
};


#endif //ANTARCTICA_USER_H
