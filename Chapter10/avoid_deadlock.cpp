#include <mutex>
#include <iostream>
#include <gtest/gtest.h>

// This example demonstrates how to use std::lock to lock multiple locks at the same time.
// We do that to avoid the risk of having deadlocks in the transfer function.

struct Account {
  explicit Account(int credit) : credit_{credit} {}
  int credit_{};
  std::mutex m_{};
};

void transfer(Account& from, Account& to, int amount) {
  std::unique_lock<std::mutex> lock1(from.m_, std::defer_lock);
  std::unique_lock<std::mutex> lock2(to.m_, std::defer_lock);

  // Lock both unique_locks at the same time
  std::lock(lock1, lock2);

  from.credit_ -= amount;
  to.credit_ += amount;
}

TEST(AvoidDeadlock, MoneyTransfer) {
  Account account1{100};
  Account account2{30};
  transfer(account1, account2, 20);

  std::cout << "Account 1: " << account1.credit_ << '\n';
  std::cout << "Account 2: " << account2.credit_ << '\n';

  ASSERT_EQ(80, account1.credit_);
  ASSERT_EQ(50, account2.credit_);
}
