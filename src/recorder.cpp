#include "recorder.hpp"

namespace extortion { namespace detail {

std::ostream& recorder::stream() {
    return do_stream();
}

recorder::~recorder() {}


storage_recorder::storage_recorder(
    const std::tr1::function<void(const std::string&)> &cb_
) :
    cb(cb_)
{}

storage_recorder::~storage_recorder() {
    if (cb) {
        cb(oss.str());
        oss.str("");
    }
}

std::ostream& storage_recorder::do_stream() {
    return oss;
}

cb_buf::cb_buf(const std::tr1::function<void(const char*, size_t, bool)> &cb_)
    : cb(cb_),
      first_write(true)
{}

std::streamsize cb_buf::xsputn(const char *data, std::streamsize size) {
    cb(data, size, first_write);
    first_write=true;
    return size;
}

std::streambuf::int_type cb_buf::overflow(
    std::streambuf::int_type v
) {
    char ch = std::streambuf::traits_type::to_char_type(v);
    cb(&ch, 1, first_write);
    first_write=true;
    return std::streambuf::traits_type::not_eof(v);
}

stream_recorder::stream_recorder(
    const std::tr1::function<void(const char*, size_t, bool)>& cb
) :
    buf(cb),
    os(&buf)
{}

std::ostream& stream_recorder::do_stream() {
    return os;
}

}}
