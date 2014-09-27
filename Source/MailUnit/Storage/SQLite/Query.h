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

#ifndef __MU_STORAGE_SQLITE_QUERY_H__
#define __MU_STORAGE_SQLITE_QUERY_H__

#include <sstream>
#include <array>
#include <tuple>
#include <type_traits>
#include <boost/lexical_cast.hpp>

namespace MailUnit {
namespace Storage {
namespace SQLiteQuery {

template<typename T>
inline std::string quoteValue(const T & _value)
{
    return boost::lexical_cast<std::string>(_value);
}

inline std::string quoteValue(const std::string & _value)
{
    return std::string("'") + _value + '\'';
}

template<size_t index = 0, typename... Types>
inline auto makeValueList(const std::tuple<Types...> & _tuple, std::stringstream & _out) ->
    typename std::enable_if<sizeof...(Types) == index, std::stringstream &>::type
{
    return _out;
}

template<size_t index = 0, typename... Types>
inline auto makeValueList(const std::tuple<Types...> & _tuple, std::stringstream & _out) ->
    typename std::enable_if<index < sizeof...(Types), std::stringstream &>::type
{
    if(index > 0) _out << ", ";
    _out << quoteValue(std::get<index>(_tuple));
    makeValueList<index + 1, Types...>(_tuple, _out);
    return _out;
}

template<typename... Types>
std::string insert(const std::string & _table,
    const std::array<std::string, sizeof...(Types)> & _columns,
    const std::tuple<Types...> & _values)
{
    std::stringstream query;
    query << "INSERT INTO \"" << _table << "\" (";
    for(size_t i = 0; i < _columns.size(); ++i)
    {
        if(i > 0) query << ", ";
        query << '"' << _columns[i] << '"';
    }
    query << ") VALUES (";
    makeValueList(_values, query) << "); SELECT last_insert_rowid();";
    return query.str();
}

} // namespace SQLiteQuery
} // namespace Storage
} // namespace MailUnit

#define SQLQUERY_COLUMNS(...) { __VA_ARGS__ }
#define SQLQUERY_VALUES(...) std::make_tuple( __VA_ARGS__ )
#define SQLQUERY_INSERT(table, columns, values) MailUnit::Storage::SQLiteQuery::insert(table, columns, values)

#endif // __MU_STORAGE_SQLITE_QUERY_H__
