#define BOOST_TEST_MODULE stream_xml

#include <string>
#include <boost/test/included/unit_test.hpp>
#include <sstream>
#include <extortion/extortion.hpp>

class accumulator {
    std::stringstream oss;
    unsigned element_count;
public:
    accumulator()
        : element_count(0)
    {}

    void write(const char *p, size_t len, bool start) {
        oss.write(p, len);
        element_count += start;
    }

    std::string result() const { return oss.str(); }
    unsigned elements() const { return element_count; }
};

BOOST_AUTO_TEST_CASE(basic_stream) {
    extortion::extortion e;
    accumulator a;
    BOOST_REQUIRE(
        e.stream(
            "/hello/world",
            boost::bind(&accumulator::write, boost::ref(a), _1, _2, _3)
        )
    );

    std::string data(
        "<hello>"
        "<world planet='mercury'>!</world>"
        "<world planet='venus'>?</world>"
        "<world planet='earth'>.</world>"
        "</hello>"
    );

    BOOST_REQUIRE(e.parse_string(data));
    BOOST_CHECK_EQUAL(a.elements(), 3u);
    BOOST_CHECK_EQUAL(a.result(), "!?.");
}

BOOST_AUTO_TEST_CASE(stream_match_attribute) {
    extortion::extortion e;
    accumulator a;
    BOOST_REQUIRE(
        e.stream(
            "/hello/world[planet='earth']",
            boost::bind(&accumulator::write, boost::ref(a), _1, _2, _3)
        )
    );

    std::string data(
        "<hello>"
        "<world planet='mercury'>!</world>"
        "<world planet='venus'>?</world>"
        "<world planet='earth'>.</world>"
        "</hello>"
    );

    BOOST_REQUIRE(e.parse_string(data));
    BOOST_CHECK_EQUAL(a.elements(), 1u);
    BOOST_CHECK_EQUAL(a.result(), ".");
}
