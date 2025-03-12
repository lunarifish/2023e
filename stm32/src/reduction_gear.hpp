
/**
 * @file    reduction_gear.hpp
 * @brief   用软件方式计算非直驱电机的输出轴位置
 */

#ifndef REDUCTION_GEAR_HPP
#define REDUCTION_GEAR_HPP

template <typename T>
class ReductionGear {
 public:
  ReductionGear() = delete;
  ~ReductionGear() = default;
  explicit ReductionGear(T cycle, T reduction_ratio = 1) : kCycle_{cycle}, kReductionRatio_{reduction_ratio} {}

  void Update(T encoder) {
    if (encoder < previous_ && previous_ - encoder > kCycle_ / 2) {
      cycles_++;
    } else if (encoder > previous_ && encoder - previous_ > kCycle_ / 2) {
      cycles_--;
    }
    previous_ = encoder;
  }

  void Reset() {
    previous_ = 0;
    cycles_ = 0;
  }

  [[nodiscard]] T value() const { return (previous_ + cycles_ * kCycle_) % (kCycle_ * kReductionRatio_); }

 private:
  T previous_{};
  int cycles_{};
  const T kCycle_{};
  const T kReductionRatio_{};
};

#endif  // REDUCTION_GEAR_HPP