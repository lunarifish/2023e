
#include <memory>

#include "librm.hpp"
#include "fdcan.h"

#include "reduction_gear.hpp"

std::unique_ptr<rm::hal::Can> can1{nullptr};
std::unique_ptr<rm::device::M2006> yaw_motor{nullptr}, pitch_motor{nullptr};
ReductionGear yaw_encoder{8191, 36}, pitch_encoder{8191, 36};

void MotorTest() {
  yaw_encoder.Update(yaw_motor->encoder());
  pitch_encoder.Update(pitch_motor->encoder());
  // yaw_motor->SetCurrent(1800);
  // rm::device::DjiMotor<>::SendCommand();
}

extern "C" {
void Init(void) {
  can1 = std::make_unique<rm::hal::Can>(hfdcan1);
  yaw_motor = std::make_unique<rm::device::M2006>(*can1, 1);
  pitch_motor = std::make_unique<rm::device::M2006>(*can1, 2);

  can1->SetFilter(0, 0);
  can1->Begin();
}
void MainLoop(void) { MotorTest(); }
}
