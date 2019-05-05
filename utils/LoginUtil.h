/*!
 * \file util/LoginUtil.h
 * \author Nikita Mironov <nickfrom22nd@gmail.com>
 * \brief The login utility class
 *
 * \section LICENSE
 *
 * Copyright (c) 2019 Penguins of Madagascar

 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

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
