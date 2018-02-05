#include <atomic>
#include <array>
#include <cassert>
#include <iostream>
#include <thread>
#include <gtest/gtest.h>

template <class T, size_t N>
class LockFreeQueue {
public:
  LockFreeQueue()
    : size_{0}
    , read_pos_{0}
    , write_pos_{0} {
    assert(size_.is_lock_free());
  }

  auto size() const {
    return size_.load();
  }

  // Writer thread
  auto push(const T& t) {
    if (size_.load() >= N) {
      throw std::overflow_error("Queue is full");
    }
    buffer_[write_pos_] = t;
    write_pos_ = (write_pos_ + 1) % N;
    size_.fetch_add(1);
  }

  // Reader thread
  auto& front() const {
    auto s = size_.load();
    if (s == 0) {
      throw std::underflow_error("Queue is empty");
    }
    return buffer_[read_pos_];
  }

  // Reader thread
  auto pop() {
    if (size_.load() == 0) {
      throw std::underflow_error("Queue is empty");
    }
    read_pos_ = (read_pos_ + 1) % N;
    size_.fetch_sub(1);
  }

private:
  std::array<T, N> buffer_{};  // Used by both threads
  std::atomic<size_t> size_{}; // Used by both threads
  size_t read_pos_ = 0;    // Used by reader thread
  size_t write_pos_ = 0;   // Used by writer thread
};

constexpr auto max_size = 10000;
constexpr auto done = -1;

TEST(LockFreeQueue, PuthAndPop) {
  LockFreeQueue<int, max_size> queue;
  std::vector<int> result;

  std::thread writer{[&queue]() {
    for (auto i = 0; i < 10; ++i) {
      queue.push(i);
    }
    queue.push(done);
  }};

  std::thread reader{[&queue, &result]() {
    for (;;) {
      while (queue.size() == 0) {
        // busy wait
      }
      auto element = queue.front();
      queue.pop();
      if (element == done) {
        break;
      }
      std::cout << "Got: " << element << '\n';
      result.push_back(element);
    }
  }};

  writer.join();
  reader.join();

  ASSERT_EQ(std::vector<int>({0, 1, 2, 3, 4, 5, 6, 7, 8, 9}), result);
}
