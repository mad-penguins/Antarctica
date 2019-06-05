/*!
 * \file ui/LoginWindow.cpp
 * \author Nikita Mironov <nickfrom22nd@gmail.com>
 * \brief The login window implementation
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


#include <QGridLayout>
#include <QLabel>
#include <QDebug>
#include <QMessageBox>
#include <QDesktopServices>
#include <QDesktopWidget>

#include <api/Wrapper.h>

#include "LoginWindow.h"
#include "utils/SSL.hpp"
#include "MainWindow.h"


LoginWindow::LoginWindow() {
    Wrapper::init(Utils::SSL::getConfig());
    initUI();
}

void LoginWindow::initUI() {
    this->setWindowTitle("Antarctica login");

    this->setWindowIcon(QIcon(":/img/icon.png"));

    setGeometry(0, 0, 320, 500);

    QDesktopWidget dw;
    QRect rc = dw.screenGeometry(this);
    move((rc.width() - width()) / 2, (rc.height() - height()) / 2 - 20);

    auto layout = new QGridLayout;

    logoLabel = new QLabel();
    logoLabel->setAlignment(Qt::AlignCenter);
    logoLabel->setPixmap(QPixmap(":/img/logo.png").scaledToHeight(static_cast<int>(this->height() / 1.5), Qt::SmoothTransformation));
    layout->addWidget(logoLabel, 1, 1, 1, 2);

    auto loginLabel = new QLabel("Username");
    auto passwordLabel = new QLabel("Password");
    layout->addWidget(loginLabel, 3, 1);
    layout->addWidget(passwordLabel, 4, 1);
    layout->setAlignment(loginLabel, Qt::AlignHCenter);
    layout->setAlignment(passwordLabel, Qt::AlignHCenter);

    loginField = new QLineEdit;
    passwordField = new QLineEdit;
    passwordField->setEchoMode(QLineEdit::Password);
    layout->addWidget(loginField, 3, 2);
    layout->addWidget(passwordField, 4, 2);

    registerButton = new QPushButton("Register");
    logInButton = new QPushButton("Log in");
    logInButton->setDefault(true);
    layout->addWidget(registerButton, 5, 1);
    layout->addWidget(logInButton, 5, 2);

    connect(registerButton, &QPushButton::clicked, this, &LoginWindow::registerClicked);
    connect(logInButton, &QPushButton::clicked, this, &LoginWindow::logInClicked);

    layout->setRowStretch(0, 2);
    layout->setRowStretch(2, 1);
    layout->setRowStretch(6, 2);

    layout->setColumnStretch(0, 1);
    layout->setColumnStretch(1, 1);
    layout->setColumnStretch(2, 2);
    layout->setColumnStretch(3, 1);

    this->setLayout(layout);
}

void LoginWindow::registerClicked() {
    QDesktopServices::openUrl(QUrl("https://antarctica-server.tk/register"));
}

void LoginWindow::logInClicked() {
    try {
        auto user = Wrapper::authorize(loginField->text(), passwordField->text());
        emit loggedIn(user);
        this->close();
    } catch (Response::Exception &e) {
        QString text;
        switch (e.code) {
            case Response::Error::WrongLogin:
                text = "Wrong login or password";
                break;
            case Response::Error::MissingFields:
                text = "Wrong server response";
                break;
            default:
                break;
        }
        QMessageBox::critical(this, "Login error", text);
    }
}

void LoginWindow::resizeEvent(QResizeEvent *e) {
    this->logoLabel->setPixmap(QPixmap(":/img/logo.png").scaledToHeight(static_cast<int>(e->size().height() / 1.5), Qt::SmoothTransformation));
}

void LoginWindow::keyReleaseEvent(QKeyEvent *e) {
    if (e->key() == Qt::Key_Enter || e->key() == Qt::Key_Return) {
        logInClicked();
    }
}
