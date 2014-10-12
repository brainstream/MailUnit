#include <boost/algorithm/string.hpp>
#include <boost/iostreams/copy.hpp>
#include <MailUnit/Storage/Email.h>
#include <LibMailUnit/MailUnit.h>

using namespace MailUnit::Storage;
namespace fs = boost::filesystem;

namespace {

void collectAddressesFromHeader(MU_MAIL_HEADERLIST _headers, const char * _header_name,
    std::vector<std::string> & _collection)
{
    MU_MAIL_HEADER header = muMailHeaderByName(_headers, _header_name);
    if(MU_INVALID_HANDLE == header)
        return;
    size_t value_count = muMailHeaderValueCount(header);
    for(size_t value_index = 0; value_index < value_count; ++value_index)
    {
        const char * header_value = muMailHeaderValue(header, value_index);
        if(nullptr == header_value)
            continue;
        MU_MAILBOXGROUP group = muMailboxGroupParse(header_value);
        if(MU_INVALID_HANDLE == group)
            continue;
        size_t mailbox_count = muMailboxCount(group);
        for(size_t mailbox_index = 0; mailbox_index < mailbox_count; ++mailbox_index)
        {
            MU_MAILBOX mailbox = muMailbox(group, mailbox_index);
            if(MU_INVALID_HANDLE != mailbox)
                _collection.push_back(muMailboxAddress(mailbox));
        }
        muFree(group);
    }
    muFree(header);
}

} // namespace


Email::Email(RawEmail & _raw, boost::filesystem::path & _data_file_path)
{
    std::ifstream raw_stream(_raw.dataFilePath().string());
    std::fstream data(_data_file_path.string(), std::ios_base::out | std::ios_base::in | std::ios_base::trunc);
    boost::iostreams::copy(raw_stream, data);
    raw_stream.close();
    parseHeaders(data);
    data.close();
    appendBcc(_raw);
}

void Email::parseHeaders(std::fstream & _data)
{
    // TODO: parse

//    MU_MAIL_HEADERLIST headers = muMailHeadersParseString(_smtp_message.data.c_str());
//    if(MU_INVALID_HANDLE == headers)
//        return;
//    MU_MAIL_HEADER subject_header = muMailHeaderByName(headers, MU_MAILHDR_SUBJECT);
//    if(MU_INVALID_HANDLE != subject_header && muMailHeaderValueCount(subject_header) > 0)
//        m_subject = muMailHeaderValue(subject_header, 0);
//    m_data = _smtp_message.data;
//    collectAddressesFromHeader(headers, MU_MAILHDR_FROM, m_from_addresses);
//    collectAddressesFromHeader(headers, MU_MAILHDR_TO, m_to_addresses);
//    collectAddressesFromHeader(headers, MU_MAILHDR_CC, m_cc_addresses);
//    collectAddressesFromHeader(headers, MU_MAILHDR_BCC, m_bcc_addresses);
}

void Email::appendBcc(const RawEmail & _raw)
{
    // TODO: append

//    for(const std::string & raw_to: _smtp_message.to)
//    {
//        MU_MAILBOXGROUP group = muMailboxGroupParse(raw_to.c_str());
//        if(MU_INVALID_HANDLE == group)
//            continue;
//        size_t mailbox_count = muMailboxCount(group);
//        for(size_t i = 0; i < mailbox_count; ++i)
//        {
//            MU_MAILBOX mailbox = muMailbox(group, i);
//            if(MU_INVALID_HANDLE == mailbox)
//                continue;
//            const char * address = muMailboxAddress(mailbox);
//            if(nullptr == address)
//                continue;
//            std::string address_str(address);
//            if(AddressType::Invalid == containsAddress(address_str))
//                m_bcc_addresses.push_back(address_str);
//        }
//        muFree(group);
//    }
}

boost::optional<Email::AddressType> Email::containsAddress(const std::string & _address)
{
    auto predicate = [_address](const std::string & _other) {
        return boost::algorithm::iequals(_address, _other);
    };
    if(std::find_if(m_from_addresses.begin(), m_from_addresses.end(), predicate) != m_from_addresses.end())
        return AddressType::From;
    if(std::find_if(m_to_addresses.begin(), m_to_addresses.end(), predicate) != m_to_addresses.end())
        return AddressType::To;
    if(std::find_if(m_cc_addresses.begin(), m_cc_addresses.end(), predicate) != m_cc_addresses.end())
        return AddressType::Cc;
    if(std::find_if(m_bcc_addresses.begin(), m_bcc_addresses.end(), predicate) != m_bcc_addresses.end())
        return AddressType::Bcc;
    return nullptr;
}
