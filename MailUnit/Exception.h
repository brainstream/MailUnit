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

#ifndef __MU_EXCEPTION_H__
#define __MU_EXCEPTION_H__

#include <stdexcept>

namespace MailUnit {

class Excpetion : public std::runtime_error
{
public:
    Excpetion(const std::string & _message) :
        runtime_error(_message)
    {
    }
}; // class Excpetion


#define MU_EXCEPTION(name)                   \
    class name : public MailUnit::Excpetion  \
    {                                        \
    public:                                  \
        name(const std::string & _message) : \
            Excpetion(_message)              \
        {                                    \
        }                                    \
    };

} // namespace MailUnit


#endif // __MU_EXCEPTION_H__
