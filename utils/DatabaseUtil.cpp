#include "DatabaseUtil.h"

void DatabaseUtil::initDatabase(const User& user) {
    QSqlDatabase database = QSqlDatabase::addDatabase("QMYSQL");
    database.setHostName("165.22.64.118");
    database.setDatabaseName("id" + QString::number(user.id));
    database.setUserName("id" + QString::number(user.id));
    database.setPassword(user.password);
    database.open();
}
