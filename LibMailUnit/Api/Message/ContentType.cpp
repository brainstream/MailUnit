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

#include <LibMailUnit/Memory.h>
#include <Include/LibMailUnit/Message/ContentType.h>
#include <LibMailUnit/Mail/ContentType.h>

using namespace LibMailUnit::Mail;

MU_MAIL_HEADER_CONTENT_TYPE MU_CALL muContentTypeParse(const char * _raw_content_type)
{
    ContentType * content_type = parseContentType(_raw_content_type).release();
    if(nullptr != content_type)
        return new MHandle(content_type, true);
    return MU_INVALID_HANDLE;
}

MBool MU_CALL muContentType(MU_MAIL_HEADER_CONTENT_TYPE _content_type, const char ** _type_out, const char ** _subtype_out)
{
    if(MU_INVALID_HANDLE == _content_type || nullptr == _content_type)
        return mfalse;
    ContentType * content_type = _content_type->pointer<ContentType>();
    *_type_out = content_type->type.c_str();
    *_subtype_out = content_type->subtype.c_str();
    return mtrue;
}

const size_t MU_CALL muContentTypeParamsCount(MU_MAIL_HEADER_CONTENT_TYPE _content_type)
{
    if(MU_INVALID_HANDLE == _content_type || nullptr == _content_type)
        return 0;
    return _content_type->pointer<ContentType>()->params.size();
}

MBool MU_CALL muContentTypeParam(MU_MAIL_HEADER_CONTENT_TYPE _content_type, size_t _index,
    const char ** _name_out, const char ** _value_out)
{
    if(MU_INVALID_HANDLE == _content_type || nullptr == _content_type)
        return mfalse;
    ContentTypeParam & param = _content_type->pointer<ContentType>()->params[_index];
    *_name_out = param.name.c_str();
    *_value_out = param.value.c_str();
    return mtrue;
}
