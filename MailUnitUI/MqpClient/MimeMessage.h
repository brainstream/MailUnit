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

#ifndef __MUGUI_MQPCLIENT_MIMEMESSAGE_H__
#define __MUGUI_MQPCLIENT_MIMEMESSAGE_H__

#include <memory>
#include <vector>
#include <QObject>
#include <QString>
#include <Include/LibMailUnit/Message.h>

namespace MailUnit {
namespace Gui {

class MimeMessagePartPrivate;

class MimeMessagePart
{
protected:
    MimeMessagePart();

public:
    explicit MimeMessagePart(const MimeMessagePartPrivate * _private);
    MimeMessagePart(const MimeMessagePart & _part);
    MimeMessagePart(MimeMessagePart && _part);
    virtual ~MimeMessagePart();
    MimeMessagePart & operator = (const MimeMessagePart & _part);
    MimeMessagePart & operator = (MimeMessagePart && _part);
    const QByteArray & content() const;
    const std::vector<std::unique_ptr<const MimeMessagePart>> & parts() const;
    const QString & contentType() const;
    const QString & contentSubtype() const;

protected:
    void setup(const MimeMessagePartPrivate * _private);

private:
    const MimeMessagePartPrivate * mp_private;
}; // class MimeMessagePart

class MimeMessage : public MimeMessagePart
{
public:
    explicit MimeMessage(const QByteArray & _raw_data);
    MimeMessage(const MimeMessage &) = default;
    MimeMessage(MimeMessage &&) = default;
    ~MimeMessage() = default;
    MimeMessage & operator = (const MimeMessage &) = default;
    MimeMessage & operator = (MimeMessage &&) = default;

    const QString & subject() const
    {
        return m_subject;
    }

private:
    QString m_subject;
}; // class MimeMessage

} // namespace Gui
} // namespace MailUnit

#endif // __MUGUI_MQPCLIENT_MIMEMESSAGE_H__
