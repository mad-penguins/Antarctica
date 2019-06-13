/*!
 * \file
 * \author Nikita Mironov <nickfrom22nd@gmail.com>
 * \brief Some SSL utilities
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

#ifndef ANTARCTICA_REPOSITORIES_HPP
#define ANTARCTICA_REPOSITORIES_HPP


#include <QtCore/QMap>
#include <api/models/Repository.h>
#include <QtCore/QDir>

#include "Packages.hpp"

/*!
 * \namespace Utils
 * \brief Namespace which contains some utilities classes with static methods
 */
namespace Utils {
    /*!
     * \class Utils::Repositories
     * \brief Class with repositories utilities
     */
    class Repositories {
        inline static QMap<QString, Repository *> *systemRepos;
    public:
        /*!
         * \brief Get system repositories list
         * \param forced Force parse system repositories
         * \return Map of system repositories objects by name
         */
        static const QMap<QString, Repository *> *getSystemRepos(bool forced = false) { // only zypper now :(
            if (!systemRepos || forced) {
                systemRepos = new QMap<QString, Repository *>;
                systemRepos->insert("Default", Repository::Default);
                QDir reposDir("/etc/zypp/repos.d");
                if (reposDir.exists()) {
                    auto entries = reposDir.entryInfoList(QDir::NoDotAndDotDot | QDir::Files);
                    for (auto &&entry : entries) {
                        QFile repoConfig(entry.absoluteFilePath());
                        if (repoConfig.open(QIODevice::ReadOnly)) {
                            auto content = repoConfig.readAll().split('\n');
                            QString name;
                            QString url;
                            for (auto &&line : content) {
                                if (line.contains("name")) {
                                    name = line.mid(line.indexOf('=') + 1, -1);
                                }
                                if (line.contains("baseurl")) {
                                    url = line.mid(line.indexOf('=') + 1, -1);
                                }
                            }
                            if (!(name.isEmpty() && url.isEmpty())) {
                                systemRepos->insert(name, new Repository(name, url, Packages::getManagerName()));
                            }
                        }
                    }
                }
            }

            return systemRepos;
        }

        /*!
         * \brief Update ids of system repositories objects according to server repositories
         * \param forced Force parse system repositories
         */
        static void syncRepos(bool forced = false) {
            auto system = getSystemRepos(forced);
            auto server = Wrapper::Repositories::getAllMapped();

            for (auto &&repo : *system) {
                if (server.keys().contains(repo->name)) {
                    repo->id = server[repo->name]->id;
                }
            }
        }
    };
}

#endif //ANTARCTICA_REPOSITORIES_HPP
