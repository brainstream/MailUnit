#include <sstream>
#include <algorithm>
#include <MailUnit/OS/FileSystem.h>
#include <MailUnit/Storage/Email.h>
#include <Include/LibMailUnit/Message/MailHeader.h>
#include <Include/LibMailUnit/Message/Mailbox.h>
#include <Include/LibMailUnit/Message/DateTime.h>

using namespace MailUnit::Storage;
namespace fs = boost::filesystem;

namespace {

void collectAddressesFromHeader(const MU_MailHeaderList * _headers, const char * _header_name,
    Email::AddressSet & _collection)
{
    const MU_MailHeader * header = muMailHeaderByName(_headers, _header_name);
    if(nullptr == header)
        return;
    size_t value_count = muMailHeaderValueCount(header);
    for(size_t value_index = 0; value_index < value_count; ++value_index)
    {
        const char * header_value = muMailHeaderValue(header, value_index);
        if(nullptr == header_value)
            continue;
        const MU_MailboxGroup * group = muMailboxGroupParse(header_value);
        if(nullptr == group)
            continue;
        size_t mailbox_count = muMailboxCount(group);
        for(size_t mailbox_index = 0; mailbox_index < mailbox_count; ++mailbox_index)
        {
            const MU_Mailbox * mailbox = muMailbox(group, mailbox_index);
            if(nullptr != mailbox)
                _collection.insert(muMailboxAddress(mailbox));
            muFree(mailbox);
        }
        muFree(group);
    }
    muFree(header);
}

std::time_t getDateTimeFromHeaders(const MU_MailHeaderList * _headers)
{
    const MU_MailHeader * date_time_header = muMailHeaderByName(_headers, MU_MAILHDR_DATE);
    if(nullptr == date_time_header || muMailHeaderValueCount(date_time_header) == 0)
        return 0;
    const char * date_time_string = muMailHeaderValue(date_time_header, 0);
    muFree(date_time_header);
    if(nullptr == date_time_string)
        return 0;
    MU_DateTime date_time;
    if(!muDateTimeParse(date_time_string, &date_time))
        return 0;
    return muDateTimeToUnixTime(&date_time);
}

} // namespace

Email::Email(uint32_t _id, const boost::filesystem::path & _data_file_path, bool _parse_file) :
    m_id(_id),
    m_data_file_path(_data_file_path),
    m_sending_time(0)
{
    if(!fs::is_regular_file(m_data_file_path) && !fs::is_symlink(m_data_file_path))
    {
        std::stringstream message;
        message << "File not found: \"" << m_data_file_path.string() << "\"";
        throw StorageException(message.str());
    }
    if(_parse_file)
    {
        OS::File file(m_data_file_path, OS::file_open_read);
        parseHeaders(file);
    }
}

Email::Email(const RawEmail & _raw, const boost::filesystem::path & _data_file_path) :
    m_id(new_object_id),
    m_data_file_path(_data_file_path)
{
    fs::copy(_raw.dataFilePath(), m_data_file_path);
    OS::File file(m_data_file_path, OS::file_open_read);
    parseHeaders(file);
    appendFrom(_raw);
    appendBcc(_raw);
}

void Email::parseHeaders(MU_NATIVE_FILE _file)
{
    const MU_MailHeaderList * headers = muMailHeadersParseFile(_file);
    if(nullptr == headers)
        return;
    const MU_MailHeader * subject_header = muMailHeaderByName(headers, MU_MAILHDR_SUBJECT);
    if(nullptr != subject_header && muMailHeaderValueCount(subject_header) > 0)
        m_subject = muMailHeaderValue(subject_header, 0);
    muFree(subject_header);
    m_sending_time = getDateTimeFromHeaders(headers);
    collectAddressesFromHeader(headers, MU_MAILHDR_FROM, m_from_addresses);
    collectAddressesFromHeader(headers, MU_MAILHDR_TO, m_to_addresses);
    collectAddressesFromHeader(headers, MU_MAILHDR_CC, m_cc_addresses);
    collectAddressesFromHeader(headers, MU_MAILHDR_BCC, m_bcc_addresses);
    muFree(headers);
}

void Email::appendFrom(const RawEmail & _raw)
{
    for(const std::string raw_from : _raw.fromAddresses())
    {
        const MU_MailboxGroup * group = muMailboxGroupParse(raw_from.c_str());
        size_t mailbox_count = muMailboxCount(group);
        for(size_t i = 0; i < mailbox_count; ++i)
        {
            const MU_Mailbox * mailbox = muMailbox(group, i);
            if(nullptr == mailbox)
            {
                muFree(mailbox);
                continue;
            }
            const char * address = muMailboxAddress(mailbox);
            if(nullptr == address)
            {
                muFree(mailbox);
                continue;
            }
            std::string address_str(address);
            if(!containsAddress(address_str, AddressType::from))
                m_from_addresses.insert(address_str);
            muFree(mailbox);
        }
        muFree(group);
    }
}

void Email::appendBcc(const RawEmail & _raw)
{
    for(const std::string & raw_to: _raw.toAddresses())
    {
        const MU_MailboxGroup * group = muMailboxGroupParse(raw_to.c_str());
        if(nullptr == group)
            continue;
        size_t mailbox_count = muMailboxCount(group);
        for(size_t i = 0; i < mailbox_count; ++i)
        {
            const MU_Mailbox * mailbox = muMailbox(group, i);
            if(nullptr == mailbox)
            {
                muFree(mailbox);
                continue;
            }
            const char * address = muMailboxAddress(mailbox);
            if(nullptr == address)
            {
                muFree(mailbox);
                continue;
            }
            std::string address_str(address);
            if(!containsAddress(address_str, AddressType::to) && !containsAddress(address_str, AddressType::cc))
                m_bcc_addresses.insert(address_str);
            muFree(mailbox);
        }
        muFree(group);
    }
}

boost::optional<Email::AddressType> Email::findAddress(const std::string & _address)
{
    auto predicate = [_address](const std::string & _other) {
        return boost::algorithm::iequals(_address, _other);
    };
    if(std::find_if(m_from_addresses.begin(), m_from_addresses.end(), predicate) != m_from_addresses.end())
        return AddressType::from;
    if(std::find_if(m_to_addresses.begin(), m_to_addresses.end(), predicate) != m_to_addresses.end())
        return AddressType::to;
    if(std::find_if(m_cc_addresses.begin(), m_cc_addresses.end(), predicate) != m_cc_addresses.end())
        return AddressType::cc;
    if(std::find_if(m_bcc_addresses.begin(), m_bcc_addresses.end(), predicate) != m_bcc_addresses.end())
        return AddressType::bcc;
    return boost::optional<Email::AddressType>();
}

bool Email::containsAddress(const std::string & _address, AddressType _type) const
{
    const AddressSet & addrs = addresses(_type);
    auto predicate = [_address](const std::string & _other) {
        return boost::algorithm::iequals(_address, _other);
    };
    return addrs.cend() != std::find_if(addrs.cbegin(), addrs.cend(), predicate);
}

const Email::AddressSet & Email::addresses(Email::AddressType _type) const
{
    switch(_type)
    {
    case AddressType::from:
        return m_from_addresses;
    case AddressType::to:
        return m_to_addresses;
    case AddressType::cc:
        return m_cc_addresses;
    case AddressType::bcc:
        return m_bcc_addresses;
    default:
        throw StorageException("Invalid address type");
    }
}

bool Email::addAddress(Email::AddressType _type, const std::string & _address)
{
    std::string address = boost::trim_copy(_address);
    AddressSet * addresses = nullptr;
    switch(_type)
    {
    case AddressType::from:
        addresses = &m_from_addresses;
        break;
    case AddressType::to:
        addresses = &m_to_addresses;
        break;
    case AddressType::cc:
        addresses = &m_cc_addresses;
        break;
    case AddressType::bcc:
        addresses = &m_bcc_addresses;
        break;
    }
    return addresses->insert(address).second;
}
