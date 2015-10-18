/***********************************************************************************************
 *                                                                                             *
 * This file is part of MailUnit Library.                                                      *
 *                                                                                             *
 * MailUnit Library is free software: you can redistribute it and/or modify it under the terms *
 * of the GNU Lesser General Public License as published by the Free Software Foundation,      *
 * either version 3 of the License, or (at your option) any later version.                     *
 *                                                                                             *
 * MailUnit Library is distributed in the hope that it will be useful, but WITHOUT ANY         *
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  *
 * PURPOSE. See the GNU Lesser General Public License for more details.                        *
 *                                                                                             *
 * You should have received a copy of the GNU License General Public License along with        *
 * MailUnit Library. If not, see <http://www.gnu.org/licenses>.                                *
 *                                                                                             *
 ***********************************************************************************************/

#ifndef __LIBMU_API_MESSAGE_MAILBOX_H__
#define __LIBMU_API_MESSAGE_MAILBOX_H__

#include <LibMailUnit/Api/ApiObject.h>
#include <LibMailUnit/Mail/Mailbox.h>
#include <Include/LibMailUnit/Message/Mailbox.h>

MU_DEFINE_API_TYPE(MU_Mailbox, LibMailUnit::Mail::Mailbox)
MU_DEFINE_API_TYPE(MU_MailboxGroup, LibMailUnit::Mail::MailboxGroup)

#endif // __LIBMU_API_MESSAGE_MAILBOX_H__
