#pragma once

#include <array>
#include <mutex>
#include <condition_variable>

template <typename T, size_t MAX_SIZE = 1024>
class BlockingQueue {
public:
    static_assert(MAX_SIZE > 0, "Max size must be greater than 0");

    BlockingQueue() = default;
    BlockingQueue(const BlockingQueue&) = delete;
    BlockingQueue& operator=(const BlockingQueue&) = delete;

    void Push(const T& item) {
        std::unique_lock<std::mutex> lock(mutex_);
        condVar_.wait(lock, [this]() { return Size() < Capacity(); });
        queue_[tail_] = item;
        tail_ = (tail_ + 1) % Capacity();
        condVar_.notify_all();
    }

    void Push(T&& item) {
        std::unique_lock<std::mutex> lock(mutex_);
        condVar_.wait(lock, [this]() { return Size() < Capacity(); });
        queue_[tail_] = std::move(item);
        tail_ = (tail_ + 1) % Capacity();
        condVar_.notify_all();
    }

    T Pop() {
        std::unique_lock<std::mutex> lock(mutex_);
        condVar_.wait(lock, [this]() { return Size() > 0; });
        T item = queue_[front_];
        front_ = (front_ + 1) % Capacity();
        condVar_.notify_all();
        return item;
    }

    bool TryPush(const T& item) {
        std::lock_guard<std::mutex> lock(mutex_);
        if (Size() >= Capacity()) {
            return false;
        }
        queue_[tail_] = item;
        tail_ = (tail_ + 1) % Capacity();
        condVar_.notify_all();
        return true;
    }

    bool TryPop(T& item) {
        std::lock_guard<std::mutex> lock(mutex_);
        if (Size() == 0) {
            return false;
        }
        item = queue_[front_];
        front_ = (front_ + 1) % Capacity();
        condVar_.notify_all();
        return true;
    }

    size_t Size() {
        return tail_ - front_;
    }

    size_t Capacity() const {
        return MAX_SIZE;
    }

private:
    uint32_t front_{0};
    uint32_t tail_{0};
    std::array<T, MAX_SIZE> queue_;
    std::mutex mutex_;
    std::condition_variable condVar_;
};