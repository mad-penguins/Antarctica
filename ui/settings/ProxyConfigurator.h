#ifndef ANTARCTICA_PROXYCONFIGURATOR_H
#define ANTARCTICA_PROXYCONFIGURATOR_H


#include <QtWidgets/QWidget>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QSpinBox>

#include "SectionConfigurator.h"

class ProxyConfigurator : public SectionConfigurator {
public:
    explicit ProxyConfigurator(QWidget *parent = nullptr);

public slots:
    void apply() override;

    void save() override;

private:
    void initUI() override;

    void loadSettings() override;

    QCheckBox *proxyChecker{};
    QGroupBox *socks5Settings{};
    QLineEdit *hostname{};
    QSpinBox *port{};
    QLineEdit *username{};
    QLineEdit *password{};

private slots:
    void proxyStateChanged(int state);

};


#endif //ANTARCTICA_PROXYCONFIGURATOR_H
