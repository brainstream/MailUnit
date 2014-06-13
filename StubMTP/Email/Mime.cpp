#include <sstream>
#include <StubMTP/Email/Mime.h>

using namespace StubMTP;
using namespace StubMTP::Email;

namespace {

    template<typename ResultT>
    class ValueExtractor
    {
    public:
        bool extract(const HeaderMap & _headers, const char * _key, ResultT & _result) const;

    protected:
        virtual bool convert(const std::string & _string, ResultT & _result) const = 0;
    }; // class ValueExtractor

    class StringExtractor : public ValueExtractor<std::string>
    {
    protected:
        inline bool convert(const std::string & _string, std::string & _result) const override;
    }; // class StringExtractor

    class AddressExtractor : public ValueExtractor<Address>
    {
    protected:
        inline bool convert(const std::string & _string, Address & _result) const override;
    }; // class AddressExtractor

    class AddressGroupExtractor : public ValueExtractor<AddressGroup>
    {
    protected:
        inline bool convert(const std::string & _string, AddressGroup & _result) const override;
    }; // class AddressGroupExtractor

    class DateTimeExtractor : public ValueExtractor<DateTimePtr>
    {
    protected:
        inline bool convert(const std::string & _string, DateTimePtr & _result) const override;
    }; // class DateTimeExtractor

} // namespace


template<typename ResultT>
bool ValueExtractor<ResultT>::extract(const HeaderMap & _headers,
    const char * _key, ResultT & _result) const
{
    HeaderMap::const_iterator it = _headers.find(_key);
    if(_headers.end() != it)
    {
        return convert(it->second, _result);
    }
    return false;
}

bool StringExtractor::convert(const std::string & _string, std::string & _result) const
{
    _result = _string;
    return true;
}

bool AddressExtractor::convert(const std::string & _string, Address & _result) const
{
    return parseAddress(_string, _result);
}

bool AddressGroupExtractor::convert(const std::string & _string, AddressGroup & _result) const
{
    return parseAddressGroup(_string, _result);
}

bool DateTimeExtractor::convert(const std::string & _string, DateTimePtr & _result) const
{
    _result = parseDateTime(_string);
    return nullptr != _result;
}


std::shared_ptr<Mime> StubMTP::Email::parseMime(const Smtp::Message & _message)
{
    std::shared_ptr<Mime> result = std::make_shared<Mime>();
    {
        std::stringstream data_stream(_message.data);
        parseHeaders(data_stream, result->all_headers);
    }
    StringExtractor string_extractor;
    AddressExtractor address_extractor;
    AddressGroupExtractor address_group_extractor;
    DateTimeExtractor date_time_extractor;
    string_extractor.extract(result->all_headers, HeaderKey::message_id, result->message_id);
    address_extractor.extract(result->all_headers, HeaderKey::sender, result->sender);
    address_group_extractor.extract(result->all_headers, HeaderKey::from, result->from);
    address_group_extractor.extract(result->all_headers, HeaderKey::to, result->to);
    address_group_extractor.extract(result->all_headers, HeaderKey::cc, result->cc);
    address_group_extractor.extract(result->all_headers, HeaderKey::bcc, result->bcc);
    address_group_extractor.extract(result->all_headers, HeaderKey::reply_to, result->reply_to);
    date_time_extractor.extract(result->all_headers, HeaderKey::date, result->date);
    // TODO: other headers
    // TODO: bcc from _message
    // TODO: interpret headers
    // TODO: contents
    return result;
}
