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

/**
 * @file
 * @brief Main and auxiliary definitions
 */

#ifndef __LIBMU_PUBAPI_DEF_H__
#define __LIBMU_PUBAPI_DEF_H__

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
#else
#   error Curretn compiler is not supported yet
#endif

#ifdef _WIN32
#   include <windows.h>
    typedef HANDLE MU_File;
#   define MU_INVALID_FILE INVALID_HANDLE_VALUE
#   if !defined(NOMINMAX) && defined(_MU_DISABLE_NOT_STANDARD_CPP_API)
#       define NOMINMAX
#   endif
#   ifdef _MU_LIB
#       define MU_API __declspec(dllexport)
#   else
#       define MU_API __declspec(dllimport)
#   endif
#else
    typedef int MU_File;
#   define MU_API
#   define MU_INVALID_FILE -1
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
    mu_false = 0, /**< The @a false value */
    mu_true  = 1  /**< The @a true value */
} MU_Bool;

#define MU_DECLARE_API_TYPE(type)   \
    struct __ ## type;              \
    typedef struct __ ## type type;

/**
 * @brief Releases an allocated memory.
 */
MU_API void MU_CALL muFree(void * _object);

#endif /* __LIBMU_PUBAPI_DEF_H__ */
