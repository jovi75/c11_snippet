#include <memory>
#include <inttypes.h>
#include <functional>

// 为定义的模板类，编译期显示推导错误
// template<typename T>
// class TD;

// void testDeclType() {
//     const int theNum = 10;

//     auto x = theNum;
//     auto y = &theNum;

//     TD<decltype(x)> xType;
//     TD<decltype(y)> yType;
// }


template <typename T, typename... Ts>
std::unique_ptr<T> make_unique(Ts &&... params)
{
    return std::unique_ptr<T>(new T(std::forward<Ts>(params)...));
}

// 以编译期常量形式返回数组尺寸(数组实参包含尺寸)
template <typename T, std::size_t N>
constexpr std::size_t arraySize(T (&)[N]) noexcept
{
    return N;
}

// C++11实现C++14几个工具
namespace std
{
    template <class T>
    using remove_const_t = typename remove_const<T>::type;

    template <class T>
    using remove_reference_t = typename remove_reference<T>::type;

    template <class T>
    using add_lvalue_reference_t = typename add_lvalue_reference<T>::type;
} // namespace std


class Person {
public:

    template<
        typename T,
        typename = typename std::enable_if<
            !std::is_same<Person, typename std::decay<T>::type>::value
        >::type
    >
    explicit Person(T&& n);

    template<
        typename T,
        typename = typename std::enable_if<
            !std::is_base_of<Person, typename std::decay<T>::type>::value
        >::type
    >
    explicit Person(T&& n);
};

