/************************************************************************************
*                                                                                   *
*   Copyright (c) 2014, 2015 - 2017 Axel Menzel <info@rttr.org>                     *
*                                                                                   *
*   This file is part of RTTR (Run Time Type Reflection)                            *
*   License: MIT License                                                            *
*                                                                                   *
*   Permission is hereby granted, free of charge, to any person obtaining           *
*   a copy of this software and associated documentation files (the "Software"),    *
*   to deal in the Software without restriction, including without limitation       *
*   the rights to use, copy, modify, merge, publish, distribute, sublicense,        *
*   and/or sell copies of the Software, and to permit persons to whom the           *
*   Software is furnished to do so, subject to the following conditions:            *
*                                                                                   *
*   The above copyright notice and this permission notice shall be included in      *
*   all copies or substantial portions of the Software.                             *
*                                                                                   *
*   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR      *
*   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,        *
*   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE     *
*   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER          *
*   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,   *
*   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE   *
*   SOFTWARE.                                                                       *
*                                                                                   *
*************************************************************************************/

#ifndef RTTR_ASSOCIATIVE_MAPPER_H_
#define RTTR_ASSOCIATIVE_MAPPER_H_

#include "rttr/detail/base/core_prerequisites.h"

namespace rttr
{

/*!
 * The \ref associative_container_mapper class is a class template to access an associative container via one common interface.
 *
 * This class will be only used internally by RTTR via the variant_associative_view class to get access to elements of an associative container.
 * In order to use your own custom associative container type, you have to provide a specialization of this class.
 *
 * Out of the box, RTTR has specialization for following associative container types:
 * - \p `std::set<Key>`
 * - \p `std::map<Key, T>`
 * - \p `std::multiset<Key>`
 * - \p `std::multimap<Key, T>`
 * - \p `std::unordered_set<Key>`
 * - \p `std::unordered_map<Key, T>`
 * - \p `std::unordered_multiset<Key>`
 * - \p `std::unordered_multimap<Key, T>`
 *
 * Custom associative container
 * -----------------------------
 * For a specialization of the class \ref rttr::associative_container_mapper<T> "associative_container_mapper<T>"
 * you have to provide some nested alias templates:
 * 1. `using container_t = T;`
 * 2. `using key_t       = typename T::key_type;`
 * 3. `using value_t     = typename T::mapped_type;` \remark When you have a key-only container, like `std::set<T>`, use as `void` as `value_t`
 * 4. `using itr_t       = typename T::iterator;`
 * 5. `using const_itr_t = typename T::const_iterator;`
 *
 * and following member functions:
 * 1. `static const key_t& get_key(const const_itr_t& itr);`
 * 2. `static value_t& get_value(itr_t& itr);`
 * 3. `static const value_t& get_value(const const_itr_t& itr);`
 * 4. `static itr_t begin(container_t& container);`
 * 5. `static const_itr_t begin(const container_t& container);`
 * 6. `static const_itr_t end(const container_t& container);`
 * 7. `static itr_t find(container_t& container, const key_t& key);`
 * 8. `static const_itr_t find(const container_t& container, const key_t& key);`
 * 9. `static std::pair<itr_t, itr_t> equal_range(container_t& container, const key_t& key);`
 * 10. `static std::pair<const_itr_t, const_itr_t> equal_range(const container_t& container, const key_t& key);`
 * 11. `static void clear(container_t& container);`
 * 12. `static std::size_t get_size(const container_t& container);`
 * 13. `static std::size_t erase(container_t& container, const key_t& key);`
 * 14. `static std::pair<itr_t, bool> insert_key(container_t& container, const key_t& key);`
 * 15. `static std::pair<itr_t, bool> insert_key_value(container_t& container, const key_t& key, const value_t& value);`
 *
 * Following code example for  <a target="_blank" href=http://doc.qt.io/qt-5/qhash.html>QHash<K, T></a>
 * illustrates how to add a specialization:
 *
 * \code{.cpp}
 * namespace rttr
 * {
 * template<typename K, typename T>
 * struct associative_container_mapper<QHash<K, T>>
 * {
 *     using container_t   = QHash<K, T>;
 *     using key_t         = typename QHash<K, T>::key_type;
 *     using value_t       = typename QHash<K, T>::mapped_type;
 *     using itr_t         = typename QHash<K, T>::iterator;
 *     using const_itr_t   = typename QHash<K, T>::const_iterator;
 *
 *     static const key_t& get_key(const const_itr_t& itr)
 *     {
 *         return itr.key();
 *     }
 *
 *     static value_t& get_value(itr_t& itr)
 *     {
 *         return itr.value();
 *     }
 *
 *     static const value_t& get_value(const const_itr_t& itr)
 *     {
 *         return itr.value();
 *     }
 *
 *     static itr_t begin(container_t& container)
 *     {
 *         return container.begin();
 *     }
 *
 *     static const_itr_t begin(const container_t& container)
 *     {
 *         return container.begin();
 *     }
 *
 *     static itr_t end(container_t& container)
 *     {
 *         return container.end();
 *     }
 *
 *     static const_itr_t end(const container_t& container)
 *     {
 *         return container.end();
 *     }
 *
 *     static itr_t find(container_t& container, key_t& key)
 *     {
 *         return container.find(key);
 *     }
 *
 *     static const_itr_t find(const container_t& container, key_t& key)
 *     {
 *         return container.find(key);
 *     }
 *
 *     static std::pair<itr_t, itr_t> equal_range(container_t& container, key_t& key)
 *     {
 *         auto ret = container.equal_range(key);
 *         return std::make_pair(ret.first, ret.second);
 *     }
 *
 *     static std::pair<const_itr_t, const_itr_t> equal_range(const container_t& container, key_t& key)
 *     {
 *         auto ret = container.equal_range(key);
 *         return std::make_pair(ret.first, ret.second);
 *     }
 *
 *     static void clear(container_t& container)
 *     {
 *         container.clear();
 *     }
 *
 *     static std::size_t get_size(const container_t& container)
 *     {
 *         return container.size();
 *     }
 *
 *     static std::size_t erase(container_t& container, key_t& key)
 *     {
 *         return container.remove(key);
 *     }
 *
 *     static std::pair<itr_t, bool> insert_key_value(container_t& container, key_t& key, value_t& value)
 *     {
 *         return std::make_pair(container.insert(key, value), true);
 *     }
 * };
 * } // end namespace rttr
 * \endcode
 *
 * \remark
 * Make sure you put your specialization inside the namespace `rttr`.
 * The best place for this code, is below the declaration of your custom associative container type.
 * When this is not possible, include your specialization code before registering your types to RTTR.
 */
template<typename T>
struct associative_container_mapper
{
#ifndef DOXYGEN
    using is_valid = std::false_type;
#else
    using container_t = T;
    using key_t       = typename T::key_type;
    using value_t     = typename T::mapped_type; //!< When you have a key only container use `void` as value type.
                                                 //!< Then you also dont need to add a insert_key_value() function
    using itr_t       = typename T::iterator;
    using const_itr_t = typename T::const_iterator;

    /////////////////////////////////////////////////////////////////////////////////////

    static const key_t& get_key(const const_itr_t& itr)
    {
        return itr->first;
    }

    /////////////////////////////////////////////////////////////////////////////////////

    static value_t& get_value(itr_t& itr)
    {
        return itr->second;
    }

    static const value_t& get_value(const const_itr_t& itr)
    {
        return itr->second;
    }

    /////////////////////////////////////////////////////////////////////////////////////

    static itr_t begin(container_t& container)
    {
        return container.begin();
    }

    static const_itr_t begin(const container_t& container)
    {
        return container.begin();
    }

    /////////////////////////////////////////////////////////////////////////////////////

    static itr_t end(container_t& container)
    {
        return container.end();
    }

    static const_itr_t end(const container_t& container)
    {
        return container.end();
    }

    /////////////////////////////////////////////////////////////////////////////////////

    static itr_t find(container_t& container, const key_t& key)
    {
        return container.find(key);
    }

    static const_itr_t find(const container_t& container, const key_t& key)
    {
        return container.find(key);
    }

    /////////////////////////////////////////////////////////////////////////////////////

    static std::pair<itr_t, itr_t> equal_range(container_t& container, const key_t& key)
    {
        return container.equal_range(key);
    }

    static std::pair<const_itr_t, const_itr_t> equal_range(const container_t& container, const key_t& key)
    {
        return container.equal_range(key);
    }

    /////////////////////////////////////////////////////////////////////////////////////

    static void clear(container_t& container)
    {
        container.clear();
    }

    static std::size_t get_size(const container_t& container)
    {
        return container.size();
    }

    static std::size_t erase(container_t& container, const key_t& key)
    {
        return container.erase(key);
    }

    /*!
     * This method is only necessary, when you have a key-only container. Like `std::set<T>`.
     * Otherwise you don't need to declare it.
     */
    static std::pair<itr_t, bool> insert_key(container_t& container, const key_t& key)
    {
        return { container.end(), false };
    }

    /*!
     * This method is only necessary, when you have a key-value container. Like `std::map<T>`.
     * Otherwise you don't need to declare it.
     */
    static std::pair<itr_t, bool> insert_key_value(container_t& container, const key_t& key, const value_t& value)
    {
        return container.insert(std::make_pair(key, value));
    }
#endif
};

} // end namespace rttr

#include "rttr/detail/impl/associative_mapper_impl.h"

#endif // RTTR_ASSOCIATIVE_MAPPER_H_
