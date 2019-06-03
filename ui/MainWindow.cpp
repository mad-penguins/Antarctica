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
#include <QDesktopWidget>
#include <api/Wrapper.h>
#include <ui/models/FileTreeModel.h>

#include "MainWindow.h"
#include "utils/SSL.hpp"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {

}


void MainWindow::initUI() {
    setWindowTitle(this->user.displayName + " - Antarctica");
    setWindowIcon(QIcon(QPixmap(":/img/icon.png")));
    moveToCenter();

    filesTree = new QTreeView;
    packagesTree = new QTreeView;

    tabWidget = new QTabWidget;
    tabWidget->addTab(filesTree, "Files");
    tabWidget->addTab(packagesTree, "Packages");
    setCentralWidget(tabWidget);

    updateFiles();
    updatePackages();
}

void MainWindow::updateFiles() {
    QStringList headers;
    headers << tr("Name");
    FileTreeModel *model = new FileTreeModel(headers, Wrapper::Files::getAll());
    filesTree->setModel(model);
}

void MainWindow::updatePackages() {

}

void MainWindow::showUser(User usr) {
    this->user = std::move(usr);
    this->initUI();
    this->show();
}

void MainWindow::moveToCenter() {
    setGeometry(0, 0, 900, 500);

    QDesktopWidget dw;
    QRect rc = dw.screenGeometry(this);
    move((rc.width() - width()) / 2, (rc.height() - height()) / 2 - 20);
}
