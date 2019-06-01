/*!
 * \file util/LoginUtil.cpp
 * \author Nikita Mironov <nickfrom22nd@gmail.com>
 * \brief The login utility implementation
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

#include <QUrl>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QApplication>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>
#include <QtCore/QUrlQuery>
#include <QtWidgets/QMessageBox>
#include <QtNetwork/QSslSocket>

#include "LoginUtil.h"
#include "SSLUtil.hpp"


const User LoginUtil::logIn(const QString &login, const QString &password) noexcept(false) {
    qDebug() << "logging into " + login + " " + password;

    auto loginUrl = QUrl("https://antarctica-server.tk/api/login");

    QUrlQuery postData;
    postData.addQueryItem("login", login);
    postData.addQueryItem("password", password);

    auto manager = new QNetworkAccessManager;
    QNetworkRequest request(loginUrl);
    request.setSslConfiguration(SSLUtil::getSSLConfig());
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    auto reply = manager->post(request,
                               postData.toString(QUrl::FullyEncoded).toUtf8());
    while (!reply->isFinished()) {
        qApp->processEvents();
    }

    QByteArray buffer = reply->readAll();
    auto json = QJsonDocument::fromJson(buffer);
    reply->deleteLater();

    QJsonObject jsonReply = json.object();
    if (jsonReply.contains("error")) {
        switch (jsonReply["error"].toString().toInt()) {
            case -1:
            case -2:
                throw LoginException(LoginException::WRONG_LOGIN_DATA);
            default:
                break;
        }
    }

    if (!jsonReply.keys().contains("id")
        && !jsonReply.keys().contains("login")
        && !jsonReply.keys().contains("name")
        && !jsonReply.keys().contains("token")) {
        qDebug() << buffer;
        throw LoginException(LoginException::Kind::WRONG_RESPONSE);
    }

    return User(jsonReply);
}
