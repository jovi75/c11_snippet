#include <atomic>
#include <iostream>
#include <thread>

using namespace std;

atomic<int> a {0};
atomic<int> b {0};

int ValueSet(int) {
    int t = 1;
    //a = t;
    a.store(t, memory_order_relaxed);
    //b = 2;
    b.store(2, memory_order_relaxed);
    return 0;
}

int Observer(int) {
    // 可能有多种输出
    cout << "(" << a << ", " << b <<")" << endl;
    return 0;
}

int main() {
    thread t1(ValueSet, 0);
    thread t2(Observer, 0);

    t1.join();
    t2.join();

    // got (1, 2)
    cout << "got (" << a << ", " << b << ")" << endl;

    cout << "cpu cores: " << std::thread::hardware_concurrency() << endl;
}