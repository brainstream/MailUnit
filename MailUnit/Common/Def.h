/***********************************************************************************************
 *                                                                                             *
 * This file is part of MailUnit Client Library.                                                *
 *                                                                                             *
 * MailUnit Client Library is free software: you can redistribute it and/or modify it under     *
 * the terms of the GNU Lesser General Public License as published by the Free Software        *
 * Foundation, either version 3 of the License, or (at your option) any later version.         *
 *                                                                                             *
 * MailUnit Client Library is distributed in the hope that it will be useful,                   *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS   *
 * FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.       *
 *                                                                                             *
 * You should have received a copy of the GNU License General Public License along with        *
 * MailUnit Client Library.  If not, see <http://www.gnu.org/licenses>.                         *
 *                                                                                             *
 ***********************************************************************************************/

#ifndef __MU_COMMON_DEF_H__
#define __MU_COMMON_DEF_H__

#include <stdint.h>

#ifdef _WIN32
#   ifdef MU
#       define STUBMPT_API __declspec(dllexport) extern "C"
#   else
#       define STUBMPT_API __declspec(dllimport) extern "C"
#   endif
#else
#   define STUBMPT_API extern "C"
#endif

#endif // __MU_COMMON_DEF_H__
