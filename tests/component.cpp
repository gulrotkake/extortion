#define BOOST_TEST_MODULE component

#include <string>
#include "../src/component.hpp"
#include <boost/test/included/unit_test.hpp>
#include <boost/assign.hpp>

BOOST_AUTO_TEST_CASE(match_name) {
    extortion::detail::component c("hello");
    const char *attr[] = {"name", "world", NULL};
    BOOST_CHECK_EQUAL(c.match("hello", NULL), true);
    BOOST_CHECK_EQUAL(c.match("hello", attr), true);
    BOOST_CHECK_EQUAL(c.match("foo", NULL), false);
}

BOOST_AUTO_TEST_CASE(match_attribute_key) {
    using boost::assign::map_list_of;
    typedef std::map<std::string, extortion::detail::optional_string> map_type;
    extortion::detail::component c(
        std::make_pair<std::string, map_type>(
            "hello",
            map_list_of<std::string, extortion::detail::optional_string>
                ("who", extortion::detail::optional_string())
        )
    );

    const char *attr_set1[] = {"who", "world", NULL};
    const char *attr_set2[] = {"foo", "bar", NULL};

    BOOST_CHECK_EQUAL(c.match("hello", NULL), false);
    BOOST_CHECK_EQUAL(c.match("hello", attr_set1), true);
    BOOST_CHECK_EQUAL(c.match("hello", attr_set2), false);
}

BOOST_AUTO_TEST_CASE(match_attribute_value) {
    using boost::assign::map_list_of;
    typedef std::map<std::string, extortion::detail::optional_string> map_type;
    extortion::detail::component c(
        std::make_pair<std::string, map_type>(
            "hello",
            map_list_of<std::string, extortion::detail::optional_string>
                ("who", extortion::detail::optional_string("world"))
        )
    );

    const char *attr_set1[] = {"who", "world", NULL};
    const char *attr_set2[] = {"who", "michael", NULL};

    BOOST_CHECK_EQUAL(c.match("hello", NULL), false);
    BOOST_CHECK_EQUAL(c.match("hello", attr_set1), true);
    BOOST_CHECK_EQUAL(c.match("hello", attr_set2), false);
}
