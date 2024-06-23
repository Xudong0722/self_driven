#pragma once
#include <thread>
#include <functional>
#include <future>
#include <utility>
#include <memory>
#include <mutex>
#include <vector>
#include "TaskQueue.hpp"

class ThreadPool {
private:
	class ThreadWorker {
	public:
		ThreadWorker(ThreadPool* pool_ = nullptr, const int id_ = 0)
			: m_pool(pool_), m_id(id_) { }

		void operator()() {
			std::function<void()> func;
			bool is_dequeued{ false };
			while (!m_pool->m_shutdown) {
				{
					std::unique_lock<std::mutex> lock(m_pool->m_conditional_mutex);

					if (m_pool->m_queue.empty()) {
						m_pool->m_contionional_lock.wait(lock);
					}
					is_dequeued = m_pool->m_queue.get_front(func);
				}
				if (is_dequeued) func();
			}
		}

	private:
		ThreadPool* m_pool;
		int m_id;
	};

	bool m_shutdown;  //Is current thread pool closed?

	TaskQueue<std::function<void()> > m_queue;  //task queue

	std::vector<std::thread> m_threads; //thread queue

	std::mutex m_conditional_mutex;  //线程休眠锁

	std::condition_variable m_contionional_lock;  //线程环境锁

public:
	ThreadPool(const int thread_nums = 10)
		: m_threads(std::vector<std::thread>(thread_nums)), m_shutdown(false) {}

	ThreadPool(const ThreadPool&) = delete;
	ThreadPool(ThreadPool &&) = delete;
	ThreadPool& operator=(const ThreadPool&) = delete;
	ThreadPool& operator=(ThreadPool&&) = delete;
	
	template<typename F, typename... Args>
	auto submit(F&& f, Args&&... args) -> std::future<decltype(f(args...))> {

		std::function<decltype(f(args...))()> func = std::bind(std::forward<F>(f), std::forward<Args>(args)...);

		auto task_ptr = std::make_shared< std::packaged_task<decltype(f(args...))()>>(func);

		std::function<void()> wrapper_func = [task_ptr]() {
			(*task_ptr)();
		};

		m_queue.emplace(wrapper_func);

		m_contionional_lock.notify_one();

		return task_ptr->get_future();
	}

	void init() {
		for (std::vector<std::thread>::size_type i = 0; i < m_threads.size(); ++i) {
			m_threads.at(i) = std::thread(ThreadWorker{ this, static_cast<int>(i) });
		}
	}

	void shutdown() {
		m_shutdown = true;
		m_contionional_lock.notify_all();

		for (std::vector<std::thread>::size_type i = 0; i < m_threads.size(); ++i) {
			if (m_threads.at(i).joinable()) {
				m_threads.at(i).join();
			}
		}
	}
};