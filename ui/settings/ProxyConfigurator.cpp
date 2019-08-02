#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtNetwork/QNetworkProxy>
#include <QtCore/QSettings>
#include "ProxyConfigurator.h"

ProxyConfigurator::ProxyConfigurator(QWidget *parent) : OptionalSection(parent) {
    initUI();
}

void ProxyConfigurator::initUI() {
    auto lay = new QGridLayout(this);
    lay->addWidget(new QLabel("Enable proxy", this), 0, 0, 1, 1);
    lay->setColumnStretch(2, 1);
    proxyChecker = new QCheckBox(this);
    connect(proxyChecker, &QCheckBox::stateChanged, this, &ProxyConfigurator::stateChanged);
    lay->addWidget(proxyChecker, 0, 2, 1, 1);

    socks5Settings = new QGroupBox("SOCKS5 settings", this);
    auto socksLay = new QGridLayout(socks5Settings);

    hostname = new QLineEdit(socks5Settings);
    port = new QSpinBox(socks5Settings);
    username = new QLineEdit(socks5Settings);
    password = new QLineEdit(socks5Settings);
    password->setEchoMode(QLineEdit::Password);

    socksLay->addWidget(new QLabel("Server", socks5Settings), 0, 0, 1, 1);
    socksLay->addWidget(hostname, 0, 2, 1, 4);
    socksLay->addWidget(port, 0, 6, 1, 2);

    socksLay->addWidget(new QLabel("User", socks5Settings), 1, 0, 1, 1);
    socksLay->addWidget(username, 1, 2, 1, 3);
    socksLay->addWidget(password, 1, 5, 1, 3);

    socksLay->setColumnStretch(2, 1);

    lay->addWidget(socks5Settings, 1, 0, 2, 8);
    lay->setRowStretch(3, 1);

    loadSettings();
}

void ProxyConfigurator::stateChanged(int state) {
    switch (state) {
        case Qt::CheckState::Checked:
            socks5Settings->setDisabled(false);
            break;
        case Qt::CheckState::Unchecked:
            socks5Settings->setDisabled(true);
            break;
        default:
            break;
    }
}

void ProxyConfigurator::apply() {
    if (proxyChecker->isChecked()) {
        QNetworkProxy proxy;
        proxy.setType(QNetworkProxy::Socks5Proxy);
        proxy.setHostName(hostname->text().trimmed());
        proxy.setPort(static_cast<quint16>(port->value()));
        proxy.setUser(username->text().trimmed());
        proxy.setPassword(password->text().trimmed());
        QNetworkProxy::setApplicationProxy(proxy);
    } else {
        QNetworkProxy::setApplicationProxy(QNetworkProxy::NoProxy);
    }
}

void ProxyConfigurator::save() {
    apply();

    QSettings settings("MadPenguins", "Antarctica");
    settings.beginGroup("proxy");

    if (proxyChecker->isChecked()) {
        settings.setValue("enabled", true);
        settings.setValue("host", hostname->text().trimmed());
        settings.setValue("port", port->value());
        settings.setValue("user", username->text().trimmed());
        settings.setValue("password", password->text().trimmed());
    } else {
        settings.setValue("enabled", false);
    }

    settings.endGroup();
}

void ProxyConfigurator::loadSettings() {
    QSettings settings("MadPenguins", "Antarctica");
    settings.beginGroup("proxy");
    proxyChecker->setChecked(settings.value("enabled").toBool());
    if (!proxyChecker->isChecked()) {
        socks5Settings->setDisabled(true);
    }
    hostname->setText(settings.value("host").toString());
    port->setValue(settings.value("port").toInt());
    username->setText(settings.value("user").toString());
    password->setText(settings.value("password").toString());
    settings.endGroup();
}
