#ifndef HUSKY_BASE__HUSKY_HARDWARE_HPP_
#define HUSKY_BASE__HUSKY_HARDWARE_HPP_

#include <memory>
#include <string>
#include <vector>

#include "hardware_interface/handle.hpp"
#include "hardware_interface/hardware_info.hpp"
#include "hardware_interface/system_interface.hpp"
#include "hardware_interface/types/hardware_interface_return_values.hpp"
#include "rclcpp/clock.hpp"
#include "rclcpp/duration.hpp"
#include "rclcpp/macros.hpp"
#include "rclcpp/time.hpp"
#include "rclcpp_lifecycle/node_interfaces/lifecycle_node_interface.hpp"
#include "rclcpp_lifecycle/state.hpp"
#include "hardware_interface/visibility_control.h"

#include <chrono>

#include "rclcpp/rclcpp.hpp"

#include "husky_base/horizon_legacy_wrapper.h"

using namespace std::chrono_literals;
using CallbackReturn = rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn;

namespace husky_base
{

class HuskyHardware : public hardware_interface::SystemInterface
{
public:
  RCLCPP_SHARED_PTR_DEFINITIONS(HuskyHardware)

  HARDWARE_INTERFACE_PUBLIC
  CallbackReturn on_init(const hardware_interface::HardwareInfo & info) override;

  HARDWARE_INTERFACE_PUBLIC
  std::vector<hardware_interface::StateInterface> export_state_interfaces() override;

  HARDWARE_INTERFACE_PUBLIC
  std::vector<hardware_interface::CommandInterface> export_command_interfaces() override;

  HARDWARE_INTERFACE_PUBLIC
  CallbackReturn on_activate(const rclcpp_lifecycle::State & previous_state) override;

  HARDWARE_INTERFACE_PUBLIC
  CallbackReturn on_deactivate(const rclcpp_lifecycle::State & previous_state) override;

  HARDWARE_INTERFACE_PUBLIC
  hardware_interface::return_type read() override;

  HARDWARE_INTERFACE_PUBLIC
  hardware_interface::return_type write() override;

private:
  void resetTravelOffset();
  double linearToAngular(const double &travel) const;
  double angularToLinear(const double &angle) const;
  void writeCommandsToHardware();
  void limitDifferentialSpeed(double &diff_speed_left, double &diff_speed_right);
  void updateJointsFromHardware();
  uint8_t isLeft(const std::string &str);

  // ROS Parameters
  std::string serial_port_;
  double polling_timeout_;
  double wheel_diameter_, max_accel_, max_speed_;

  // Store the command for the robot
  std::vector<double> hw_commands_;
  std::vector<double> hw_states_position_, hw_states_position_offset_, hw_states_velocity_;

  uint8_t left_cmd_joint_index_, right_cmd_joint_index_;
};

}  // namespace husky_base

#endif  // HUSKY_BASE__HUSKY_HARDWARE_HPP_
