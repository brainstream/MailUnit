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

#ifndef __STUBMTP_APPLICATION_H__
#define __STUBMTP_APPLICATION_H__

#include <boost/filesystem.hpp>
#include <StubMTP/Server/Aux.h>
#include <StubMTP/Server/Config.h>
#include <StubMTP/Server/Logger.h>
#include <StubMTP/Server/Exception.h>

namespace StubMTP {

STUBMTP_EXCEPTION(ApplicationException)

class Application
{
    STUBMTP_DISABLE_COPY(Application)

public:
    Application(int _argc, const char **_argv);
    ~Application();

    const Config & config() const
    {
        return *mp_config;
    }

    Logger & log()
    {
        return *mp_logger;
    }

    const boost::filesystem::path & startDir() const
    {
        return mr_start_dir;
    }

private:
    Config * mp_config;
    Logger * mp_logger;
    const boost::filesystem::path & mr_start_dir;
}; // class Application

Application & app();

} // namespace StubMTP

#endif // __STUBMTP_APPLICATION_H__
