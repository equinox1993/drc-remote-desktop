// Author: Yuwei Huang, 9/17/2020

#include <X11/Xlib.h>

namespace drc {

class InputData;

} // namespace drc


namespace drd {

class InputInjector {
 public:
  explicit InputInjector(Display* display);
  ~InputInjector();

  void HandleInputData(const drc::InputData& input_data);

 private:
  void HandleButtonState(bool* last_state, bool new_state, unsigned int button);

  Display* display_;
  bool is_left_button_clicked_ = false;
  bool is_right_button_clicked_ = false;
};

} // namespace drd
