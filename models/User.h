#ifndef ANTARCTICA_USER_H
#define ANTARCTICA_USER_H


#include <utility>
#include <QtCore/QString>

using namespace std;

class User {
public:
    User(unsigned id, QString login, QString password, QString displayName)
            : id(id), login(move(login)), password(move(password)), displayName(move(displayName)) {}

    User() = default;

    unsigned id;
    QString login;
    QString displayName;
    QString password;
};


#endif //ANTARCTICA_USER_H
