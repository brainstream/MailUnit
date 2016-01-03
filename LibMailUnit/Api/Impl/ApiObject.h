/***********************************************************************************************
 *                                                                                             *
 * This file is part of the MailUnit Library.                                                  *
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

#ifndef __LIBMUIMPL_APIIMPL_APIOBJECT_H__
#define __LIBMUIMPL_APIIMPL_APIOBJECT_H__

#include <boost/noncopyable.hpp>
#include <LibMailUnit/Api/Include/Def.h>

namespace LibMailUnit {

class Object
{
protected:
    Object() { }

public:
    virtual ~Object() { }
}; // class Object

template<typename PointerType>
class ApiObject : public Object, private boost::noncopyable
{
public:
    ApiObject(PointerType * _pointer, bool _destructible) :
        mp_pointer(_pointer),
        m_is_destructible(_destructible)
    {
    }

    ~ApiObject() override
    {
        if(m_is_destructible)
           delete mp_pointer;
    }

    PointerType * pointer() const
    {
        return mp_pointer;
    }

private:
    PointerType * mp_pointer;
    bool m_is_destructible;
}; // class ApiObject

} // namespace LibMailUnit

#define MU_DEFINE_API_TYPE(type, ptr_type)                                         \
    struct __ ## type : public LibMailUnit::ApiObject<ptr_type>                    \
    {                                                                              \
    public:                                                                        \
        __ ## type(ptr_type * _pointer, bool _destructible) :                      \
            ApiObject(_pointer, _destructible)                                     \
        {                                                                          \
        }                                                                          \
    };

#endif // __LIBMUIMPL_APIIMPL_APIOBJECT_H__
