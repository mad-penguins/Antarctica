#ifndef ANTARCTICA_REPOSITORIES_HPP
#define ANTARCTICA_REPOSITORIES_HPP


#include <QtCore/QMap>
#include <api/models/Repository.h>
#include <QtCore/QDir>

namespace Utils {
    class Repositories {
        inline static QMap<QString, Repository *> *systemRepos;
    public:
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
                                systemRepos->insert(name, new Repository(name, url, "zypper"));
                            }
                        }
                    }
                }
            }

            return systemRepos;
        }

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
