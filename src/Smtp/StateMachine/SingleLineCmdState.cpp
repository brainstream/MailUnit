#include "SingleLineCmdState.h"
#include "../ProtocolDef.h"

using namespace StubMTP::Smtp;

SingleLineCmdState::SingleLineCmdState(size_t args_pos) :
    m_args_pos(args_pos),
    m_state(ProcessResult::Incomplete)
{
}

SingleLineCmdState::ProcessResult SingleLineCmdState::internalProcessInput(const std::string & _input)
{
    if(ProcessResult::Incomplete != m_state)
    {
        return m_state;
    }
    size_t end_pos = _input.find(SMTP_NEWLINE);
    if(std::string::npos == end_pos)
    {
        m_data += _input;
    }
    else
    {
        m_data += _input.substr(0, end_pos);
        m_state = m_data.length() < m_args_pos + 1 ? ProcessResult::Error : ProcessResult::Success;
    }
    return m_state;
}

bool SingleLineCmdState::commandArgString(std::string & _result) const
{
    if(ProcessResult::Success != m_state)
        return false;
    _result = m_data.substr(m_args_pos);
    return true;
}

void SingleLineCmdState::reset()
{
    m_data.clear();
    m_state = ProcessResult::Incomplete;
}
