#include <QtSql/QSqlQuery>
#include <QDebug>
#include "FileDAO.h"

QList<shared_ptr<File>> FileDAO::getAll() {
    QSqlQuery query;
    QList<shared_ptr<File>> result;
    query.exec("SELECT id, name, path, created, modified, package_id FROM files");
    while (query.next()) {
        result.append(make_shared<File>(
                query.value(0).toUInt(),
                query.value(1).toString(),
                query.value(2).toString(),
                query.value(3).toDateTime(),
                query.value(4).toDateTime(),
                query.value(5).toInt()
                ));
    }
    return result;
}

shared_ptr<File> FileDAO::find(uint id) {
    QSqlQuery query;
    query.prepare("SELECT id, name, path, created, modified, package_id FROM files WHERE id = ?");
    query.addBindValue(id);
    query.exec();
    while (query.next()) {
        return make_shared<File>(
                query.value(0).toUInt(),
                query.value(1).toString(),
                query.value(2).toString(),
                query.value(4).toDateTime(),
                query.value(5).toDateTime(),
                query.value(6).toInt()
        );
    }
    throw NotFoundException(id);
}

void FileDAO::add(shared_ptr<File> file) {
    QSqlQuery query;
    query.prepare("INSERT INTO files (id, name, path, content, created, modified, package_id) VALUES (?, ?, ?, ?, ?, ?, ?)");
    query.addBindValue(file->id);
    query.addBindValue(file->name);
    query.addBindValue(file->path);
    query.addBindValue(file->content);
    query.addBindValue(file->created);
    query.addBindValue(file->modified);
    query.addBindValue(file->package_id);
    query.exec();
}

void FileDAO::remove(shared_ptr<File> file) {
    QSqlQuery query;
    query.prepare("DELETE FROM files WHERE id = ?");
    query.addBindValue(file->id);
    query.exec();
}

void FileDAO::update(shared_ptr<File> file) {
    QSqlQuery query;
    query.prepare("UPDATE files SET name = ?, path = ?, content = ?, created = ?, modified = ?, package_id = ? WHERE id = ?");
    query.addBindValue(file->name);
    query.addBindValue(file->path);
    query.addBindValue(file->content);
    query.addBindValue(file->created);
    query.addBindValue(file->modified);
    query.addBindValue(file->package_id);
    query.addBindValue(file->id);
    query.exec();
}

