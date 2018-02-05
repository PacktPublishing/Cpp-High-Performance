#include <gtest/gtest.h>

auto less_by_second = [](const auto& a, const auto& b) {
  return a.second < b.second;
};

template <typename T>
auto is_second_equal(const T& bingo) {
  return [&bingo](const auto& p) { return p.second == bingo; };
}

TEST(LambdaPolymorphic, BasicSyntaxExample) {
  auto v = 3;
  auto lambda = [v](auto v0, auto v1){ return v + v0*v1; };

  auto res_int = lambda(1, 2);
  ASSERT_EQ(5, res_int);

  auto res_float = lambda(1.0f, 2.0f);
  ASSERT_EQ(5.0f, res_float);
}

TEST(LambdaPredicates, Farm) {
  auto farm = std::vector< std::pair<std::string, int> >{
    {"Bear", 5},
    {"Deer", 0},
    {"Pig", 4}
  };
  std::sort(farm.begin(), farm.end(), less_by_second);
  auto missing_animals = std::count_if(farm.begin(), farm.end(), is_second_equal(0));
  ASSERT_TRUE(missing_animals == 1);
}

TEST(LambdaPredicates, Continents) {
  enum class EContinent { Europe, Asia, America };
  auto countries = std::vector<std::pair<std::string, EContinent>>{
    {"Sweden", EContinent::Europe},
    {"India", EContinent::Asia},
    {"Belarus", EContinent::Europe},
    {"Mexico", EContinent::America}
  };
  std::sort(countries.begin(), countries.end(), less_by_second);
  auto num_european_countries = std::count_if(
    countries.begin(),
    countries.end(),
    is_second_equal(EContinent::Europe)
  );
  ASSERT_TRUE(num_european_countries == 2);
}