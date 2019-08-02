#ifndef ANTARCTICA_BACKGROUNDCONFIGURATOR_H
#define ANTARCTICA_BACKGROUNDCONFIGURATOR_H


#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QSpinBox>
#include "SectionConfigurator.h"

class BackgroundConfigurator : public OptionalSection {
    Q_OBJECT
public:
    explicit BackgroundConfigurator(QWidget *parent = nullptr);

    void apply() override;

    void save() override;

private:
    void initUI() override;

    void loadSettings() override;

    QCheckBox *backgroundConnChecker{};
    QGroupBox *backgroundConnSettings{};
    QSpinBox *frequency{};

private slots:
    void stateChanged(int state) override;

};


#endif //ANTARCTICA_BACKGROUNDCONFIGURATOR_H
