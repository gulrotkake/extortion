#include "optional_string.hpp"

namespace extortion { namespace detail {

optional_string::optional_string(const std::string& s)
    : boost::optional<std::string>(s)
{}

optional_string::optional_string(const char *s)
    : boost::optional<std::string>(s)
{}

optional_string::optional_string()
    : boost::optional<std::string>()
{}

}}
