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
#include "vector_desktop_frame.h"

int main(int, char**) {
  std::unique_ptr<Display, decltype(&XCloseDisplay)>
  display(XOpenDisplay(NULL), XCloseDisplay);

  webrtc::XAtomCache atom_cache(display.get());
  webrtc::XServerPixelBuffer xspb;
  xspb.Init(&atom_cache, DefaultRootWindow(display.get()));

  drc::Streamer streamer;
  if (!streamer.Start()) {
    std::cerr << "Unable to start streamer" << std::endl;
    return 1;
  }


  while (true) {
    webrtc::DesktopRect window_rect = xspb.window_rect();
    RTC_CHECK_EQ(0, window_rect.left());
    RTC_CHECK_EQ(0, window_rect.top());
    RTC_CHECK_LT(0, window_rect.width());
    RTC_CHECK_LT(0, window_rect.height());

    drd::VectorDesktopFrame desktop_frame(window_rect.size());

    webrtc::DesktopSize desktop_size = desktop_frame.size();

    xspb.CaptureRect(window_rect, &desktop_frame);
    streamer.PushVidFrame(desktop_frame.vector_data(), desktop_size.width(),
        desktop_size.height(), drc::PixelFormat::kBGRA);
    usleep(16666);  // 60fps
  }

  streamer.Stop();
  xspb.Release();
}
