#include <api/Wrapper.h>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <utils/Files.hpp>
#include <QtWidgets/QMessageBox>

#include "AddPackageDialog.h"
#include "ui/models/files/FileTreeModel.h"

AddPackageDialog::AddPackageDialog(QWidget *parent) : PackageConfigurator(parent) {
    initUI();
}

void AddPackageDialog::initUI() {
    setWindowTitle(tr("New package"));
    auto mainLay = new QGridLayout(this);

    auto label = new QLabel(tr("Add a package"), this);
    label->setStyleSheet("font-size: 20pt;");
    mainLay->addWidget(label, 0, 1, 1, 5);

    nameInput = new QLineEdit(this);
    mainLay->addWidget(new QLabel(tr("Name"), this), 1, 1);
    mainLay->addWidget(nameInput, 1, 2, 1, 4);

    repoSelector = new QComboBox(this);
    repoSelector->addItem("Default"); // TODO: implement system repositories scanning
    mainLay->addWidget(new QLabel(tr("Repository"), this), 2, 1);
    mainLay->addWidget(repoSelector, 2, 2, 1, 4);

    mainLay->addWidget(new QLabel(tr("Configuration files:"), this), 3, 1);
    filesTree = new QTreeView(this);
    updateFiles();

    mainLay->addWidget(filesTree, 4, 1, 5, 5);
    addFileButton = new QPushButton(tr("Add"), this);
    removeFileButton = new QPushButton(tr("Remove"), this);

    connect(addFileButton, &QPushButton::clicked, this, &AddPackageDialog::addFileClicked);
    connect(removeFileButton, &QPushButton::clicked, this, &AddPackageDialog::removeFileClicked);
    mainLay->addWidget(addFileButton, 3, 2);
    mainLay->addWidget(removeFileButton, 3, 3);

    okButton = new QPushButton("OK", this);
    okButton->setDefault(true);
    cancelButton = new QPushButton(tr("Cancel"), this);
    connect(okButton, &QPushButton::clicked, this, &AddPackageDialog::okClicked);
    connect(cancelButton, &QPushButton::clicked, this, &AddPackageDialog::cancelClicked);
    mainLay->addWidget(okButton, 10, 5);
    mainLay->addWidget(cancelButton, 10, 4);
}

void AddPackageDialog::okClicked() {
    if (nameInput->text().isEmpty()) {
        close();
        return;
    }

    int createdID = Wrapper::Packages::upload(new Package(nameInput->text().trimmed(), Repository::Default));
    if (createdID == -1) {
        close();
        return;
    }

    auto newPackage = Wrapper::Packages::get(createdID);
    for (auto & config : configs) {
        config->package = newPackage;
        if (!Wrapper::Files::upload(config)) {
            if (!Wrapper::Files::update(config)) {
                close();
                return;
            }
        }
    }
    close();
}
