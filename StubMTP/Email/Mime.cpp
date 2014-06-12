#include <sstream>
#include <StubMTP/Email/Mime.h>

using namespace StubMTP;
using namespace StubMTP::Email;

namespace {

class MimeHeaderParser
{
public:
    MimeHeaderMap && parse(std::istream & _input);

private:
    bool parseLine(const std::string & _line);
    void pushPair();
    void cleanUp();

private:
    MimeHeaderMap m_result;
    std::string m_current_key;
    std::string m_current_value;
}; // class MimeHeaderParser


} // namespace


std::shared_ptr<Mime> StubMTP::Email::parseMime(const Smtp::Message & _message)
{
    std::shared_ptr<Mime> result = std::make_shared<Mime>();
    std::stringstream data_stream(_message.data);
    result->all_headers = MimeHeaderParser().parse(data_stream);
    MimeHeaderMap::const_iterator from_it = result->all_headers.find(MimeHeaderKey::from);
    if(result->all_headers.end() != from_it)
        parseAddress(from_it->second, result->from);
    MimeHeaderMap::const_iterator to_it = result->all_headers.find(MimeHeaderKey::to);
    if(result->all_headers.end() != to_it)
        parseAddressGroup(to_it->second, result->to);
    MimeHeaderMap::const_iterator cc_it = result->all_headers.find(MimeHeaderKey::cc);
    if(result->all_headers.end() != cc_it)
        parseAddressGroup(cc_it->second, result->cc);
    MimeHeaderMap::const_iterator bcc_it = result->all_headers.find(MimeHeaderKey::bcc);
    if(result->all_headers.end() != bcc_it)
        parseAddressGroup(bcc_it->second, result->bcc);
    // TODO: other headers
    // TODO: bcc from _message
    // TODO: interpret headers
    // TODO: contents
    return result;
}

MimeHeaderMap && MimeHeaderParser::parse(std::istream & _input)
{
    std::string line;
    while(std::getline(_input, line))
    {
        boost::algorithm::trim_right(line);
        if(!parseLine(line))
            break;
    }
    cleanUp();
    return std::move(m_result);
}

bool MimeHeaderParser::parseLine(const std::string & _line)
{
    if(_line.empty())
    {
        pushPair();
        return false;
    }
    if(' ' == _line[0])
    {
        if(!m_current_key.empty())
            m_current_value += _line;
    }
    else
    {
        pushPair();
        size_t colon_pos = _line.find(':');
        if(std::string::npos != colon_pos)
        {
            m_current_key = _line.substr(0, colon_pos);
            m_current_value = boost::algorithm::trim_copy(_line.substr(colon_pos + 1));
        }
    }
    return true;
}

void MimeHeaderParser::pushPair()
{
    if(!m_current_key.empty() && !m_current_value.empty())
        m_result[m_current_key] = m_current_value;
    cleanUp();
}

void MimeHeaderParser::cleanUp()
{
    m_current_key.clear();
    m_current_value.clear();
}
