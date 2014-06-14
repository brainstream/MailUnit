#include <StubMTP/Email/Header.h>

using namespace StubMTP::Email;

namespace {

class HeaderParser
{
private:
    inline HeaderParser(std::istream & _input, HeaderMap & _output);

public:
    static inline void parse(std::istream & _input, HeaderMap & _output);

private:
    void parse();
    bool parseLine(const std::string & _line);
    void pushPair();
    void cleanUp();

private:
    std::istream & mr_input;
    HeaderMap & mr_output;
    std::string m_current_key;
    std::string m_current_value;
}; // class HeaderParser

} // namespace

void StubMTP::Email::parseHeaders(std::istream & _input, HeaderMap & _output)
{
    HeaderParser::parse(_input, _output);
}

HeaderParser::HeaderParser(std::istream & _input, HeaderMap & _output) :
    mr_input(_input),
    mr_output(_output)
{
}

void HeaderParser::parse(std::istream & _input, HeaderMap & _output)
{
    HeaderParser(_input, _output).parse();
}

void HeaderParser::parse()
{
    std::string line;
    while(std::getline(mr_input, line))
    {
        boost::algorithm::trim_right(line);
        if(!parseLine(line))
            break;
    }
    cleanUp();
}

bool HeaderParser::parseLine(const std::string & _line)
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

void HeaderParser::pushPair()
{
    if(!m_current_key.empty() && !m_current_value.empty())
        mr_output[m_current_key] = m_current_value;
    cleanUp();
}

void HeaderParser::cleanUp()
{
    m_current_key.clear();
    m_current_value.clear();
}
