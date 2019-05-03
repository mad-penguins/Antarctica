#include <QGridLayout>
#include <QLabel>
#include <QDebug>
#include <QMessageBox>

#include "LoginWindow.h"
#include "models/User.h"
#include "utils/LoginUtil.h"
#include "MainWindow.h"


LoginWindow::LoginWindow() {
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

