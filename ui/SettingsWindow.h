#ifndef ANTARCTICA_SETTINGSWINDOW_H
#define ANTARCTICA_SETTINGSWINDOW_H

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QToolBar>
#include <QTabWidget>

class SettingsWindow : public QWidget {
    Q_OBJECT

public:
    explicit SettingsWindow(QWidget *parent = nullptr);

private:
    QHBoxLayout *mainLay{};

    QToolBar *toolBar{};
    QAction *test1Action{};
    QAction *test2Action{};

    QTabWidget *tabWidget{};

    QWidget *test1_1;
    QWidget *test1_2;
    QWidget *test2_1;
    QWidget *test2_2;

    void initUI();

    void moveToCenter();

    void createToolBar();
    void createTabWidget();

    void createTest1_1();
    void createTest1_2();
    void createTest2_1();
    void createTest2_2();

private slots:
    void test1Slot();
    void test2Slot();
};


#endif //ANTARCTICA_SETTINGSWINDOW_H