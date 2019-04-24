#ifndef ANTARCTICA_MAINWINDOW_H
#define ANTARCTICA_MAINWINDOW_H


#include <QMainWindow>
#include <QPushButton>
#include <QCloseEvent>
#include <QtSql/QSqlRelationalTableModel>
#include <QtWidgets/QTreeView>

#include "models/User.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);

private:
    User user;
    QTabWidget *tabWidget;
    void initUI();

    QSqlRelationalTableModel *filesModel;
    QTreeView *filesTree;
    void initFiles();
    void updateFiles();

    QSqlRelationalTableModel *packagesModel;
    QTreeView *packagesTree;
    void initPackages();
    void updatePackages();

public slots:
    void showUser(User user);

signals:
    void closed();

protected:
    void closeEvent(QCloseEvent* event) override {
        QMainWindow::closeEvent(event);

        if (event->isAccepted()) {
            emit closed();
        }
    }

};


#endif //ANTARCTICA_MAINWINDOW_H
