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
#include <boost/noncopyable.hpp>
#include <boost/program_options.hpp>
#include <boost/filesystem/path.hpp>
#include <MailUnit/Logger.h>

namespace MailUnit {

struct Config
{
    boost::filesystem::path app_dir;
    bool show_help;
    bool use_stdlog;
    boost::uintmax_t log_max_size;
    boost::filesystem::path log_filepath;
    uint16_t smtp_port;
    uint16_t mqp_port;
    uint16_t thread_count;
    LogLevel log_level;
}; // struct Config


class ConfigLoader final : private boost::noncopyable
{
public:
    ConfigLoader(int _argc, const char ** _argv, const boost::filesystem::path & _app_dir);

    ~ConfigLoader();

    std::shared_ptr<const Config> config() const
    {
        return std::const_pointer_cast<const Config>(m_config_ptr);
    }

    void flush()
    {
        boost::program_options::notify(*mp_var_map);
    }

    void print(std::ostream & _stream) const
    {
        _stream << *mp_cmd_line_description;
    }

private:
    std::shared_ptr<Config> m_config_ptr;
    boost::program_options::variables_map * mp_var_map;
    boost::program_options::options_description * mp_cmd_line_description;
}; // class ConfigLoader

} // namespace MailUnit

inline std::ostream & operator << (std::ostream & _stream, const MailUnit::ConfigLoader & _config_loader)
{
    _config_loader.print(_stream);
    return _stream;
}

#endif // __MU_CONFIG_H__
