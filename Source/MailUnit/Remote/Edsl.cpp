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

#include <boost/fusion/adapted/struct.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <MailUnit/Remote/Edsl.h>

using namespace MailUnit::Remote::Edsl;
namespace qi = boost::spirit::qi;

BOOST_FUSION_ADAPT_STRUCT(
    BinaryCondition,
    (Identifier, identifier)
    (ConditionBinaryOperator, operator_)
    (ConditionValue, value)
)

BOOST_FUSION_ADAPT_STRUCT(
    UnaryCondition,
    (ConditionUnaryOperator, operator_)
    (Identifier, identifier)
)

BOOST_FUSION_ADAPT_STRUCT(
    RightConditionSequence,
    (ConditionJoinOperator, operator_)
    (ConditionSequenceOperand, operand)
)

BOOST_FUSION_ADAPT_STRUCT(
    ConditionSequence,
    (ConditionSequenceOperand, operand)
    (std::vector<RightConditionSequence>, right)
)

BOOST_FUSION_ADAPT_STRUCT(
    Expression,
    (Target, target)
    (Source, source)
    (boost::optional<ConditionSequence>, conditions)
)

namespace {

class SpecialTargetSymbols : public qi::symbols<char, SpecialTarget>
{
public:
    SpecialTargetSymbols()
    {
        add("*", SpecialTarget::All);
    }
}; // class SpecialTargetSymbols

class ConditionBinaryOperatorSymbols : public qi::symbols<char, ConditionBinaryOperator>
{
public:
    ConditionBinaryOperatorSymbols()
    {
        add
            ("=" , ConditionBinaryOperator::Equal)
            ("<>", ConditionBinaryOperator::NotEqual)
            (">" , ConditionBinaryOperator::Greater)
            ("<" , ConditionBinaryOperator::Less)
            (">=", ConditionBinaryOperator::GreaterOrEqual)
            ("<=", ConditionBinaryOperator::LessOrEqual);
    }
}; // class ConditionBinaryOperatorSymbols

class ConditionUnaryOperatorSymbols : public qi::symbols<char, ConditionUnaryOperator>
{
public:
    ConditionUnaryOperatorSymbols()
    {
        add("!", ConditionUnaryOperator::Not);
    }
}; // class ConditionUnaryOperatorSymbols

class ConditionJoinOperatorSymbols : public qi::symbols<char, ConditionJoinOperator>
{
public:
    ConditionJoinOperatorSymbols()
    {
        add
            ("and", ConditionJoinOperator::And)
            ("or" , ConditionJoinOperator::Or);
    }
}; // class ConditionJoinOperatorSymbols

class Grammar :
    public qi::grammar<typename std::string::const_iterator, Expression(), qi::ascii::space_type>
{
private:
    template<typename Type>
    using Rule = qi::rule<typename std::string::const_iterator, Type, qi::ascii::space_type>;

public:
    Grammar();

private:
    Rule<std::string()> m_identifier;
    Rule<std::vector<std::string>()> m_identifier_list;
    Rule<Target()> m_target;
    SpecialTargetSymbols m_special_target;
    Rule<ConditionValue()> m_condition_value;
    Rule<BinaryCondition()> m_binary_condition;
    ConditionBinaryOperatorSymbols m_binary_operator;
    Rule<UnaryCondition()> m_unary_condition;
    ConditionUnaryOperatorSymbols m_unary_operator;
    Rule<ConditionSequence()> m_bracketed_condition_sequence;
    Rule<ConditionSequenceOperand()> m_condition_sequence_operand;
    Rule<RightConditionSequence()> m_right_condition_sequence;
    ConditionJoinOperatorSymbols m_join_operator;
    Rule<ConditionSequence()> m_condition_sequence;
    Rule<Expression()> m_expression;
}; // class Grammar

Grammar::Grammar() :
    Grammar::base_type(m_expression)
{
    m_identifier %= qi::lexeme[qi::ascii::alpha >> *qi::ascii::alnum];
    m_identifier_list %= m_identifier % ',';
    m_target %= m_special_target | m_identifier_list;
    m_condition_value %= qi::int_ | ("'" >> qi::lexeme[*(qi::ascii::alnum | qi::ascii::blank)] >> "'");
    m_binary_condition %= m_identifier > m_binary_operator > m_condition_value;
    m_unary_condition %= m_unary_operator > m_identifier;
    m_bracketed_condition_sequence %= "(" >> m_condition_sequence >> ")";
    m_condition_sequence_operand %= m_binary_condition | m_unary_condition |
        m_bracketed_condition_sequence;
    m_right_condition_sequence %= qi::ascii::no_case[m_join_operator] >> m_condition_sequence_operand;
    m_condition_sequence %= m_condition_sequence_operand >> *m_right_condition_sequence;
    m_expression %= qi::ascii::no_case["select"] >> m_target >>
        qi::ascii::no_case["from"] >> m_identifier >>
        -(qi::ascii::no_case["where"] >> m_condition_sequence) >> ";";
}

class GenericPriter : public boost::static_visitor<>
{
public:
    explicit GenericPriter(std::ostream & _stream) :
        mr_stream(_stream)
    {
    }

    template<typename Type>
    void operator ()(const Type & _value)
    {
        mr_stream << _value;
    }

private:
    std::ostream & mr_stream;
}; // class GenericPriter

class ConditionValuePrinter : public boost::static_visitor<>
{
public:
    explicit ConditionValuePrinter(std::ostream & _stream) :
        mr_stream(_stream)
    {
    }

    void operator () (int _int)
    {
        mr_stream << _int;
    }

    void operator () (const std::string & _string)
    {
        mr_stream << "'" << _string << "'";
    }

private:
    std::ostream & mr_stream;
}; // class ConditionValuePrinter

class TargetPrinter : public boost::static_visitor<>
{
public:
    explicit TargetPrinter(std::ostream & _stream) :
        mr_stream(_stream)
    {
    }

    void operator () (SpecialTarget _target)
    {
        mr_stream << _target;
    }

    void operator () (const std::vector<Identifier> & _ids)
    {
        bool first = true;
        for(const Identifier & id : _ids)
        {
            if(first)
                first = false;
            else
                mr_stream << ", ";
            mr_stream << id;
        }
    }

private:
    std::ostream & mr_stream;
}; // class TargetPrinter

} // namespace

std::ostream & operator << (std::ostream & _stream, SpecialTarget _target)
{
    if(_target == SpecialTarget::All)
        _stream << "*";
    return _stream;
}

std::ostream & operator << (std::ostream & _stream, const Target & _target)
{
    TargetPrinter printer(_stream);
    boost::apply_visitor(printer, _target);
    return _stream;
}

std::ostream & operator << (std::ostream & _stream, const ConditionValue & _value)
{
    ConditionValuePrinter printer(_stream);
    boost::apply_visitor(printer, _value);
    return _stream;
}

std::ostream & operator << (std::ostream & _stream, ConditionBinaryOperator _operator)
{
    switch(_operator)
    {
    case ConditionBinaryOperator::Equal:
        _stream << "=";
        break;
    case ConditionBinaryOperator::NotEqual:
        _stream << "<>";
        break;
    case ConditionBinaryOperator::Greater:
        _stream << ">";
        break;
    case ConditionBinaryOperator::Less:
        _stream << "<";
        break;
    case ConditionBinaryOperator::GreaterOrEqual:
        _stream << ">=";
        break;
    case ConditionBinaryOperator::LessOrEqual:
        _stream << "<=";
        break;
    }
    return _stream;
}

std::ostream & operator << (std::ostream & _stream, ConditionUnaryOperator _operator)
{
    switch(_operator)
    {
    case ConditionUnaryOperator::Not:
        _stream << '!';
        break;
    }
    return _stream;
}

std::ostream & operator << (std::ostream & _stream, ConditionJoinOperator _operator)
{
    switch(_operator)
    {
    case ConditionJoinOperator::And:
        _stream << "AND";
        break;
    case ConditionJoinOperator::Or:
        _stream << "OR";
        break;
    }
    return _stream;
}

std::ostream & operator << (std::ostream & _stream, const BinaryCondition & _condition)
{
    _stream << _condition.identifier << ' ' << _condition.operator_ << ' ' << _condition.value;
    return _stream;
}

std::ostream & operator << (std::ostream & _stream, const UnaryCondition & _condition)
{
    _stream << _condition.operator_ << _condition.identifier;
    return _stream;
}

std::ostream & operator << (std::ostream & _stream, const ConditionSequenceOperand & _operand)
{
    GenericPriter printer(_stream);
    boost::apply_visitor(printer, _operand);
    return _stream;
}

std::ostream & operator << (std::ostream & _stream, const ConditionSequence & _condition)
{
    _stream << '(' << _condition.operand;
    GenericPriter printer(_stream);
    for(const RightConditionSequence & right : _condition.right)
    {
        _stream << ' ' << right.operator_ << ' ';
        boost::apply_visitor(printer, right.operand);
    }
    _stream << ')';
    return _stream;
}

std::ostream & operator << (std::ostream & _stream, const Expression & _expression)
{
    _stream << "SELECT " << _expression.target << " FROM " << _expression.source;
    if(_expression.conditions.is_initialized())
        _stream << " WHERE " << _expression.conditions.get();
    _stream << ';';
    return _stream;
}

std::shared_ptr<Expression> MailUnit::Remote::Edsl::parse(const std::string & _input)
{
    Grammar grammar;
    Expression * expression = new Expression();
    std::shared_ptr<Expression> result = std::shared_ptr<Expression>(expression);
    if(qi::phrase_parse(_input.begin(), _input.end(), grammar, qi::ascii::space, *expression))
    {
        return result;
    }
    else
    {
        return nullptr;
    }
}

