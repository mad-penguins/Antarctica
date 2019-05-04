#ifndef ANTARCTICA_APIWRAPPER_H
#define ANTARCTICA_APIWRAPPER_H


#include <QtCore/QString>

#include "../../models/File.h"
#include "../../models/Package.h"
#include "../../models/Repository.h"

class APIWrapper {
public:
    class Files {
        APIWrapper *parent;
    public:
        explicit Files(APIWrapper *parent) : parent(parent) {}

        static QList<File *> getAll();

        static File *get(unsigned id);

        static bool upload(const File &);

        static bool update(const File &);

        static bool remove(unsigned id);
    };

    class Packages {
        APIWrapper *parent;
    public:
        explicit Packages(APIWrapper *parent) : parent(parent) {}

        static QList<Package *> getAll();

        static Package *get(unsigned id);

        static bool upload(const Package &);

        static bool update(const Package &);

        static bool remove(unsigned id);
    };

    class Repositories {
        APIWrapper *parent;
    public:
        explicit Repositories(APIWrapper *parent) : parent(parent) {}

        static QList<Repository *> getAll();

        static Repository *get(unsigned id);

        static bool upload(const Repository &);

        static bool update(const Repository &);

        static bool remove(unsigned id);
    };

private:
    static unsigned id;
    static QString accessToken;

    Files filesWrapper;
    Packages packagesWrapper;
    Repositories repositories;

    APIWrapper() : filesWrapper(this), packagesWrapper(this), repositories(this) {}

};


#endif //ANTARCTICA_APIWRAPPER_H
