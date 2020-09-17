// Author: Yuwei Huang, 9/17/2020

#include "vector_desktop_frame.h"

namespace drd {


VectorDesktopFrame::VectorDesktopFrame(const webrtc::DesktopSize& size)
  : VectorDesktopFrame(
      size,
      std::vector<uint8_t>(kBytesPerPixel * size.width() * size.height(), 0)) {}

VectorDesktopFrame::VectorDesktopFrame(const webrtc::DesktopSize& size,
                                     std::vector<uint8_t> vector_data)
  : webrtc::DesktopFrame(
        size, kBytesPerPixel * size.width(), vector_data.data(), nullptr),
    vector_data_(std::move(vector_data)) {}

VectorDesktopFrame::~VectorDesktopFrame() = default;

std::vector<uint8_t>* VectorDesktopFrame::vector_data() {
  return &vector_data_;
}

} // namespace drd
