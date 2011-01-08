#ifndef NT_ITERATOR_HPP
#define NT_ITERATOR_HPP
#pragma once

#include <iterator>

/**
   Lifted from:
   http://stackoverflow.com/questions/4033071/\
   iterate-over-null-terminated-array-of-strings-using-for-each/4033248#4033248

   and slightly extended.
*/
template <class T>
class nt_iterator: public std::iterator<std::input_iterator_tag, T> {
    typedef typename nt_iterator<T>::pointer pointer;
    typedef typename nt_iterator<T>::value_type value_type;
    pointer p;
    bool pte;
public:

    nt_iterator();
    nt_iterator(pointer p_);
    nt_iterator(const nt_iterator<T> &rhs);

    nt_iterator<T> & operator++();

    bool operator==(const nt_iterator<T> &rhs) const;
    bool operator!=(const nt_iterator<T> &rhs) const;
    value_type operator*();
};

#endif // NT_ITERATOR_HPP
