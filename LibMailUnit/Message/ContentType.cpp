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

#include <boost/spirit/include/qi.hpp>
#include <boost/fusion/adapted/struct.hpp>
#include <LibMailUnit/Message/ContentType.h>

using namespace LibMailUnit::Message;
namespace qi = boost::spirit::qi;

BOOST_FUSION_ADAPT_STRUCT(
    ContentTypeParam,
    (std::string, name)
    (std::string, value)
)

BOOST_FUSION_ADAPT_STRUCT(
    ContentType,
    (std::string, type)
    (std::string, subtype)
    (std::vector<ContentTypeParam>, params)
)

namespace {

class Grammar :
    public qi::grammar<typename std::string::const_iterator, ContentType(), qi::ascii::space_type>
{
private:
    template<typename Type>
    using Rule = qi::rule<typename std::string::const_iterator, Type, qi::ascii::space_type>;

public:
    Grammar() : Grammar::base_type(m_expression)
    {
        m_token %= qi::lexeme[+(qi::alnum | qi::char_('_') | qi::char_('-'))];
        m_param_name %= qi::lexeme[+(qi::print - ' ' - ';' - '=')];
        m_param_value %= m_token | '"' >> qi::lexeme[+(qi::char_ - '"')] >> '"';
        m_param %= m_param_name >> '=' >> m_param_value;
        m_expression %= m_token >> '/' > m_token > *(';' >> m_param);
    }

private:
    Rule<std::string()> m_token;
    Rule<ContentTypeParam()> m_param;
    Rule<std::string()> m_param_name;
    Rule<std::string()> m_param_value;
    Rule<ContentType()> m_expression;
}; // class Grammar

} // namespace

std::unique_ptr<ContentType> LibMailUnit::Message::parseContentType(const std::string & _raw_content_type)
{
    static Grammar grammar;
    ContentType * content_type = new ContentType();
    if(qi::phrase_parse(_raw_content_type.cbegin(), _raw_content_type.cend(), grammar, qi::ascii::space, *content_type))
        return std::unique_ptr<ContentType>(content_type);
    return nullptr;
}
