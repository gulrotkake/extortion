/* The actual grammar is soon to be replaced by:

namespace extortion { namespace detail {

struct attribute {
    std::string name;
    optional_string value;
};

struct component;

typedef boost::variant<std::vector<component>, attribute> sub_expression;

struct component {
    component() {}

    component(const std::string& name) {
    }

    component(
        const std::string& name,
        const std::vector<sub_expression>& sub_expressions
    ) {
    }
};

}}

BOOST_FUSION_ADAPT_STRUCT(
    extortion::detail::attribute,
    (std::string, name)
    (extortion::detail::optional_string, value)
);

namespace extortion { namespace detail {

namespace qi = boost::spirit::qi;
template <typename Iterator>
struct path : qi::grammar<Iterator, std::vector<component>()> {
    // boost::spirit::qi::rule<Iterator, expression()> expression_r;
    boost::spirit::qi::rule<Iterator, std::vector<component>()> expression_r;
    qi::rule<Iterator, component()> component_r;
    // boost::spirit::qi::rule<Iterator, tuple()> tuple_r;
    // boost::spirit::qi::rule<Iterator, pairs_type()> attributes_r;
    qi::rule<Iterator, std::vector<sub_expression>()> sub_expressions_r;
    qi::rule<Iterator, sub_expression()> sub_expression_r;
    qi::rule<Iterator, attribute()> attribute_r;
    qi::rule<Iterator, std::string()> key_r, value_r;
    qi::rule<Iterator, boost::variant<std::vector<component>, attribute>()> foo_r;
    path()
        : path::base_type(expression_r)
    {
        namespace qi = boost::spirit::qi;

        // expression =  "/" , component , { "/" , component } , [ "/" ]
        expression_r = +(qi::omit['/'] >> component_r) >> qi::omit[-qi::char_('/')];
        // component  = key , [ "[" , sub_expressions , "]" ]
        component_r = key_r >> -('[' >> sub_expressions_r >> ']');

        // sub_expressionss = subexpr , { '&' , subexpr }
        sub_expressions_r = sub_expression_r % '&';

        // sub_expression = expression_r | attribute_r
        sub_expression_r = expression_r | attribute_r;

        // attribute  = "@" , key , [ "=" , value ]
        attribute_r   = '@' >> key_r >> -('=' >> value_r);

        // key        = alpha , { alnum }
        key_r         = qi::char_("a-zA-Z_") >> *qi::char_("a-zA-Z_0-9");

        // value      = "'" , data , "'"
        value_r       = '\'' >> *(qi::char_ -qi::char_('\'')) >> '\'';
    }
};

}}
*/
#include "expression.hpp"
#include <boost/spirit/include/qi.hpp>
#include <boost/fusion/include/std_pair.hpp>

BOOST_FUSION_ADAPT_STRUCT(
    extortion::detail::expression,
    (std::vector<extortion::detail::component>, components)
);

namespace extortion { namespace detail {

template <typename Iterator>
struct path : boost::spirit::qi::grammar<Iterator, expression()> {
    typedef std::map<std::string, optional_string> pairs_type;
    typedef std::pair<std::string, optional_string> pair_type;
    typedef std::pair<std::string, pairs_type> tuple;

    boost::spirit::qi::rule<Iterator, expression()> expression_r;
    boost::spirit::qi::rule<Iterator, component()> component_r;
    boost::spirit::qi::rule<Iterator, tuple()> tuple_r;
    boost::spirit::qi::rule<Iterator, pairs_type()> attributes_r;
    boost::spirit::qi::rule<Iterator, pair_type()> pair_r;
    boost::spirit::qi::rule<Iterator, std::string()> key_r, value_r;

    path()
        : path::base_type(expression_r)
    {
        namespace qi = boost::spirit::qi;
        /*
          The rules written below are prefixed with a comment showing their
          representation in EBNF. The grammar is slightly expanded, with tuple
          and component, to help qi construct the correct types.
        */

        // expression =  "/" , component , { "/" , component } , [ "/" ]
        expression_r  = +('/' >> component_r) >> -qi::char_('/');

        // component  = tuple | key
        component_r   = tuple_r | key_r;

        // tuple      = key , [ "[" , attributes , "]" ]
        tuple_r       = key_r >> -('[' >> attributes_r >> ']');

        // attributes = pair , { ( ";" | "&" ) , pair }
        attributes_r  = pair_r >> *((qi::lit(';') | '&') >> pair_r);

        // pair       = ("@" | "attribue::") , key , [ "=" , value ]
        pair_r        = qi::omit[(qi::lit("@") | qi::lit("attribute::"))]
                        >> key_r >> -('=' >> value_r);

        // key        = alpha , { alnum }
        key_r         = qi::char_("a-zA-Z_") >> *qi::char_("a-zA-Z_0-9");

        // value      = "'" , data , "'"
        value_r       = '\'' >> *(qi::char_ -qi::char_('\'')) >> '\'';
    }
};

bool parse(const std::string& path, expression& x) {
    std::string::const_iterator first = path.begin();
    std::string::const_iterator last = path.end();

    ::extortion::detail::path<std::string::const_iterator> p;

    return boost::spirit::qi::parse(first, last, p, x) && first==last;
}

}}
