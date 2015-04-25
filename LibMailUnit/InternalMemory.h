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
#include <boost/noncopyable.hpp>
#include <LibMailUnit/Def.h>
#include <LibMailUnit/Memory.h>

namespace LibMailUnit {

class Handle : private boost::noncopyable
{
public:
    explicit Handle(void * _pointer) :
        mp_pointer(_pointer)
    {
    }

    virtual ~Handle()
    {
    }

    void * pointer()
    {
        return mp_pointer;
    }

private:
    void * mp_pointer;
}; // class Handle

template<typename Type>
class DestructibleHandle : public Handle
{
public:
    explicit DestructibleHandle(Type * _pointer) :
        Handle(_pointer)
    {
    }

    ~DestructibleHandle() override
    {
        delete static_cast<Type *>(pointer());
    }
}; // class DestructibleHandle

template<typename Type>
inline MU_HANDLE wrapPointer(Type * _pointer, bool _destructible)
{
    return _destructible ? new DestructibleHandle<Type>(_pointer) : new Handle(_pointer);
}

template<typename Type, typename... CtorArgs>
inline MU_HANDLE makeObjectHandle(CtorArgs... _ctor_args)
{
    return new DestructibleHandle<Type>(new Type(_ctor_args...));
}

template<typename Type>
inline Type * handlePointer(MU_HANDLE _handle)
{
    return static_cast<Type *>(static_cast<Handle *>(_handle)->pointer());
}

} // namespace LibMailUnit

#endif // __LIBMU_INTERNALMEMORY_H__
