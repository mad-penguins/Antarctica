#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtCore/QSettings>
#include "BackgroundConfigurator.h"


BackgroundConfigurator::BackgroundConfigurator(QWidget *parent) : OptionalSection(parent) {
    initUI();
}

void BackgroundConfigurator::initUI() {
    auto lay = new QGridLayout(this);
    lay->addWidget(new QLabel("Enable background connection", this), 0, 0, 1, 1);
    lay->setColumnStretch(2, 1);
    backgroundConnChecker = new QCheckBox(this);
    connect(backgroundConnChecker, &QCheckBox::stateChanged, this, &BackgroundConfigurator::stateChanged);
    lay->addWidget(backgroundConnChecker, 0, 2, 1, 1);

    backgroundConnSettings = new QGroupBox("Background connection settings", this);
    auto backgroundConnLay = new QGridLayout(backgroundConnSettings);

    frequency = new QSpinBox(backgroundConnSettings);
    frequency->setMinimum(1);
    frequency->setMaximum(120);

    backgroundConnLay->addWidget(new QLabel("Update frequency", backgroundConnSettings), 0, 0, 1, 1);
    backgroundConnLay->addWidget(frequency, 0, 2, 1, 2);
    backgroundConnLay->setColumnStretch(3, 1);
    backgroundConnLay->addWidget(new QLabel("minute(s)", backgroundConnSettings), 0, 4, 1, 1);

    lay->addWidget(backgroundConnSettings, 1, 0, 2, 5);
    lay->setRowStretch(3, 1);

    loadSettings();
}

void BackgroundConfigurator::stateChanged(int state) {
    switch (state) {
        case Qt::CheckState::Checked:
            backgroundConnSettings->setDisabled(false);
            break;
        case Qt::CheckState::Unchecked:
            backgroundConnSettings->setDisabled(true);
            break;
        default:
            break;
    }
}

void BackgroundConfigurator::apply() {
    save();
}

void BackgroundConfigurator::save() {
    QSettings settings("MadPenguins", "Antarctica");
    settings.beginGroup("background");

    if (backgroundConnChecker->isChecked()) {
        settings.setValue("enabled", true);
        settings.setValue("freq", frequency->value());
    } else {
        settings.setValue("enabled", false);
    }

    settings.endGroup();
}

void BackgroundConfigurator::loadSettings() {
    QSettings settings("MadPenguins", "Antarctica");
    settings.beginGroup("background");
    backgroundConnChecker->setChecked(settings.value("enabled").toBool());
    if (!backgroundConnChecker->isChecked()) {
        backgroundConnSettings->setDisabled(true);
    }
    frequency->setValue(settings.value("freq").toInt());
    settings.endGroup();
}
