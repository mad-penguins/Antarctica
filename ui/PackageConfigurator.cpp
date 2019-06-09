#include <QtWidgets/QMessageBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>

#include "PackageConfigurator.h"
#include "models/files/FileTreeModel.h"
#include "utils/Files.hpp"
#include "utils/Repositories.hpp"
#include "utils/UI.hpp"

PackageConfigurator::PackageConfigurator(QWidget *parent) : QDialog(parent) {
}

PackageConfigurator::PackageConfigurator(Package *pkg, QWidget *parent) : QDialog(parent) {
    package = pkg;
    configs = Wrapper::Packages::getConfigs(package->id);
    initUI();
}

void PackageConfigurator::updateFiles() {
    QStringList headers;
    headers << tr("Name") << tr("Created") << tr("Modified") << tr("Downloaded");
    auto model = new FileTreeModel(headers, configs);
    filesTree->setModel(model);
    connect(filesTree, &QTreeView::expanded, [=]() {
        filesTree->resizeColumnToContents(0);
    });
    filesTree->expandToDepth(1);
}

void PackageConfigurator::addFileClicked() {
    for (auto &&filename : Utils::Files::openFiles(this)) {
        QFileInfo info(filename);
        if (info.isFile()) {
            configs.append(Utils::Files::parseFile(filename));
        } else if (info.isDir()) {
            Utils::Files::parseDir(filename, configs);
        }
    }
    updateFiles();
}

void PackageConfigurator::removeFileClicked() {
    if (auto indexes = filesTree->selectionModel()->selectedIndexes(); !indexes.empty()) {
        int lastRow = -1;
        for (auto &&index : indexes) {
            if (lastRow != index.row()) {
                if (auto item = Utils::UI::getCurrentItem<FileTreeItem, FileTreeModel>(filesTree->model(), index);
                        item->childCount() == 0) {
                    configs.removeOne(item->getFile());
                } else {
                    Utils::Files::removeTempDir(item, configs);
                }
            }
            lastRow = index.row();
        }
        updateFiles();
    } else {
        QMessageBox::information(this, tr("Package selector"), tr("Please select a file to remove"));
    }
}

void PackageConfigurator::cancelClicked() {
    close();
}

void PackageConfigurator::initUI() {
    setWindowTitle(tr("Configure package"));
    auto mainLay = new QGridLayout(this);

    auto label = new QLabel(tr("Package") + " " + package->name, this);
    label->setStyleSheet("font-size: 20pt;");
    mainLay->addWidget(label, 1, 1, 1, 5);

    repoSelector = new QComboBox(this);
    repoSelector->addItems(Utils::Repositories::getSystemRepos()->keys());
    repoSelector->setCurrentIndex(repoSelector->findText(package->repository->name));
    mainLay->addWidget(new QLabel(tr("Repository"), this), 2, 1);
    mainLay->addWidget(repoSelector, 2, 2, 1, 4);

    mainLay->addWidget(new QLabel(tr("Configuration files:"), this), 3, 1);
    filesTree = new QTreeView(this);
    updateFiles();

    mainLay->addWidget(filesTree, 4, 1, 5, 5);
    addFileButton = new QPushButton(tr("Add"), this);
    removeFileButton = new QPushButton(tr("Remove"), this);

    connect(addFileButton, &QPushButton::clicked, this, &PackageConfigurator::addFileClicked);
    connect(removeFileButton, &QPushButton::clicked, this, &PackageConfigurator::removeFileClicked);
    mainLay->addWidget(addFileButton, 3, 2);
    mainLay->addWidget(removeFileButton, 3, 3);

    okButton = new QPushButton("OK", this);
    okButton->setDefault(true);
    cancelButton = new QPushButton(tr("Cancel"), this);
    connect(okButton, &QPushButton::clicked, this, &PackageConfigurator::okClicked);
    connect(cancelButton, &QPushButton::clicked, this, &PackageConfigurator::cancelClicked);
    mainLay->addWidget(okButton, 10, 5);
    mainLay->addWidget(cancelButton, 10, 4);
}

void PackageConfigurator::okClicked() {
    if (package->repository->id != Utils::Repositories::getSystemRepos()->value(repoSelector->currentText())->id) {
        if (!Wrapper::Repositories::getAllMapped().keys().contains(repoSelector->currentText())) {
            Wrapper::Repositories::upload(Utils::Repositories::getSystemRepos()->value(repoSelector->currentText()));
            Utils::Repositories::syncRepos();
        }
        package->repository = Utils::Repositories::getSystemRepos()->value(repoSelector->currentText());
    }

    for (auto &&config : Wrapper::Packages::getConfigs(package->id)) {
        if (!configs.contains(config)) {
            Wrapper::Files::remove(config->id);
        }
    }
    for (auto &config : configs) {
        config->package = package;
        if (!Wrapper::Files::upload(config)) {
            if (!Wrapper::Files::update(config)) {
                close();
                return;
            }
        }
    }
    close();
}
