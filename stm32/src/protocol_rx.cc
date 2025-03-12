
#include "protocol_rx.hpp"

#include "librm.hpp"

void ProtocolRx::operator<<(rm::u8 data) {
  switch (deserialize_fsm_state_) {
    case DeserializeFsmState::kSof: {
      if (data == kProtocolHeaderSof) {
        deserialize_fsm_state_ = DeserializeFsmState::kCrc16;
        valid_data_so_far_[valid_data_so_far_idx_++] = data;
      } else {
        valid_data_so_far_idx_ = 0;
      }
      break;
    }

    case DeserializeFsmState::kCrc16: {
      if (valid_data_so_far_idx_ < (kProtocolFrameLength)) {
        valid_data_so_far_[valid_data_so_far_idx_++] = data;
      }
      if (valid_data_so_far_idx_ >= (kProtocolFrameLength)) {
        deserialize_fsm_state_ = DeserializeFsmState::kSof;
        valid_data_so_far_idx_ = 0;
        crc16_this_time_ =
            (valid_data_so_far_[kProtocolFrameLength - 1] << 8) | valid_data_so_far_[kProtocolFrameLength - 2];

        if (rm::modules::algorithm::Crc16(valid_data_so_far_.data(), kProtocolFrameLength - 2,
                                          rm::modules::algorithm::CRC16_INIT) == crc16_this_time_) {
          // 整包接收完+校验通过
          // TODO
        }
      }
      break;
    }

    default: {
      deserialize_fsm_state_ = DeserializeFsmState::kSof;
      valid_data_so_far_idx_ = 0;
      break;
    }
    }
  }

