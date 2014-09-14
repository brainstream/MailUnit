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

#ifndef __MU_STORAGE_DBOBJECT_H__
#define __MU_STORAGE_DBOBJECT_H__

#include <string>
#include <list>
#include <unordered_map>

namespace MailUnit {
namespace Storage {

class DBObject
{
public:
    inline void addField(const std::string & _name, const std::string & _value);

private:
    std::unordered_multimap<std::string, std::string> m_fields;
}; // class DBObject

class DBObjectSet
{
public:
    typedef typename std::list<DBObject>::iterator Iterator;
    typedef typename std::list<DBObject>::const_iterator ConstIterator;

public:
    inline DBObject & addObject();
    inline Iterator begin();
    inline ConstIterator cbegin();
    inline Iterator end();
    inline ConstIterator cend();

private:
    std::list<DBObject> m_objects;
}; // class DBObjectSet


void DBObject::addField(const std::string & _name, const std::string & _value)
{
    m_fields.insert({ _name, _value });
}

DBObject & DBObjectSet::addObject()
{
    m_objects.push_back(DBObject());
    return m_objects.back();
}

DBObjectSet::Iterator DBObjectSet::begin()
{
    return m_objects.begin();
}

DBObjectSet::ConstIterator DBObjectSet::cbegin()
{
    return m_objects.cbegin();
}

DBObjectSet::Iterator DBObjectSet::end()
{
    return m_objects.end();
}

DBObjectSet::ConstIterator DBObjectSet::cend()
{
    return m_objects.cend();
}

} // namespace Storage
} // namespace MailUnit


#endif // __MU_STORAGE_DBOBJECT_H__
