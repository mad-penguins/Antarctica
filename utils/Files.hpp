/*!
 * \file
 * \author Nikita Mironov <nickfrom22nd@gmail.com>
 * \brief Some files utilities
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

#ifndef ANTARCTICA_FILES_HPP
#define ANTARCTICA_FILES_HPP


#include <QtCore/QString>
#include <api/Wrapper.h>
#include <QtCore/QDir>
#include <api/models/Package.h>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <utime.h>
#include <fcntl.h>
#include <QtWidgets/QFileDialog>
#include <QtCore/QList>

#include "ui/models/files/FileTreeItem.h"

/*!
 * \namespace Utils
 * \brief Namespace which contains some utilities classes with static methods
 */
namespace Utils {
    /*!
     * \class Utils::Files
     * \brief Class with files utilities
     */
    class Files {
    public:
        /*!
         * \brief Wrap an existing file into an entity object
         * \param filename Full filepath
         * \return Generated file entity
         */
        static File *parseFile(const QString &filename) {
            QFile file(filename);
            if (file.open(QIODevice::ReadOnly)) {
                QFileInfo info(filename);
                auto name = info.fileName();
                auto path = info.absolutePath();
                auto created = info.birthTime();
                auto modified = info.lastModified();
                auto content = file.readAll();
                auto checksum = QCryptographicHash::hash((content), QCryptographicHash::Md5).toHex();
                return new File(name, path.replace(QDir::homePath(), "~"), checksum, created, modified, content);
            } else {
                return nullptr;
            }
        }

        /*!
         * \brief Wrap all files in a directory and its subdirectories into entity objects
         * \param [in] dirname Full path to directory
         * \param [out] files Container to append files
         */
        static void parseDir(const QString &dirname, QList<File *> &files) {
            auto dir = QDir(dirname);
            for (auto &&info : dir.entryInfoList(QDir::NoDotAndDotDot | QDir::Files | QDir::Dirs)) {
                if (info.isFile()) {
                    files.append(parseFile(info.absoluteFilePath()));
                } else if (info.isDir()) {
                    parseDir(info.absoluteFilePath(), files);
                }
            }
        }

        /*!
         * \brief Upload a file into server
         * \param filename Full filepath
         * \param pkg Package to associate with
         */
        static void uploadFile(const QString &filename, const Package *pkg = Package::Default) {
            QFile file(filename);
            if (file.open(QFile::ReadOnly)) {
                auto content = file.readAll();
                file.close();
                QFileInfo info(filename);
                Wrapper::Files::upload(new File(
                        info.fileName(),
                        info.absoluteDir().absolutePath().replace(QDir::homePath(), "~"),
                        QCryptographicHash::hash((content), QCryptographicHash::Md5).toHex(),
                        info.fileTime(QFile::FileTime::FileBirthTime),
                        info.fileTime(QFile::FileTime::FileModificationTime),
                        content.toBase64()
                ));
            }
        }

        /*!
         * \brief Upload all files from directory and its subdirectories into server
         * \param dirname Full path to directory
         * \param pkg Package to associate these files with
         */
        static void uploadDir(const QString &dirname, const Package *pkg = Package::Default) {
            auto dir = QDir(dirname);
            for (auto &&info : dir.entryInfoList(QDir::NoDotAndDotDot | QDir::Files | QDir::Dirs)) {
                if (info.isFile()) {
                    uploadFile(info.absoluteFilePath(), pkg);
                } else if (info.isDir()) {
                    uploadDir(info.absoluteFilePath(), pkg);
                }
            }
        }

        /*!
         * \brief Check file existence on local machine
         * \param f File entity object to check
         * \return true or false
         */
        inline static bool downloaded(const File *f) {
            return QFileInfo::exists(QString(f->path + "/" + f->name).replace('~', QDir::homePath()));
        }

        /*!
         * \brief Create a file on a local machine
         * \param f File entity object
         */
        static void createLocal(const File *f) {
            QDir dir;
            if (!dir.exists(QString(f->path).replace('~', QDir::homePath()))) {
                dir.mkpath(QString(f->path).replace('~', QDir::homePath()));
            }
            QFile file(QString(f->path + "/" + f->name).replace('~', QDir::homePath()));
            if (file.open(QIODevice::WriteOnly)) {
                file.write(Wrapper::Files::getContent(f->id));
                file.close();

                struct utimbuf timeBuffer{};
                timeBuffer.modtime = f->modified.toTime_t();
                const char *fileName = QString(f->path + "/" + f->name).replace('~', QDir::homePath()).toUtf8().data();
                if (utime(fileName, &timeBuffer) < 0) {
                    qDebug() << "Error changing time";
                }
            }
        }

        /*!
         * \brief Download a file from files tree row
         * \param item Files tree row
         */
        static inline void downloadFile(FileTreeItem *item) {
            if (!downloaded(item->getFile())) {
                createLocal(item->getFile());
            }
        }

        /*!
         * \brief Download a whole directory from files tree row
         * \param item Files tree row
         */
        static void downloadDir(FileTreeItem *item) {
            for (int i = 0; i < item->childCount(); ++i) {
                auto child = item->child(i);
                if (child->childCount() == 0) {
                    Utils::Files::createLocal(child->getFile());
                } else {
                    downloadDir(child);
                }
            }
        }

        /*!
         * \brief Check if file is up-to-date with server
         * \param f File entity object
         * \return Is file up-to-date or not
         */
        static bool actual(const File *f) {
            if (downloaded(f)) {
                QFile file(QString(f->path + "/" + f->name).replace('~', QDir::homePath()));
                if (file.open(QIODevice::ReadOnly)) {
                    auto local = file.readAll();
                    file.close();

                    auto localSum = QCryptographicHash::hash((local), QCryptographicHash::Md5).toHex();
                    return f->checksum == localSum;
                }
                return false;
            }
            return false;
        }

        /*!
         * \brief Select files
         * \param parent Parent widget
         * \param pkg Package to associate selected files with
         * \return List of selected filenames
         */
        inline static QStringList openFiles(QWidget *parent, Package *pkg = Package::Default) {
            QFileDialog dialog(parent);
            dialog.setFileMode(QFileDialog::ExistingFiles);
            dialog.setViewMode(QFileDialog::Detail);
            QStringList filenames;
            if (dialog.exec()) {
                return dialog.selectedFiles();
            }
            return QStringList();
        }

        /*!
         * \brief Remove files with specific directory from temporary file entity objects container
         * \param item Files tree row to delete
         * \param files File entity objects container
         */
        static void removeTempDir(FileTreeItem *item, QList<File *> &files) {
            for (int i = 0; i < item->childCount(); ++i) {
                auto child = item->child(i);
                if (child->childCount() == 0) {
                    files.removeOne(child->getFile());
                } else {
                    removeTempDir(child, files);
                }
                delete child;
            }
        }

        /*!
         * \brief Remove files with specific directory from server
         * \param item Files tree row to delete
         */
        static void removeServerDir(FileTreeItem *item) {
            for (int i = 0; i < item->childCount(); ++i) {
                auto child = item->child(i);
                if (child->childCount() == 0) {
                    Wrapper::Files::remove(child->getFile()->id);
                } else {
                    removeServerDir(child);
                }
            }
        }
    };
}


#endif //ANTARCTICA_FILES_HPP
