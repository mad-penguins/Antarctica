#ifndef ANTARCTICA_MAINWINDOW_H
#define ANTARCTICA_MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtGui/QCloseEvent>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QTableView>

#include "models/User.h"

class MainWindow : public QMainWindow {
Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private:
    User user;
    QTabWidget *tabWidget;

    void initUI();

    QAbstractTableModel *filesModel;
    QTableView *filesTree;

    void initFiles();

    void updateFiles();

    QAbstractTableModel *packagesModel;
    QTableView *packagesTree;

    void initPackages();

    void updatePackages();

public slots:

    void showUser(User user);

signals:

    void closed();

protected:
    void closeEvent(QCloseEvent *event) override {
        QMainWindow::closeEvent(event);

        if (event->isAccepted()) {
            emit closed();
        }
    }

};


#endif //ANTARCTICA_MAINWINDOW_H
