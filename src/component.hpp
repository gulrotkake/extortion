#ifndef COMPONENT_HPP
#define COMPONENT_HPP
#pragma once

#include "optional_string.hpp"
#include <string>
#include <map>

namespace extortion { namespace detail {

class component {
private:
    typedef std::map<std::string, optional_string> attribute_map;
    std::string name;
    attribute_map attributes;

public:
    component();
    component(const std::string& name_);
    component(const std::pair<std::string, attribute_map>& p);

    const std::string& get_name() const;
    const attribute_map& get_attributes() const;

    // The reason for the C types is that these come directly from a C-library.
    bool match(const char *element, const char **attr) const;
};

}}

#endif // COMPONENT_HPP
