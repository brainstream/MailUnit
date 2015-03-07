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

#ifndef __MU_SMTP_PROTOCOL_H__
#define __MU_SMTP_PROTOCOL_H__

#include <queue>
#include <string>
#include <boost/noncopyable.hpp>
#include <MailUnit/Smtp/Response.h>
#include <MailUnit/Storage/Repository.h>

namespace MailUnit {
namespace Smtp {

class ProtocolTransport
{
public:
    typedef std::function<void()> Action;

public:
    virtual ~ProtocolTransport() { }
    virtual void requestForRead() = 0;
    virtual void requestForWrite(const Response & _response) = 0;
    virtual void requestForSwitchToTls() = 0;
    virtual void requestForExit() = 0;

    void addNextAction(Action _action)
    {
        m_next_actions.push(_action);
    }

protected:
    bool callNextAction();

private:
    std::queue<Action> m_next_actions;
}; // class ProtocolTransport

class Protocol final : private boost::noncopyable
{
public:
    Protocol(Storage::Repository & _repository, ProtocolTransport & _transport);
    ~Protocol();
    void enableStartTls(bool _enable);
    void start() noexcept;
    void processInput(const char * _data, size_t _data_length) noexcept;

private:
    void resetData() noexcept;
    void extendData(const char * _data, size_t _data_length) noexcept;

private:
    class ProtocolImpl;
    ProtocolImpl * mp_impl;
    char * mp_data;
    size_t m_data_length;
}; // class Protocol

} // namespace Smtp
} // namespace MailUnit

#endif // __MU_SMTP_PROTOCOL_H__
