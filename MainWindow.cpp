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

    initPackages();
    updatePackages();

    tabWidget = new QTabWidget;
    tabWidget->addTab(filesTree, "Files");
    tabWidget->addTab(packagesTree, "Packages");
    this->setCentralWidget(tabWidget);
}

void MainWindow::initFiles() {
    filesModel = new QSqlRelationalTableModel(this);
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

    filesTree = new QTreeView;
    filesTree->setModel(filesModel);
    filesTree->setItemDelegate(new QSqlRelationalDelegate(filesTree));
    filesTree->setColumnHidden(0, true);
    filesTree->setColumnHidden(3, true);
    for (uint i = 0; i < filesModel->columnCount(); ++i) {
        filesTree->resizeColumnToContents(i);
    }
    filesTree->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void MainWindow::initPackages() {
    packagesModel = new QSqlRelationalTableModel(this);
    packagesModel->setTable("packages");
    packagesModel->setRelation(2, QSqlRelation("repositories", "id", "name"));
    auto headerData = QStringList()
            << "id"
            << "Name"
            << "Repository";

    for (uint i = 0; i < packagesModel->columnCount(); i++) {
        packagesModel->setHeaderData(i, Qt::Horizontal, headerData[i]);
    }
    packagesModel->setSort(0, Qt::AscendingOrder);
}

void MainWindow::updatePackages() {
    packagesModel->select();

    packagesTree = new QTreeView;
    packagesTree->setModel(packagesModel);
    packagesTree->setItemDelegate(new QSqlRelationalDelegate(packagesTree));
    packagesTree->setColumnHidden(0, true);
    packagesTree->setRowHidden(0, packagesModel->index(0, 0).parent(), true);
    for (uint i = 0; i < packagesModel->columnCount(); ++i) {
        packagesTree->resizeColumnToContents(i);
    }
    packagesTree->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void MainWindow::showUser(User user) {
    this->user = std::move(user);
    this->initUI();
    this->show();
}
