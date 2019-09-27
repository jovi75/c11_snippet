#include <algorithm>

// c++11实现非成员版cbegin()
template <class C>
auto cbegin(const C &c) -> decltype(std::begin(c))
{
    return std::begin(c)
}
