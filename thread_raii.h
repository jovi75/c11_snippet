#include <thread>

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
