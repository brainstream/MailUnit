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

#ifndef __MU_STRINGEX_H__
#define __MU_STRINGEX_H__

#include <boost/algorithm/string.hpp>

namespace MailUnit {

struct StringLessICompare
{
    template<typename Char>
    bool operator ()(const std::basic_string<Char> _left, const std::basic_string<Char> _right)
    {
        return boost::ilexicographical_compare(_left, _right);
    }
}; // struct StringLessICompare

} // namespace MailUnit

#endif // __MU_STRINGEX_H__
