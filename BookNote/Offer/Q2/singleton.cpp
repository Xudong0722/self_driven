#include <iostream>
#include <thread>
#include <mutex>

class Single_1
{
    Single_1()
    {
        std::cout << "Single_1\n";
    }

    ~Single_1()
    {
        std::cout << "~Single_1()\n";
    }

public:
    static Single_1 *GetInstance();
    static void DestoryInst();

private:
    static Single_1 *inst;
};

Single_1 *Single_1::inst = nullptr;

Single_1 *Single_1::GetInstance()
{
    if (nullptr == inst)
    {
        inst = new Single_1{};
    }
    return inst;
}

void Single_1::DestoryInst()
{
    if (nullptr != inst)
    {
        delete inst;
    }
}

class Single_2
{
private:
    Single_2()
    {
        std::cout << "Single_2()\n";
    }
    ~Single_2()
    {
        std::cout << "~Single_3()\n";
    }

public:
    static Single_2 *GetInstance()
    {
        std::unique_lock<std::mutex> lock(inst_mutex);

        if (nullptr == inst)
        {
            inst = new Single_2();
        }
        return inst;
    }

private:
    static std::mutex inst_mutex;
    static Single_2 *inst;
};

std::mutex Single_2::inst_mutex{};
Single_2 *Single_2::inst = nullptr;

class Single_3
{
private:
    Single_3()
    {
        std::cout << "Single_2()\n";
    }
    ~Single_3()
    {
        std::cout << "~Single_2()\n";
    }

public:
    static Single_3 &GetInstance()
    {
        static Single_3 inst;
        return inst;
    }
};

void test_single_1()
{
    auto t = Single_1::GetInstance();
    std::cout << std::this_thread::get_id() << " " << t << std::endl;
}

void test_single_2()
{
    auto t = Single_2::GetInstance();
    std::cout << std::this_thread::get_id() << " " << t << std::endl;
}

int main()
{
    // std::thread t2(test_single_1);
    // auto t1 = Single_1::GetInstance();
    // std::cout << std::this_thread::get_id() << " " << t1 << std::endl;

    // std::thread t3(test_single_1);
    // t2.join();
    // t3.join();
    // auto &t3 = Single_3::GetInstance();
    // std::cout << &t3 << std::endl;

    // Single_1::DestoryInst();

    std::thread a(test_single_2);
    auto m = Single_2::GetInstance();
    std::cout << std::this_thread::get_id() << " " << m << std::endl;

    std::thread b(test_single_2);
    a.join();
    b.join();
    return 0;
}