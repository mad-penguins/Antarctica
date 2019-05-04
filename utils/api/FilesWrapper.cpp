#include "APIWrapper.h"

QList<File *> APIWrapper::Files::getAll() {
    return QList<File *>();
}

File *APIWrapper::Files::get(unsigned id) {
    return nullptr;
}

bool APIWrapper::Files::upload(const File &) {
    return false;
}

bool APIWrapper::Files::update(const File &) {
    return false;
}

bool APIWrapper::Files::remove(unsigned id) {
    return false;
}
