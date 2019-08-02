#include <QtCore/QTimer>
#include "FilesMonitor.h"
#include "Files.hpp"
#include "Settings.hpp"

using namespace Utils;

#pragma clang diagnostic push
#pragma ide diagnostic ignored "InfiniteRecursion"

void Checker::watch(const QList<File *> &fs) {
    if (!fs.empty()) {
        bool changedNow = false;
        for (auto &&file : fs) {
            if (!Files::actual(file)) {
                auto _tempFile = Files::parseFile(file->getAbsoluteName());
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
            for (auto &&file : checksums.keys()) {
                if (Files::updateFileContent(file)) {
                    qDebug() << "\tFile" << file->getRelativeName() << "updated successfully";
                }
            }
        }
    }

    switch (state) {
        case State::Active:
            QTimer::singleShot(1000, [=]() { watch(fs); });
            break;
        case State::Background:
            int backgroundFreq = 60'000;
            if (auto _f = Settings::readProperty("background", "freq").toInt(); _f != 0) {
                backgroundFreq *= _f;
            }
            QTimer::singleShot(backgroundFreq, [=]() { watch(fs); });
            break;
    }
}

#pragma clang diagnostic pop

FilesMonitor::FilesMonitor(const QList<File *> &fs, Checker::State state) {
    files = fs;
    startChecker(state);
}

FilesMonitor::~FilesMonitor() {
    monitorThread.quit();
    monitorThread.wait();
}

void FilesMonitor::startChecker(Checker::State state) {
    delete checker;

    checker = new Checker{state};
    checker->moveToThread(&monitorThread);

    connect(this, &FilesMonitor::operate, checker, &Checker::watch);
    connect(checker, &Checker::changed, this, &FilesMonitor::filesChanged);

    qRegisterMetaType<QList<File *>>("QList<File*>");
    emit operate(files);
    monitorThread.start();
}

void FilesMonitor::goActive() {
    monitorThread.quit();
    startChecker(Checker::State::Active);
}

void FilesMonitor::goBackground() {
    checker->goBackground();
}
