#ifndef __STUBMTP_SMTP_STATEMACHINE_DATASTATE_H__
#define __STUBMTP_SMTP_STATEMACHINE_DATASTATE_H__

#include <StubMTP/Smtp/StateMachine/StartState.h>

namespace StubMTP {
namespace Smtp {


class DataState : public State
{
public:
    DataState();

    void processInput(const std::string & _input, Message & _message)  override;

    bool isInutProcessingCompleted() const  override
    {
        return m_complete;
    }

    bool isProtocolProcessingCompleted() const  override
    {
        return false;
    }

    bool response(ResponseCode * _response) const override;
    void reset() override;

private:
    void processHeader(const std::string & _input);
    void processData(const std::string & _input, Message & _message);

private:
    bool m_header_accepted;
    bool m_complete;
    bool m_error;
    std::string m_data;
}; // class DataState


} // namespace Smtp
} // namespace StubMTP

#endif // __STUBMTP_SMTP_STATEMACHINE_DATASTATE_H__
