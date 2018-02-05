#include <gtest/gtest.h>
#include <vector>
#include <cassert>
#include <string>
#include <algorithm>
#include <cctype>

namespace preds {

const auto less_by_size = [](const auto& a, const auto& b){
  return a.size() < b.size();
};
const auto equal_by_size = [](auto size){
  return [size](const auto& v){ return size == v.size(); };
};

auto equal_case_insensitive = [](const std::string& bingo) {
  return [&bingo](const std::string& s){
    if(bingo.size() != s.size())
      return false;
    return std::equal(s.begin(), s.end(), bingo.begin(), [](char a, char b){
      return std::tolower(a) == std::tolower(b);
    });
  };
};

} // namespace preds

TEST(Preds, SortAndFindByLength) {
  auto names = std::vector<std::string> { "Ralph", "Lisa", "Homer", "Maggie", "Apu", "Bart" };
  std::sort(names.begin(), names.end(), [](const std::string& a, const std::string& b){
    return a.size() < b.size();
  });
  // names is now "Apu", "Lisa", "Bart", "Ralph", "Homer", "Maggie"

  auto target_length = size_t{3};
  auto bingo = std::find_if(names.begin(), names.end(), [target_length](const auto& v){
    return v.size() == target_length;
  });
  ASSERT_TRUE(*bingo == "Apu");
}

TEST(Preds, CountMaggie) {
  auto names = std::vector<std::string> { "Ralph", "Lisa", "Homer", "Maggie", "Apu", "Bart" };
  std::sort(names.begin(), names.end(), preds::less_by_size);
  auto bingo = std::find_if(names.begin(), names.end(), preds::equal_by_size(3));
  ASSERT_TRUE(*bingo == "Apu");

  auto maggie = std::string{"maggie"};
  auto num_maggies = std::count_if(names.begin(), names.end(), preds::equal_case_insensitive(maggie));
  ASSERT_TRUE(num_maggies == 1);
}
