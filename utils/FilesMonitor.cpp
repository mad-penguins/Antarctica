#include <QtCore/QMutexLocker>
#include "FilesMonitor.h"
#include "Files.hpp"

using namespace Utils;

void Checker::watch(const QList<File *> &fs) {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
    while (true) {
        if (!fs.empty()) {
            bool changedNow = false;
            for (auto &&file : fs) {
                if (!Files::actual(file)) {
                    auto _tempFile = Files::parseFile(
                            QString(file->path + "/" + file->name).replace("~", QDir::homePath()));
                    auto actualChecksum = move(_tempFile->checksum);
                    delete _tempFile;

                    if (!checksums.contains(file)) {
                        changedNow = true;
                        checksums.insert(file, actualChecksum);
                    }
                    if (actualChecksum != checksums[file]) {
                        changedNow = true;
                        checksums[file] = actualChecksum;
                    }
                } else {
                    if (checksums.contains(file)) {
                        changedNow = true;
                        checksums.remove(file);
                    }
                }
            }
            if (changedNow) {
                emit changed(checksums.keys());
            }
        }
    }
#pragma clang diagnostic pop
}

FilesMonitor::FilesMonitor(const QList<File *> &fs) {
    auto checker = new Checker;
    checker->moveToThread(&monitorThread);
    connect(&monitorThread, &QThread::finished, checker, &QObject::deleteLater);
    connect(this, &FilesMonitor::operate, checker, &Checker::watch);
    connect(checker, &Checker::changed, this, &FilesMonitor::filesChanged);

    qRegisterMetaType<QList<File *>>("QList<File*>");
    emit operate(fs);
    monitorThread.start();
}

FilesMonitor::~FilesMonitor() {
    monitorThread.quit();
    monitorThread.wait();
}
