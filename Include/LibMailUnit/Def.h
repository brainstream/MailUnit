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

/**
 * @cond HIDDEN
 */

#ifdef _WIN32
#   ifdef _MU_LIB
#       define MUAPI __declspec(dllexport) extern "C"
#   else
#       define MUAPI __declspec(dllimport) extern "C"
#   endif
#   define MU_NATIVE_FILE HFILE
#else
#   define MUAPI extern "C"
#   define MU_NATIVE_FILE int
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
