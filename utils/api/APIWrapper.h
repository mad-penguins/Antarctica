/**
 * @file
 * @author  Nikita Mironov <nickfrom22nd@gmail.com>
 *
 * @section LICENSE
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
 *
 * @section DESCRIPTION
 *
 * The API wrapper class
 */

#ifndef ANTARCTICA_APIWRAPPER_H
#define ANTARCTICA_APIWRAPPER_H


#include <QtCore/QString>

#include "../../models/File.h"
#include "../../models/Package.h"
#include "../../models/Repository.h"

using namespace std;

class APIWrapper {
public:
    static void setUserData(unsigned id, QString token) {
        userId = id;
        accessToken = move(token);
    }

    class Files {
        APIWrapper *parent;
    public:
        //explicit Files(APIWrapper *parent) : parent(parent) {}

        static QList<File *> getAll();

        static File *get(unsigned id);

        static bool upload(const File &);

        static bool update(const File &);

        static bool remove(unsigned id);
    };

    class Packages {
        APIWrapper *parent;
    public:
        //explicit Packages(APIWrapper *parent) : parent(parent) {}

        static QList<Package *> getAll();

        static Package *get(unsigned id);

        static bool upload(const Package &);

        static bool update(const Package &);

        static bool remove(unsigned id);
    };

    class Repositories {
        APIWrapper *parent;
    public:
        //explicit Repositories(APIWrapper *parent) : parent(parent) {}

        static QList<Repository *> getAll();

        static Repository *get(unsigned id);

        static bool upload(const Repository &);

        static bool update(const Repository &);

        static bool remove(unsigned id);
    };

private:
    static unsigned userId;
    static QString accessToken;

    Files filesWrapper;
    Packages packagesWrapper;
    Repositories repositories;

    class Utils {
    public:
        enum RequestType {
            GET, POST, PUT, DELETE
        };
        static QJsonDocument execute(const QUrl &requestUrl, RequestType type);
    };

    //APIWrapper() : filesWrapper(this), packagesWrapper(this), repositories(this) {}

};


#endif //ANTARCTICA_APIWRAPPER_H
