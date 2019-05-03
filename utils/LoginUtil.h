#ifndef ANTARCTICA_LOGINUTIL_H
#define ANTARCTICA_LOGINUTIL_H


#include <QtCore/QPair>

#include "../models/User.h"

using namespace std;

class LoginException : public exception {
public:
    enum Kind {
        WRONG_LOGIN_DATA,
        WRONG_RESPONSE,
        UNKNOWN_ERROR
    } kind;

    explicit LoginException(Kind kind) : kind(kind) {}

};

class LoginUtil {
public:
    static const User logIn(const QString &login, const QString &password) noexcept(false);
};


#endif //ANTARCTICA_LOGINUTIL_H
