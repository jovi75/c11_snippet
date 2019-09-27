/*
    atomic_flag 实现自旋锁
*/

#include <atomic>
#include <iostream>
#include <thread>
#include <unistd.h>

using namespace std;

std::atomic_flag lock = ATOMIC_FLAG_INIT;


void f(int n) {
    // 尝试获得锁: atomic_flag::test_and_set()设置true并返回旧值
    while (lock.test_and_set(std::memory_order_acquire)) {
        cout << "f " << n << " waiting" << endl;        // 自旋
    }
    cout << "f " << n << " start" << endl;
}

void g(int n) {
    cout << "g " << n << " begin" << endl;
    lock.clear();
    cout << "g " << n << " end" << endl;
}

int main() {
    lock.test_and_set();
    thread t1(f, 1);
    thread t2(g, 2);

    t1.join();
    ::usleep(100);
    t2.join();
}
