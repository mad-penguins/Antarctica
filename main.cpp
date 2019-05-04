#include <QApplication>
#include "LoginWindow.h"
#include "MainWindow.h"

int main(int argc, char **argv) {
    QApplication app(argc, argv);
    LoginWindow loginWindow;
    MainWindow mainWindow;

    QObject::connect(&loginWindow, &LoginWindow::loggedIn, &mainWindow, &MainWindow::showUser);
    QObject::connect(&loginWindow, &LoginWindow::loggedIn, &QWidget::close);

    QObject::connect(&app, &QApplication::lastWindowClosed, &app, &QCoreApplication::quit);

    loginWindow.show();
    return QApplication::exec();
}