#ifndef ANTARCTICA_LOGINUTIL_H
#define ANTARCTICA_LOGINUTIL_H


#include <QPair>
#include "../models/User.h"

class LoginException : public std::exception {
public:
    enum Kind {
        WRONG_LOGIN,
        WRONG_PASSWORD,
        WRONG_RESPONSE,
        UNKNOWN_ERROR
    } kind;

    explicit LoginException(Kind kind) : kind(kind) {}

};

class LoginUtil {
public:
    static const User logIn(const QString& login, const QString& password) noexcept(false);
};


#endif //ANTARCTICA_LOGINUTIL_H
