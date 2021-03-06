/*!
 * \file
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


LoginWindow::LoginWindow(bool local) {
    Wrapper::init(Utils::SSL::getConfig(), local);
    initUI();
}

void LoginWindow::initUI() {
    setWindowTitle("Antarctica | Sign In");
    setWindowIcon(QIcon(":/img/icon.png"));

    setGeometry(0, 0, 320, 500);

    QDesktopWidget dw;
    QRect rc = dw.screenGeometry(this);
    move((rc.width() - width()) / 2, (rc.height() - height()) / 2 - 20);

    auto mainLay = new QVBoxLayout(this);
    auto layout = new QGridLayout(this);
    auto layoutButton = new QGridLayout(this);

    logoLabel = new QLabel(this);
    logoLabel->setAlignment(Qt::AlignCenter);
    logoLabel->setPixmap(
            QPixmap(":/img/logo.png").scaledToHeight(static_cast<int>(height() / 1.5), Qt::SmoothTransformation));
    layout->addWidget(logoLabel, 1, 1, 1, 2);

    auto loginLabel = new QLabel("Username", this);
    auto passwordLabel = new QLabel("Password", this);
    layout->addWidget(loginLabel, 3, 1);
    layout->addWidget(passwordLabel, 4, 1);
    layout->setAlignment(loginLabel, Qt::AlignHCenter);
    layout->setAlignment(passwordLabel, Qt::AlignHCenter);

    loginField = new QLineEdit(this);
    passwordField = new QLineEdit(this);
    passwordField->setEchoMode(QLineEdit::Password);
    layout->addWidget(loginField, 3, 2);
    layout->addWidget(passwordField, 4, 2);

    registerButton = new QPushButton("Sign Up", this);
    registerButton->setFlat(true);
    registerButton->setStyleSheet("QPushButton{"
                                  "text-decoration: underline;"
                                  "color: #00c7be"
                                  "}" );

    logInButton = new QPushButton("Sign In", this);
    logInButton->setStyleSheet("QPushButton{"
                               "background-color: #004fc5;"
                               "}" );

    layoutButton->addWidget(logInButton, 1, 1);
    layoutButton->addWidget(registerButton, 1, 2);
    layout->addLayout(layoutButton, 5, 2);

    auto line = new QFrame();
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);

    auto copyrightLabel = new QLabel(this);
    copyrightLabel->setAlignment(Qt::AlignCenter);
    copyrightLabel->setText("<a href=\\\"http://mad-penguins.tk\\\" "
                          "style=\"color: rgba(90, 200, 255, 50%);"
                          "font-size: 10px;"
                          "\">"
                          "(C) 2020 Mad-Penguins</a>");

    connect(copyrightLabel, &QLabel::linkActivated, this, &LoginWindow::openMadPenguinsSite);
    connect(registerButton, &QPushButton::clicked, this, &LoginWindow::registerClicked);
    connect(logInButton, &QPushButton::clicked, this, &LoginWindow::logInClicked);

    layout->setRowStretch(0, 2);
    layout->setRowStretch(2, 1);
    layout->setRowStretch(6, 2);

    layout->setColumnStretch(0, 1);
    layout->setColumnStretch(1, 1);
    layout->setColumnStretch(2, 2);
    layout->setColumnStretch(3, 1);

    mainLay->addLayout(layout);
    mainLay->addSpacing(10);
    mainLay->addWidget(line);
    mainLay->addWidget(copyrightLabel);
    setLayout(mainLay);
}

void LoginWindow::registerClicked() {
    QDesktopServices::openUrl(QUrl("https://antarctica-server.tk/register"));
}

void LoginWindow::openMadPenguinsSite() {
    QDesktopServices::openUrl(QUrl("http://mad-penguins.tk"));
}

void LoginWindow::logInClicked() {
    try {
        auto user = Wrapper::authorize(loginField->text(), passwordField->text());
        emit loggedIn(user);
        close();
    } catch (Response::Exception &e) {
        QString text;
        switch (e.code) {
            case Response::Error::Code::WrongLogin:
                text = "Wrong login or password";
                break;
            case Response::Error::Code::NoResponse:
            case Response::Error::Code::MissingFields:
                text = "Wrong server response";
                break;
            default:
                break;
        }
        QMessageBox::critical(this, "Login error", text);
    }
}

void LoginWindow::resizeEvent(QResizeEvent *e) {
    logoLabel->setPixmap(QPixmap(":/img/logo.png").scaledToHeight(static_cast<int>(e->size().height() / 1.5),
                                                                  Qt::SmoothTransformation));
}

void LoginWindow::keyReleaseEvent(QKeyEvent *e) {
    if (e->key() == Qt::Key_Enter || e->key() == Qt::Key_Return) {
        logInClicked();
    }
}
