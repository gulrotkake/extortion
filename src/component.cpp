#include "component.hpp"

namespace extortion { namespace detail {

component::component() {}

component::component(const std::string& name_)
    : name(name_)
{}

component::component(const std::pair<std::string, attribute_map>& p)
    : name(p.first), attributes(p.second)
{}

const std::string& component::get_name() const {
    return name;
}

const component::attribute_map& component::get_attributes() const {
    return attributes;
}

// The reason for the C types is that these come directly from a C-library.
bool component::match(const char *element, const char **attr) const {
    if (name != element) return false;

    size_t expected_matches = attributes.size();

    while(attr && *attr && expected_matches > 0) {
        const char *key = *attr++;
        const char *value = *attr++;

        attribute_map::const_iterator itr = attributes.find(key);
        if (itr != attributes.end()) {
            if (!itr->second || *(itr->second) == value) {
                --expected_matches;
            } else {
                // Attribute mismatch, bailing out.
                break;
            }
        }
    }
    return expected_matches == 0;
}

}}
