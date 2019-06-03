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
#include <QMessageBox>

#include <api/Wrapper.h>

#include "MainWindow.h"
#include "utils/SSL.hpp"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {

}


void MainWindow::initUI() {
    setWindowTitle(this->user.displayName + " - Antarctica");

    setWindowIcon(QIcon(":/img/icon.png"));

    setGeometry(0,0,900, 500);

    QDesktopWidget dw;
    QRect rc = dw.screenGeometry(this);
    move((rc.width() - width()) / 2, (rc.height() - height()) / 2 - 20);

    initFiles();
    updateFiles();

    initPackages();
    updatePackages();

    hBoxLayout = new QHBoxLayout();
    hBoxLayout->setContentsMargins(0,0,0,0);

    createToolbars();
    auto *vBoxLayout = new QVBoxLayout;
    vBoxLayout->addWidget(toolBarUp);
    vBoxLayout->addStretch();
    vBoxLayout->addWidget(toolBarDown);
    vBoxLayout->setContentsMargins(0,0,0,0);
    hBoxLayout->addLayout(vBoxLayout);

    tabWidget = new QTabWidget;
    tabWidget->addTab(filesTree, "Files");
    tabWidget->addTab(packagesTree, "Packages");
    hBoxLayout->addWidget(tabWidget);

    hBoxLayout->setSpacing(0);

    auto *ui_area = new QWidget;
    ui_area->setLayout(hBoxLayout);
    this->setCentralWidget(ui_area);

    for (auto &&file : Wrapper::Files::getAll()) {
        qDebug() << file->path + "/" + file->name + " from " + file->package->repository->name + "@" +
                    file->package->name;
        file->name += "____";
        Wrapper::Files::update(file);
    }
}

void MainWindow::initFiles() {

}

void MainWindow::updateFiles() {
    filesTree = new QTableView;
}

void MainWindow::initPackages() {

}

void MainWindow::updatePackages() {
    packagesTree = new QTableView;
}

void MainWindow::showUser(User usr) {
    this->user = std::move(usr);
    this->initUI();
    this->show();
}

void MainWindow::createToolbars() {
    toolBarUp = new QToolBar;
    toolBarUp->setStyleSheet("QToolButton\n"
                           "    {\n"
                           "        height: 48px;\n"
                           "        width: 48px;\n"
                           "    }");
    toolBarUp->setIconSize(QSize(50,50));
    toolBarUp->setOrientation(Qt::Vertical);
    toolBarUp->addAction(QIcon(":/img/add_green.png"), "test", this, SLOT(testSlot()));
    toolBarUp->addAction(QIcon(":/img/remove_green.png"), "test", this, SLOT(testSlot()));
    toolBarUp->addAction(QIcon(":/img/update_green.png"), "test", this, SLOT(testSlot()));

    toolBarDown = new QToolBar;
    toolBarDown->setStyleSheet("QToolButton\n"
                             "    {\n"
                             "        height: 48px;\n"
                             "        width: 48px;\n"
                             "    }");
    toolBarDown->setIconSize(QSize(50,50));
    toolBarDown->setOrientation(Qt::Vertical);
    toolBarDown->addSeparator();
    toolBarDown->addAction(QIcon(":/img/menu_green.png"), "test", this, SLOT(testSlot()));
    toolBarDown->addAction(QIcon(":/img/settings_green.png"), "test", this, SLOT(testSlot()));
}

void MainWindow::testSlot() {
    QMessageBox::information(0, "Message", "Not implemented");
}
