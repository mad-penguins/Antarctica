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
 * The API wrapper utilities implementation
 */

#include <QtCore/QUrl>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QtCore/QJsonDocument>
#include <QtCore/QCoreApplication>

#include "APIWrapper.h"

unsigned APIWrapper::userId = 0;
QString APIWrapper::accessToken = "";

// init specified static fields
template<> QString APIWrapper::Section<File>::prefix = "file";
template<> QString APIWrapper::Section<Package>::prefix = "pkg";
template<> QString APIWrapper::Section<Repository>::prefix = "repo";

template<class Entity>
QList<Entity *> APIWrapper::Section<Entity>::getAll() {
    auto getUrl = QUrl(
            QString("http://antarctica-server.tk/api/user/%1/%2s/%3").arg(
                    QString::number(userId),
                    prefix,
                    accessToken)
    );
    auto json = Utils::execute(getUrl, Utils::GET);

    auto respJson = json.array();
    QList<Entity *> objects;
            foreach (QJsonValue val, respJson) {
            if (val.isObject()) {
                auto fileJson = val.toObject();
                objects << new Entity(fileJson);
            }
        }

    return objects;
}

template<class Entity>
Entity *APIWrapper::Section<Entity>::get(unsigned id) {
    auto getUrl = QUrl(
            QString("http://antarctica-server.tk/api/user/%1/%2/%3/%4").arg(
                    QString::number(userId),
                    prefix,
                    QString::number(id),
                    accessToken
            )
    );
    auto json = Utils::execute(getUrl, Utils::GET);

    auto respJson = json.object();
    return new Entity(respJson);
}

template<class Entity>
bool APIWrapper::Section<Entity>::upload(const Entity &entity) { // TODO: implement
    return false;
}

template<class Entity>
bool APIWrapper::Section<Entity>::update(const Entity &entity) { // TODO: implement
    return false;
}

template<class Entity>
bool APIWrapper::Section<Entity>::remove(unsigned id) {
    auto deleteFileUrl = QUrl(
            QString("http://antarctica-server.tk/api/user/%1/%2/%3/%4").arg(
                    QString::number(userId),
                    prefix,
                    QString::number(id),
                    accessToken
            )
    );
    auto json = Utils::execute(deleteFileUrl, Utils::DELETE);

    auto respJson = json.object();
    return true; // TODO: implement return status on the server
}

QJsonDocument APIWrapper::Utils::execute(const QUrl &requestUrl, RequestType type) {
    qDebug() << "Executing " + requestUrl.toString();

    auto manager = new QNetworkAccessManager;

    QNetworkRequest request(requestUrl);
    QNetworkReply *reply;
    switch (type) {
        case GET:
            reply = manager->get(request);
            break;
        case DELETE:
            reply = manager->deleteResource(request);
            break;
        default:
            return QJsonDocument();
    }
    while (!reply->isFinished()) { // make thread not blocked by waiting for response
        qApp->processEvents();
    }

    QByteArray buffer = reply->readAll();
    auto json = QJsonDocument::fromJson(buffer);
    reply->deleteLater();
    return json;
}

QJsonDocument
APIWrapper::Utils::executeForm(const QUrl &requestUrl, QHttpMultiPart *formData, APIWrapper::Utils::RequestType type) {
    qDebug() << "Executing " + requestUrl.toString();

    auto manager = new QNetworkAccessManager;

    QNetworkRequest request(requestUrl);
    QNetworkReply *reply;
    switch (type) {
        case POST:
            reply = manager->post(request, formData);
            break;
        case PUT:
            reply = manager->put(request, formData);
            break;
        default:
            return QJsonDocument();
    }
    while (!reply->isFinished()) { // make thread not blocked by waiting for response
        qApp->processEvents();
    }

    QByteArray buffer = reply->readAll();
    auto json = QJsonDocument::fromJson(buffer);
    reply->deleteLater();
    return json;
}

// tell the compiler to "implement" methods from super class
template
class APIWrapper::Section<File>;

template
class APIWrapper::Section<Package>;

template
class APIWrapper::Section<Repository>;
