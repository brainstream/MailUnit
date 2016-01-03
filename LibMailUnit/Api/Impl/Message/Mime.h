/***********************************************************************************************
 *                                                                                             *
 * This file is part of the MailUnit Library.                                                  *
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

#ifndef __LIBMU_APIIMPL_MESSAGE_MIME_H__
#define __LIBMU_APIIMPL_MESSAGE_MIME_H__

#include <LibMailUnit/Message/Mime.h>
#include <LibMailUnit/Api/Impl/ApiObject.h>
#include <LibMailUnit/Api/Include/Message/Mime.h>

MU_DEFINE_API_TYPE(MU_MimeMessage, const LibMailUnit::Message::MimeMessage)
MU_DEFINE_API_TYPE(MU_MimePart, const LibMailUnit::Message::MimePart)

#endif // __LIBMU_APIIMPL_MESSAGE_MIME_H__
