#if defined(__cpp_lib_optional) || _MSC_VER >= 1912

#include <gtest/gtest.h>
#include <optional>
#include <cassert>
#include <vector>

struct Hat {};
class Head {
public:
  Head() { assert(!hat_); } // hat_ is empty by default
  auto set_hat(const Hat& h){ hat_ = h; }
  auto has_hat() const { return hat_.has_value(); }
  auto& get_hat() const { assert(hat_.has_value()); return *hat_; }
  auto remove_hat() { hat_ = {}; } // Hat is cleared by assigning to {}
private:
  std::optional<Hat> hat_;
};

TEST(Optional, Head) {
  auto a = Head{};
  auto b = a;
  ASSERT_FALSE(a.has_hat());
  ASSERT_FALSE(b.has_hat());
  a.set_hat(Hat{});
  ASSERT_TRUE(a.has_hat());
  ASSERT_FALSE(b.has_hat());
}

TEST(Optional, Sort) {
  auto c = std::vector<std::optional<int>>{{3}, {}, {1}, {}, {2}};
  std::sort(c.begin(), c.end());
  const auto sorted = std::vector<std::optional<int>>{ {}, {}, {1}, {2}, {3} };
  ASSERT_EQ(sorted, c);
}

#endif
