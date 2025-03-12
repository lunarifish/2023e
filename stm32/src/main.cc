
#include <memory>

#include "librm.hpp"
#include "fdcan.h"

std::unique_ptr<rm::hal::Can> can1{nullptr};
std::unique_ptr<rm::device::M2006> yaw_motor{nullptr};

void MotorTest() {
  yaw_motor->SetCurrent(1800);
  rm::device::DjiMotor<>::SendCommand();
}

extern "C" {
void Init(void) {
  can1 = std::make_unique<rm::hal::Can>(hfdcan1);
  yaw_motor = std::make_unique<rm::device::M2006>(*can1, 8);

  can1->SetFilter(0, 0);
  can1->Begin();
}
void MainLoop(void) { MotorTest(); }
}
