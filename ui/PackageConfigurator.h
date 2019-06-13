/*!
 * \file
 * \author Nikita Mironov <nickfrom22nd@gmail.com>
 * \brief The package configurator class
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
