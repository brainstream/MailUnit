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

#include <LibMailUnit/Api/Impl/StringList.h>

size_t MU_CALL muStringListLength(const MU_StringList * _string_list)
{
    return nullptr == _string_list ? 0 : _string_list->pointer()->size();
}

const char * MU_CALL muStringListString(const MU_StringList * _string_list, size_t _index)
{
    if(nullptr == _string_list)
        return nullptr;
    const std::vector<std::string> * ptr = _string_list->pointer();
    return ptr->size() < _index ? ptr->at(_index).c_str() : nullptr;
}
