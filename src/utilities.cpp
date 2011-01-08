#include "utilities.hpp"
#include "nt_iterator.hpp"
#include "recorder.hpp"

#include <boost/foreach.hpp>

namespace extortion { namespace detail {

class attribute_printer {
    std::ostream &os;
    bool key;
public:
    attribute_printer(std::ostream &os_)
        : os(os_), key(true)
    {}

    void operator()(const char * s) {
        if (key) {
            os << ' ' << s;
        } else {
            os << "='" << s << "'";
        }
        key=!key;
    }
};

active_set::active_set(){}
active_set::~active_set(){}

void active_set::add(
    unsigned depth,
    const std::tr1::function<void(const std::string&)> &func
) {
    // Lifetime of storage_recorder is controlled by ptr_map
    recorders.insert(
        depth,
        new storage_recorder(func)
    );
}

void active_set::add(
    unsigned depth,
    const std::tr1::function<void(const char*, size_t, bool)> &func
) {
    // Lifetime of storage_recorder is controlled by ptr_map
    recorders.insert(
        depth,
        new stream_recorder(func)
    );
}

void active_set::clear(unsigned depth) {
    recorders.erase(
        recorders.lower_bound(depth),
        recorders.upper_bound(depth)
    );
}

active_set& active_set::operator<<(const char **str) {
    std::string data;
    {
        std::ostringstream oss;
        std::for_each(
            nt_iterator<const char*>(str),
            nt_iterator<const char*>(),
            attribute_printer(oss)
        );
        data = oss.str();
    }

    this->write(data.data(), data.size());
    return *this;
}

active_set& active_set::write(const char *raw, size_t len) {
    typedef boost::ptr_multimap<unsigned, recorder> rec_t;
    BOOST_FOREACH(rec_t::value_type rec, recorders) {
        rec.second->stream().write(raw, len);
    }
    return *this;
}

active_set& active_set::operator<<(const std::string &data) {
    this->write(data.data(), data.size());
    return *this;
}

template<typename T>
active_set& active_set::operator<<(T t) {
    typedef boost::ptr_multimap<unsigned, recorder> rec_t;
    BOOST_FOREACH(rec_t::value_type rec, recorders) {
        rec.second->stream() << t;
    }
    return *this;
}

template active_set& active_set::operator<<(const char *);
template active_set& active_set::operator<<(unsigned char);
template active_set& active_set::operator<<(char);

}}
