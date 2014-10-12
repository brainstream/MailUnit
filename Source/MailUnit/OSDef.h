/***********************************************************************************************
 *                                                                                             *
 * This file is part of MailUnit.                                                              *
 *                                                                                             *
 * MailUnit is free software: you can redistribute it and/or modify it under the terms of      *
 * the GNU General Public License as published by the Free Software Foundation,                *
 * either version 3 of the License, or (at your option) any later version.                     *
 *                                                                                             *
 * MailUnit is distributed in the hope that it will be useful,  but WITHOUT ANY WARRANTY;      *
 * without even the implied warranty of  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  *
 * See the GNU General Public License for more details.                                        *
 *                                                                                             *
 * You should have received a copy of the GNU General Public License along with MailUnit.      *
 * If not, see <http://www.gnu.org/licenses/>.                                                 *
 *                                                                                             *
 ***********************************************************************************************/

#ifndef __MU_OSDEF_H__
#define __MU_OSDEF_H__

#include <boost/filesystem/path.hpp>

namespace MailUnit {

#ifdef BOOST_WINDOWS_API
#   define MU_PATHISWIDECHAR
#   define MU_PATHSTR(str) L ## str;
#else
#   define MU_PATHSTR(str) str
#endif

typedef typename boost::filesystem::path::string_type PathString;

} // namespace MailUnit

#endif // __MU_OSDEF_H__
