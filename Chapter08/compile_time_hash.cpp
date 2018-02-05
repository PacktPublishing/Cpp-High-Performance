#if _MSC_VER
  #pragma warning(disable:4996)
#endif

#include <algorithm>
#include <functional>
#include <unordered_map>
#include <gtest/gtest.h>

constexpr auto hash_function(const char* istring) -> size_t {
  auto sum = size_t{0};
  for (auto ptr = istring; *ptr != '\0'; ++ptr)
    sum += *ptr;
  return sum;
}

class PrehashedString {
public:
  template<size_t N>
  constexpr PrehashedString(const char(&istr)[N])
    : hash_(hash_function(&istr[0]))
    , size_(N - 1) // The subtraction is to avoid the null character at the end of the string
    , strptr_(&istr[0]) {
  }

  auto operator==(const PrehashedString& iother) const {
    return
      size_ == iother.size_ &&
      std::equal(c_str(), c_str() + size_, iother.c_str());
  }

  auto operator!=(const PrehashedString& iother) const {
    return !(*this == iother);
  }

  constexpr auto size() const {
    return size_;
  }

  constexpr auto get_hash() const {
    return hash_;
  }

  constexpr auto c_str() const -> const char* {
    return strptr_;
  }

private:
  size_t hash_{};
  size_t size_{};
  const char* strptr_{nullptr};
};

namespace std {
  template <>
  struct hash<PrehashedString> {
    constexpr auto operator()(const PrehashedString& istr) const {
      return istr.get_hash();
    }
  };
}

struct Bitmap {};

auto load_bitmap_from_filesystem(const char* path) -> Bitmap {
  // ...
  return Bitmap{};
}

auto get_bitmap_resource(const PrehashedString& path) -> const Bitmap& {
  // Static storage of all loaded bitmaps
  static auto loaded_bitmaps = std::unordered_map<PrehashedString, Bitmap>{};

  // If the bitmap is already in loaded_bitmaps, return it
  if (loaded_bitmaps.count(path) > 0) {
    return loaded_bitmaps.at(path);
  }

  // The bitmap isn't already loaded, load and return it
  auto bitmap = load_bitmap_from_filesystem(path.c_str());
  loaded_bitmaps.emplace(path, std::move(bitmap));

  return loaded_bitmaps.at(path);
}

auto test_prehashed_string() {
  const auto& hash_fn = std::hash<PrehashedString>{};
  const auto& str = PrehashedString("abc");
  return hash_fn(str);
}

TEST(CompileTimeHash, HashOfString) {
  auto prehashed = test_prehashed_string();
  auto hash = hash_function("abc");
  ASSERT_EQ(hash, prehashed);
}
