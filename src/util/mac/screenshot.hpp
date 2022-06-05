#ifndef MAC_SCREENSHOT_HPP_
#define MAC_SCREENSHOT_HPP_

#include <CoreGraphics/CGWindow.h>
#include <stdint.h>

#include "../../../config.hpp"
#include "../compiletime.hpp"
#include "../xoshiro.hpp"

namespace screen {
screen::type shot();  // namespace issues
}  // namespace screen

screen::type screen::shot() {
  static uint64_t rnd = 0;

  // This makes the dubious assumption that
  //   this screen is the same one that we used
  //   to configure the program.
  // It's mildly error-prone, but
  //   unless we can assert only ONCE
  //   that the screen dimensions are the same,
  //   it's worth it for the compile-time guarantees.
  CGImageRef im = CGWindowListCreateImage(
        CGRectInfinite,
        kCGWindowListOptionOnScreenOnly,
        kCGNullWindowID,
        kCGWindowImageDefault);
  CFDataRef data = CGDataProviderCopyData(CGImageGetDataProvider(im));
  CGImageRelease(im);
  // Let's manually take 1 every screen::stride pixels in each dimension.
  if (!rnd) rnd = xoshiro::next();  // technically inaccurate but good enough
  uint8_t oy = rnd & screen::stride_mask;
  rnd >>= screen::lgs;
  uint8_t ox = rnd & screen::stride_mask;
  rnd >>= screen::lgs;
  // It's gotta take a fucking long time caching for row- to column-major...
  // Unfortunately it seems like TensorMap<TensorFixedSize> is broken,
  //   or I'm just using it wrong--might come back to it later
  // Eigen requires long rather than size_t
  uint8_t const* bptr = CFDataGetBytePtr(data) + (((oy * w) + ox) << 2);
  screen::type t;
  for (long y = 0; y < static_cast<long>(screen::h_post); ++y) {
    for (long x = 0; x < static_cast<long>(screen::w_post); ++x) {
      t(y, x, 2) = bptr[0];
      t(y, x, 1) = bptr[1];
      t(y, x, 0) = bptr[2];
      bptr += compiletime<screen::stride << 2>;
    }
    bptr += compiletime<((screen::stride - 1) * screen::w) << 2>;
  }
  CFRelease(data);
  return t;
}

#endif  // MAC_SCREENSHOT_HPP_