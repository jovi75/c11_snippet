#include <memory>
#include <inttypes.h>
#include <thread>

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

// c++11实现非成员版cbegin()
template <class C>
auto cbegin(const C &c) -> decltype(std::begin(c))
{
    return std::begin(c)
}

//C++14 lambda记录任意函数的执行时间
auto timeFuncInvocation =
    [](auto &&func, auto &&... params) { // c++14
        //...... 计时器启动;
        std::forward<decltype(func)>(func)(
            std::forward<decltype(params)>(params)...);
        //...... 计时器停止并记录流逝的时间;
    };

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

class PolyWidget {
public:
  template<typename T>
  void operator()(const T& param);

  //...
};


PolyWidget pw;
auto boundPW = std::bind(pw, _1);

//可接受任何类型的实参
boundPW(123);
boundPW(nullptr);
boundPW("hello");


class ThreadRAII {
public:
    enum class DtorAction { join, detach };
    // 右值thread，移动接管
    ThreadRAII(std::thread&& t, DtorAction a)
    : act(a), t(std::move(t)) {}

    // 确保在thread析构前调用join/detach
    ~ThreadRAII() {
        if (t.joinable()) {
            if (act == DtorAction::join) {
                t.join()
            } else {
                t.detach()
            }
        }
    }

    //支持移动操作
    ThreadRAII(ThreadRAII&&) = default;
    ThreadRAII& operator=(ThreadRAII&&) = default;

    std::thread& get() { return t; }

private:
    DtorAction act;
    std::thread t;  //thread最后一个声明，确保依赖已初始化
};
