#include <sstream>
#include <algorithm>
#include <MailUnit/OS.h>
#include <MailUnit/Storage/Email.h>
#include <LibMailUnit/MailUnit.h>

using namespace MailUnit::Storage;
namespace fs = boost::filesystem;

namespace {

void collectAddressesFromHeader(MU_MAIL_HEADERLIST _headers, const char * _header_name,
    Email::AddressSet & _collection)
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
                _collection.insert(muMailboxAddress(mailbox));
        }
        muFree(group);
    }
    muFree(header);
}

} // namespace

Email::Email(uint32_t _id, const boost::filesystem::path & _data_file_path, bool _parse_file) :
    m_id(_id),
    m_data_file_path(_data_file_path)
{
    if(!fs::is_regular_file(m_data_file_path) && !fs::is_symlink(m_data_file_path))
    {
        std::stringstream message;
        message << "File not found: \"" << m_data_file_path.string() << "\"";
        throw StorageException(message.str());
    }
    if(_parse_file)
    {
        NativeFile file(m_data_file_path, open_nf_read);
        parseHeaders(file);
    }
}

Email::Email(const RawEmail & _raw, const boost::filesystem::path & _data_file_path) :
    m_id(new_object_id),
    m_data_file_path(_data_file_path)
{
    fs::copy(_raw.dataFilePath(), m_data_file_path);
    NativeFile file(m_data_file_path, open_nf_read);
    parseHeaders(file);
    appendBcc(_raw);
}

void Email::parseHeaders(MU_NATIVE_FILE _file)
{
    MU_MAIL_HEADERLIST headers = muMailHeadersParseFile(_file);
    if(MU_INVALID_HANDLE == headers)
        return;
    MU_MAIL_HEADER subject_header = muMailHeaderByName(headers, MU_MAILHDR_SUBJECT);
    if(MU_INVALID_HANDLE != subject_header && muMailHeaderValueCount(subject_header) > 0)
        m_subject = muMailHeaderValue(subject_header, 0);
    collectAddressesFromHeader(headers, MU_MAILHDR_FROM, m_from_addresses);
    collectAddressesFromHeader(headers, MU_MAILHDR_TO, m_to_addresses);
    collectAddressesFromHeader(headers, MU_MAILHDR_CC, m_cc_addresses);
    collectAddressesFromHeader(headers, MU_MAILHDR_BCC, m_bcc_addresses);
}

void Email::appendBcc(const RawEmail & _raw)
{
    for(const std::string & raw_to: _raw.toAddresses())
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
            if(!findAddress(address_str).is_initialized())
                m_bcc_addresses.insert(address_str);
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
        return AddressType::From;
    if(std::find_if(m_to_addresses.begin(), m_to_addresses.end(), predicate) != m_to_addresses.end())
        return AddressType::To;
    if(std::find_if(m_cc_addresses.begin(), m_cc_addresses.end(), predicate) != m_cc_addresses.end())
        return AddressType::Cc;
    if(std::find_if(m_bcc_addresses.begin(), m_bcc_addresses.end(), predicate) != m_bcc_addresses.end())
        return AddressType::Bcc;
    return nullptr;
}

const Email::AddressSet & Email::addresses(Email::AddressType _type) const
{
    switch(_type)
    {
    case AddressType::From:
        return m_from_addresses;
    case AddressType::To:
        return m_to_addresses;
    case AddressType::Cc:
        return m_cc_addresses;
    case AddressType::Bcc:
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
    case AddressType::From:
        addresses = &m_from_addresses;
        break;
    case AddressType::To:
        addresses = &m_to_addresses;
        break;
    case AddressType::Cc:
        addresses = &m_cc_addresses;
        break;
    case AddressType::Bcc:
        addresses = &m_bcc_addresses;
        break;
    }
    return addresses->insert(address).second;
}
