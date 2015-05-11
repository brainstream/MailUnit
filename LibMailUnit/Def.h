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
 * @brief Contains auxiliary definitions.
 */

#ifndef __LIBMU_DEF_H__
#define __LIBMU_DEF_H__

#include <stdlib.h>

/**
 * @cond HIDDEN
 */

#ifdef __APPLE__
#   error OX X is not supported yet
#endif


#ifdef __GNUC__
#   if (__GNUC__ < 4) || (__GNUC__ == 4 && __GNUC_MINOR__ < 9)
#       error g++ 4.9 or greater is required
#   endif
#elif __MINGW32__
#   if (__MINGW32_MAJOR_VERSION < 4) || (__MINGW32_MAJOR_VERSION == 4 && __MINGW32_MINOR_VERSION < 9)
#       error MinGW (or MinGW-w64) 4.9 or greater is required
#   endif
#endif


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
#   include <windows.h>
#   ifdef __cplusplus
#      ifdef _MU_LIB
#           define MU_EXPORT extern "C" __declspec(dllexport)
#       else
#           define MU_EXPORT extern "C" __declspec(dllimport)
#       endif
#   else
#      ifdef _MU_LIB
#           define MU_EXPORT __declspec(dllexport)
#       else
#           define MU_EXPORT __declspec(dllimport)
#       endif
#   endif
#   define MU_NATIVE_FILE HANDLE
#   define MU_INVALID_NATIVE_FILE INVALID_HANDLE_VALUE
#else
#   ifdef __cplusplus
#       define MU_EXPORT extern "C"
#   else
#       define MU_EXPORT
#   endif
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
    mfalse = 0, /**< The @a true value */
    mtrue  = 1  /**< The @a false value */
} MBool;

#endif // __LIBMU_DEF_H__
