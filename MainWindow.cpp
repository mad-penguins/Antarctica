#include <utility>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <QtCore/QUrl>
#include <QtCore/QDebug>
#include <QtCore/QFileInfo>

#include "MainWindow.h"


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {

}


void MainWindow::initUI() {
    setWindowTitle(this->user.displayName + " - Antarctica");

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
    /*filesModel = new QAbstractTableModel(this);
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
    }*/
}

void MainWindow::updateFiles() {
    filesTree = new QTableView;
    /*filesTree->setModel(filesModel);
    filesTree->setItemDelegate(new QSqlRelationalDelegate(filesTree));
    filesTree->setColumnHidden(0, true);
    filesTree->setColumnHidden(3, true);
    for (uint i = 0; i < filesModel->columnCount(); ++i) {
        filesTree->resizeColumnToContents(i);
    }
    filesTree->setEditTriggers(QAbstractItemView::NoEditTriggers);*/
}

void MainWindow::initPackages() {
    /*packagesModel = new QTableModel(this);
    auto headerData = QStringList()
            << "id"
            << "Name"
            << "Repository";

    for (uint i = 0; i < packagesModel->columnCount(); i++) {
        packagesModel->setHeaderData(i, Qt::Horizontal, headerData[i]);
    }*/
}

void MainWindow::updatePackages() {
    packagesTree = new QTableView;
    /*packagesTree->setModel(packagesModel);
    packagesTree->setItemDelegate(new QSqlRelationalDelegate(packagesTree));
    packagesTree->setColumnHidden(0, true);
    packagesTree->setRowHidden(0, packagesModel->index(0, 0).parent(), true);
    for (uint i = 0; i < packagesModel->columnCount(); ++i) {
        packagesTree->resizeColumnToContents(i);
    }
    packagesTree->setEditTriggers(QAbstractItemView::NoEditTriggers);*/
}

void MainWindow::showUser(User user) {
    this->user = std::move(user);
    this->initUI();
    this->show();
}
