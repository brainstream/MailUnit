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

/**
 * @file
 * @brief API for working with the Content-Type message header.
 *
 * @anchor rfc-content-type-id
 * @htmlinclude RFC/ContentTypeSpec.html
*/

#ifndef __LIBMU_MESSAGE_CONTENTTYPE_H__
#define __LIBMU_MESSAGE_CONTENTTYPE_H__

#include "../Def.h"

#ifdef __cplusplus
extern "C" {
#endif

MU_DECLARE_HANDEL(MU_MAIL_HEADER_CONTENT_TYPE);

/**
 * @brief Parses string described in @ref rfc-content-type-id "RFC"
 * @param _raw_content_type
 *     String from a mail header.
 * @return
 *     Handle to a parsed ContentType or @a MU_INVALID_HANDLE
 *     The @a _content_type object will not be modified if parsing failed.
 * @remarks
 *     Returned handle must be destroyed by calling the @ref muFree function.
 */
MU_API MU_MAIL_HEADER_CONTENT_TYPE MU_CALL muContentTypeParse(const char * _raw_content_type);

/**
 * @brief Extracts type and subtype from the parsed ContentType
 * @param _content_type
 *     Parsed ContentType handle
 * @param _type_out
 *     Pointer to a pointer into which the @a type will be recorded
 * @param _subtype_out
 *     Pointer to a pointer into which the @a subtype will be recorded
 * @return
 *     @ref MBool::mtrue and @ref MBool::mfalse otherwise.
 * @sa muContentTypeParse
 */
MU_API MBool MU_CALL muContentType(MU_MAIL_HEADER_CONTENT_TYPE _content_type,
    const char ** _type_out, const char ** _subtype_out);

/**
 * @brief Extracts parameters count from the parsed ContentType
 * @param _content_type
 *     Parsed ContentType handle
 * @return
 *     A count of the ContentType parameters.
 * @sa muContentTypeParse
 */
MU_API const size_t MU_CALL muContentTypeParamsCount(MU_MAIL_HEADER_CONTENT_TYPE _content_type);

/**
 * @brief Extracts parameter from the parsed ContentType
 * @param _content_type
 *     Parsed ContentType handle
 * @param _index
 *     Zero-based index of parameter
 * @param _name_out
 *     Pointer to a pointer into which the @a name will be recorded
 * @param _value_out
 *     Pointer to a pointer into which the @a value will be recorded
 * @return
 *     @ref MBool::mtrue and @ref MBool::mfalse otherwise.
 * @sa muContentTypeParse
 */
MU_API MBool MU_CALL muContentTypeParam(MU_MAIL_HEADER_CONTENT_TYPE _content_type, size_t _index,
    const char ** _name_out, const char ** _value_out);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // __LIBMU_MESSAGE_CONTENTTYPE_H__
