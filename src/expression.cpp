#include "expression.hpp"
#include <boost/spirit/include/qi.hpp>
#include <boost/fusion/include/std_pair.hpp>

BOOST_FUSION_ADAPT_STRUCT(
    extortion::detail::expression,
    (std::vector<extortion::detail::component>, components)
);

namespace extortion { namespace detail {

namespace qi = boost::spirit::qi;

template <typename Iterator>
struct path : qi::grammar<Iterator, expression()> {
    typedef std::map<std::string, optional_string> pairs_type;
    typedef std::pair<std::string, pairs_type> tuple;
    typedef std::pair<std::string, optional_string> attribute;

    qi::rule<Iterator, expression()> expression_r;
    qi::rule<Iterator, component()> component_r;
    qi::rule<Iterator, tuple()> tuple_r;
    qi::rule<Iterator, pairs_type()> expression_group_r;
    qi::rule<Iterator, attribute()> attribute_r;
    qi::rule<Iterator, std::string()> key_r, value_r;

    path()
        : path::base_type(expression_r)
    {
        /*
          The rules written below are prefixed with a comment showing their
          representation in EBNF. The grammar is slightly expanded, with tuple
          and component, to help qi construct the correct types.
        */
        expression_r  = +(qi::omit['/'] >> component_r) >> -qi::omit['/'];

        // component = tuple
        component_r %= tuple_r;

        // tuple = key , [ "[" , expression_group , "]" ]
        tuple_r %= key_r >> -('[' >> expression_group_r >> ']');

        // expression_group = { attribute }
        expression_group_r %= attribute_r % '&';

        // attribute  = "@" , key , [ "=" , value ]
        attribute_r   %= '@' >> key_r >> -('=' >> value_r);

        // key        = alpha , { alnum }
        key_r         %= qi::char_("a-zA-Z_") >> *qi::char_("a-zA-Z_0-9");

        // value      = "'" , data , "'"
        value_r       %= '\'' >> *(qi::char_ -qi::char_('\'')) >> '\'';
    }
};

bool parse(const std::string& path, expression& x) {
    std::string::const_iterator first = path.begin();
    std::string::const_iterator last = path.end();

    ::extortion::detail::path<std::string::const_iterator> p;

    return boost::spirit::qi::parse(first, last, p, x) && first==last;
}

}}
