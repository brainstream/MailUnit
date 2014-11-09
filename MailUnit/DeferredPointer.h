/***********************************************************************************************
 *                                                                                             *
 * This file is part of MailUnit.                                                              *
 *                                                                                             *
 * MailUnit is free software: you can redistribute it and/or modify it under the terms of      *
 * the GNU General Public License as published by the Free Software Foundation,                *
 * either version 3 of the License, or (at your option) any later version.                     *
 *                                                                                             *
 * MailUnit is distributed in the hope that it will be useful,  but WITHOUT ANY WARRANTY;      *
 * without even the implied warranty of  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  *
 * See the GNU General Public License for more details.                                        *
 *                                                                                             *
 * You should have received a copy of the GNU General Public License along with MailUnit.      *
 * If not, see <http://www.gnu.org/licenses/>.                                                 *
 *                                                                                             *
 ***********************************************************************************************/

#ifndef __MU_DEFERRED_POINTER_H__
#define __MU_DEFERRED_POINTER_H__

#include <cstdlib>

namespace MailUnit {

template<typename Type>
class DeferredPointer
{
public:
    DeferredPointer() :
        mp_pointer(static_cast<Type *>(malloc(sizeof(Type)))),
        m_constructed(false)
    {
    }

    ~DeferredPointer()
    {
        destruct();
        free(mp_pointer);
    }

    template<typename... CtorArgs>
    void construct(CtorArgs... _ctor_args)
    {
        destruct();
        new(mp_pointer) Type(_ctor_args...);
        m_constructed = true;
    }

    void destruct()
    {
        if(m_constructed)
        {
            mp_pointer->~Type();
            m_constructed = false;
        }
    }

    bool constructed() const
    {
        return m_constructed;
    }

    operator bool ()
    {
        return m_constructed;
    }

    operator Type * ()
    {
        return get();
    }

    operator const Type * () const
    {
        return get();
    }

    const Type * get() const
    {
        return m_constructed ? mp_pointer : nullptr;
    }

    Type * get()
    {
        return m_constructed ? mp_pointer : nullptr;
    }

    const Type * unsafeGet() const
    {
        return mp_pointer;
    }

    Type * unsafeGet()
    {
        return mp_pointer;
    }

private:
    Type * mp_pointer;
    bool m_constructed;
}; // class DeferredPointer

} // namespace MailUnit

#endif // __MU_DEFERRED_POINTER_H__
