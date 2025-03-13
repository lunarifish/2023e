
#include "librm.hpp"
#include "fdcan.h"
#include "usart.h"

#include "reduction_gear.hpp"

float yaw_target = 20000.f, pitch_target = 20000.f;
rm::hal::Can *can1{nullptr};
rm::device::M2006 *yaw_motor{nullptr}, *pitch_motor{nullptr};
rm::hal::Serial *dbus{nullptr};
rm::device::DR16 *dr16{nullptr};
struct {
  rm::modules::algorithm::PID<rm::modules::algorithm::PIDType::kPosition>  //
      yaw_pos{0.8, 0, 9, 1000, 0},                                         //
      yaw_speed{0.7, 0, 9, 5000, 0},                                       //
      pitch_pos{0.6, 0, 7, 1000, 0},                                       //
      pitch_speed{0.55, 0, 9, 5000, 0};                                    //
} pid;
ReductionGear yaw_encoder{8191, 36}, pitch_encoder{8191, 36};

void MotorTest() {
  yaw_encoder.Update(yaw_motor->encoder());
  pitch_encoder.Update(pitch_motor->encoder());
  const float yaw_pos = yaw_encoder.value();
  const float pitch_pos = pitch_encoder.value();

  pid.yaw_pos.Update(yaw_target, yaw_pos);
  pid.yaw_speed.Update(pid.yaw_pos.value(), yaw_motor->rpm());
  pid.pitch_pos.Update(pitch_target, pitch_pos);
  pid.pitch_speed.Update(pid.pitch_pos.value(), pitch_motor->rpm());
  yaw_motor->SetCurrent(pid.yaw_speed.value());
  pitch_motor->SetCurrent(pid.pitch_speed.value());
  rm::device::DjiMotor<>::SendCommand();
}

extern "C" {
void Init(void) {
  can1 = new rm::hal::Can(hfdcan1);
  yaw_motor = new rm::device::M2006(*can1, 2);
  pitch_motor = new rm::device::M2006(*can1, 1);
  dbus = new rm::hal::Serial(huart4, 50, rm::hal::stm32::UartMode::kNormal, rm::hal::stm32::UartMode::kDma);
  dr16 = new rm::device::DR16(*dbus);

  can1->SetFilter(0, 0);
  can1->Begin();
  // dr16->Begin();
}
void MainLoop(void) { MotorTest(); }
}
