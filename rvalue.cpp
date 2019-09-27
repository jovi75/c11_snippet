#include <iostream>
#include <utility>

// 左值
void process_value(int& i) {
    std::cout << "LValue processed: " << i << std::endl;
}
void process_value(const int& i) {
    std::cout << "LValue processed const: " << i << std::endl;
}
// 右值
void process_value(int&& i) {
    std::cout << "RValue processed: " << i << std::endl;
}

// 转发传递：右值->左值
void forward_value(int&& i) {
    process_value(i);
}
// 精确传递:std::forward
template <typename T>
void perfect_forward_value(T&& val) {
    process_value(std::forward<T>(val));
}

int main() {
    int a = 0;

    //LValue processed: 0
    process_value(a);   //调用左值函数

    //RValue processed: 1
    process_value(1);   //调用右值函数

    //LValue processed: 2
    forward_value(2);   //转发传递：右值->左值

    // 精确传递
    a = 3;

    //LValue processed: 3
    perfect_forward_value(a);  //调用左值函数

    //RValue processed: 4
    perfect_forward_value(std::move(++a));   //std::move转为右值

    const int &b = 5;

    //LValue processed const: 5
    perfect_forward_value(b);

    //RValue processed: 6
    perfect_forward_value(6);

    return 0;
}
