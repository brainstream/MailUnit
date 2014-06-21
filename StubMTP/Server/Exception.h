/***********************************************************************************************
 *                                                                                             *
 * This file is part of StubMTP.                                                               *
 *                                                                                             *
 * StubMTP is free software: you can redistribute it and/or modify it under the terms of       *
 * the GNU General Public License as published by the Free Software Foundation,                *
 * either version 3 of the License, or (at your option) any later version.                     *
 *                                                                                             *
 * StubMTP is distributed in the hope that it will be useful,  but WITHOUT ANY WARRANTY;       *
 * without even the implied warranty of  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  *
 * See the GNU General Public License for more details.                                        *
 *                                                                                             *
 * You should have received a copy of the GNU General Public License along with StubMTP.       *
 * If not, see <http://www.gnu.org/licenses/>.                                                 *
 *                                                                                             *
 ***********************************************************************************************/

#ifndef __STUBMTP_EXCEPTION_H__
#define __STUBMTP_EXCEPTION_H__

#include <stdexcept>

namespace StubMTP {

class Excpetion : public std::runtime_error
{
public:
    Excpetion(const std::string & _message) :
        runtime_error(_message)
    {
    }
}; // class Excpetion


#define STUBMTP_EXCEPTION(name)              \
    class name : public StubMTP::Excpetion   \
    {                                        \
    public:                                  \
        name(const std::string & _message) : \
            Excpetion(_message)              \
        {                                    \
        }                                    \
    };

} // namespace StubMTP


#endif // __STUBMTP_EXCEPTION_H__
