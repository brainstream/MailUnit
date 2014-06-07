#include "DataState.h"
#include "../ProtocolDef.h"

#define COMMAND SMTP_CMD_DATA SMTP_NEWLINE

using namespace StubMTP::Smtp;

DataState::DataState() :
    m_header_accepted(false),
    m_complete(false),
    m_error(false)
{
}

void DataState::processInput(const std::string & _input, Message & _message)
{
    if(m_error)
        return;
    if(m_header_accepted)
        processData(_input, _message);
    else
        processHeader(_input);
}

void DataState::processHeader(const std::string & _input)
{
    const size_t cmd_len = sizeof(COMMAND) - 1;
    m_data += _input;
    if(m_data.length() < cmd_len)
        return;
    if(m_data.compare(0, cmd_len, COMMAND) == 0)
        m_header_accepted = true;
    else
        m_error = true;
}

void DataState::processData(const std::string & _input, Message & _message)
{
    size_t end_pos = _input.find(SMTP_DATA_END);
    if(std::string::npos == end_pos)
    {
        m_data += _input;
        return;
    }
    m_data += _input.substr(0, end_pos);
    m_complete = true;
    _message.body = m_data.substr(sizeof(COMMAND) - 1);
}

bool DataState::response(ResponseCode * _response) const
{
    if(m_error)
    {
        *_response = ResponseCode::InternalError;
        return true;
    }
    if(m_complete)
    {
        *_response = ResponseCode::Ok;
        return true;
    }
    else if(m_header_accepted)
    {
        *_response = ResponseCode::Intermediate;
        return true;
    }
    else
    {
        return false;
    }
}

void DataState::reset()
{
    m_complete = false;
    m_header_accepted = false;
    m_error = false;
    m_data.clear();
}
