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

#include <LibMailUnit/Memory.h>

struct MUHandle
{
    MUHandle(void * _pointer, bool _deletable) :
        pointer(_pointer),
        destructor(nullptr),
        deletable(_deletable)
    {
    }

    MUHandle(void * _pointer, MU_DESTRUCTOR _destructor, bool _deletable) :
        pointer(_pointer),
        destructor(_destructor),
        deletable(_deletable)
    {
    }

    void * pointer;
    MU_DESTRUCTOR destructor;
    bool deletable;
}; // struct MUHandle


namespace LibMailUnit {

template<typename Type, typename... CtorArgs>
inline MU_HANDLE makeHandle(CtorArgs... _ctor_args)
{
    MU_HANDLE handle = muAlloc(sizeof(Type), [](void * _pointer) {
        delete static_cast<Type *>(_pointer);
    });
    new(handle->pointer) Type(_ctor_args...);
    return handle;
}

template<typename Type>
inline Type * handlePointer(MU_HANDLE _handle)
{
    return static_cast<Type *>(_handle->pointer);
}

} // namespace LibMailUnit

#endif // __LIBMU_INTERNALMEMORY_H__
