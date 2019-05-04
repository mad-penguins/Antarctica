#include "APIWrapper.h"

QList<Package *> APIWrapper::Packages::getAll() {
    return QList<Package *>();
}

Package *APIWrapper::Packages::get(unsigned id) {
    return nullptr;
}

bool APIWrapper::Packages::upload(const Package &) {
    return false;
}

bool APIWrapper::Packages::update(const Package &) {
    return false;
}

bool APIWrapper::Packages::remove(unsigned id) {
    return false;
}
