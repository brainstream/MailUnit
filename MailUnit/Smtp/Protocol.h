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

#include <string>
#include <functional>
#include <boost/noncopyable.hpp>
#include <boost/optional.hpp>
#include <MailUnit/Storage/Repository.h>
#include <MailUnit/Smtp/ProtocolDef.h>
#include <MailUnit/Smtp/ResponseCode.h>

namespace MailUnit {
namespace Smtp {

class ProtocolTransport
{
public:
    typedef std::function<void()> Action;

public:
    virtual ~ProtocolTransport() { }
    virtual void readRequest() = 0;
    virtual void writeRequest(const std::string & _data) = 0;
    virtual void switchToTlsRequest() = 0;
    virtual void exitRequest() = 0;

    void setAfterWriting(Action _action)
    {
        m_next_action = _action;
    }

protected:
    void afterWritingAction()
    {
        if(nullptr != m_next_action)
        {
            Action action = nullptr;
            std::swap(action, m_next_action);
            action();
        }
    }

private:
    Action m_next_action;
}; // class ProtocolTransport

class Protocol final : private boost::noncopyable
{
    struct Data;
public:
    Protocol(Storage::Repository & _repository, ProtocolTransport & _transport);
    ~Protocol();
    void processInput(const char * _data);
    Storage::RawEmail & email();
    void beginMessageData();
    void endMessageData();
    void storeEmail();
    void terminate();

private:
    void nextState(const char * _data);
    void continueState(const char * _data);
    void processResponseCode(const boost::optional<ResponseCode> & _code);

private:
    Data * mp_data;
}; // class Protocol

} // namespace Smtp
} // namespace MailUnit

#endif // __MU_SMTP_PROTOCOL_H__
