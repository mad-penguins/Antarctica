#ifndef ANTARCTICA_PACKAGES_HPP
#define ANTARCTICA_PACKAGES_HPP


#include <QtCore/QMap>
#include <QtCore/QDirIterator>
#include <QtCore/QDebug>

namespace Utils {
    class Packages {
        inline static bool detected = false;
    public:
        inline static enum Manager {
            apt, zypper, dnf, pacman, unknown
        } manager = unknown;

        static const Manager getManager() {
            if (!detected) {
                QDirIterator it("/etc", QStringList() << "*-release", QDir::Files);
                QByteArray contents;
                while (it.hasNext()) {
                    QFile releaseFile(it.next());
                    if (releaseFile.open(QIODevice::ReadOnly)) {
                        contents.append(releaseFile.readAll());
                    }
                }
                if (QRegExp idRegexp(R"(ID_LIKE="(\w+)(\s|\w+)*")"); idRegexp.indexIn(contents) > 1) {
                    auto names = idRegexp.capturedTexts().mid(1);
                    if (names.contains("debian") || names.contains("ubuntu")) {
                        manager = apt;
                    } else if (names.contains("opensuse") || names.contains("suse")) {
                        manager = zypper;
                    } else if (names.contains("fedora") || names.contains("rhel")) {
                        manager = dnf;
                    } else if (names.contains("arch") || names.contains("archlinux")) {
                        manager = pacman;
                    }
                }
                detected = true;
            }
            return manager;
        }

        inline static const QString getManagerName() {
            return managers[getManager()];
        }

    private:
        inline static const QMap<Manager, QString> managers{
                {apt,     "apt"},
                {zypper,  "zypper"},
                {dnf,     "dnf"},
                {pacman,  "pacman"},
                {unknown, "unknown"}
        };
    };
}


#endif //ANTARCTICA_PACKAGES_HPP
