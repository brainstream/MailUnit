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

#ifndef __MU_EMAIL_DATETIME_H__
#define __MU_EMAIL_DATETIME_H__

#include <memory>
#include <string>
#include <boost/date_time.hpp>

/*
 * Most important RFC parts:
 *
 * RFC 5322 - Internet Message Format. Section 3.3 - Date and Time Specification
 *     http://tools.ietf.org/html/rfc5322#section-3.3
 */

namespace MailUnit {
namespace Email {

typedef boost::local_time::local_date_time DateTime;
typedef std::shared_ptr<DateTime> DateTimePtr;

DateTimePtr parseDateTime(const std::string & _input);

} // namespace Email
} // namespace MailUnit


#endif // __MU_EMAIL_DATETIME_H__
