#ifndef UTILITIES_HPP
#define UTILITIES_HPP
#pragma once

#include <string>
#include <boost/ptr_container/ptr_map.hpp>
#include <tr1/functional>

namespace extortion { namespace detail {

class recorder;
class active_set {
    boost::ptr_multimap<unsigned, recorder> recorders;
public:
    active_set();
    ~active_set();

    void add(
        unsigned depth,
        const std::tr1::function<void(const std::string&)> &func
    );

    void add(
        unsigned depth,
        const std::tr1::function<void(const char*, size_t, bool)> &func
    );

    void clear(unsigned);

    active_set& write(const char *, size_t);
    active_set& operator<<(const std::string&);
    active_set& operator<<(const char **);
    template<typename T> active_set& operator<<(T);
};

}}

#endif // UTILITIES_HPP
