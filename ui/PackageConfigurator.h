#ifndef ANTARCTICA_PACKAGECONFIGURATOR_H
#define ANTARCTICA_PACKAGECONFIGURATOR_H


#include <QtWidgets/QDialog>
#include <api/models/Package.h>
#include <api/models/File.h>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QPushButton>

class PackageConfigurator : public QDialog {
Q_OBJECT

public:
    explicit PackageConfigurator(QWidget *parent);

    PackageConfigurator(Package *pkg, QWidget *parent);

protected:
    QList<File *> configs{};

    QComboBox *repoSelector{};
    QTreeView *filesTree{};
    QPushButton *addFileButton{};
    QPushButton *removeFileButton{};
    QPushButton *okButton{};
    QPushButton *cancelButton{};

    virtual void initUI();

    void updateFiles();

protected slots:

    void addFileClicked();

    void removeFileClicked();

    virtual void okClicked();

    void cancelClicked();

private:
    Package *package{};

};


#endif //ANTARCTICA_PACKAGECONFIGURATOR_H
