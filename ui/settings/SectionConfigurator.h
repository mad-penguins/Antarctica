#ifndef ANTARCTICA_SECTIONCONFIGURATOR_H
#define ANTARCTICA_SECTIONCONFIGURATOR_H

#include <QtWidgets/QWidget>

class SectionConfigurator : public QWidget {
    Q_OBJECT
public:
    explicit SectionConfigurator(QWidget *parent = nullptr) : QWidget(parent) {}

public slots:
    virtual void apply() = 0;
    virtual void save() = 0;

protected:
    virtual void initUI() = 0;

    virtual void loadSettings() = 0;
};

class OptionalSection : public SectionConfigurator {
Q_OBJECT
public:
    explicit OptionalSection(QWidget *parent = nullptr) : SectionConfigurator(parent) {}

protected slots:

    virtual void stateChanged(int state) = 0;
};

#endif //ANTARCTICA_SECTIONCONFIGURATOR_H
