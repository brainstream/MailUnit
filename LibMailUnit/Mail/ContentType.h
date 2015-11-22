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

#ifndef __LIBMU_MAIL_CONTENTTYPE_H__
#define __LIBMU_MAIL_CONTENTTYPE_H__

#include <memory>
#include <string>
#include <vector>
#include <LibMailUnit/ApiObject.h>
#include <Include/LibMailUnit/Message/ContentType.h>

namespace LibMailUnit {
namespace Mail {

struct ContentTypeParam
{
    std::string name;
    std::string value;
}; // struct ContentTypeParam

struct ContentType
{
    std::string type;
    std::string subtype;
    std::vector<ContentTypeParam> params;
}; // struct ContentType

std::unique_ptr<ContentType> parseContentType(const std::string & _raw_content_type);

} // namespace Mail
} // namespace LibMailUnit

MU_DEFINE_API_TYPE(MU_MailHeaderContentType, const LibMailUnit::Mail::ContentType)

#endif // __LIBMU_MAIL_CONTENTTYPE_H__
