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

#include "Def.h"

/**
 * @brief Type of object to handle and manipulate a memory.
 * @sa muFree
*/
typedef struct MHandle * MU_HANDLE;

/**
 * @brief Invalid handle value.
 */
#define MU_INVALID_HANDLE (MU_HANDLE)-1

/**
 * @brief Releases an allocated memory.
 */
MUAPI void muFree(MU_HANDLE _handle);

#endif // __LIBMU_MAIN_H__
