#ifndef RECORDER_HPP
#define RECORDER_HPP
#pragma once

#include <sstream>
#include <tr1/functional>

namespace extortion { namespace detail {

class recorder {
public:
    std::ostream& stream();
    virtual ~recorder();

private:
    virtual std::ostream& do_stream() = 0;
};

class storage_recorder : public recorder {
    std::ostringstream oss;
    std::tr1::function<void(const std::string&)> cb;
public:
    storage_recorder(const std::tr1::function<void(const std::string&)> &cb_);
    ~storage_recorder();

private:
    std::ostream& do_stream();
};

class cb_buf : public std::streambuf {
    std::tr1::function<void(const char*, size_t, bool)> cb;
    bool first_write; // Uck.
public:
    cb_buf(const std::tr1::function<void(const char*, size_t, bool)> &cb_);
    std::streamsize xsputn(const char *data, std::streamsize size);
    std::streambuf::int_type overflow(std::streambuf::int_type v);
};

class stream_recorder : public recorder {
    cb_buf buf;
    std::ostream os;
public:
    stream_recorder(
        const std::tr1::function<void(const char*, size_t, bool)>& cb
    );

private:
    std::ostream& do_stream();
};

}}

#endif // RECORDER_HPP
