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

#ifndef __LIBMU_DEF_H__
#define __LIBMU_DEF_H__

#ifdef _WIN32
#   ifdef _MU_LIB
#       define MUAPI __declspec(dllexport) extern "C"
#   else
#       define MUAPI __declspec(dllimport) extern "C"
#   endif
#else
#   define MUAPI extern "C"
#endif

#define MU_UNUSED(var) (void)var

#if defined __cplusplus && __cplusplus >= 201103L
#   define MUCPP11
#endif // __cplusplus && __cplusplus >= 201103L

#ifdef MUCPP11
#   define MU_DISABLE_COPY(name)                    \
        name(const name &)                = delete; \
        name(name &&)                     = delete; \
        name & operator = (const name &)  = delete; \
        name & operator = (name &&)       = delete;

#   define MU_DEFAULT_COPY(name)                     \
        name(const name &)                = default; \
        name(name &&)                     = default; \
        name & operator = (const name &)  = default; \
        name & operator = (name &&)       = default;
#endif // MUCPP11

#endif // __LIBMU_DEF_H__
