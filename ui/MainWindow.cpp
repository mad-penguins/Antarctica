/*!
 * \file
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
#include <QtWidgets/QDesktopWidget>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QMenu>
#include <api/Wrapper.h>
#include <QtWidgets/QFileDialog>
#include <QtCore/QStandardPaths>
#include <QApplication>

#include "MainWindow.h"
#include "ui/models/files/FileTreeModel.h"
#include "ui/models/packages/PackageTreeModel.h"
#include "utils/Repositories.hpp"
#include "utils/Files.hpp"
#include "utils/UI.hpp"
#include "ui/packages/AddPackageDialog.h"
#include "utils/Settings.hpp"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {

}

void MainWindow::initUI() {
    setWindowTitle(user.displayName + " - Antarctica");
    setWindowIcon(QIcon(":/img/icon.png"));
    moveToCenter();

    // display all the widgets
    auto centralWidget = new QWidget(this);

    mainLay = new QHBoxLayout(centralWidget);
    mainLay->setContentsMargins(0, 0, 0, 0);
    mainLay->setSpacing(0);

    createTrayIcon();

    // init toolbars
    createToolBars();
    auto *toolBarLay = new QVBoxLayout();
    toolBarLay->addWidget(toolBarTop);
    toolBarLay->addStretch();
    toolBarLay->addWidget(toolBarBottom);
    toolBarLay->setContentsMargins(0, 0, 0, 0);
    mainLay->addLayout(toolBarLay);

    // init tabs
    tabWidget = new QTabWidget(centralWidget);
    filesTree = new QTreeView(tabWidget);
    packagesTree = new QTreeView(tabWidget);
    connect(packagesTree, &QTreeView::doubleClicked, this, &MainWindow::managePackage);
    tabWidget->addTab(filesTree, "Files");
    tabWidget->addTab(packagesTree, "Packages");
    updateFiles();
    updatePackages();
    mainLay->addWidget(tabWidget);

    setCentralWidget(centralWidget);

    Utils::Repositories::syncRepos();
}

void MainWindow::updateFiles() {
    QStringList headers;
    headers << tr("Name") << tr("Created") << tr("Modified") << tr("Downloaded") << tr("Up to date");
    auto files = Wrapper::Files::getAll();

    monitor = new Utils::FilesMonitor(files, Utils::Checker::State::Active);
    auto model = new FileTreeModel(headers, files);
    connect(monitor, &Utils::FilesMonitor::filesChanged, model, &FileTreeModel::handleChanges);

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
    user = std::move(usr);
    initUI();
    show();
}

void MainWindow::moveToCenter() {
    setGeometry(0, 0, 900, 500);

    QDesktopWidget dw;
    QRect rc = dw.screenGeometry(this);
    move((rc.width() - width()) / 2, (rc.height() - height()) / 2 - 20);
}

void MainWindow::createTrayIcon() {
    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(QIcon(":/img/icon.png"));
    trayIcon->setToolTip("Antarctica\n"
                         "Cloud sync of your files, preferences and packages");

    auto menu = new QMenu(this);
    auto showAction = new QAction("Show Antarctica", this);
    auto quitAction = new QAction("Quit", this);

    connect(showAction, &QAction::triggered, [this]() {
        this->show();
        monitor->goActive();
    });
    connect(quitAction, &QAction::triggered, &QApplication::quit);

    menu->addAction(showAction);
    menu->addAction(quitAction);

    trayIcon->setContextMenu(menu);
    trayIcon->show();

    connect(trayIcon, &QSystemTrayIcon::activated, this, &MainWindow::iconActivated);
}

void MainWindow::createToolBars() {
    const QString style = "QToolButton { height: 30px; width: 30px; }";

    toolBarTop = new QToolBar(this);
    toolBarTop->setStyleSheet(style);
    toolBarTop->setOrientation(Qt::Vertical);

    addAction = new QAction(QIcon(":/main_icons/orange/add.png"), "Add", toolBarTop);
    removeAction = new QAction(QIcon(":/main_icons/orange/remove.png"), "Remove", toolBarTop);
    downloadAction = new QAction("Download", toolBarTop);
    refreshAction = new QAction(QIcon(":/main_icons/orange/refresh.png"), "Refresh", toolBarTop);

    connect(addAction, &QAction::triggered, this, &MainWindow::addSlot);
    connect(removeAction, &QAction::triggered, this, &MainWindow::removeSlot);
    connect(downloadAction, &QAction::triggered, this, &MainWindow::downloadSlot);
    connect(refreshAction, &QAction::triggered, this, &MainWindow::refreshSlot);

    toolBarTop->addAction(addAction);
    toolBarTop->addAction(removeAction);
    toolBarTop->addAction(downloadAction);
    toolBarTop->addAction(refreshAction);


    toolBarBottom = new QToolBar(this);
    toolBarBottom->setStyleSheet(style);
    toolBarBottom->setOrientation(Qt::Vertical);
    toolBarBottom->addSeparator();

    toolButtonOther = new QToolButton(toolBarBottom);
    toolButtonOther->setPopupMode(QToolButton::InstantPopup);
    toolButtonOther->setIcon(QIcon(":/main_icons/orange/menu.png"));
    toolButtonOther->setFocusPolicy(Qt::NoFocus);

    auto *menuOther = new QMenu(toolBarBottom);
    menuOther->addAction("Not implemented");
    menuOther->addAction("Not implemented");
    menuOther->addAction("Not implemented");
    menuOther->addSeparator();
    menuOther->addAction("Not implemented");
    toolButtonOther->setMenu(menuOther);

    //otherAction = new QAction("Other", menuOther);
    settingsAction = new QAction(QIcon(":/main_icons/orange/settings.png"), "Settings", toolBarBottom);

    //connect(otherAction, &QAction::triggered, this, &MainWindow::otherSlot);
    connect(settingsAction, &QAction::triggered, this, &MainWindow::settingsSlot);

    toolBarBottom->addWidget(toolButtonOther);
    toolBarBottom->addAction(settingsAction);
}

void MainWindow::testSlot() {
    QMessageBox::information(nullptr, "Message", "Not implemented");
}

void MainWindow::addSlot() {
    switch (tabWidget->currentIndex()) {
        case 0: // files tab
            addFile();
            break;
        case 1: // packages tab
            addPkg();
            break;
        default:
            break;
    }
}

void MainWindow::removeSlot() {
    switch (tabWidget->currentIndex()) {
        case 0: // files tab
            removeFile();
            break;
        case 1: // packages tab
            removePkg();
            break;
        default:
            break;
    }
}

void MainWindow::downloadSlot() {
    switch (tabWidget->currentIndex()) {
        case 0: // files tab
            download();
            break;
        case 1: // packages tab
            installPkg();
            break;
        default:
            break;
    }
}

void MainWindow::refreshSlot() {
    switch (tabWidget->currentIndex()) {
        case 0: // files tab
            updateFiles();
            break;
        case 1: // packages tab
            updatePackages();
            break;
        default:
            break;
    }
}

void MainWindow::addFile() {
    for (auto &&filename : Utils::Files::openFiles(this)) {
        if (QFileInfo info(filename); info.isFile()) {
            Utils::Files::uploadFile(filename);
        } else if (info.isDir()) {
            Utils::Files::uploadDir(filename);
        }
    }
    updateFiles();
}

void MainWindow::addPkg() {
    auto addPackageDialog = new AddPackageDialog(this);
    addPackageDialog->exec();
    updatePackages();
    updateFiles();
    delete addPackageDialog;
}

void MainWindow::removeFile() {
    if (auto indexes = filesTree->selectionModel()->selectedIndexes(); !indexes.empty()) {
        int lastRow = -1;
        for (auto &&index : indexes) {
            if (lastRow != index.row()) {
                auto item = Utils::UI::getCurrentItem<FileTreeItem, FileTreeModel>(filesTree->model(), index);
                if (item->childCount() == 0) {
                    Wrapper::Files::remove(item->getFile()->id);
                } else {
                    Utils::Files::removeServerDir(item);
                }
            }
            lastRow = index.row();
        }
    }
    updateFiles();
}

void MainWindow::removePkg() {
    if (auto indexes = packagesTree->selectionModel()->selectedIndexes(); !indexes.empty()) {
        int lastRow = -1;
        for (auto &&index : indexes) {
            if (lastRow != index.row()) {
                auto item = Utils::UI::getCurrentItem<PackageTreeItem, PackageTreeModel>(packagesTree->model(), index);
                if (item->childCount() == 0) {
                    Wrapper::Packages::remove(item->getPackage()->id);
                } else {
                    Wrapper::Repositories::remove(
                            item->child(0)->getPackage()->repository->id);
                }
            }
            lastRow = index.row();
        }
    }
    updatePackages();
    updateFiles();
}

void MainWindow::settingsSlot() {
    SettingsWindow settings{};
    settings.exec();
}

void MainWindow::download() {
    if (auto indexes = filesTree->selectionModel()->selectedIndexes(); !indexes.empty()) {
        int lastRow = -1;
        for (auto &&index : indexes) {
            if (lastRow != index.row()) {
                if (auto item = Utils::UI::getCurrentItem<FileTreeItem, FileTreeModel>(filesTree->model(), index);
                        item->childCount() == 0) {
                    Utils::Files::downloadFile(item);
                } else {
                    Utils::Files::downloadDir(item);
                }
            }
            lastRow = index.row();
        }
    } else {
        auto model = dynamic_cast<FileTreeModel *>(filesTree->model());
        Utils::Files::downloadDir(dynamic_cast<FileTreeItem *>(model->getItem(model->index(0, 0))));
    }
    updateFiles();
}

void MainWindow::installPkg() {
    testSlot(); // TODO: implement
}

void MainWindow::managePackage(const QModelIndex &idx) {
    if (auto indexes = packagesTree->selectionModel()->selectedIndexes(); !indexes.empty()) {
        int lastRow = -1;
        for (auto &&index : indexes) {
            if (lastRow != index.row()) {
                if (auto item = Utils::UI::getCurrentItem<PackageTreeItem, PackageTreeModel>(packagesTree->model(),
                                                                                             index);
                        item->childCount() == 0) {
                    PackageConfigurator packageConfigurator(item->getPackage(), this);
                    packageConfigurator.exec();
                    updatePackages();
                    updateFiles();
                }
            }
            lastRow = index.row();
        }
    }
}

void MainWindow::closeEvent(QCloseEvent *event) {
    if (this->isVisible() && Utils::Settings::readProperty("background", "enabled").toBool()) {
        event->ignore();
        this->hide();
        monitor->goBackground();

        auto icon = QSystemTrayIcon::MessageIcon(QSystemTrayIcon::Information);
        trayIcon->showMessage("Antarctica",
                              "Antarctica is still syncing your files",
                              icon,
                              2000);
    } else {
        QApplication::quit();
    }
}

void MainWindow::iconActivated(QSystemTrayIcon::ActivationReason reason) {
    if (reason == QSystemTrayIcon::Trigger) {
        if (!this->isVisible()) {
            this->show();
            monitor->goActive();
        } else {
            this->hide();
            monitor->goBackground();
        }
    }
}
