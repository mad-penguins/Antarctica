/*!
 * \file ui/LoginWindow.h
 * \author Nikita Mironov <nickfrom22nd@gmail.com>
 * \brief The login window class
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

#ifndef ANTARCTICA_LOGINWINDOW_H
#define ANTARCTICA_LOGINWINDOW_H


#include <QtWidgets/QDialog>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

#include "../models/User.h"

class LoginWindow : public QDialog {
Q_OBJECT

public:
    LoginWindow();

private:
    QLineEdit *loginField;
    QLineEdit *passwordField;

    QPushButton *registerButton;
    QPushButton *logInButton;

    bool isLodggedIn;

    void initUI();

signals:

    void loggedIn(User user);
    void closedWithoutLogin();

public slots:

    void logInClicked();

    void registerClicked();

protected:
    void closeEvent(QCloseEvent *event) override;

};


#endif //ANTARCTICA_LOGINWINDOW_H
