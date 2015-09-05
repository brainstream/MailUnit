/***********************************************************************************************
 *                                                                                             *
 * This file is part of MailUnit Library.                                                      *
 *                                                                                             *
 * MailUnit Library is free software: you can redistribute it and/or modify it under the terms *
 * of the GNU Lesser General Public License as published by the Free Software Foundation,      *
 * either version 3 of the License, or (at your option) any later version.                     *
 *                                                                                             *
 * MailUnit Library is distributed in the hope that it will be useful, but WITHOUT ANY         *
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  *
 * PURPOSE. See the GNU Lesser General Public License for more details.                        *
 *                                                                                             *
 * You should have received a copy of the GNU License General Public License along with        *
 * MailUnit Library. If not, see <http://www.gnu.org/licenses>.                                *
 *                                                                                             *
 ***********************************************************************************************/

#ifndef __LIBMUIMPL_MEMORY_H__
#define __LIBMUIMPL_MEMORY_H__

#include <stdexcept>
#include <functional>
#include <boost/any.hpp>
#include <boost/noncopyable.hpp>
#include <Include/LibMailUnit/Def.h>

struct MHandle : private boost::noncopyable
{
public:
    template<typename Type>
    MHandle(Type * _object, bool _destructible)
    {
        if(nullptr != _object)
            m_object = _object;
        if(_destructible)
        {
            m_destruct_funct = [this]() {
                delete boost::any_cast<Type *>(m_object);
            };
        }
    }

    ~MHandle()
    {
        if(m_destruct_funct && !m_object.empty())
        {
           m_destruct_funct();
        }
    }

    template<typename Type>
    Type * pointer() const
    {
        if(m_object.empty())
            return nullptr;
        Type * ptr = boost::any_cast<Type *>(m_object);
        if(nullptr == ptr)
            throw std::runtime_error("Unable to cast a handle pointer to a requested type");
        return ptr;
    }

private:
    boost::any m_object;
    std::function<void()> m_destruct_funct;
}; // struct MHandle

#endif // __LIBMUIMPL_MEMORY_H__
