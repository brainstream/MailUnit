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

#include <typeinfo>
#include <stdexcept>
#include <functional>
#include <boost/noncopyable.hpp>
#include <Include/LibMailUnit/Def.h>

struct MHandle : private boost::noncopyable
{
private:
    template<typename Type>
    class Object { virtual ~Object() { } };

public:
    template<typename Type>
    MHandle(Type * _object, bool _destructible) :
        m_handle_object_type(typeid(MHandle::Object<Type>)),
        mp_object(_object)
    {
        if(_destructible)
        {
            m_destruct_funct = [this]() {
                delete static_cast<Type *>(mp_object);
            };
        }
    }

    ~MHandle()
    {
        if(m_destruct_funct)
        {
           m_destruct_funct();
        }
    }

    template<typename Type>
    Type * pointer() const
    {
        const std::type_info & typeinfo = typeid(MHandle::Object<Type>);
        if(m_handle_object_type != typeinfo)
        {
            throw std::runtime_error("Unable to cast a handle pointer to a requested type");
        }
        return static_cast<Type *>(mp_object);
    }

private:
    const std::type_info & m_handle_object_type;
    void * mp_object;
    std::function<void()> m_destruct_funct;
}; // struct MHandle

#endif // __LIBMUIMPL_MEMORY_H__
