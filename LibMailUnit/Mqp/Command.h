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

#ifndef __LIBMU_MQP_COMMAND_H__
#define __LIBMU_MQP_COMMAND_H__

#include <string>
#include <set>
#include <functional>
#include <boost/system/error_code.hpp>
#include <LibMailUnit/Mqp/Response.h>
#include <LibMailUnit/Mqp/Message.h>

namespace LibMailUnit {
namespace Mqp {

class Command;

class CommandExecutionObserver
{
public:
    virtual ~CommandExecutionObserver() { }
    virtual void onError(const Command & _command, const boost::system::error_code & _error) = 0;
    virtual void onResponseHeaderReceived(const Command & _command, const ResponseHeader & _header) = 0;
    virtual void onMessageReceived(const Command & _command, const Message & _message)  = 0;
    virtual void onFinished(const Command & _command) = 0;
}; // class CommandExecutionObserver

class Command : private boost::noncopyable
{
public:
    enum MqpOption : unsigned short
    {
        abortable  = 0x0001,
        compressed = 0x0010
    };

public:
    Command(const std::string & _query, unsigned short _options) :
        m_query(_query),
        m_options(_options)
    {
    }

    virtual ~Command()
    {
    }

    // TODO: copy constructor and so on...

    bool subscribe(CommandExecutionObserver & _observer)
    {
        return m_observers.insert(&_observer).second;
    }

    bool unsubscribe(CommandExecutionObserver & _observer)
    {
        return m_observers.erase(&_observer) > 0;
    }

    void callObservers(std::function<void(CommandExecutionObserver &)> _callback) const // FIXME: WTF? Where is encapsulation?
    {
        for(auto observer : m_observers)
            _callback(*observer);
    }

    const std::string & query() const
    {
        return m_query;
    }

    unsigned short options() const
    {
        return m_options;
    }

private:
    const std::string & m_query;
    const unsigned short m_options;
    std::set<CommandExecutionObserver *> m_observers;
}; // class Command

} // namespace Mqp
} // namespace LibMailUnit

#endif // __LIBMU_MQP_COMMAND_H__
