#ifndef EXTORTION_HPP
#define EXTORTION_HPP
#pragma once

#include <string>
#include <tr1/functional>

namespace extortion {

class extortion {
private:
    class impl;
    impl *pimpl;
public:
    extortion();
    ~extortion();

    /**
       @param path The path pattern used to determine what to stream
       @param cb Callback function, if the path is not unique the callback will
                 be called multiple times.

       @return Whether or not the parser understood the path expression, if
               false is returned the statement is void and nothing will happen.
    */
    bool get(
        const std::string & path,
        const std::tr1::function<void(const std::string&)> & cb
    );

    /**
       @param path The path pattern used to determine what to stream
       @param cb Callback function, if the path is not unique, the a true bool
                 value indicates the start of a new block.

       @return Whether or not the parser understood the path expression, if
               false is returned the statement is void and nothing will happen.
    */
    bool stream(
        const std::string & path,
        const std::tr1::function<void(const char*, size_t, bool)> & cb
    );

    /**
       @param data A pointer to a buffer of XML data to parse
       @param len Number of bytes in the data buffer. 0 indicates no more data.
       @return Whether or not the data was successfully parsed.

       It is important that the last call to this method has a len of zero
       bytes.
    */
    bool parse(const char *data, size_t len);

    /**
       @param data A buffer of XML data to parse
       @return Whether or not the data was successfully parsed.

       This function assumes the entire XML document is within the "data"
       buffer. Calling this function multiple times on the same parser will
       result in undefined behaviour.
    */
    bool parse_string(const std::string & data);
};

}

#endif // EXTORTION_HPP
