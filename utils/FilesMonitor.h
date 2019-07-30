#ifndef ANTARCTICA_FILESMONITOR_H
#define ANTARCTICA_FILESMONITOR_H


#include <QtCore/QObject>
#include <QtCore/QThread>
#include <QtCore/QMutex>

#include <api/models/File.h>

namespace Utils {
    class Checker : public QObject {
    Q_OBJECT
        QMap<File *, QByteArray> checksums{};
    public slots:

        void watch(const QList<File *> &fs);

    signals:

        void changed(const QList<File *> &changes);
    };

    class FilesMonitor : public QObject {
    Q_OBJECT
        QThread monitorThread;
    public:
        explicit FilesMonitor(const QList<File *> &fs);

        ~FilesMonitor() override;

    signals:

        void operate(const QList<File *> &fs);

        void filesChanged(const QList<File *> &changes);


    };
}


#endif //ANTARCTICA_FILESMONITOR_H
