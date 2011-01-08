#ifndef OPTIONAL_STRING_HPP
#define OPTIONAL_STRING_HPP
#pragma once

#include <string>
#include <boost/optional.hpp>

namespace extortion { namespace detail {

class optional_string : public boost::optional<std::string> {
public:
    optional_string(const std::string& s);
    optional_string(const char *s);
    optional_string();
};

}}

#endif // OPTIONAL_STRING_HPP
