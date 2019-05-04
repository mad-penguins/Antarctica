#ifndef ANTARCTICA_LOGINWINDOW_H
#define ANTARCTICA_LOGINWINDOW_H


#include <QtWidgets/QWidget>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include "models/User.h"

class LoginWindow : public QWidget {
Q_OBJECT

public:
    LoginWindow();

private:
    QLineEdit *loginField;
    QLineEdit *passwordField;

    QPushButton *registerButton;
    QPushButton *logInButton;

    QGridLayout *layout;
    QLabel *loginLabel;
    QLabel *passwordLabel;

    void initUI();
    void createElements();
    void placeElements();

signals:

    void loggedIn(User user);

public slots:

    void logInClicked();

    void registerClicked();

};


#endif //ANTARCTICA_LOGINWINDOW_H
