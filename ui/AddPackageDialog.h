#ifndef ANTARCTICA_ADDPACKAGEDIALOG_H
#define ANTARCTICA_ADDPACKAGEDIALOG_H


#include <QtWidgets/QDialog>
#include <QtCore/QList>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QPushButton>
#include <api/models/File.h>
#include <api/models/Package.h>

class AddPackageDialog : public QDialog {
Q_OBJECT

public:
    explicit AddPackageDialog(QWidget *parent = nullptr);

private:
    QList<File *> configs{};

    QLineEdit *nameInput{};
    QComboBox *repoSelector{};
    QTreeView *filesTree{};
    QPushButton *addFileButton{};
    QPushButton *removeFileButton{};
    QPushButton *okButton{};
    QPushButton *cancelButton{};

    void initUI();

    void updateFiles();

private slots:
    void addFileClicked();

    void removeFileClicked();

    void okClicked();

    void cancelClicked();

};


#endif //ANTARCTICA_ADDPACKAGEDIALOG_H
