// Author: Yuwei Huang, 9/17/2020

#include "input_injector.h"

#include <X11/extensions/XTest.h>
#include <drc/input.h>

namespace drd {

namespace {

constexpr int kMotionScale = 25;

constexpr unsigned int kLeftButton = 1;
constexpr unsigned int kRightButton = 3;

} // namespace


InputInjector::InputInjector(Display* display) : display_(display) {}

InputInjector::~InputInjector() = default;

void InputInjector::HandleInputData(const drc::InputData& input_data) {
  if (!input_data.valid) {
    return;
  }

  int scaled_x_motion =
      static_cast<int32_t>(kMotionScale * input_data.left_stick_x);
  int scaled_y_motion =
      static_cast<int32_t>(-kMotionScale * input_data.left_stick_y);
  if (scaled_x_motion != 0 || scaled_y_motion != 0) {
    XTestFakeRelativeMotionEvent(
        display_, scaled_x_motion, scaled_y_motion, CurrentTime);
  }

  HandleButtonState(&is_left_button_clicked_,
      input_data.buttons & input_data.kBtnA, kLeftButton);
  HandleButtonState(&is_right_button_clicked_,
      input_data.buttons & input_data.kBtnB, kLeftButton);
}

void InputInjector::HandleButtonState(
    bool* last_state, bool new_state, unsigned int button) {
  if (*last_state != new_state) {
    XTestFakeButtonEvent(display_, button, new_state, CurrentTime);
    *last_state = new_state;
  }
}

} // namespace drd
