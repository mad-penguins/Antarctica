/*!
 * \file utils/api/APIWrapper.cpp
 * \author Nikita Mironov <nickfrom22nd@gmail.com>
 * \brief The API wrapper implementation
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


#include <QtCore/QUrl>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QtCore/QJsonDocument>
#include <QtCore/QCoreApplication>
#include <QtCore/QUrlQuery>
#include <QtCore/QMimeDatabase>

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

template<>
bool APIWrapper::Section<File>::upload(const File *file) {
    auto uploadUrl = QUrl(
            QString("http://antarctica-server.tk/api/user/%1/%2s/%3").arg(
                    QString::number(userId),
                    prefix,
                    accessToken
            )
    );
    auto multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

    QHttpPart contentPart;
    contentPart.setHeader(
            QNetworkRequest::ContentDispositionHeader,
            QString(
                    R"(form-data; name="%1"; path="%2"; created="%3"; modified="%4"; package_id="%5")").arg(
                            file->name,
                            file->path,
                            QString::number(file->created.toSecsSinceEpoch()),
                            QString::number(file->modified.toSecsSinceEpoch()),
                            QString::number(file->package->id)
                            )
                    );
    contentPart.setHeader(QNetworkRequest::ContentTypeHeader, QMimeDatabase().mimeTypeForData(file->content).name());
    contentPart.setBody(file->content);
    multiPart->append(contentPart);

    auto json = Utils::executeForm(uploadUrl, multiPart, Utils::POST);
    auto respJson = json.object();
    return true; // TODO: implement return status on the server
}

template<>
bool APIWrapper::Section<Package>::upload(const Package *pkg) { // TODO: implement
    auto updateUrl = QUrl(
            QString("http://antarctica-server.tk/api/user/%1/%2s/%3").arg(
                    QString::number(userId),
                    prefix,
                    accessToken
            )
    );

    QUrlQuery formData;
    formData.addQueryItem("id", QString::number(pkg->id));
    formData.addQueryItem("name", pkg->name);
    formData.addQueryItem("repo_id", QString::number(pkg->repository->id));

    auto json = Utils::executeForm(updateUrl, formData, Utils::POST);
    auto respJson = json.object();
    return true; // TODO: implement return status on the server
}

template<>
bool APIWrapper::Section<Repository>::upload(const Repository *repo) {
    auto updateUrl = QUrl(
            QString("http://antarctica-server.tk/api/user/%1/%2s/%3").arg(
                    QString::number(userId),
                    prefix,
                    accessToken
            )
    );

    QUrlQuery formData;
    formData.addQueryItem("id", QString::number(repo->id));
    formData.addQueryItem("name", repo->name);
    formData.addQueryItem("url", repo->url);
    formData.addQueryItem("manager", repo->manager);

    auto json = Utils::executeForm(updateUrl, formData, Utils::POST);
    auto respJson = json.object();
    return true; // TODO: implement return status on the server
}

template<>
bool APIWrapper::Section<File>::update(const File *file) {
    auto uploadUrl = QUrl(
            QString("http://antarctica-server.tk/api/user/%1/%2s/%3").arg(
                    QString::number(userId),
                    prefix,
                    accessToken
            )
    );
    auto multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

    QHttpPart contentPart;
    contentPart.setHeader(
            QNetworkRequest::ContentDispositionHeader,
            QString(
                    R"(form-data; name="%1"; path="%2"; created="%3"; modified="%4"; package_id="%5")").arg(
                    file->name,
                    file->path,
                    QString::number(file->created.toSecsSinceEpoch()),
                    QString::number(file->modified.toSecsSinceEpoch()),
                    QString::number(file->package->id)
            )
    );
    contentPart.setHeader(QNetworkRequest::ContentTypeHeader, QMimeDatabase().mimeTypeForData(file->content).name());
    contentPart.setBody(file->content);
    multiPart->append(contentPart);

    auto json = Utils::executeForm(uploadUrl, multiPart, Utils::PUT);
    auto respJson = json.object();
    return true; // TODO: implement return status on the server
}

template<>
bool APIWrapper::Section<Package>::update(const Package *pkg) {
    auto updateUrl = QUrl(
            QString("http://antarctica-server.tk/api/user/%1/%2s/%3").arg(
                    QString::number(userId),
                    prefix,
                    accessToken
            )
    );

    QUrlQuery formData;
    formData.addQueryItem("name", pkg->name);
    formData.addQueryItem("repo_id", QString::number(pkg->repository->id));

    auto json = Utils::executeForm(updateUrl, formData, Utils::PUT);
    auto respJson = json.object();
    return true; // TODO: implement return status on the server
}

template<>
bool APIWrapper::Section<Repository>::update(const Repository *repo) { // TODO: implement
    auto updateUrl = QUrl(
            QString("http://antarctica-server.tk/api/user/%1/%2s/%3").arg(
                    QString::number(userId),
                    prefix,
                    accessToken
            )
    );

    QUrlQuery formData;
    formData.addQueryItem("name", repo->name);
    formData.addQueryItem("url", repo->url);
    formData.addQueryItem("manager", repo->manager);

    auto json = Utils::executeForm(updateUrl, formData, Utils::PUT);
    auto respJson = json.object();
    return true; // TODO: implement return status on the server
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
    qDebug() << buffer;
    auto json = QJsonDocument::fromJson(buffer);
    reply->deleteLater();
    return json;
}

QJsonDocument
APIWrapper::Utils::executeForm(const QUrl &requestUrl, QUrlQuery &formData, APIWrapper::Utils::RequestType type) {
    qDebug() << "Executing " + requestUrl.toString();

    auto manager = new QNetworkAccessManager;

    QNetworkRequest request(requestUrl);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    QNetworkReply *reply;
    switch (type) {
        case POST:
            reply = manager->post(request, formData.toString(QUrl::FullyEncoded).toUtf8());
            break;
        case PUT:
            reply = manager->put(request, formData.toString(QUrl::FullyEncoded).toUtf8());
            break;
        default:
            return QJsonDocument();
    }
    while (!reply->isFinished()) { // make thread not blocked by waiting for response
        qApp->processEvents();
    }

    QByteArray buffer = reply->readAll();
    qDebug() << buffer;
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
