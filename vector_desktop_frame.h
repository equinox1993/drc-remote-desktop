// Author: Yuwei Huang, 9/17/2020

#include <stdint.h>

#include <vector>

#include "modules/desktop_capture/desktop_frame.h"

namespace drd {

class VectorDesktopFrame : public webrtc::DesktopFrame {
 public:
  explicit VectorDesktopFrame(const webrtc::DesktopSize& size);
  ~VectorDesktopFrame() override;

  std::vector<uint8_t>* vector_data();

 private:
  VectorDesktopFrame(const webrtc::DesktopSize& size,
                     std::vector<uint8_t> vector_data);

  std::vector<uint8_t> vector_data_;
};

} // namespace drd
