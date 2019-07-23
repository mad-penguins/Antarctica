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

class SettingsWindow : public QDialog {
    Q_OBJECT

public:
    explicit SettingsWindow(QWidget *parent = nullptr);

private:
    QToolBar *toolBar{};
    QAction *uiAction{};
    QAction *connectionAction{};
    QAction *securityAction{};

    QTabWidget *tabWidget{};

    QWidget *themeTab{};
    QWidget *layoutTab{};
    ProxyConfigurator *proxyTab{};
    QWidget *signatureTab{};

    QPushButton *ok{};
    QPushButton *apply{};
    QPushButton *cancel{};

    void initUI();

    void moveToCenter();

    void createToolBar();

    void createThemeTab();
    void createLayoutTab();
    void createSignatureTab();

private slots:
    void okClicked();
    void applyClicked();

    void showUiTab();
    void showConnectionTab();
    void showSecurityTab();
};


#endif //ANTARCTICA_SETTINGSWINDOW_H