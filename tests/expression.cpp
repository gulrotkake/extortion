#define BOOST_TEST_MODULE expression

#include "../src/expression.hpp"
#include <boost/test/included/unit_test.hpp>
#include <boost/assign.hpp>
#include <boost/lambda/bind.hpp>
#include <boost/cast.hpp>

class match {
    const extortion::detail::expression& ex;
    std::vector<std::string> names;
    std::vector< std::vector<const char*> > attrs;
public:
    match(const extortion::detail::expression& ex_)
        : ex(ex_)
    {}

    match& operator()(const std::string& name) {
        return this->operator()(name, std::vector<const char*>());
    }

    match& operator()(
        const std::string& name,
        const std::vector<const char*>& attr
    ) {
        names.push_back(name);
        attrs.push_back(attr);
        return *this;
    }

    operator bool() const {
        assert(names.size() == attrs.size());
        bool res = ex.components.size() == names.size();
        for (unsigned i = 0; i<names.size() && res; ++i) {
            res &= ex.components[i].match(
                names[i].c_str(),
                attrs[i].size()
                    ? const_cast<const char**>(&(attrs[i][0]))
                    : static_cast<const char**>(NULL)
            );
        }
        return res;
    }
};

BOOST_AUTO_TEST_CASE(parse_simple) {
    extortion::detail::expression x;
    BOOST_REQUIRE(extortion::detail::parse("/hello/world", x));

    BOOST_CHECK_EQUAL(
        match(x)("hello")("world"),
        true
    );

    BOOST_CHECK_EQUAL(
        match(x)("hello")("foo"),
        false
    );

    BOOST_CHECK_EQUAL(
        match(x)("")(""),
        false
    );
}

BOOST_AUTO_TEST_CASE(parse_attribute_key_values) {
    using boost::assign::list_of;

    extortion::detail::expression x;
    BOOST_REQUIRE(
        extortion::detail::parse(
            "/hello[name='world']/where[planet='earth']/",
            x
        )
    );

    BOOST_CHECK_EQUAL(
        match(x)
            ("hello", list_of<const char*>("name")("world")(NULL))
            ("where", list_of<const char*>("planet")("earth")(NULL)),
        true
    );

    BOOST_CHECK_EQUAL(
        match(x)
            ("hello", list_of<const char*>("name")("world")(NULL))
            ("where", list_of<const char*>("planet")("mars")(NULL)),
        false
    );
}

BOOST_AUTO_TEST_CASE(parse_attribute_key) {
    using boost::assign::list_of;

    extortion::detail::expression x;
    BOOST_REQUIRE(
        extortion::detail::parse("/hello[name='world']/where[planet]/", x)
    );

    BOOST_CHECK_EQUAL(
        match(x)
            ("hello", list_of<const char*>("name")("world")(NULL))
            ("where", list_of<const char*>("planet")("earth")(NULL)),
        true
    );

    BOOST_CHECK_EQUAL(
        match(x)
            ("hello", list_of<const char*>("name")("world")(NULL))
            ("where", list_of<const char*>("planet")("mars")(NULL)),
        true
    );
}

BOOST_AUTO_TEST_CASE(parse_multiple_attributes) {
    using boost::assign::list_of;

    extortion::detail::expression x;
    BOOST_REQUIRE(
        extortion::detail::parse(
            "/greeting[type='hello'&name='world'&planet]",
            x
        )
    );

    BOOST_CHECK_EQUAL(
        match(x)
            ("greeting", list_of<const char*>
                ("name")("world")
                ("type")("hello")
                ("planet")("earth")
                (NULL)
        ),
        true
    );

    BOOST_CHECK_EQUAL(
        match(x)
            ("greeting", list_of<const char*>
                ("type")("hello")
                ("name")("world")
                ("planet")("mars")
                (NULL)
        ),
        true
    );

    BOOST_CHECK_EQUAL(
        match(x)
            ("greeting", list_of<const char*>
                ("type")("goodbye")
                ("name")("world")
                ("planet")("mars")
                (NULL)
        ),
        false
    );
}

BOOST_AUTO_TEST_CASE(parse_bad_input) {
    extortion::detail::expression x;
    std::string bad[] = {
        "",
        "/",
        "//",
        "/hello//",
        "/hello[]",
        "/hello[name='''",
        "/hello[n/ame]",
        "/hello[name",
        "/[name]"
    };

    BOOST_CHECK_EQUAL(
        std::count_if(
            &bad[0],
            &bad[0]+sizeof(bad)/sizeof(bad[0]),
            boost::bind(&extortion::detail::parse, _1, x) == false
        ),
        boost::numeric_cast<int>(sizeof(bad)/sizeof(bad[0]))
    );
}

BOOST_AUTO_TEST_CASE(parse_two_simple) {
    extortion::detail::expression x1;
    extortion::detail::expression x2;
    BOOST_REQUIRE(extortion::detail::parse("/hello/world", x1));
    BOOST_REQUIRE(extortion::detail::parse("/hello/world", x2));

    BOOST_CHECK_EQUAL(
        match(x1)("hello")("world"),
        match(x2)("hello")("world")
    );
}
