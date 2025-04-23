

# 定义

```
Coroutines are computer program components that allow execution to be suspended and resumed, generalizing subroutines for cooperative multitasking. 
Coroutines are well-suited for implementing familiar program components such as cooperative tasks, exceptions, event loops, iterators, infinite lists and pipes.

They have been described as "functions whose execution you can pause".

Melvin Conway coined the term coroutine in 1958 when he applied it to the construction of an assembly program. The first published explanation of the coroutine appeared later, in 1963.
```

# 示例

```c++
#include <coroutine>
#include <iostream>
#include <functional>
#include <chrono>
#include <unistd.h>
#include <thread>

class IntReader {
public:
        bool await_ready() {
                return false;
        }

        void await_suspend(std::coroutine_handle<> handle) {
                std::thread thread([this, handle]() {
                                        sleep(3);
                                        value_ = 101;

                                        handle.resume();
                                });
                thread.detach();
        }

        int await_resume() {
                return value_;
        }

private:
        int value_{0};

};

struct Task {
        class promise_type {
        public:
                Task get_return_object() { return {};}
                std::suspend_never initial_suspend() { return {};}
                std::suspend_never final_suspend() noexcept { return {};}
                void unhandled_exception() {}
                void return_void() {}
        };
};

Task func() {
        IntReader reader1;
        int total = co_await reader1;
        std::cout << "after resume from reader1, current thread id: " << std::this_thread::get_id()<< '\n';
        IntReader reader2;
        total += co_await reader2;
        std::cout << "after resume from reader2, current thread id: " << std::this_thread::get_id()<< '\n';
        IntReader reader3;
        total += co_await reader3;
        std::cout << "after resume from reader3, current thread id: " << std::this_thread::get_id()<< '\n';
        std::cout << total << std::endl;
}
int main()
{
        func();

        for(int i = 0; i < 15; ++i) {
                std::cout <<"current thread id: " << std::this_thread::get_id() <<" this is " << i << " second.\n";
                sleep(1);
        }
        return 0;
}
```

```c++
#include <coroutine>
#include <iostream>
#include <functional>
#include <chrono>
#include <unistd.h>
#include <thread>

class IntReader {
public:
        bool await_ready() {
                return false;
        }

        void await_suspend(std::coroutine_handle<> handle) {
                std::thread thread([this, handle]() {
                                        static int seed = 0;
                                        value_ = ++ seed;

                                        handle.resume();
                                });
                thread.detach();
        }

        int await_resume() {
                return value_;
        }

private:
        int value_{0};

};

struct Task {
        class promise_type {
        public:
                Task get_return_object() {
                        return Task{ std::coroutine_handle<promise_type>::from_promise(*this) };
                }

                std::suspend_always yield_value(int val) {
                        value_ = val;
                        return {};
                }
                std::suspend_never initial_suspend() { return {};}
                std::suspend_never final_suspend() noexcept { return {};}
                void unhandled_exception() {}
                void return_void() {}

                int get_val() const {
                        return value_;
                }
        private:
                int value_{};
        };

public:
        Task(std::coroutine_handle<promise_type> handle) : coroutine_handle_(handle) {}
        int get_val() const {
                return coroutine_handle_.promise().get_val();
        }

        void Next() {
                coroutine_handle_.resume();
        }
private:
        std::coroutine_handle<promise_type> coroutine_handle_;
};

Task GetInt() {
        while(true) {
                IntReader reader;
                int value = co_await reader;
                co_yield value;
        }
}

Task func() {
        IntReader reader1;
        int total = co_await reader1;
        std::cout << "after resume from reader1, current thread id: " << std::this_thread::get_id()<< '\n';
        IntReader reader2;
        total += co_await reader2;
        std::cout << "after resume from reader2, current thread id: " << std::this_thread::get_id()<< '\n';
        IntReader reader3;
        total += co_await reader3;
        std::cout << "after resume from reader3, current thread id: " << std::this_thread::get_id()<< '\n';
        std::cout << total << std::endl;
}
int main()
{
        auto task = GetInt();
        std::string line;
        while(std::cin >> line) {
                std::cout << task.get_val() << std::endl;
                task.Next();
        }
        return 0;
}
```
创建协程的流程：
- 创建一个协程帧
- 在协程帧里面构建promise对象
- 把协程的参数拷贝到协程帧里
- 调用promise.get_return_object() 返回一个对象

promise_type中的定制点：
- initial_suspend: 在协程创建后调用， 可以控制是否挂起
- final_suspend: 
- return_value: 保存协程返回值
- unhandled_exception: 处理异常

```c++
#include <coroutine>
#include <iostream>
#include <thread>

namespace Coroutine {
  struct task {
    struct promise_type {
      promise_type() {
        std::cout << "1.create promie object\n";
      }
      task get_return_object() {
        std::cout << "2.create coroutine return object, and the coroutine is created now\n";
        return {std::coroutine_handle<task::promise_type>::from_promise(*this)};
      }
      std::suspend_never initial_suspend() {
        std::cout << "3.do you want to susupend the current coroutine?\n";
        std::cout << "4.don't suspend because return std::suspend_never, so continue to execute coroutine body\n";
        return {};
      }
      std::suspend_never final_suspend() noexcept {
        std::cout << "13.coroutine body finished, do you want to susupend the current coroutine?\n";
        std::cout << "14.don't suspend because return std::suspend_never, and the continue will be automatically destroyed, bye\n";
        return {};
      }
      void return_void() {
        std::cout << "12.coroutine don't return value, so return_void is called\n";
      }
      void unhandled_exception() {}
    };

    std::coroutine_handle<task::promise_type> handle_;
  };

  struct awaiter {
    bool await_ready() {
      std::cout << "6.do you want to suspend current coroutine?\n";
      std::cout << "7.yes, suspend becase awaiter.await_ready() return false\n";
      return false;
    }
    void await_suspend(
      std::coroutine_handle<task::promise_type> handle) {
      std::cout << "8.execute awaiter.await_suspend()\n";
      std::thread([handle]() mutable { handle(); }).detach();
      std::cout << "9.a new thread lauched, and will return back to caller\n";
    }
    void await_resume() {}
  };

  task test() {
    std::cout << "5.begin to execute coroutine body, the thread id=" << std::this_thread::get_id() << "\n";//#1
    co_await awaiter{};
    std::cout << "11.coroutine resumed, continue execcute coroutine body now, the thread id=" << std::this_thread::get_id() << "\n";//#3
  }
}// namespace Coroutine

int main() {
  Coroutine::test();
  std::cout << "10.come back to caller becuase of co_await awaiter\n";
  std::this_thread::sleep_for(std::chrono::seconds(1));

  return 0;
}
```

# 有栈协程

# 无栈协程

# 应用

# 参考

https://www.xinfinite.net/t/topic/3518 
http://www.uml.org.cn/c%2B%2B/202503064.asp
https://zplutor.github.io/2022/03/25/cpp-coroutine-beginner/
https://www.bennyhuo.com/book/cpp-coroutines/02-generator.html#%E9%97%AE%E9%A2%98-1-%E6%97%A0%E6%B3%95%E7%A1%AE%E5%AE%9A%E6%98%AF%E5%90%A6%E5%AD%98%E5%9C%A8%E4%B8%8B%E4%B8%80%E4%B8%AA%E5%85%83%E7%B4%A0
https://zhuanlan.zhihu.com/p/497224333
https://lewissbaker.github.io/2017/11/17/understanding-operator-co-await