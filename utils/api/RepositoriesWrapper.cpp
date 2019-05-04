#include "APIWrapper.h"

QList<Repository *> APIWrapper::Repositories::getAll() {
    return QList<Repository *>();
}

Repository *APIWrapper::Repositories::get(unsigned id) {
    return nullptr;
}

bool APIWrapper::Repositories::upload(const Repository &) {
    return false;
}

bool APIWrapper::Repositories::update(const Repository &) {
    return false;
}

bool APIWrapper::Repositories::remove(unsigned id) {
    return false;
}
