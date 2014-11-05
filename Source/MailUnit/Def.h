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

#ifndef __MU_DEF_H__
#define __MU_DEF_H__

#if !defined __cplusplus || __cplusplus < 201300L
#   error This project requires a C++14 compatible compiler!
#endif // __cplusplus && __cplusplus >= 201300L

#include <MailUnit/Logger.h>

namespace MailUnit {

extern Logger * const logger;

} // namespace MailUnit

#define LOG_INFO  ::MailUnit::LogWriter(*::MailUnit::logger, ::MailUnit::LogLevel::info)
#define LOG_WARN  ::MailUnit::LogWriter(*::MailUnit::logger, ::MailUnit::LogLevel::warning)
#define LOG_ERROR ::MailUnit::LogWriter(*::MailUnit::logger, ::MailUnit::LogLevel::error)

#endif // __MU_DEF_H__
