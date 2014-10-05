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

#ifndef __MU_STORAGE_DATABASE_H__
#define __MU_STORAGE_DATABASE_H__

#include <memory>
#include <boost/noncopyable.hpp>
#include <MailUnit/Email.h>
#include <MailUnit/Storage/DBObject.h>
#include <MailUnit/Storage/DatabaseException.h>

namespace MailUnit {
namespace Storage {

class Database : private boost::noncopyable
{
public:
    Database() { }
    virtual ~Database() { }
    virtual void storeEmail(const Email & _email) = 0;
    virtual std::unique_ptr<DBObjectSet> query(const std::string & _dsel_query) = 0;
}; // class Database

} // namespace Storage
} // namespace MailUnit


#endif // __MU_STORAGE_DATABASE_H__
