#include <utility>
#include <QVBoxLayout>
#include <QWidget>
#include <QUrl>
#include <QDebug>
#include <QFileInfo>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRelationalDelegate>

#include "MainWindow.h"
#include "utils/DatabaseUtil.h"
#include "DAO/FileDAO.h"


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {

}


void MainWindow::initUI() {
    DatabaseUtil::initDatabase(user);

    setWindowTitle(this->user.displayName + " - Antarctica");
    this->setFixedSize(600, 300);

    initFiles();
    updateFiles();
}

void MainWindow::initFiles() {
    filesModel = new QSqlRelationalTableModel;
    filesModel->setTable("files");
    filesModel->setRelation(6, QSqlRelation("packages", "id", "name"));
    auto headerData = QStringList()
            << "id"
            << "Name"
            << "Path"
            << "Content"
            << "Created"
            << "Modified"
            << "Package";

    for (uint i = 0; i < filesModel->columnCount(); i++) {
        filesModel->setHeaderData(i, Qt::Horizontal, headerData[i]);
    }
    filesModel->setSort(0, Qt::AscendingOrder);
}

void MainWindow::updateFiles() {
    filesModel->select();

    auto tree = new QTreeView;
    tree->setModel(filesModel);
    tree->setItemDelegate(new QSqlRelationalDelegate(tree));
    tree->setColumnHidden(0, true);
    tree->setColumnHidden(3, true);
    for (uint i = 0; i < filesModel->columnCount(); ++i) {
        tree->resizeColumnToContents(i);
    }
    tree->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->setCentralWidget(tree);
}

void MainWindow::showUser(User user) {
    this->user = std::move(user);
    this->initUI();
    this->show();
}
