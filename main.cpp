#include <QApplication>
#include "LoginWindow.h"
#include "MainWindow.h"

int main(int argc, char** argv) {
    QApplication app(argc, argv);
    QApplication::setQuitOnLastWindowClosed(false);
    LoginWindow loginWindow;
    MainWindow mainWindow;

    QObject::connect(&loginWindow, &LoginWindow::loggedIn, &QDialog::close);
    QObject::connect(&loginWindow, &LoginWindow::loggedIn, &mainWindow, &MainWindow::showUser);

    QObject::connect(&loginWindow, &LoginWindow::rejected, &app, &QCoreApplication::quit);
    QObject::connect(&app, &QApplication::lastWindowClosed, &app, &QCoreApplication::quit);

    loginWindow.show();
    return QApplication::exec();
}