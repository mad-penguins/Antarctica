#include <QApplication>
#include "ui/LoginWindow.h"
#include "ui/MainWindow.h"

int main(int argc, char **argv) {
    QApplication app(argc, argv);
    QApplication::setQuitOnLastWindowClosed(false);
    LoginWindow loginWindow;
    MainWindow mainWindow;

    QObject::connect(&loginWindow, &LoginWindow::loggedIn,
                     &QDialog::close);
    QObject::connect(&loginWindow, &LoginWindow::loggedIn, &mainWindow, &MainWindow::showUser);

    QObject::connect(&loginWindow, &LoginWindow::closedWithoutLogin, &app, &QCoreApplication::quit);
    QObject::connect(&mainWindow, &MainWindow::closed, &app, &QCoreApplication::quit);

    loginWindow.show();
    return QApplication::exec();
}