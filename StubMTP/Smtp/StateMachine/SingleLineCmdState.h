#ifndef __STUBMTP_SMTP_STATEMACHINE_SINGLELINECMDSTATE_H__
#define __STUBMTP_SMTP_STATEMACHINE_SINGLELINECMDSTATE_H__

#include <StubMTP/Smtp/StateMachine/StartState.h>

namespace StubMTP {
namespace Smtp {


class SingleLineCmdState : public State
{
protected:
    enum class ProcessResult
    {
        Error,
        Success,
        Incomplete
    }; // enum class ProcessResult

protected:
    SingleLineCmdState(size_t args_pos);
    ProcessResult internalProcessInput(const std::string & _input);
    bool commandArgString(std::string & _result) const;
    ProcessResult currentState() const { return m_state; }
    void reset() override;

private:
    const size_t m_args_pos;
    std::string m_data;
    ProcessResult m_state;
}; // class SingleLineCmdState


} // namespace Smtp
} // namespace StubMTP

#endif // __STUBMTP_SMTP_STATEMACHINE_SINGLELINECMDSTATE_H__
