#include <functional>

//C++14 lambda记录任意函数的执行时间
auto timeFuncInvocation =
    [](auto &&func, auto &&... params) { // c++14

        //...... 计时器启动;

        std::forward<decltype(func)>(func)(
            std::forward<decltype(params)>(params)...);

        //...... 计时器停止并记录流逝的时间;
    };


class PolyWidget {
public:
  template<typename T>
  void operator()(const T& param);

  //...
};


int main() {
    PolyWidget pw;
    auto boundPW = std::bind(pw, _1);

    //可接受任何类型的实参
    boundPW(123);
    boundPW(nullptr);
    boundPW("hello");
}
