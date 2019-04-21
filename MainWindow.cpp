#include <utility>
#include <QVBoxLayout>
#include <QWidget>
#include <QUrl>
#include <QDebug>
#include <QFileInfo>
#include <QtSql/QSqlQuery>

#include "MainWindow.h"
#include "utils/DatabaseUtil.h"
#include "DAO/FileDAO.h"


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {

}


void MainWindow::initUI() {
    setWindowTitle(this->user.displayName + " - Antarctica");
    DatabaseUtil::initDatabase(user);
    FileDAO fileDao(user);
    for (const auto& file : fileDao.getAll()) {
        qDebug() << file->name << "at" << file->path;
    }
}

void MainWindow::showUser(User user) {
    this->user = std::move(user);
    this->initUI();
    this->show();
}
