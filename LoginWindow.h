#ifndef ANTARCTICA_LOGINWINDOW_H
#define ANTARCTICA_LOGINWINDOW_H


#include <QtWidgets/QDialog>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

#include "models/User.h"

class LoginWindow : public QDialog {
Q_OBJECT

public:
    LoginWindow();

private:
    QLineEdit *loginField;
    QLineEdit *passwordField;

    QPushButton *registerButton;
    QPushButton *logInButton;

    void initUI();

signals:

    void loggedIn(User user);

public slots:

    void logInClicked();

    void registerClicked();

};


#endif //ANTARCTICA_LOGINWINDOW_H
