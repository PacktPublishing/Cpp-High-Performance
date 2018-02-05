#include <queue>
#include <string>
#include <condition_variable>
#include <iostream>
#include <thread>
#include <future>
#include <chrono>
#include <gtest/gtest.h>

std::mutex mtx;
std::condition_variable cv;
std::queue<int> q;
const int done = -1;
std::vector<int> result;

void print_positive_ints() {
  auto i = 0;
  while (i != done) {
    {
      std::unique_lock<std::mutex> lock(mtx);
      while (q.empty()) {
        cv.wait(lock);
      }
      i = q.front();
      q.pop();
    }
    if (i != done) {
      std::cout << "Got: " << i << '\n';
      result.push_back(i);
    }
  }
}

void generate_ints() {
  for (auto i : {1, 2, 3, done}) {
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    {
      std::lock_guard<std::mutex> lock(mtx);
      q.push(i);
    }
    cv.notify_one();
  }
}

TEST(ProducerConsumer, PushPopInts) {
  std::thread producer(generate_ints);
  std::thread consumer(print_positive_ints);

  producer.join();
  consumer.join();

  ASSERT_EQ(std::vector<int>({1, 2, 3}), result);
}
