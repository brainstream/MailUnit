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

#ifndef __MULIB_INTERNALMEMORY_H__
#define __MULIB_INTERNALMEMORY_H__

#include <LibMailUnit/Memory.h>

void * muPointer(MU_HANDLE _handle);

template<typename T>
inline T * muPointerT(MU_HANDLE _handle)
{
    return static_cast<T *>(muPointer(_handle));
}

#endif // __MULIB_INTERNALMEMORY_H__
