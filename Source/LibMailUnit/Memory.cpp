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

#include <cstdlib>
#include <LibMailUnit/InternalMemory.h>


MU_HANDLE muAlloc(size_t _size, MU_DESTRUCTOR _dtor /*= NULL*/)
{
    return new MUHandle(std::malloc(_size), _dtor, true);
}

void muFree(MU_HANDLE _handle)
{
    if(_handle->deletable && nullptr != _handle->destructor)
    {
        _handle->destructor(_handle->pointer);
    }
    std::free(_handle->pointer);
    delete _handle;
}
