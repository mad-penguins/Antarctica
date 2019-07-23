#include <QDesktopWidget>
#include <QLabel>

#include "SettingsWindow.h"

SettingsWindow::SettingsWindow(QWidget *parent) : QDialog(parent) {
    initUI();
}

void SettingsWindow::initUI() {
    setWindowTitle("Settings");
    setWindowIcon(QIcon(":/img/icon.png"));
    moveToCenter();

    mainLay = new QHBoxLayout(this);
    mainLay->setContentsMargins(0, 0, 0, 0);
    mainLay->setSpacing(0);

    createThemeTab();
    createLayoutTab();
    createProxyTab();
    createSignatureTab();

    createToolBar();
    tabWidget = new QTabWidget(this);

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

    uiAction = new QAction("Interface", toolBar);
    connectionAction = new QAction("Connection", toolBar);
    securityAction = new QAction("Security", toolBar);

    connect(uiAction, &QAction::triggered, this, &SettingsWindow::ShowUiTab);
    connect(connectionAction, &QAction::triggered, this, &SettingsWindow::showConnectionTab);
    connect(securityAction, &QAction::triggered, this, &SettingsWindow::showSecurityTab);

    toolBar->addAction(uiAction);
    toolBar->addAction(connectionAction);
    toolBar->addAction(securityAction);
}

void SettingsWindow::ShowUiTab() {
    tabWidget->clear();
    tabWidget->addTab(themeTab, "Theme");
    tabWidget->addTab(layoutTab, "Layout");
    tabWidget->setCurrentIndex(0);
}

void SettingsWindow::showConnectionTab() {
    tabWidget->clear();
    tabWidget->addTab(proxyTab, "Proxy");
    tabWidget->setCurrentIndex(0);
}

void SettingsWindow::showSecurityTab() {
    tabWidget->clear();
    tabWidget->addTab(signatureTab, "Digital Signature");
    tabWidget->setCurrentIndex(0);
}

void SettingsWindow::createThemeTab() {
    auto la = new QLabel("Theme settings");
    auto l = new QVBoxLayout();
    l->addWidget(la);

    themeTab = new QWidget();
    themeTab->setLayout(l);
}

void SettingsWindow::createLayoutTab() {
    auto la = new QLabel("Layout settings");
    auto l = new QVBoxLayout();
    l->addWidget(la);

    layoutTab = new QWidget();
    layoutTab->setLayout(l);
}

void SettingsWindow::createProxyTab() {
    auto la = new QLabel("Proxy settings");
    auto l = new QVBoxLayout();
    l->addWidget(la);

    proxyTab = new QWidget();
    proxyTab->setLayout(l);
}

void SettingsWindow::createSignatureTab() {
    auto la = new QLabel("Signature settings");
    auto l = new QVBoxLayout();
    l->addWidget(la);

    signatureTab = new QWidget();
    signatureTab->setLayout(l);
}
