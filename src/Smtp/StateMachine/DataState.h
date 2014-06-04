#ifndef __STUBMTP_SMTP_STATEMACHINE_DATASTATE_H__
#define __STUBMTP_SMTP_STATEMACHINE_DATASTATE_H__

#include "StateBase.h"

namespace StubMTP {
namespace Smtp {


class DataState : public State
{
public:
    DataState() :
        m_header_accepted(false),
        m_data_accepted(false)
    {
    }

    void processInput(const std::string & _input)  override
    {
        // TODO: implement!
        STUBMTP_UNUSED(_input);
        if(m_header_accepted)
            m_data_accepted = true;
        else
            m_header_accepted = true;
    }

    bool isInutProcessingCompleted() const  override
    {
        // TODO: implement!
        return m_header_accepted && m_data_accepted;
    }

    bool isProtocolProcessingCompleted() const  override
    {
        return false;
    }

    void apply(Message & _message) const  override
    {
        // TODO: implement!
        STUBMTP_UNUSED(_message);
    }

    bool response(ResponseCode * _response) const override
    {
        // TODO: implement!
        *_response = m_data_accepted ? ResponseCode::Ok : ResponseCode::Intermediate;
        return true;
    }

private:
    bool m_header_accepted;
    bool m_data_accepted;
}; // class DataState


} // namespace Smtp
} // namespace StubMTP

#endif // __STUBMTP_SMTP_STATEMACHINE_DATASTATE_H__
