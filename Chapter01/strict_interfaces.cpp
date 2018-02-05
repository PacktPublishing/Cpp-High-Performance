#include <cassert>
#include <memory>
#include <gtest/gtest.h>


class Engine {
public:
  auto set_oil_amount(float v) { oil_ = v; }
  auto get_oil_amount() const { return oil_; }
private:
  float oil_{};
};
class YamahaEngine : public Engine {
  public:
  private:
};


class Boat {
public:
  Boat(std::shared_ptr<Engine> e, float l)
  : engine_{e}
  , length_{l}
  {}
  auto set_length(float l) { length_ = l; }
  auto& get_engine() { return engine_; }
private:
  // Being a derivable class, engine_ has to be heap allocated
  std::shared_ptr<Engine> engine_;
  float length_{};
};


class BoatNoncopyable {
private:
  // Noncopyable
  BoatNoncopyable(const BoatNoncopyable& iother) = delete;
  auto operator=(const BoatNoncopyable& iother) -> BoatNoncopyable& = delete;
public:
  BoatNoncopyable(std::shared_ptr<Engine> e, float l) : length_{l}, engine_{e} {}
  auto set_length(float l) { length_ = l; }
  auto& get_engine() { return engine_; }
private:
  float length_{};
  std::shared_ptr<Engine> engine_;
};


TEST(StrictInterfaces, Boat) {

  {
    auto boat0 = Boat(std::make_shared<YamahaEngine>(), 6.7f);
    auto boat1 = boat0;
    // ... and does not realize that the oil amount applies to both boats
    boat1.set_length(8.56f);
    boat1.get_engine()->set_oil_amount(3.4f);
  }

  {
    auto&& boat0 = BoatNoncopyable{std::make_shared<YamahaEngine>(), 6.7f}; // [auto&& is needed in current version of MSVC]
    // Below code won't compile, the second programmer will have to find another solution compliant with the limitations of the Boat
    /* auto boat1 = boat0; */
  }
}

