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

#include "LoginWindow.h"
#include "../models/User.h"
#include "../utils/LoginUtil.h"
#include "MainWindow.h"


LoginWindow::LoginWindow() {
    isLodggedIn = false;
    initUI();
}

void LoginWindow::initUI() {
    this->setWindowTitle("Antarctica login");

    auto layout = new QGridLayout;
    auto loginLabel = new QLabel("Username");
    auto passwordLabel = new QLabel("Password");

    loginField = new QLineEdit;
    passwordField = new QLineEdit;
    passwordField->setEchoMode(QLineEdit::Password);

    registerButton = new QPushButton("Register");
    logInButton = new QPushButton("Log in");
    logInButton->setDefault(true);

    connect(registerButton, &QPushButton::clicked, this, &LoginWindow::registerClicked);
    connect(logInButton, &QPushButton::clicked, this, &LoginWindow::logInClicked);

    layout->addWidget(registerButton, 3, 1);
    layout->addWidget(logInButton, 3, 2);
    layout->addWidget(loginLabel, 1, 1);
    layout->addWidget(passwordLabel, 2, 1);
    layout->addWidget(loginField, 1, 2);
    layout->addWidget(passwordField, 2, 2);

    this->setLayout(layout);
}

void LoginWindow::registerClicked() {

}

void LoginWindow::logInClicked() {
    try {
        User user = LoginUtil::logIn(loginField->text(), passwordField->text());
        isLodggedIn = true;
        emit loggedIn(user);
    } catch (LoginException &e) {
        QString text;
        switch (e.kind) {
            case LoginException::WRONG_LOGIN_DATA:
                text = "Wrong login or password";
                break;
            case LoginException::WRONG_RESPONSE:
                text = "Wrong response from login server";
                break;
            case LoginException::UNKNOWN_ERROR:
                text = "Something went wrong";
                break;
        }
        QMessageBox::critical(this, "Login error", text);
    }
}

void LoginWindow::closeEvent(QCloseEvent *event) {
    QDialog::closeEvent(event);

    if (event->isAccepted() && !isLodggedIn) {
        emit closedWithoutLogin();
    }
}
