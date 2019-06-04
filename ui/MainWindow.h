/*!
 * \file ui/MainWindow.h
 * \author Nikita Mironov <nickfrom22nd@gmail.com>
 * \brief The main window class
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


#ifndef ANTARCTICA_MAINWINDOW_H
#define ANTARCTICA_MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtGui/QCloseEvent>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QTableView>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QToolButton>

#include <api/models/User.h>


class MainWindow : public QMainWindow {
Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private:
    User user;

    QHBoxLayout *mainLay{};
    QTabWidget *tabWidget{};
    QToolBar *toolBarTop{};
    QToolBar *toolBarBottom{};
    QToolButton *toolButtonOther{};


    QAction *addAction{};
    QAction *removeAction{};
    QAction *refreshAction{};
    QAction *otherAction{};
    QAction *settingsAction{};

    void initUI();

    void moveToCenter();

    QTreeView *filesTree{};

    void updateFiles();

    QTreeView *packagesTree{};

    void updatePackages();

    void createToolBars();

    void addFile();

    void addPkg();

    void removeFile();

    void removePkg();

public slots:

    void showUser(User usr);

private slots:

    void addSlot();

    void removeSlot();

    void refreshSlot();

    void otherSlot();

    void settingsSlot();

    static void testSlot();

};


#endif //ANTARCTICA_MAINWINDOW_H
