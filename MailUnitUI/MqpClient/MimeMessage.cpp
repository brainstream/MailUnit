/***********************************************************************************************
 *                                                                                             *
 * This file is part of MailUnit.                                                              *
 *                                                                                             *
 * MailUnit is free software: you can redistribute it and/or modify it under the terms of      *
 * the GNU General Public License as published by the Free Software Foundation,                *
 * either version 3 of the License, or (at your option) any later version.                     *
 *                                                                                             *
 * MailUnit is distributed in the hope that it will be useful,  but WITHOUT ANY WARRANTY;      *
 * without even the implied warranty of  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  *
 * See the GNU General Public License for more details.                                        *
 *                                                                                             *
 * You should have received a copy of the GNU General Public License along with MailUnit.      *
 * If not, see <http://www.gnu.org/licenses/>.                                                 *
 *                                                                                             *
 ***********************************************************************************************/

#include <MailUnitUI/MqpClient/MimeMessage.h>

using namespace MailUnit::Gui;

namespace MailUnit {
namespace Gui {

class MimeMessagePartPrivate final
{
public:
    explicit MimeMessagePartPrivate(const MU_MimeMessage * _message);
    explicit MimeMessagePartPrivate(const MU_MimePart * _part);
    MimeMessagePartPrivate(const MimeMessagePartPrivate & _private);
    MimeMessagePartPrivate(MimeMessagePartPrivate &&) = default;
    ~MimeMessagePartPrivate() = default;
    MimeMessagePartPrivate & operator = (const MimeMessagePartPrivate & _private);
    MimeMessagePartPrivate & operator = (MimeMessagePartPrivate &&) = default;

    const QByteArray & content() const
    {
        return m_content;
    }

    const QString & contentType() const
    {
        return m_content_type;
    }

    const QString & contentSubtype() const
    {
        return m_content_subtype;
    }

    const std::vector<std::unique_ptr<const MimeMessagePart>> & parts() const
    {
        return m_parts;
    }

private:
    void init(const MU_MimePart * _part);

private:
    QByteArray m_content;
    QString m_content_type;
    QString m_content_subtype;
    std::vector<std::unique_ptr<const MimeMessagePart>> m_parts;
}; // class MimeMessagePartPrivate

} // namespace Gui
} // namespace MailUnit

MimeMessagePartPrivate::MimeMessagePartPrivate(const MU_MimeMessage * _message)
{
    const MU_MimePart * part = muMimeToPart(_message);
    init(part);
    muFree(part);
}

MimeMessagePartPrivate::MimeMessagePartPrivate(const MU_MimePart * _part)
{
    init(_part);
}

void MimeMessagePartPrivate::init(const MU_MimePart * _part)
{
    const MU_MailHeaderContentType * content_type_hdr = muMimeContentType(_part);
    const char * content_type = nullptr;
    const char * content_subtype = nullptr;
    muContentType(content_type_hdr, &content_type, &content_subtype);
    muFree(content_type_hdr);
    m_content_type = content_type;
    m_content_subtype = content_subtype;
    if(m_content_type.compare("multipart", Qt::CaseInsensitive) != 0)
        //|| m_content_subtype.compare("alternative", Qt::CaseInsensitive) != 0)
    {
        m_content = muMimeContent(_part);
        return;
    }
    size_t part_count = muMimePartCount(_part);
    if(0 == part_count)
    {
        m_content = muMimeContent(_part);
        return;
    }
    for(size_t i = 0; i < part_count; ++i)
    {
        const MU_MimePart * child_part = muMimePart(_part, i);
        if(nullptr == child_part)
            continue;
        m_parts.push_back(std::make_unique<const MimeMessagePart>(new MimeMessagePartPrivate(child_part)));
        muFree(child_part);
    }
}

MimeMessagePartPrivate::MimeMessagePartPrivate(const MimeMessagePartPrivate & _private) :
    m_content(_private.m_content),
    m_content_type(_private.m_content_type),
    m_content_subtype(_private.m_content_subtype)
{
    m_parts.reserve(_private.m_parts.size());
    for(const std::unique_ptr<const MimeMessagePart> & part : _private.parts())
        m_parts.push_back(std::make_unique<const MimeMessagePart>(*part.get()));
}

MimeMessagePartPrivate & MimeMessagePartPrivate::operator = (const MimeMessagePartPrivate & _private)
{
    if(this == &_private) return *this;
    m_parts.clear();
    m_content = _private.m_content;
    m_content_type = _private.m_content_type;
    m_content_subtype = _private.m_content_subtype;
    m_parts.reserve(_private.m_parts.size());
    for(const std::unique_ptr<const MimeMessagePart> & part : _private.parts())
        m_parts.push_back(std::make_unique<const MimeMessagePart>(*part.get()));
    return *this;
}

MimeMessagePart::MimeMessagePart() :
    mp_private(nullptr)
{
}

MimeMessagePart::MimeMessagePart(const MimeMessagePartPrivate *_private) :
    mp_private(_private)
{
}

MimeMessagePart::MimeMessagePart(const MimeMessagePart & _part) :
    mp_private(new MimeMessagePartPrivate(*_part.mp_private))
{
}

MimeMessagePart::MimeMessagePart(MimeMessagePart && _part) :
    mp_private(_part.mp_private)
{
    _part.mp_private = nullptr;
}

MimeMessagePart::~MimeMessagePart()
{
    delete mp_private;
}

MimeMessagePart & MimeMessagePart::operator = (const MimeMessagePart & _part)
{
    if(this != &_part)
    {
        delete mp_private;
        mp_private = new MimeMessagePartPrivate(*_part.mp_private);
    }
    return *this;
}

MimeMessagePart & MimeMessagePart::operator = (MimeMessagePart && _part)
{
    if(this != &_part)
    {
        delete mp_private;
        mp_private = _part.mp_private;
        _part.mp_private = nullptr;
    }
    return *this;
}

void MimeMessagePart::setup(const MimeMessagePartPrivate * _private)
{
    delete mp_private;
    mp_private = _private;
}

const QByteArray & MimeMessagePart::content() const
{
    static const QByteArray empty_content;
    return mp_private ? mp_private->content() : empty_content;
}

const std::vector<std::unique_ptr<const MimeMessagePart>> & MimeMessagePart::parts() const
{
    if(mp_private)
        return mp_private->parts();
    else
        return std::move(std::vector<std::unique_ptr<const MimeMessagePart>>());
}

const QString & MimeMessagePart::contentType() const
{
    static const QString default_content_type;
    return mp_private ? mp_private->contentType() : default_content_type;
}

const QString & MimeMessagePart::contentSubtype() const
{
    static const QString default_content_subtype;
    return mp_private ? mp_private->contentSubtype() : default_content_subtype;
}

MimeMessage::MimeMessage(const QByteArray &_raw_data)
{
    const MU_MimeMessage * message = muMimeParseString(_raw_data);
    if(nullptr == message)
        return;
    setup(new MimeMessagePartPrivate(message));
    m_subject = muMimeSubject(message);
    muFree(message);
}
