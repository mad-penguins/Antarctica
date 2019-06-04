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
#include "ui/models/files/FileTreeModel.h"
#include "ui/models/packages/PackageTreeModel.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {

}


void MainWindow::initUI() {
    setWindowTitle(this->user.displayName + " - Antarctica");

    setWindowIcon(QIcon(":/img/icon.png"));
    moveToCenter();

    filesTree = new QTreeView;
    packagesTree = new QTreeView;

    hBoxLayout = new QHBoxLayout();
    hBoxLayout->setContentsMargins(0, 0, 0, 0);

    createToolbars();
    auto *vBoxLayout = new QVBoxLayout;
    vBoxLayout->addWidget(toolBarUp);
    vBoxLayout->addStretch();
    vBoxLayout->addWidget(toolBarDown);
    vBoxLayout->setContentsMargins(0, 0, 0, 0);
    hBoxLayout->addLayout(vBoxLayout);

    tabWidget = new QTabWidget;
    tabWidget->addTab(filesTree, "Files");
    tabWidget->addTab(packagesTree, "Packages");

    hBoxLayout->addWidget(tabWidget);
    hBoxLayout->setSpacing(0);

    auto *ui_area = new QWidget;
    ui_area->setLayout(hBoxLayout);
    this->setCentralWidget(ui_area);

    updateFiles();
    updatePackages();
}

void MainWindow::updateFiles() {
    QStringList headers;
    headers << tr("Name") << tr("Created") << tr("Modified");
    auto model = new FileTreeModel(headers, Wrapper::Files::getAll());
    filesTree->setModel(model);
    connect(filesTree, &QTreeView::expanded, [=]() {
        filesTree->resizeColumnToContents(0);
    });
    filesTree->expandToDepth(1);
}

void MainWindow::updatePackages() {
    QStringList headers;
    headers << tr("Name");
    auto model = new PackageTreeModel(headers, Wrapper::Packages::getAll());
    packagesTree->setModel(model);
    packagesTree->expandAll();
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

void MainWindow::createToolbars() {
    const QString style = "QToolButton { height: 30px; width: 30px; }";

    toolBarUp = new QToolBar;
    toolBarUp->setStyleSheet(style);
    toolBarUp->setOrientation(Qt::Vertical);
    toolBarUp->addAction(QIcon::fromTheme("list-add"), "Add", this, SLOT(testSlot()));
    toolBarUp->addAction(QIcon::fromTheme("list-remove"), "Remove", this, SLOT(testSlot()));
    toolBarUp->addAction(QIcon::fromTheme("view-refresh"), "Refresh", this, SLOT(testSlot()));

    toolBarDown = new QToolBar;
    toolBarDown->setStyleSheet(style);
    toolBarDown->setOrientation(Qt::Vertical);
    toolBarDown->addSeparator();
    toolBarDown->addAction(QIcon::fromTheme("preferences-other"), "Other", this, SLOT(testSlot()));
    toolBarDown->addAction(QIcon::fromTheme("preferences-system"), "Settings", this, SLOT(testSlot()));
}

void MainWindow::testSlot() {
    QMessageBox::information(nullptr, "Message", "Not implemented");
}
