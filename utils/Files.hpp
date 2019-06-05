#ifndef ANTARCTICA_FILES_HPP
#define ANTARCTICA_FILES_HPP


#include <QtCore/QString>
#include <api/models/Package.h>
#include <QtCore/QFile>

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
    };
}


#endif //ANTARCTICA_FILES_HPP
