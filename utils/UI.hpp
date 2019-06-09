#ifndef ANTARCTICA_UI_HPP
#define ANTARCTICA_UI_HPP


#include <QtCore/QAbstractItemModel>

namespace Utils {
    class UI {
    public:
        template<class ItemType, class ModelType>
        inline static ItemType *getCurrentItem(QAbstractItemModel *model, QModelIndex index) {
            return reinterpret_cast<ItemType *>(reinterpret_cast<ModelType *>(model)->getItem(index));
        }
    };
}

#endif //ANTARCTICA_UI_HPP
