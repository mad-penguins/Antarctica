#ifndef ANTARCTICA_DATABASEUTIL_H
#define ANTARCTICA_DATABASEUTIL_H


#include <QtSql/QSqlDatabase>

#include "../models/User.h"

class DatabaseUtil {
public:
    static void initDatabase(const User&);
};


#endif //ANTARCTICA_DATABASEUTIL_H
