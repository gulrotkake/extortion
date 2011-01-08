#include "nt_iterator.hpp"

template<class T>
nt_iterator<T>::nt_iterator()
    : p(), pte(true)
{}

template<class T>
nt_iterator<T>::nt_iterator(pointer p_)
    : p(p_),
      pte(!p_ || !(*p_))
{}

template<class T>
nt_iterator<T>::nt_iterator(const nt_iterator<T> &rhs)
    : p(rhs.p),
      pte(rhs.pte)
{}

template<class T>
nt_iterator<T> & nt_iterator<T>::operator++() {
    ++p;
    if (!*p) pte = true; // once pass-the-end, always pass-the-end
    return *this;
}

template<class T>
bool nt_iterator<T>::operator==(const nt_iterator<T> &rhs) const {
    return (pte && rhs.pte) || p == rhs.p;
}

template<class T>
bool nt_iterator<T>::operator!=(const nt_iterator<T> &rhs) const {
    return !(operator==(rhs));
}

template<class T>
typename nt_iterator<T>::value_type nt_iterator<T>::operator*() {
    return *p;
}

template class nt_iterator<const char*>;
