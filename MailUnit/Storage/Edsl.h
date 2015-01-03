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

#ifndef __MU_STORAGE_EDSL_H__
#define __MU_STORAGE_EDSL_H__

#include <memory>
#include <string>
#include <ostream>
#include <vector>
#include <boost/variant.hpp>
#include <boost/optional.hpp>
#include <MailUnit/Exception.h>

namespace MailUnit {
namespace Storage {
namespace Edsl {

MU_EXCEPTION(EdslException)

enum class Operation
{
    get,
    drop
}; // enum class Operation

typedef std::string Identifier;

typedef boost::variant<int, std::string> ConditionValue;

enum class ConditionBinaryOperator
{
    equal,
    not_equal,
    greater,
    less,
    greater_or_equal,
    less_or_equal
}; // enum class ConditionBinaryOperator

struct BinaryCondition
{
    Identifier identifier;
    ConditionBinaryOperator operator_;
    ConditionValue value;
}; // struct SimpleCondition

enum class ConditionJoinOperator
{
    and_,
    or_
}; // enum class ConditionJoinOperator

struct ConditionSequence;

typedef boost::variant<
        BinaryCondition,
        boost::recursive_wrapper<ConditionSequence>
    > ConditionSequenceOperand;

struct RightConditionSequence
{
    ConditionJoinOperator operator_;
    ConditionSequenceOperand operand;
}; // struct RightConditionSequence

struct ConditionSequence
{
    ConditionSequenceOperand operand;
    std::vector<RightConditionSequence> right;
}; // struct ConditionSequence

struct Expression
{
    Operation operation;
    boost::optional<ConditionSequence> conditions;
}; // struct Expression

std::unique_ptr<Expression> parse(const std::string & _input);

} // namespace Dsel
} // namespace Storage
} // namespace MailUnit

std::ostream & operator << (std::ostream & _stream, const MailUnit::Storage::Edsl::ConditionValue & _value);
std::ostream & operator << (std::ostream & _stream, MailUnit::Storage::Edsl::ConditionBinaryOperator _operator);
std::ostream & operator << (std::ostream & _stream, MailUnit::Storage::Edsl::ConditionJoinOperator _operator);
std::ostream & operator << (std::ostream & _stream, const MailUnit::Storage::Edsl::BinaryCondition & _condition);
std::ostream & operator << (std::ostream & _stream, const MailUnit::Storage::Edsl::ConditionSequenceOperand & _operand);
std::ostream & operator << (std::ostream & _stream, const MailUnit::Storage::Edsl::ConditionSequence & _condition);
std::ostream & operator << (std::ostream & _stream, const MailUnit::Storage::Edsl::Operation & _operation);
std::ostream & operator << (std::ostream & _stream, const MailUnit::Storage::Edsl::Expression & _expression);

#endif // __MU_STORAGE_EDSL_H__
