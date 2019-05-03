#ifndef ANTARCTICA_USER_H
#define ANTARCTICA_USER_H


#include <utility>
#include <QtCore/QString>

class User {
public:
    User(uint id, QString login, QString password, QString displayName)
            : id(id), login(std::move(login)), password(std::move(password)), displayName(std::move(displayName)) {}

    User() {}

    uint id;
    QString login;
    QString displayName;
    QString password;
};


#endif //ANTARCTICA_USER_H
