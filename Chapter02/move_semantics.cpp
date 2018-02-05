#if _MSC_VER
  #pragma warning(disable:4996)
#endif

#include <gtest/gtest.h>
#include <cctype>
#include <algorithm>
#include <initializer_list>
#include <locale>
#include <vector>

class Bird {
public:
  Bird() {}

  // Copy-constructor/copy-assignment
  Bird(const Bird&) = default;
  auto operator=(const Bird&)->Bird& = default;

  // Move-constructor/move-assignment
  Bird(Bird&&) noexcept = default;
  auto operator=(Bird&&) noexcept->Bird& = default;

  // Destructor
  ~Bird() = default;

  auto set_song(const std::string& s) {
    song_ = s;
  }

  auto set_song(std::string&& s) {
    song_ = std::move(s);
  }

  std::string song_;
};

class Buffer {
  public:

  // Constructor
  Buffer(const std::initializer_list<float>& values)
    : size_{values.size()} {
    ptr_ = new float[values.size()];
    std::copy(values.begin(), values.end(), ptr_);
  }

  // 1. Copy constructor
  Buffer(const Buffer& other)
    : size_{other.size_} {
    ptr_ = new float[size_];
    std::copy(other.ptr_, other.ptr_ + size_, ptr_);
  }

  // 2. Copy assignment
  auto& operator=(const Buffer& other) {
    delete [] ptr_;
    ptr_ = new float[other.size_];
    size_ = other.size_;
    std::copy(other.ptr_, other.ptr_ + size_, ptr_);
    return *this;
  }

  // 3. Destructor
  ~Buffer() {
    delete [] ptr_; // Note, it is valid to delete a nullptr
    ptr_ = nullptr;
  }

  // Iterators for accessing the data
  auto begin() const {
    return ptr_;
  }

  auto end() const {
    return ptr_ + size_;
  }

private:
  size_t size_{0};
  float* ptr_{nullptr};
};

class TowerList {
public:
  TowerList()
  : max_height_idx_{2}
  , tower_heights_{25.0f, 33.0f, 44.0f, 12.0f} {
  }
  TowerList(const TowerList&) = default;
  auto operator=(const TowerList&) -> TowerList& = default;

  TowerList(TowerList&& tl) noexcept {
    this->swap(tl);
  }
  auto operator=(TowerList&& tl) noexcept -> TowerList& {
    this->swap(tl);
    return *this;
  }
  auto swap(TowerList& tl) noexcept -> void {
    std::swap(tower_heights_, tl.tower_heights_);
    std::swap(max_height_idx_, tl.max_height_idx_);
  }
  auto get_max_tower_height() const {
    return max_height_idx_ >= 0 ?
      tower_heights_[max_height_idx_] :
      0.0f;
  }
  auto clear() {
    tower_heights_.clear();
    max_height_idx_ = -1;
  }
  std::vector<float> tower_heights_{};
  int max_height_idx_{};
};

auto str_to_lower(std::string str) {
  for(auto& c: str) c = std::tolower(c);
  return str;
}

TEST(MoveSemantics, Buffer) {
  // Construct
  auto b0 = Buffer({0.0f, 0.5f, 1.0f, 1.5f});
  // 1. Copy-construct
  auto b1 = b0;
  // 2. Copy-assignment as b0 is already initialized
  b0 = b1;
  // 3. When the function exits, the destructors are automatically invoked
}

TEST(MoveSemantics, TowerList) {
  auto a = TowerList{};
  auto b = TowerList{};
  b.clear();
  b = std::move(a);
  ASSERT_TRUE(a.get_max_tower_height() == 0);
  ASSERT_TRUE(b.get_max_tower_height() > 0);
}

TEST(MoveSemantics, StrToLower) {
  auto upper = std::string{"ABC"};
  auto lower = str_to_lower(std::move(upper));
  ASSERT_TRUE(upper.empty());
  ASSERT_FALSE(lower.empty());
}
