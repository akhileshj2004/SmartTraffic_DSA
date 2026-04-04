#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>

#if defined(_WIN32) && !defined(_GLIBCXX_HAS_GTHREADS)
#include <windows.h>
namespace std {
    class mutex {
        CRITICAL_SECTION cs;
    public:
        mutex() { InitializeCriticalSection(&cs); }
        ~mutex() { DeleteCriticalSection(&cs); }
        void lock() { EnterCriticalSection(&cs); }
        void unlock() { LeaveCriticalSection(&cs); }
    };
    template <typename M>
    class lock_guard {
        M& m_ref;
    public:
        explicit lock_guard(M& m) : m_ref(m) { m_ref.lock(); }
        ~lock_guard() { m_ref.unlock(); }
    };
    namespace this_thread {
        template <class Rep, class Period>
        void sleep_for(const std::chrono::duration<Rep, Period>& rel_time) {
            auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(rel_time).count();
            Sleep(ms);
        }
    }
    class thread {
        HANDLE h;
    public:
        thread() : h(nullptr) {}
        template <typename Func, typename Obj>
        thread(Func f, Obj obj) {
            struct Context { Func f; Obj obj; };
            Context* ctx = new Context{f, obj};
            h = CreateThread(NULL, 0, [](LPVOID param) -> DWORD {
                Context* c = static_cast<Context*>(param);
                auto func = c->f;
                auto object = c->obj;
                (object->*func)();
                delete c;
                return 0;
            }, ctx, 0, NULL);
        }
        bool joinable() const { return h != nullptr; }
        void join() {
            if (h) {
                WaitForSingleObject(h, INFINITE);
                CloseHandle(h);
                h = nullptr;
            }
        }
    };
}
#endif

class TrafficApp {
public:
    void backgroundSimulation() {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        std::cout << "Thread run!" << std::endl;
    }
};

int main() {
    TrafficApp t;
    std::thread th(&TrafficApp::backgroundSimulation, &t);
    th.join();
    return 0;
}
