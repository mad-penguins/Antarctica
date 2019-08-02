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
    public:
        enum class State {
            Background, Active
        } state;

        explicit Checker(State s) : state(s) {}

    public slots:

        void watch(const QList<File *> &fs);

        inline void goBackground() {
            state = State::Background;
        }

    signals:

        void changed(const QList<File *> &changes);
    };

    class FilesMonitor : public QObject {
    Q_OBJECT
    public:
        explicit FilesMonitor(const QList<File *> &fs, Checker::State state);

        ~FilesMonitor() override;

    private:
        QThread monitorThread;
        Checker *checker{};

        QList<File *> files{};

        void startChecker(Checker::State state);

    public slots:

        void goActive();

        void goBackground();

    signals:

        void operate(const QList<File *> &fs);

        void filesChanged(const QList<File *> &changes);


    };
}


#endif //ANTARCTICA_FILESMONITOR_H
