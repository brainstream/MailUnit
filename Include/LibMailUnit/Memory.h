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

/**
 * @file
 * @brief Common functions for memory allocation.
 */

#ifndef __LIBMU_MAIN_H__
#define __LIBMU_MAIN_H__

#include <string.h>
#include "Def.h"

/**
 * @brief Type of object to handle and manipulate a memory.
 * @sa muAlloc
 * @sa muFree
*/
typedef struct MUHandle * MU_HANDLE;

/**
 * @brief Object desctuctor function type.
 * @sa muAlloc
 */
typedef void (* MU_DESTRUCTOR)(void *);

/**
 * @brief Invalid handle value.
 */
#define MU_INVALID_HANDLE (MU_HANDLE)-1

/**
 * @brief Allocates memory for object.
 * @remarks
 *     Memory allocated by the @a muAlloc function must be released by the @ref muFree function.
 * @param _size
 *     Count of bytes to allocate.
 * @param _dtor
 *     Function that will be called by the @a muFree function before the memory will be released.
 * @return
 *     Object to handle and manipulate a memory.
 * @sa muFree
 */
MUAPI MU_HANDLE muAlloc(size_t _size, MU_DESTRUCTOR _dtor = NULL);

/**
 * @brief Wraps @a _pointer into handle.
 *
 * @param _pointer
 *     A pointer to wrap.
 * @return
 *     Handle contained a @a _pointer.
 * @remarks
 *     Calling the @ref muFree function is not required for handles created by this function.
 *     The @ref muFree function will do nothing with handles created by this functuin.
 */
MUAPI MU_HANDLE muWrapPointer(void * _pointer);

/**
 * @brief Releases a memory allocated by the @ref muAlloc function.
 * @sa muAlloc
 */
MUAPI void muFree(MU_HANDLE _handle);

#endif // __LIBMU_MAIN_H__
