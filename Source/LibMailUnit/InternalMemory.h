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

#ifndef __LIBMU_INTERNALMEMORY_H__
#define __LIBMU_INTERNALMEMORY_H__

#include <cstdlib>
#include <LibMailUnit/Def.h>
#include <LibMailUnit/Memory.h>

struct MUHandle
{
    MU_DISABLE_COPY(MUHandle)

public:
    explicit MUHandle(void * _pointer) :
        mp_pointer(_pointer),
        m_destructor(nullptr),
        m_destructible(false)
    {
    }

    MUHandle(size_t _mem_size, MU_DESTRUCTOR _destructor) :
        mp_pointer(malloc(_mem_size)),
        m_destructor(_destructor),
        m_destructible(true)
    {
    }

    ~MUHandle()
    {
        if(nullptr != m_destructor)
        {
            m_destructor(mp_pointer);
        }
        if(m_destructible)
        {
            free(mp_pointer);
            mp_pointer = nullptr;
        }
    }

    void * pointer()
    {
        return mp_pointer;
    }

private:
    void * mp_pointer;
    MU_DESTRUCTOR m_destructor;
    bool m_destructible;
}; // struct MUHandle


namespace LibMailUnit {

inline MU_HANDLE wrapPointer(void * _pointer)
{
    return new MUHandle(_pointer);
}

template<typename Type, typename... CtorArgs>
inline MU_HANDLE makeObjectHandle(CtorArgs... _ctor_args)
{
    MU_HANDLE handle = muAlloc(sizeof(Type), [](void * _pointer) {
        static_cast<Type *>(_pointer)->~Type();
    });
    new(handle->pointer()) Type(_ctor_args...);
    return handle;
}

template<typename Type>
inline Type * handlePointer(MU_HANDLE _handle)
{
    return static_cast<Type *>(_handle->pointer());
}

} // namespace LibMailUnit

#endif // __LIBMU_INTERNALMEMORY_H__
