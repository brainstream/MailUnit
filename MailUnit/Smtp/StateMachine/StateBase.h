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

#ifndef __MU_SMTP_STATEMACHINE_STATEBASE_H__
#define __MU_SMTP_STATEMACHINE_STATEBASE_H__

#include <string>
#include <boost/noncopyable.hpp>
#include <boost/optional.hpp>
#include <boost/msm/front/states.hpp>
#include <MailUnit/Exception.h>
#include <MailUnit/Smtp/ResponseCode.h>
#include <MailUnit/Smtp/Protocol.h>

namespace MailUnit {
namespace Smtp {

class StateException : public Excpetion
{
public:
    StateException(ResponseCode _response_code, const std::string & _message) :
        Excpetion(_message),
        m_response_code(_response_code)
    {
    }

    ResponseCode responseCode() const
    {
        return m_response_code;
    }

private:
    ResponseCode m_response_code;
}; // class StateException

class StateBase : private boost::noncopyable
{
public:
    virtual ~StateBase() { }
    virtual boost::optional<ResponseCode> processInput(const char * _data, Protocol & _protocol) = 0;
}; // class StateBase


class State : public boost::msm::front::state<StateBase>
{
public:
    template <typename EventT, typename FsmT>
    void on_entry(const EventT &, FsmT &)
    {
    }

    template <typename EventT, typename FsmT>
    void on_exit(const EventT &, FsmT &)
    {
        reset();
    }

protected:
    virtual void reset() = 0;
}; // class State


} // namespace Smtp
} // namespace MailUnit

#endif // __MU_SMTP_STATEMACHINE_STATEBASE_H__
