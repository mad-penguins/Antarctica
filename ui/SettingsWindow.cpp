#include <QDesktopWidget>
#include <QLabel>

#include "SettingsWindow.h"

SettingsWindow::SettingsWindow(QWidget *parent) {
    initUI();
}

void SettingsWindow::initUI() {
    setWindowTitle("Antarctica login");
    setWindowIcon(QIcon(":/img/icon.png"));
    moveToCenter();

    mainLay = new QHBoxLayout(this);
    mainLay->setContentsMargins(0, 0, 0, 0);
    mainLay->setSpacing(0);

    createTest1_1();
    createTest1_2();
    createTest2_1();
    createTest2_2();

    createToolBar();
    createTabWidget();

    mainLay->addWidget(toolBar);
    mainLay->addWidget(tabWidget);

    setLayout(mainLay);
}

void SettingsWindow::moveToCenter() {
    setGeometry(0, 0, 320, 500);

    QDesktopWidget dw;
    QRect rc = dw.screenGeometry(this);
    move((rc.width() - width()) / 2, (rc.height() - height()) / 2 - 20);
}

void SettingsWindow::createToolBar() {
    toolBar = new QToolBar(this);
    //toolBarTop->setStyleSheet(style);
    toolBar->setOrientation(Qt::Vertical);

    test1Action = new QAction("Test1", toolBar);
    test2Action = new QAction("Test2", toolBar);

    connect(test1Action, &QAction::triggered, this, &SettingsWindow::test1Slot);
    connect(test2Action, &QAction::triggered, this, &SettingsWindow::test2Slot);

    toolBar->addAction(test1Action);
    toolBar->addAction(test2Action);
}

void SettingsWindow::createTabWidget() {
    tabWidget = new QTabWidget();
    tabWidget->addTab(test1_1, "Test 1");
    tabWidget->addTab(test1_2, "Test 2");
}

void SettingsWindow::test1Slot() {
    tabWidget->setCurrentIndex(0);
    tabWidget->removeTab(1);
    tabWidget->removeTab(0);
    tabWidget->addTab(test1_1, "Test 1-1");
    tabWidget->addTab(test1_2, "Test 1-2");
}

void SettingsWindow::test2Slot() {
    tabWidget->setCurrentIndex(0);
    tabWidget->removeTab(1);
    tabWidget->removeTab(0);
    tabWidget->addTab(test2_1, "Test 2-1");
    tabWidget->addTab(test2_2, "Test 2-2");
}

void SettingsWindow::createTest1_1() {
    auto la = new QLabel("Button Test1 - 1");
    auto l = new QVBoxLayout();
    l->addWidget(la);

    test1_1 = new QWidget();
    test1_1->setLayout(l);
}

void SettingsWindow::createTest1_2() {
    auto la = new QLabel("Button Test1 - 2");
    auto l = new QVBoxLayout();
    l->addWidget(la);

    test1_2 = new QWidget();
    test1_2->setLayout(l);
}

void SettingsWindow::createTest2_1() {
    auto la = new QLabel("Button Test2 - 1");
    auto l = new QVBoxLayout();
    l->addWidget(la);

    test2_1 = new QWidget();
    test2_1->setLayout(l);
}

void SettingsWindow::createTest2_2() {
    auto la = new QLabel("Button Test2 - 2");
    auto l = new QVBoxLayout();
    l->addWidget(la);

    test2_2 = new QWidget();
    test2_2->setLayout(l);
}