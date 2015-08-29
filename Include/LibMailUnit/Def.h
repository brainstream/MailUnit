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
 * @file Main and auxiliary definitions
 */

#ifndef __LIBMU_DEF_H__
#define __LIBMU_DEF_H__

#include <stddef.h>
#include <stdlib.h>

/**
 * @cond HIDDEN
 */

#if defined(_WIN32)
#   define MU_CALL __stdcall
#elif defined(__i386__) || defined(__i386) || defined(_X86_) || defined(__X86__)
#   ifdef __GNUC__
#       define MU_CALL __attribute__((__stdcall__)) __attribute__((__force_align_arg_pointer__))
#   else
#       error Curretn compiler is not supported yet
#   endif
#elif defined(__x86_64__) || defined(_M_X64)
#   define MU_CALL
#endif

#ifdef _WIN32
#   if !defined(NOMINMAX) && defined(_MU_DISABLE_NOT_STANDARD_CPP_API)
#       define NOMINMAX
#   endif
#   include <windows.h>
#   ifdef _MU_LIB
#       define MU_API __declspec(dllexport)
#   else
#       define MU_API __declspec(dllimport)
#   endif
#   define MU_NATIVE_FILE HANDLE
#   define MU_INVALID_NATIVE_FILE INVALID_HANDLE_VALUE
#else
#   define MU_API
#   define MU_NATIVE_FILE int
#   define MU_INVALID_NATIVE_FILE -1
#endif

#define MU_UNUSED(var) (void)var

/**
 * @endcond
 */

/**
 * @brief Boolean type
 */
typedef enum
{
    mfalse = 0, /**< The @a false value */
    mtrue  = 1  /**< The @a true value */
} MBool;

/**
 * @brief Type of object to handle and manipulate a memory.
 * @sa muFree
*/
typedef struct MHandle * MU_HANDLE;

#define MU_DECLARE_HANDEL(type) typedef MU_HANDLE type

/**
 * @brief Invalid handle value.
 */
#define MU_INVALID_HANDLE ((MU_HANDLE)-1)

/**
 * @brief Releases an allocated memory.
 */
MU_API void MU_CALL muFree(MU_HANDLE _handle);

#endif // __LIBMU_DEF_H__
