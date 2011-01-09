#define BOOST_TEST_MODULE basic_xml

#include <string>
#include <boost/test/included/unit_test.hpp>
#include <extortion/extortion.hpp>

BOOST_AUTO_TEST_CASE(basic_match) {
    extortion::extortion e;
    std::vector<std::string> storage;
    BOOST_REQUIRE(
        e.get(
            "/hello/world",
            boost::bind(&std::vector<std::string>::push_back, &storage, _1)
        )
    );

    std::string data(
        "<hello><world>lol</world></hello>"
    );

    BOOST_REQUIRE(e.parse_string(data));
    BOOST_REQUIRE(storage.size() > 0u);
    BOOST_CHECK_EQUAL(storage[0], "lol");
}

BOOST_AUTO_TEST_CASE(basic_match_with_attibutes) {
    extortion::extortion e;
    std::vector<std::string> storage;

    BOOST_REQUIRE(
        e.get(
            "/hello/world[@planet='earth']",
            boost::bind(&std::vector<std::string>::push_back, &storage, _1)
        )
    );

    std::string data(
        "<hello>"
        "<world planet='mercury'/>"
        "<world planet='venus'/>"
        "<world planet='earth'/>"
        "<world planet='mars'/>"
        "<world planet='jupiter'/>"
        "<world planet='saturn'/>"
        "<world planet='uranus'/>"
        "<world planet='neptune'/>"
        "<!-- world planet='pluto is not a planet'/ -->"
        "</hello>"
    );

    BOOST_REQUIRE(e.parse_string(data));
    BOOST_REQUIRE(storage.size() == 1u);
    BOOST_CHECK(storage[0].empty());
}

BOOST_AUTO_TEST_CASE(multiple_deep_match) {
    extortion::extortion e;
    std::vector<std::string> four;
    std::vector<std::string> five;
    BOOST_REQUIRE(
        e.get(
            "/hello/world/foo/bar/baz/one/two/three/four",
            boost::bind(&std::vector<std::string>::push_back, &four, _1)
        )
    );

    BOOST_REQUIRE(
        e.get(
            "/hello/world/foo/bar/baz/one/two/three/four/five",
            boost::bind(&std::vector<std::string>::push_back, &five, _1)
        )
    );

    std::string data(
        "<hello>"
        "<world planet='venus'>"
        "<foo><bar><baz><one><two><three><four>"
        "</four></three></two></one></baz></bar></foo>"
        "</world>"
        "<world planet='earth'>"
        "<foo><bar><baz><one><two><three><four><five>"
        "</five></four></three></two></one></baz></bar></foo>"
        "</world>"
        "<world planet='mars'/>"
        "<!-- world planet='pluto is not a planet'/ -->"
        "</hello>"
    );

    BOOST_REQUIRE(e.parse_string(data));
    BOOST_REQUIRE(four.size() == 2u && five.size() == 1u);

    BOOST_CHECK_EQUAL(four[0].empty(), true);
    BOOST_CHECK_EQUAL(four[1], "<five></five>");
    BOOST_CHECK_EQUAL(five[0].empty(), true);
}

BOOST_AUTO_TEST_CASE(test_xml_ns) {
    std::vector<std::string> xml;
    extortion::extortion e;
    BOOST_REQUIRE(
        e.get(
            "/bar",
            boost::bind(&std::vector<std::string>::push_back, &xml, _1)
        )
    );

    std::string content(
        " <bar:baz>"
        "  <hello>world</hello>"
        " </bar:baz>"
    );

    std::string data(
        "<bar xmlns='http://www.example.com/foo'"
        " xmlns:bar='http://example.com/bar'>"+
        content+
        "</bar>"
    );

    BOOST_REQUIRE(e.parse_string(data));
    BOOST_REQUIRE(xml.size() == 1u);
    BOOST_CHECK_EQUAL(xml[0], content);
}

BOOST_AUTO_TEST_CASE(test_basic_multiple_hits) {
    std::vector<std::string> foo;
    extortion::extortion e;
    BOOST_REQUIRE(
        e.get(
            "/feed/entry/",
            boost::bind(&std::vector<std::string>::push_back, &foo, _1)
        )
    );
    BOOST_CHECK(
        e.parse_string(
            "<feed><entry>hello</entry><entry>world</entry></feed>"
        )
    );
    BOOST_REQUIRE(foo.size() == 2u);
    BOOST_CHECK_EQUAL(foo[0], "hello");
    BOOST_CHECK_EQUAL(foo[1], "world");
}
