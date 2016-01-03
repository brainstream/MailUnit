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

#include <LibMailUnit/Message/ContentType.h>
#include <LibMailUnit/Api/Impl/Message/ContentType.h>

using namespace LibMailUnit::Message;

MU_MailHeaderContentType * MU_CALL muContentTypeParse(const char * _raw_content_type)
{
    const ContentType * content_type = parseContentType(_raw_content_type).release();
    if(nullptr != content_type)
        return new MU_MailHeaderContentType(content_type, true);
    return nullptr;
}

MU_Bool MU_CALL muContentType(MU_MailHeaderContentType * _content_type, const char ** _type_out, const char ** _subtype_out)
{
    if(nullptr == _content_type)
        return mu_false;
    const ContentType * content_type = _content_type->pointer();
    *_type_out = content_type->type.c_str();
    *_subtype_out = content_type->subtype.c_str();
    return mu_true;
}

size_t MU_CALL muContentTypeParamsCount(MU_MailHeaderContentType * _content_type)
{
    if(nullptr == _content_type)
        return 0;
    return _content_type->pointer()->params.size();
}

MU_Bool MU_CALL muContentTypeParam(MU_MailHeaderContentType * _content_type, size_t _index,
    const char ** _name_out, const char ** _value_out)
{
    if(nullptr == _content_type)
        return mu_false;
    const ContentTypeParam & param = _content_type->pointer()->params[_index];
    *_name_out = param.name.c_str();
    *_value_out = param.value.c_str();
    return mu_true;
}
