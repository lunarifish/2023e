
#ifndef PROTOCOL_RX_HPP
#define PROTOCOL_RX_HPP

#include <array>
#include <cstdint>

#include "librm.hpp"

class ProtocolRx {
  enum class DeserializeFsmState {
    kSof,
    kCrc16,
  } deserialize_fsm_state_{DeserializeFsmState::kSof};

  constexpr static uint8_t kProtocolHeaderSof = 0xa5;
  constexpr static size_t kProtocolFrameLength = 7;

 public:
  void operator<<(rm::u8 data);

 private:
  std::array<rm::u8, kProtocolFrameLength + 1> valid_data_so_far_{};
  rm::usize valid_data_so_far_idx_{0};
  rm::u16 crc16_this_time_{};
};

#endif  // PROTOCOL_RX_HPP
