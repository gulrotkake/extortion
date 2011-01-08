#define BOOST_TEST_MODULE optional_string

#include <string>
#include "../src/optional_string.hpp"
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE(null_string) {
    extortion::detail::optional_string x;
    BOOST_CHECK_EQUAL(!!x, false);
}

BOOST_AUTO_TEST_CASE(empty_string) {
    extortion::detail::optional_string x("");
    BOOST_CHECK_EQUAL(!!x, true);
    BOOST_CHECK_EQUAL((*x).size(), 0u);
}

BOOST_AUTO_TEST_CASE(string) {
    extortion::detail::optional_string x("hello");
    BOOST_CHECK_EQUAL(!!x, true);
    BOOST_CHECK_EQUAL(*x, "hello");
}
