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

#include <memory>
#include <functional>
#include <boost/filesystem/path.hpp>
#include <boost/program_options/options_description.hpp>
#include <MailUnit/Exception.h>
#include <MailUnit/Logger.h>

#define MU_MIN_THREAD_COUNT 1
#define MU_MAX_THREAD_COUNT 255

namespace MailUnit {

struct Config
{
    uint16_t thread_count;
    uint16_t smtp_port;
    bool use_smtp_starttls;
    boost::filesystem::path smtp_cert_path;
    boost::filesystem::path smtp_privet_key_path;
    std::string smtp_privet_key_pass;
    uint16_t mqp_port;
    boost::filesystem::path data_dirpath;
    bool use_stdlog;
    LogLevel log_level;
    boost::uintmax_t log_max_size;
    boost::filesystem::path log_filepath;
}; // struct Config

class ConfigLoadingException : public Excpetion
{
public:
    ConfigLoadingException(const std::string & _message,
            std::shared_ptr<boost::program_options::options_description> _options_description) :
        Excpetion(_message),
        m_options_description_ptr(_options_description)
    {
    }

    const boost::program_options::options_description & optionsDescription() const
    {
        return *m_options_description_ptr;
    }

private:
    std::shared_ptr<boost::program_options::options_description> m_options_description_ptr;
}; // class ConfigLoadingException

typedef std::function<void(std::shared_ptr<Config>)> ConfigLoadedCallback;
typedef std::function<void(const boost::program_options::options_description &)> PrintUsageCallback;

void loadConfig(int _argc, const char ** _argv, const boost::filesystem::path & _app_dir,
    ConfigLoadedCallback _config_loaded_callback, PrintUsageCallback _print_usage_callback);

} // namespace MailUnit

#endif // __MU_CONFIG_H__
