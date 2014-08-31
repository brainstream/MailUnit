#ifndef __MU_REMOTE_EDSL_H__
#define __MU_REMOTE_EDSL_H__

#include <memory>
#include <string>
#include <ostream>
#include <vector>
#include <boost/variant.hpp>
#include <boost/optional.hpp>

namespace MailUnit {
namespace Remote {
namespace Edsl {

typedef std::string Identifier;

enum class SpecialTarget
{
    All
}; // enum class SpecialTarget

typedef boost::variant<SpecialTarget, std::vector<Identifier>> Target;

typedef boost::variant<int, std::string> ConditionValue;

typedef Identifier Source;

enum class ConditionBinaryOperator
{
    Equal,
    NotEqual,
    Greater,
    Less,
    GreaterOrEqual,
    LessOrEqual
}; // enum class ConditionBinaryOperator

enum class ConditionUnaryOperator
{
    Not
}; // enum class ConditionUnaryOperator

struct BinaryCondition
{
    Identifier identifier;
    ConditionBinaryOperator operator_;
    ConditionValue value;
}; // struct SimpleCondition

struct UnaryCondition
{
    ConditionUnaryOperator operator_;
    Identifier identifier;
}; // struct UnaryCondition

enum class ConditionJoinOperator
{
    And,
    Or
}; // enum class ConditionJoinOperator

struct ConditionSequence;

typedef boost::variant<
        BinaryCondition,
        UnaryCondition,
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
    Target target;
    Source source;
    boost::optional<ConditionSequence> conditions;
}; // struct Expression

std::shared_ptr<Expression> parse(const std::string & _input);

} // namespace Edsl
} // namespace Remote
} // namespace MailUnit

std::ostream & operator << (std::ostream & _stream, MailUnit::Remote::Edsl::SpecialTarget _target);
std::ostream & operator << (std::ostream & _stream, const MailUnit::Remote::Edsl::Target & _target);
std::ostream & operator << (std::ostream & _stream, const MailUnit::Remote::Edsl::ConditionValue & _value);
std::ostream & operator << (std::ostream & _stream, MailUnit::Remote::Edsl::ConditionBinaryOperator _operator);
std::ostream & operator << (std::ostream & _stream, MailUnit::Remote::Edsl::ConditionUnaryOperator _operator);
std::ostream & operator << (std::ostream & _stream, MailUnit::Remote::Edsl::ConditionJoinOperator _operator);
std::ostream & operator << (std::ostream & _stream, const MailUnit::Remote::Edsl::BinaryCondition & _condition);
std::ostream & operator << (std::ostream & _stream, const MailUnit::Remote::Edsl::UnaryCondition & _condition);
std::ostream & operator << (std::ostream & _stream, const MailUnit::Remote::Edsl::ConditionSequenceOperand & _operand);
std::ostream & operator << (std::ostream & _stream, const MailUnit::Remote::Edsl::ConditionSequence & _condition);
std::ostream & operator << (std::ostream & _stream, const MailUnit::Remote::Edsl::Expression & _expression);

#endif // __MU_REMOTE_EDSL_H__
