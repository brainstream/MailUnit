/***********************************************************************************************
 *                                                                                             *
 * This file is part of the MailUnit Library.                                                  *
 *                                                                                             *
 * MailUnit Library is free software: you can redistribute it and/or modify it under the terms *
 * of the GNU Lesser General Public License as published by the Free Software Foundation,      *
 * either version 3 of the License, or (at your option) any later version.                     *
 *                                                                                             *
 * MailUnit Library is distributed in the hope that it will be useful, but WITHOUT ANY         *
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  *
 * PURPOSE. See the GNU Lesser General Public License for more details.                        *
 *                                                                                             *
 * You should have received a copy of the GNU License General Public License along with        *
 * MailUnit Library. If not, see <http://www.gnu.org/licenses>.                                *
 *                                                                                             *
 ***********************************************************************************************/

#ifndef __LIBMU_MQP_CLIENT_H__
#define __LIBMU_MQP_CLIENT_H__

#include <string>
#include <vector>
#include <map>
#include <LibMailUnit/Mqp/Command.h>

namespace LibMailUnit {
namespace Mqp {

class Client
{
private:
    class Session; // TODO: Why is it here
    struct CommandInfo;

public:
    Client(const std::string & _host, unsigned short _port);
    virtual ~Client();
    // TODO: copy constructor and so on...
    void executeCommand(const Command & _command);
    void abortCommand(const Command & _command);

private:
    std::string m_host;
    unsigned short m_port;
    std::map<const Command *, CommandInfo *> m_commands;
};

} // namespace Mqp
} // namespace LibMailUnit

#endif // __LIBMU_MQP_CLIENT_H__
