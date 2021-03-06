// Author: Yuwei Huang, 9/17/2020

#include <X11/Xlib.h>
#include <drc/input.h>
#include <drc/screen.h>
#include <drc/streamer.h>
#include <unistd.h>

#include <iostream>
#include <memory>
#include <vector>

#include "modules/desktop_capture/desktop_frame.h"
#include "modules/desktop_capture/linux/x_atom_cache.h"
#include "modules/desktop_capture/linux/x_server_pixel_buffer.h"
#include "rtc_base/checks.h"
#include "rtc_base/time_utils.h"
#include "input_injector.h"
#include "vector_desktop_frame.h"

namespace {

constexpr int64_t k29_97HzInterval = 33367;

} // namespace


int main(int, char**) {
  std::unique_ptr<Display, decltype(&XCloseDisplay)>
  display(XOpenDisplay(NULL), XCloseDisplay);

  webrtc::XAtomCache atom_cache(display.get());
  webrtc::XServerPixelBuffer xspb;
  xspb.Init(&atom_cache, DefaultRootWindow(display.get()));

  drc::Streamer streamer;
  streamer.SetFrameRate(drc::VideoFrameRate::k29_97Hz);
  if (!streamer.Start()) {
    std::cerr << "Unable to start streamer" << std::endl;
    return 1;
  }

  drd::InputInjector input_injector(display.get());

  std::cout << "Streaming has started. Recommended screen resolution is "
            << drc::kScreenWidth << "x" << drc::kScreenHeight << std::endl;

  while (true) {
    int64_t start_time = rtc::TimeMicros();

    drc::InputData input_data;
    streamer.PollInput(&input_data);
    if (input_data.valid) {
      if (input_data.buttons & drc::InputData::kBtnHome) {
        break;
      }
      input_injector.HandleInputData(input_data);
    }

    webrtc::DesktopRect window_rect = xspb.window_rect();
    RTC_CHECK_EQ(0, window_rect.left());
    RTC_CHECK_EQ(0, window_rect.top());
    RTC_CHECK_LT(0, window_rect.width());
    RTC_CHECK_LT(0, window_rect.height());

    drd::VectorDesktopFrame desktop_frame(window_rect.size());

    webrtc::DesktopSize desktop_size = desktop_frame.size();

    xspb.Synchronize();
    xspb.CaptureRect(window_rect, &desktop_frame);
    streamer.PushVidFrame(desktop_frame.vector_data(), desktop_size.width(),
        desktop_size.height(), drc::PixelFormat::kBGRA);
    int64_t compute_time = rtc::TimeMicros() - start_time;
    int64_t sleep_duration =
        k29_97HzInterval - (compute_time % k29_97HzInterval);
    usleep(sleep_duration);
  }
  
  streamer.ShutdownPad();
  usleep(1000);

  streamer.Stop();
  xspb.Release();
}
