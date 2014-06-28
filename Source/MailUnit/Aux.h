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

#ifndef __MU_AUX_H__
#define __MU_AUX_H__

#define MU_UNUSED(var) (void)var

#define MU_DISABLE_COPY(name)                   \
    name(const name &)                = delete; \
    name(name &&)                     = delete; \
    name & operator = (const name &)  = delete; \
    name & operator = (name &&)       = delete;

#define MU_DEFAULT_COPY(name)                    \
    name(const name &)                = default; \
    name(name &&)                     = default; \
    name & operator = (const name &)  = default; \
    name & operator = (name &&)       = default;


#define MU_FLAGS(type)                                                           \
    inline type operator | (const type lhs, const type rhs)                      \
    {                                                                            \
        return static_cast<type>(static_cast<int>(lhs) | static_cast<int>(rhs)); \
    }                                                                            \
    inline void operator |= (type & lhs, const type rhs)                         \
    {                                                                            \
        lhs = lhs | rhs;                                                         \
    }                                                                            \
    inline type operator & (const type lhs, const type rhs)                      \
    {                                                                            \
        return static_cast<type>(static_cast<int>(lhs) & static_cast<int>(rhs)); \
    }                                                                            \
    inline void operator &= (type & lhs, const type rhs)                         \
    {                                                                            \
        lhs = lhs & rhs;                                                         \
    }

#endif // __MU_AUX_H__
