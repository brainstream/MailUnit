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

#ifndef __MU_APPLICATION_H__
#define __MU_APPLICATION_H__

#include <boost/noncopyable.hpp>
#include <boost/filesystem/path.hpp>
#include <MailUnit/Config.h>
#include <MailUnit/Logger.h>
#include <MailUnit/Exception.h>

namespace MailUnit {

MU_EXCEPTION(ApplicationException)

class Application : private boost::noncopyable
{
public:
    explicit Application(std::shared_ptr<const Config> _config);
    ~Application();

    const Config & config() const
    {
        return *m_config_ptr;
    }

    Logger & log()
    {
        return *mp_logger;
    }

private:
    std::shared_ptr<const Config> m_config_ptr;
    Logger * mp_logger;
}; // class Application

Application & app();

} // namespace MailUnit

#endif // __MU_APPLICATION_H__
