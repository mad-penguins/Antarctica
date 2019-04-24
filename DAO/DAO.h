#ifndef ANTARCTICA_DAO_H
#define ANTARCTICA_DAO_H


#include <QtCore/QList>
#include <memory>
#include <utility>

using namespace std;

class NotFoundException : public exception {
public:
    uint recordId;
    QString recordName;

    explicit NotFoundException(uint recordId) : recordId(recordId) {}
    explicit NotFoundException(QString recordName) : recordName(std::move(recordName)) {}
};

template <class Model> class DAO {
public:
    virtual QList<shared_ptr<Model>> getAll() = 0;
    virtual shared_ptr<Model> find(uint) = 0;
    virtual void add(shared_ptr<Model>) = 0;
    virtual void remove(shared_ptr<Model>) = 0;
    virtual void update(shared_ptr<Model>) = 0;
};


#endif //ANTARCTICA_DAO_H
