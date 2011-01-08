#ifndef EXPRESSION_HPP
#define EXPRESSION_HPP
#pragma once

#include <vector>
#include <boost/variant.hpp>
#include <tr1/functional>
#include "component.hpp"

namespace extortion { namespace detail {

struct expression {
    boost::variant<
        std::tr1::function<void(const std::string&)>,
        std::tr1::function<void(const char*, size_t, bool)>
    > cb;
    std::vector<std::string> *storage;
    std::vector<component> components;
};

bool parse(const std::string& path, expression& x);

}}

#endif // EXPRESSION_HPP
