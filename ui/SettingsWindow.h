#ifndef ANTARCTICA_SETTINGSWINDOW_H
#define ANTARCTICA_SETTINGSWINDOW_H

#include <QDialog>
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QToolBar>
#include <QTabWidget>

class SettingsWindow : public QDialog {
    Q_OBJECT

public:
    explicit SettingsWindow(QWidget *parent = nullptr);

private:
    QHBoxLayout *mainLay{};

    QToolBar *toolBar{};
    QAction *uiAction{};
    QAction *connectionAction{};
    QAction *securityAction{};

    QTabWidget *tabWidget{};

    QWidget *themeTab{};
    QWidget *layoutTab{};
    QWidget *proxyTab{};
    QWidget *signatureTab{};

    void initUI();

    void moveToCenter();

    void createToolBar();

    void createThemeTab();
    void createLayoutTab();
    void createProxyTab();
    void createSignatureTab();

private slots:
    void ShowUiTab();
    void showConnectionTab();
    void showSecurityTab();
};


#endif //ANTARCTICA_SETTINGSWINDOW_H