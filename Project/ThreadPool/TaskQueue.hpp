#pragma once
/*
* Thread safe task queue
 */
#include <queue>
#include <mutex>

template<typename T>
class TaskQueue {
	using container_type = T;
public:
	TaskQueue() = default;
	TaskQueue(TaskQueue&& other) = default;
	~TaskQueue() = default;

	bool empty() const {
		std::lock_guard<std::mutex> lock(access_queue_mutex);
		return task_queue.empty();
	}

	int size() const {
		std::lock_guard<std::mutex> lock(access_queue_mutex);
		return task_queue.size();
	}

	void emplace(const container_type& task) {
		std::lock_guard<std::mutex> lock(access_queue_mutex);
		task_queue.push(task);
	}

	bool get_front(container_type& task) {
		std::lock_guard<std::mutex> lock(access_queue_mutex);

		if (task_queue.empty()) return false;
		task = std::move(task_queue.front());
		task_queue.pop();
		return true;
	}
private:
	std::queue<T> task_queue;
	mutable std::mutex access_queue_mutex;
};