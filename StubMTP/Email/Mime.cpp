#include <sstream>
#include <StubMTP/Email/Mime.h>

using namespace StubMTP;
using namespace StubMTP::Email;

Mime::Mime(const Smtp::Message & _message)
{
    initHeaderMap(_message);
    initFromHeaderMap(m_message_id, HeaderKey::message_id);
    initFromHeaderMap(m_subject, HeaderKey::subject);
    initFromHeaderMap(m_mime_version, HeaderKey::mime_version);
    initFromHeaderMap(m_sender, HeaderKey::sender);
    initFromHeaderMap(m_from, HeaderKey::from);
    initFromHeaderMap(m_to, HeaderKey::to);
    initFromHeaderMap(m_cc, HeaderKey::cc);
    initFromHeaderMap(m_bcc, HeaderKey::bcc);
    initFromHeaderMap(m_reply_to, HeaderKey::reply_to);
    initFromHeaderMap(m_date, HeaderKey::date);
    // TODO: other headers
    appendBccFromSmtpMessage(_message);
    // TODO: contents
}

void Mime::initHeaderMap(const Smtp::Message & _message)
{
    std::stringstream data_stream(_message.data);
    parseHeaders(data_stream, m_header_map);
}

void Mime::initFromHeaderMap(DateTimePtr & _date_time, const char * _key)
{
    std::string value;
    if(findHeaderValue(_key, value))
        _date_time = parseDateTime(value);
}

void Mime::initFromHeaderMap(AddressGroupPtr & _address_group, const char * _key)
{
    std::string value;
    if(findHeaderValue(_key, value))
        _address_group = AddressGroup::parse(value);
    if(nullptr == _address_group)
        _address_group = makeEmptyAddressGroupPtr();
}

void Mime::initFromHeaderMap(std::string & _string, const char * _key)
{
    findHeaderValue(_key, _string);
}

bool Mime::findHeaderValue(const char * _key, std::string & _result)
{
    HeaderMap::const_iterator it = m_header_map.find(_key);
    if(m_header_map.end() != it)
    {
        _result = it->second;
        return true;
    }
    return false;
}

void Mime::appendBccFromSmtpMessage(const Smtp::Message & _message)
{
    for(const std::string & raw_to: _message.to)
    {
        AddressPtr address = Address::parse(raw_to);
        if(address &&
           !m_to->containsMailbox(address->mailbox()) &&
           !m_cc->containsMailbox(address->mailbox()) &&
           !m_bcc->containsMailbox(address->mailbox()))
        {
            m_bcc->append(address);
        }
    }
}
