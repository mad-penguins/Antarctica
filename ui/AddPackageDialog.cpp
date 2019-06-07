#include <api/Wrapper.h>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QLabel>
#include <utils/Files.hpp>

#include "AddPackageDialog.h"
#include "ui/models/files/FileTreeModel.h"

AddPackageDialog::AddPackageDialog(QWidget *parent) : QDialog(parent) {
    initUI();
}

void AddPackageDialog::initUI() {
    setWindowTitle("Add a package");
    auto mainLay = new QVBoxLayout();

    auto nameLay = new QHBoxLayout();
    nameInput = new QLineEdit(this);
    nameLay->addWidget(new QLabel(tr("Name"), this));
    nameLay->addWidget(nameInput);
    mainLay->addLayout(nameLay);

    auto repoLay = new QHBoxLayout();
    repoSelector = new QComboBox(this);
    repoSelector->addItem("Default"); // TODO: implement system repositories scanning
    repoLay->addWidget(new QLabel(tr("Repository"), this));
    repoLay->addWidget(repoSelector);
    mainLay->addLayout(repoLay);

    mainLay->addWidget(new QLabel(tr("Configuration files:"), this));
    auto filesLay = new QHBoxLayout();
    filesTree = new QTreeView(this);
    updateFiles();
    filesLay->addWidget(filesTree);
    auto filesButtonLay = new QVBoxLayout();
    addFileButton = new QPushButton(tr("Add"));
    removeFileButton = new QPushButton(tr("Remove"));
    connect(addFileButton, &QPushButton::clicked, this, &AddPackageDialog::addFileClicked);
    connect(removeFileButton, &QPushButton::clicked, this, &AddPackageDialog::removeFileClicked);
    filesButtonLay->addWidget(addFileButton);
    filesButtonLay->addWidget(removeFileButton);
    filesButtonLay->addStretch();
    filesLay->addLayout(filesButtonLay);
    mainLay->addLayout(filesLay);

    auto buttonsLay = new QHBoxLayout();
    okButton = new QPushButton("OK");
    okButton->setDefault(true);
    cancelButton = new QPushButton(tr("Cancel"));
    connect(okButton, &QPushButton::clicked, this, &AddPackageDialog::okClicked);
    connect(cancelButton, &QPushButton::clicked, this, &AddPackageDialog::cancelClicked);
    buttonsLay->addStretch();
    buttonsLay->addWidget(okButton);
    buttonsLay->addWidget(cancelButton);
    mainLay->addLayout(buttonsLay);

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
