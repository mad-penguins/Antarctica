#ifndef ANTARCTICA_SETTINGSWINDOW_H
#define ANTARCTICA_SETTINGSWINDOW_H

#include <QDialog>
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QToolBar>
#include <QTabWidget>
#include <QPushButton>

#include "ProxyConfigurator.h"
#include "BackgroundConfigurator.h"

class SettingsWindow : public QDialog {
    Q_OBJECT

public:
    explicit SettingsWindow(QWidget *parent = nullptr);

private:
    QVBoxLayout *mainLay{};

    QToolBar *toolBar{};
    QAction *uiAction{};
    QAction *connectionAction{};
    QAction *securityAction{};

    QTabWidget *tabWidget{};

    QWidget *themeTab{};
    QWidget *layoutTab{};
    BackgroundConfigurator *backgroundConnTab{};
    ProxyConfigurator *proxyTab{};
    QWidget *signatureTab{};

    QPushButton *ok{};
    QPushButton *apply{};
    QPushButton *cancel{};

    void initUI();

    void moveToCenter();

    void createToolBar();

    void createTabs();
    void createThemeTab();
    void createLayoutTab();
    void createSignatureTab();

    void createButtons();

private slots:
    void okClicked();
    void applyClicked();

    void showUiSection();

    void showConnectionSection();

    void showSecuritySection();
};


#endif //ANTARCTICA_SETTINGSWINDOW_H