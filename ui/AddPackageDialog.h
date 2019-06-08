#ifndef ANTARCTICA_ADDPACKAGEDIALOG_H
#define ANTARCTICA_ADDPACKAGEDIALOG_H


#include <QtWidgets/QLineEdit>

#include "PackageConfigurator.h"

class AddPackageDialog : public PackageConfigurator {
Q_OBJECT

public:
    explicit AddPackageDialog(QWidget *parent = nullptr);

private:
    QLineEdit *nameInput{};

    void initUI() override;

private slots:

    void okClicked() override;

};


#endif //ANTARCTICA_ADDPACKAGEDIALOG_H
