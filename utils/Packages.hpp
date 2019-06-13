/*!
 * \file
 * \author Nikita Mironov <nickfrom22nd@gmail.com>
 * \brief Some packages utilities
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

#ifndef ANTARCTICA_PACKAGES_HPP
#define ANTARCTICA_PACKAGES_HPP


#include <QtCore/QMap>
#include <QtCore/QDirIterator>
#include <QtCore/QDebug>

/*!
 * \namespace Utils
 * \brief Namespace which contains some utilities classes with static methods
 */
namespace Utils {
    /*!
     * \class Utils::Packages
     * \brief Class with packages utilities
     */
    class Packages {
        inline static bool detected = false;
    public:
        /*!
         * \enum Manager
         * \brief Supported package managers
         */
        inline static enum class Manager {
            apt, ///< apt-get (Debian-based distributions)
            zypper, ///< zypper (openSUSE)
            dnf, ///< dnf (Fedora, RHEL, CentOS)
            pacman, ///< pacman (Arch-based distros)
            unknown ///< Failed to detect or unsupported package manager
        } manager = Manager::unknown;

        /*!
         * \brief Get system package manager
         * \return Packages::Manager enum object
         */
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
                        manager = Manager::apt;
                    } else if (names.contains("opensuse") || names.contains("suse")) {
                        manager = Manager::zypper;
                    } else if (names.contains("fedora") || names.contains("rhel")) {
                        manager = Manager::dnf;
                    } else if (names.contains("arch") || names.contains("archlinux")) {
                        manager = Manager::pacman;
                    }
                }
                detected = true;
            }
            return manager;
        }

        /*!
         * \brief Get system package manager name
         * \return System package manager name
         */
        inline static const QString getManagerName() {
            return managers[getManager()];
        }

    private:
        /*!
         * \brief Mapped supported package managers to its names
         */
        inline static const QMap<Manager, QString> managers{
                {Manager::apt,     "apt"},
                {Manager::zypper,  "zypper"},
                {Manager::dnf,     "dnf"},
                {Manager::pacman,  "pacman"},
                {Manager::unknown, "unknown"}
        };
    };
}


#endif //ANTARCTICA_PACKAGES_HPP
