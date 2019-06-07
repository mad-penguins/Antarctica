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

namespace Utils {
    class Files {
    public:
        static void uploadFile(const QString &filename, const Package *pkg = Package::Default) {
            auto file = new QFile(filename);
            if (file->open(QFile::ReadOnly)) {
                auto content = file->readAll();
                file->close();
                delete file;
                QFileInfo info(filename);
                Wrapper::Files::upload(new File(
                        info.fileName(),
                        info.absoluteDir().absolutePath().replace(QDir::homePath(), "~"),
                        info.fileTime(QFile::FileTime::FileBirthTime),
                        info.fileTime(QFile::FileTime::FileModificationTime),
                        content.toBase64()
                ));
            }
        }

        static File *parseFile(const QString &filename) {
            QFile file(filename);
            if (file.open(QIODevice::ReadOnly)) {
                QFileInfo info(filename);
                auto name = info.fileName();
                auto path = info.absolutePath();
                auto created = info.created();
                auto modified = info.lastModified();
                auto content = file.readAll();
                return new File(name, path.replace(QDir::homePath(), "~"), created, modified, content);
            } else {
                return nullptr;
            }
        }

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

        inline static bool isFileDownloaded(const File *f) {
            return QFileInfo::exists(QString(f->path + "/" + f->name).replace('~', QDir::homePath()));
        }

        static void createFile(const File *f) {
            QDir dir;
            if (!dir.exists(QString(f->path).replace('~', QDir::homePath()))) {
                dir.mkpath(QString(f->path).replace('~', QDir::homePath()));
            }
            QFile file(QString(f->path + "/" + f->name).replace('~', QDir::homePath()));
            if (file.open(QIODevice::WriteOnly)) {
                file.write(f->content);
                file.close();

                struct utimbuf timeBuffer{};
                timeBuffer.modtime = f->modified.toTime_t();
                const char *fileName = QString(f->path + "/" + f->name).replace('~', QDir::homePath()).toUtf8().data();
                if (utime(fileName, &timeBuffer) < 0) {
                    qDebug() << "Error changing time";
                }
            }
        }

        static inline void downloadFile(FileTreeItem *item) {
            if (!isFileDownloaded(item->getFile())) {
                createFile(item->getFile());
            }
        }

        static void downloadDir(FileTreeItem *item) {
            for (int i = 0; i < item->childCount(); ++i) {
                auto child = reinterpret_cast<FileTreeItem *>(item->child(i));
                if (child->childCount() == 0) {
                    Utils::Files::createFile(child->getFile());
                } else {
                    downloadDir(child);
                }
            }
        }

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
    };
}


#endif //ANTARCTICA_FILES_HPP
