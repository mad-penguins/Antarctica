#include <QUrl>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QApplication>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>
#include <QtCore/QUrlQuery>
#include "LoginUtil.h"


const User LoginUtil::logIn(const QString &login, const QString &password) noexcept(false) {
    qDebug() << "logging into " + login + " " + password;

    auto loginUrl = QUrl("http://165.22.64.118:80/api/login");

    QUrlQuery postData;
    postData.addQueryItem("login", login);
    postData.addQueryItem("password", password);

    auto manager = new QNetworkAccessManager;

    QNetworkRequest request(loginUrl);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    auto reply = manager->post(request, postData.toString(QUrl::FullyEncoded).toUtf8());
    while (!reply->isFinished()) {
        qApp->processEvents();
    }

    QByteArray buffer = reply->readAll();
    auto json = QJsonDocument::fromJson(buffer);
    reply->deleteLater();

    QJsonObject jsonReply = json.object();
    if (jsonReply.contains("error")) {
        switch (jsonReply["error"].toString().toInt()) {
            case -1: // WTF?
            case -2:
                throw LoginException(LoginException::WRONG_LOGIN_DATA);
            default:
                break;
        }
    }

    if (!jsonReply.keys().contains("id") && !jsonReply.keys().contains("login") && !jsonReply.keys().contains("name")) {
        throw LoginException(LoginException::Kind::WRONG_RESPONSE);
    }

    return User(jsonReply["id"].toInt(), login, password, jsonReply["name"].toString());
}
