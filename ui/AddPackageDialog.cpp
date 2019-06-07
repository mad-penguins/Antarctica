#include <api/Wrapper.h>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <utils/Files.hpp>

#include "AddPackageDialog.h"
#include "ui/models/files/FileTreeModel.h"

AddPackageDialog::AddPackageDialog(QWidget *parent) : QDialog(parent) {
    initUI();
}

void AddPackageDialog::initUI() {
    setWindowTitle("Add a package");
    auto mainLay = new QGridLayout();

    nameInput = new QLineEdit(this);
    mainLay->addWidget(new QLabel(tr("Name"), this), 1, 1);
    mainLay->addWidget(nameInput, 1, 2, 1, 5);

    repoSelector = new QComboBox(this);
    repoSelector->addItem("Default"); // TODO: implement system repositories scanning
    mainLay->addWidget(new QLabel(tr("Repository"), this), 2, 1, 1, 2);
    mainLay->addWidget(repoSelector, 2, 5, 1, 2);

    mainLay->addWidget(new QLabel(tr("Configuration files:"), this), 3, 1, 1, 2);
    filesTree = new QTreeView();
    updateFiles();

    mainLay->addWidget(filesTree, 4, 1, 4, 5);
    addFileButton = new QPushButton(tr("Add"));
    removeFileButton = new QPushButton(tr("Remove"));

    connect(addFileButton, &QPushButton::clicked, this, &AddPackageDialog::addFileClicked);
    connect(removeFileButton, &QPushButton::clicked, this, &AddPackageDialog::removeFileClicked);
    mainLay->addWidget(addFileButton, 4, 6);
    mainLay->addWidget(removeFileButton, 5, 6);

    okButton = new QPushButton("OK");
    okButton->setDefault(true);
    cancelButton = new QPushButton(tr("Cancel"));
    connect(okButton, &QPushButton::clicked, this, &AddPackageDialog::okClicked);
    connect(cancelButton, &QPushButton::clicked, this, &AddPackageDialog::cancelClicked);
    mainLay->addWidget(okButton, 9, 5);
    mainLay->addWidget(cancelButton, 9, 4);

    setLayout(mainLay);
}

void AddPackageDialog::updateFiles() {
    QStringList headers;
    headers << tr("Name") << tr("Created") << tr("Modified") << tr("Downloaded");
    auto model = new FileTreeModel(headers, configs);
    filesTree->setModel(model);
    connect(filesTree, &QTreeView::expanded, [=]() {
        filesTree->resizeColumnToContents(0);
    });
    filesTree->expandToDepth(1);
}

void AddPackageDialog::addFileClicked() {
    QStringList filenames = Utils::Files::openFiles(this);
    for (auto &&filename : filenames) {
        QFileInfo info(filename);
        if (info.isFile()) {
            configs.append(Utils::Files::parseFile(filename));
        } else if (info.isDir()) {
            Utils::Files::parseDir(filename, configs);
        }
    }
    updateFiles();
}

void AddPackageDialog::removeFileClicked() {
    QModelIndexList indexes = filesTree->selectionModel()->selectedIndexes();
    if (!indexes.empty()) {
        int lastRow = -1;
        for (auto &&index : indexes) {
            if (lastRow != index.row()) {
                auto item = reinterpret_cast<FileTreeItem*>(reinterpret_cast<FileTreeModel*>(filesTree->model())->getItem(index));
                configs.removeOne(item->getFile());
            }
            lastRow = index.row();
        }
    };
    updateFiles();
}

void AddPackageDialog::okClicked() {
    int createdID = Wrapper::Packages::upload(new Package(nameInput->text().trimmed(), Repository::Default));
    if (createdID == -1) {
        setResult(QDialog::Rejected);
        close();
        return;
    }

    auto newPackage = Wrapper::Packages::get(createdID);
    for (auto & config : configs) {
        config->package = newPackage;
        if (!Wrapper::Files::upload(config)) {
            if (!Wrapper::Files::update(config)) {
                setResult(QDialog::Rejected);
                close();
                return;
            }
        }
    }

    setResult(QDialog::Accepted);
    close();
}

void AddPackageDialog::cancelClicked() {
    setResult(QDialog::Accepted);
    close();
}
