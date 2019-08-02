#ifndef ANTARCTICA_SETTINGS_HPP
#define ANTARCTICA_SETTINGS_HPP


#include <QtCore/QVariant>
#include <QtCore/QSettings>

namespace Utils {
    class Settings {
    public:
        static QVariant readProperty(const QString &group, const QString &name) {
            QSettings settings("MadPenguins", "Antarctica");
            settings.beginGroup(group);
            QVariant val = settings.value(name);
            settings.endGroup();
            return val;
        }
    };
}

#endif //ANTARCTICA_SETTINGS_HPP
