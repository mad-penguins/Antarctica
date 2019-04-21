#include <QVBoxLayout>
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

    auto labelsLay = new QVBoxLayout;
    auto loginLabel = new QLabel("Username");
    auto passwordLabel = new QLabel("Password");
    labelsLay->addWidget(loginLabel);
    labelsLay->addWidget(passwordLabel);
    labelsLay->setSpacing(10);

    auto fieldsLay = new QVBoxLayout;
    loginField = new QLineEdit;
    loginField->setFixedWidth(200);
    passwordField = new QLineEdit;
    passwordField->setEchoMode(QLineEdit::Password);
    fieldsLay->addWidget(loginField);
    fieldsLay->addWidget(passwordField);
    fieldsLay->setSpacing(10);

    auto formLay = new QHBoxLayout;
    formLay->addStretch();
    formLay->addLayout(labelsLay);
    formLay->addLayout(fieldsLay);
    formLay->addStretch();
    formLay->setSpacing(10);

    auto buttonsLay = new QHBoxLayout;

    registerButton = new QPushButton("Register");
    connect(registerButton, &QPushButton::clicked, this, &LoginWindow::registerClicked);
    logInButton = new QPushButton("Log in");
    logInButton->setDefault(true);
    connect(logInButton, &QPushButton::clicked, this, &LoginWindow::logInClicked);

    buttonsLay->addStretch();
    buttonsLay->addWidget(registerButton);
    buttonsLay->addWidget(logInButton);


    auto mainLay = new QVBoxLayout;
    mainLay->addStretch();
    mainLay->addLayout(formLay);
    mainLay->addLayout(buttonsLay);
    mainLay->addStretch();
    mainLay->setSizeConstraint(QLayout::SetFixedSize);

    this->setLayout(mainLay);
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
            case LoginException::WRONG_LOGIN:
                text = "Wrong login";
                break;
            case LoginException::WRONG_PASSWORD:
                text = "Wrong pasword";
                break;
            case LoginException::WRONG_RESPONSE:
                text = "Wrong response from login server";
                break;
            case LoginException::UNKNOWN_ERROR:
                text = "Unknown error";
                break;
        }
        QMessageBox::critical(this, "LoginUtil error", text);
    }
}
