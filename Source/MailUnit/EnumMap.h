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

#ifndef __MU_ENUMMAP_H__
#define __MU_ENUMMAP_H__

namespace MailUnit {
namespace EnumMaps {

template<typename EnumType>
struct EnumMap
{
    template<EnumType item>
    static constexpr std::string name()
    {
        static_assert(sizeof(item) == 0, "Default EnumMap has no implementation");
    }
};

} // namespace EnumMaps
} // namespace MailUnit

#define BEGIN_ENUM_MAP(type)        \
    namespace MailUnit {            \
    namespace EnumMaps {            \
                                    \
    template<> struct EnumMap<type> \
    {                               \
        typedef type Type;

#define END_ENUM_MAP                          \
    }; /* template<> struct EnumMap<type> */  \
    }  /* namespace EnumMaps */               \
    }  /* namespace MailUnit */

#define MAP_ENUM_ITEM(item, item_string)                                                 \
    template<Type impl_for>                                                              \
    static constexpr typename std::enable_if<impl_for == item, std::string>::type name() \
    {                                                                                    \
        return item_string;                                                              \
    }

#define ENUM_ITEM_NAME(item) ::MailUnit::EnumMaps::EnumMap<decltype(item)>::name<item>()

#endif // __MU_ENUMMAP_H__
