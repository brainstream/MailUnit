#include <boost/algorithm/string.hpp>
#include <MailUnit/Email.h>
#include <LibMailUnit/MailUnit.h>

using namespace MailUnit;

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

Email::Email(const Smtp::Message & _smtp_message)
{
    parseSmtpHeaders(_smtp_message);
    appendBccFromSmtpMessage(_smtp_message);
}

void Email::parseSmtpHeaders(const Smtp::Message & _smtp_message)
{
    MU_MAIL_HEADERLIST headers = muMailHeadersParseString(_smtp_message.data.c_str());
    if(MU_INVALID_HANDLE == headers)
        return;
    MU_MAIL_HEADER subject_header = muMailHeaderByName(headers, MU_MAILHDR_SUBJECT);
    if(MU_INVALID_HANDLE != subject_header && muMailHeaderValueCount(subject_header) > 0)
        m_subject = muMailHeaderValue(subject_header, 0);
    m_data = _smtp_message.data;
    collectAddressesFromHeader(headers, MU_MAILHDR_FROM, m_from_addresses);
    collectAddressesFromHeader(headers, MU_MAILHDR_TO, m_to_addresses);
    collectAddressesFromHeader(headers, MU_MAILHDR_CC, m_cc_addresses);
    collectAddressesFromHeader(headers, MU_MAILHDR_BCC, m_bcc_addresses);
}

void Email::appendBccFromSmtpMessage(const Smtp::Message & _smtp_message)
{
    for(const std::string & raw_to: _smtp_message.to)
    {
        MU_MAILBOXGROUP group = muMailboxGroupParse(raw_to.c_str());
        if(MU_INVALID_HANDLE == group)
            continue;
        size_t mailbox_count = muMailboxCount(group);
        for(size_t i = 0; i < mailbox_count; ++i)
        {
            MU_MAILBOX mailbox = muMailbox(group, i);
            if(MU_INVALID_HANDLE == mailbox)
                continue;
            const char * address = muMailboxAddress(mailbox);
            if(nullptr == address)
                continue;
            std::string address_str(address);
            if(AddressType::Invalid == containsAddress(address_str))
                m_bcc_addresses.push_back(address_str);
        }
        muFree(group);
    }
}

Email::AddressType Email::containsAddress(const std::string & _address)
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
    return AddressType::Invalid;
}
