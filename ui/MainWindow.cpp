/*!
 * \file ui/MainWindow.cpp
 * \author Nikita Mironov <nickfrom22nd@gmail.com>
 * \brief The main window implementation
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


#include <utility>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <QtCore/QUrl>
#include <QtCore/QDebug>
#include <QtCore/QFileInfo>

#include "MainWindow.h"
#include "../utils/api/APIWrapper.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {

}


void MainWindow::initUI() {
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

    APIWrapper::setUserData(user.id, user.accessToken);
            foreach(File *file, APIWrapper::Files::getAll()) {
            qDebug() << file->path + "/" + file->name + " from " + file->package->repository->name + "@" +
                        file->package->name;
        }
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

void MainWindow::closeEvent(QCloseEvent *event) {
    QMainWindow::closeEvent(event);

    if (event->isAccepted()) {
        emit closed();
    }
}
