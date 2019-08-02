#include <QDesktopWidget>
#include <QLabel>
#include <QtWidgets/QCheckBox>

#include "SettingsWindow.h"

SettingsWindow::SettingsWindow(QWidget *parent) : QDialog(parent) {
    initUI();
}

void SettingsWindow::initUI() {
    setWindowTitle("Settings");
    setWindowIcon(QIcon(":/img/icon.png"));
    moveToCenter();

    mainLay = new QVBoxLayout(this);

    auto controlsLay = new QHBoxLayout(this);
    createToolBar();

    createTabs();

    controlsLay->addWidget(toolBar);
    controlsLay->addWidget(tabWidget);
    mainLay->addLayout(controlsLay);

    createButtons();

    setLayout(mainLay);
    showUiSection();
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

    connect(uiAction, &QAction::triggered, this, &SettingsWindow::showUiSection);
    connect(connectionAction, &QAction::triggered, this, &SettingsWindow::showConnectionSection);
    connect(securityAction, &QAction::triggered, this, &SettingsWindow::showSecuritySection);

    toolBar->addAction(uiAction);
    toolBar->addAction(connectionAction);
    toolBar->addAction(securityAction);
}

void SettingsWindow::showUiSection() {
    tabWidget->clear();
    tabWidget->addTab(themeTab, "Theme");
    tabWidget->addTab(layoutTab, "Layout");
    tabWidget->setCurrentIndex(0);
}

void SettingsWindow::showConnectionSection() {
    tabWidget->clear();
    tabWidget->addTab(backgroundConnTab, "Background connection");
    tabWidget->addTab(proxyTab, "Proxy");
    tabWidget->setCurrentIndex(0);
}

void SettingsWindow::showSecuritySection() {
    tabWidget->clear();
    tabWidget->addTab(signatureTab, "Digital Signature");
    tabWidget->setCurrentIndex(0);
}

void SettingsWindow::createTabs() {
    tabWidget = new QTabWidget(this);

    createThemeTab();
    createLayoutTab();
    backgroundConnTab = new BackgroundConfigurator;
    proxyTab = new ProxyConfigurator;
    createSignatureTab();
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

void SettingsWindow::createSignatureTab() {
    auto la = new QLabel("Signature settings");
    auto l = new QVBoxLayout();
    l->addWidget(la);

    signatureTab = new QWidget();
    signatureTab->setLayout(l);
}

void SettingsWindow::createButtons() {
    auto buttons = new QHBoxLayout(this);
    ok = new QPushButton("OK", this);
    ok->setDefault(true);
    apply = new QPushButton("Apply", this);
    cancel = new QPushButton("Cancel", this);

    connect(ok, &QPushButton::clicked, this, &SettingsWindow::okClicked);
    connect(apply, &QPushButton::clicked, this, &SettingsWindow::applyClicked);
    connect(cancel, &QPushButton::clicked, this, &SettingsWindow::close);

    buttons->addStretch();
    buttons->addWidget(ok);
    buttons->addWidget(apply);
    buttons->addWidget(cancel);
    mainLay->addLayout(buttons);
}

void SettingsWindow::okClicked() {
    dynamic_cast<SectionConfigurator *>(tabWidget->currentWidget())->save();
    close();
}

void SettingsWindow::applyClicked() {
    dynamic_cast<SectionConfigurator *>(tabWidget->currentWidget())->apply();
}
