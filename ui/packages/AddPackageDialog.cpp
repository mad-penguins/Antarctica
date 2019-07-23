/*!
 * \file
 * \author Nikita Mironov <nickfrom22nd@gmail.com>
 * \brief The new package dialog implementation
 *
 * \section LICENSE
 *
 * Copyright (c) 2019 Penguins of Madagascar

 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */


#include <api/Wrapper.h>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <utils/Files.hpp>
#include <QtWidgets/QMessageBox>

#include "AddPackageDialog.h"
#include "ui/models/files/FileTreeModel.h"
#include "utils/Repositories.hpp"

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
    repoSelector->addItems(Utils::Repositories::getSystemRepos()->keys());
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

    if (!Wrapper::Repositories::getAllMapped().keys().contains(repoSelector->currentText())) {
        Wrapper::Repositories::upload(Utils::Repositories::getSystemRepos()->value(repoSelector->currentText()));
        Utils::Repositories::syncRepos();
    }

    int createdID = Wrapper::Packages::upload(new Package(
            nameInput->text().trimmed(),
            Utils::Repositories::getSystemRepos()->value(repoSelector->currentText())
    ));

    if (createdID == -1) {
        close();
        return;
    }

    auto newPackage = Wrapper::Packages::get(createdID);
    for (auto &config : configs) {
        config->package = const_cast<Package *>(newPackage);
        if (!Wrapper::Files::upload(config)) {
            if (!Wrapper::Files::update(config)) {
                close();
                return;
            }
        }
    }
    close();
}
