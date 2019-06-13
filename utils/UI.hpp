/*!
 * \file
 * \author Nikita Mironov <nickfrom22nd@gmail.com>
 * \brief Some GUI utilities
 *
 * \section LICENSE
 *
 * Copyright (c) 2019 Penguins of Madagascar

 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef ANTARCTICA_UI_HPP
#define ANTARCTICA_UI_HPP


#include <QtCore/QAbstractItemModel>

/*!
 * \namespace Utils
 * \brief Namespace which contains some utilities classes with static methods
 */
namespace Utils {
    /*!
     * \class Utils::UI
     * \brief Class with GUI utilities
     */
    class UI {
    public:
        /*!
         * \brief Get selected item in a entities tree and cast it to specific item object
         * \tparam ItemType Specific type of tree item to cast to
         * \tparam ModelType Specific type of a model
         * \param model An object of a model
         * \param index Current index got from Qt standard methods
         * \return
         */
        template<class ItemType, class ModelType>
        inline static ItemType *getCurrentItem(QAbstractItemModel *model, QModelIndex index) {
            return reinterpret_cast<ItemType *>(reinterpret_cast<ModelType *>(model)->getItem(index));
        }
    };
}

#endif //ANTARCTICA_UI_HPP
