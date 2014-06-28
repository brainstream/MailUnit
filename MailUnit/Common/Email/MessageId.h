/***********************************************************************************************
 *                                                                                             *
 * This file is part of MailUnit Client Library.                                                *
 *                                                                                             *
 * MailUnit Client Library is free software: you can redistribute it and/or modify it under     *
 * the terms of the GNU Lesser General Public License as published by the Free Software        *
 * Foundation, either version 3 of the License, or (at your option) any later version.         *
 *                                                                                             *
 * MailUnit Client Library is distributed in the hope that it will be useful,                   *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS   *
 * FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.       *
 *                                                                                             *
 * You should have received a copy of the GNU License General Public License along with        *
 * MailUnit Client Library.  If not, see <http://www.gnu.org/licenses>.                         *
 *                                                                                             *
 ***********************************************************************************************/

#ifndef __MU_COMMON_MESSAGEID_H__
#define __MU_COMMON_MESSAGEID_H__

#include "../Def.h"
#ifdef __cplusplus
#   include <ostream>
#endif

struct MailUnit_MessageId;
typedef struct MailUnit_MessageId MailUnit_MessageId;

#ifdef __cplusplus
inline std::ostream & operator << (std::ostream & _stream, const MessageId & _message_id)
{

}
#endif // __cplusplus

#endif // __MU_COMMON_MESSAGEID_H__
