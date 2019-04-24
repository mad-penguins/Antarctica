#ifndef ANTARCTICA_FILEDAO_H
#define ANTARCTICA_FILEDAO_H


#include <utility>

#include "DAO.h"
#include "../models/File.h"
#include "../models/User.h"

class FileDAO : public DAO<File> {
    User usr;

public:
    explicit FileDAO(User usr) : usr(std::move(usr)) {}

    QList<shared_ptr<File>> getAll() final;
    shared_ptr<File> find(uint) final;
    void add(shared_ptr<File>) final;
    void remove(shared_ptr<File>) final;
    void update(shared_ptr<File>) final;
};


#endif //ANTARCTICA_FILEDAO_H
