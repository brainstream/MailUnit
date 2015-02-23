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

#ifndef __MU_SMTP_PROTOCOLEXTENSTION_H__
#define __MU_SMTP_PROTOCOLEXTENSTION_H__

#include <string>
#include <ostream>

namespace MailUnit {
namespace Smtp {

enum class ProtocolExtenstionId
{
    startTls
}; // enum class ProtocolExtenstionId

class ProtocolExtenstion
{
public:
    virtual ~ProtocolExtenstion()
    {
    }

    virtual ProtocolExtenstionId id() const = 0;
    virtual void print(std::ostream & _stream) const = 0;
}; // class ProtocolExtenstion

class StartTlsProtocolExtenstion : public ProtocolExtenstion
{
public:
    StartTlsProtocolExtenstion()
    {
    }

    StartTlsProtocolExtenstion(const StartTlsProtocolExtenstion &) = default;
    StartTlsProtocolExtenstion & operator = (const StartTlsProtocolExtenstion &) = default;

    ProtocolExtenstionId id() const override
    {
        return ProtocolExtenstionId::startTls;
    }

    void print(std::ostream & _stream) const override
    {
        _stream << "STARTTLS";
    }
}; // class StartTlsProtocolExtenstion

} // namespace Smtp
} // namespace MailUnit

inline std::ostream & operator << (std::ostream & _stream, const MailUnit::Smtp::ProtocolExtenstion & _ext)
{
    _ext.print(_stream);
    return _stream;
}

#endif // __MU_SMTP_PROTOCOLEXTENSTION_H__
