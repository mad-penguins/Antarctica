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
 * The files API section implementation
 */

#include <QtCore/QUrl>
#include <QtCore/QUrlQuery>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QtCore/QCoreApplication>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonArray>
#include <QtCore/QDebug>

#include "APIWrapper.h"


QList<File *> APIWrapper::Files::getAll() {
    auto getFilesUrl = QUrl(
            QString("http://antarctica-server.tk/api/user/%1/files/%2").arg(
                    QString::number(userId),
                    accessToken)
                    );
    auto json = Utils::execute(getFilesUrl, Utils::GET);

    auto filesJson = json.array();
    QList<File *> files;
    foreach (QJsonValue val, filesJson) {
        if (val.isObject()) {
            auto fileJson = val.toObject();
            files << new File(fileJson);
        }
    }

    return files;
}

File *APIWrapper::Files::get(unsigned id) {
    auto getFilesUrl = QUrl(
            QString("http://antarctica-server.tk/api/user/%1/file/%2/%3").arg(
                    QString::number(userId),
                    QString::number(id),
                    accessToken
                    )
            );
    auto json = Utils::execute(getFilesUrl, Utils::GET);

    auto fileJson = json.object();
    return new File(fileJson);
}

bool APIWrapper::Files::upload(const File &) {
    return false;
}

bool APIWrapper::Files::update(const File &) {
    return false;
}

bool APIWrapper::Files::remove(unsigned id) {
    auto deleteFileUrl = QUrl(
            QString("http://antarctica-server.tk/api/user/%1/file/%2/%3").arg(
                    QString::number(userId),
                    QString::number(id),
                    accessToken
            )
    );
    auto json = Utils::execute(deleteFileUrl, Utils::DELETE);

    auto fileJson = json.object();
    return true;
}
