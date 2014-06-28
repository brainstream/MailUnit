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

#ifndef __MU_CONFIG_H__
#define __MU_CONFIG_H__

#include <ostream>
#include <boost/program_options.hpp>
#include <MailUnit/Aux.h>
#include <MailUnit/Logger.h>

namespace MailUnit {

class Config;

} // namespace MailUnit


std::ostream & operator << (std::ostream & _stream, const MailUnit::Config & _config);


namespace MailUnit {


class Config final
{
    friend std::ostream & ::operator << (std::ostream & _stream, const MailUnit::Config & _config);
    MU_DISABLE_COPY(Config)

public:
    Config(int argc, const char ** argv);
    ~Config();

    bool showHelp() const
    {
        return m_show_help;
    }

    uint16_t portNumber() const
    {
        return m_port_number;
    }

    bool logToStdout() const
    {
        return m_log_to_stdout;
    }

    boost::uintmax_t logMaxSize() const
    {
        return m_log_max_size;
    }

    std::string logFilename() const
    {
        return m_log_filename;
    }

    uint16_t threadCount() const
    {
        return m_thread_count;
    }

    LogLevel logLevel() const
    {
        return m_log_level;
    }

private:
    boost::program_options::options_description * mp_description;
    bool m_show_help;
    bool m_log_to_stdout;
    boost::uintmax_t m_log_max_size;
    std::string m_log_filename;
    uint16_t m_port_number;
    uint16_t m_thread_count;
    LogLevel m_log_level;
}; // class Config


} // namespace MailUnit



#endif // __MU_CONFIG_H__
