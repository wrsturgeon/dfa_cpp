#ifndef MAC_SCREENSHOT_HPP_
#define MAC_SCREENSHOT_HPP_

#include <CoreGraphics/CGWindow.h>
#include <stdint.h>

#include "../../../config/sizes.hpp"
#include "../xoshiro.hpp"

namespace screen {
screen::type shot();  // forward declaration to avoid namespace issues
}  // namespace screen

screen::type screen::shot() {
  static uint64_t rnd = !(
        // So I'm literally just inlining SplitMix64 to seed xoshiro
        //   ...and it looks like it works!

        xoshiro::s[0] = 0x9e3779b97f4a7c15,
        xoshiro::s[0] = (xoshiro::s[0] ^ (xoshiro::s[0] >> 30)) *
                        0xbf58476d1ce4e5b9,
        xoshiro::s[0] = (xoshiro::s[0] ^ (xoshiro::s[0] >> 27)) *
                        0x94d049bb133111eb,
        xoshiro::s[0] ^= xoshiro::s[0] >> 31,

        xoshiro::s[1] = xoshiro::s[1] + 0x9e3779b97f4a7c15,
        xoshiro::s[1] = (xoshiro::s[1] ^ (xoshiro::s[1] >> 30)) *
                        0xbf58476d1ce4e5b9,
        xoshiro::s[1] = (xoshiro::s[1] ^ (xoshiro::s[1] >> 27)) *
                        0x94d049bb133111eb,
        xoshiro::s[1] ^= xoshiro::s[1] >> 31,

        xoshiro::s[2] = xoshiro::s[2] + 0x9e3779b97f4a7c15,
        xoshiro::s[2] = (xoshiro::s[2] ^ (xoshiro::s[2] >> 30)) *
                        0xbf58476d1ce4e5b9,
        xoshiro::s[2] = (xoshiro::s[2] ^ (xoshiro::s[2] >> 27)) *
                        0x94d049bb133111eb,
        xoshiro::s[2] ^= xoshiro::s[2] >> 31,

        xoshiro::s[3] = xoshiro::s[3] + 0x9e3779b97f4a7c15,
        xoshiro::s[3] = (xoshiro::s[3] ^ (xoshiro::s[3] >> 30)) *
                        0xbf58476d1ce4e5b9,
        xoshiro::s[3] = (xoshiro::s[3] ^ (xoshiro::s[3] >> 27)) *
                        0x94d049bb133111eb,
        xoshiro::s[3] ^= xoshiro::s[3] >> 31);

  // This makes the dubious assumption that
  //   this screen is the same one that we used
  //   to configure the program.
  // It's mildly error-prone, but
  //   unless we can assert only ONCE
  //   that the screen dimensions are the same,
  //   it's worth it for the compile-time guarantees.
  CGImageRef im = std::move(CGWindowListCreateImage(
        CGRectInfinite,
        kCGWindowListOptionOnScreenOnly,
        kCGNullWindowID,
        kCGWindowImageDefault));
  CFDataRef data = std::move(
        CGDataProviderCopyData(CGImageGetDataProvider(im)));
  CGImageRelease(im);
  // Let's manually take 1 every screen::stride pixels in each dimension.
  uint8_t const* bptr = std::move(CFDataGetBytePtr(data));
  if (!rnd) rnd = xoshiro::next();  // technically inaccurate but good enough
  // uint8_t oy = rnd & screen::stride_mask;
  // rnd >>= screen::lgs;
  // uint8_t ox = rnd & screen::stride_mask;
  // rnd >>= screen::lgs;
  uint8_t oy = 0;
  uint8_t ox = 0;
  screen::type t;
  // It's gotta take a fucking long time caching for row- to column-major...
  // Unfortunately it seems like TensorMap<TensorFixedSize> is broken
  bptr += (ox * screen::c_post) +
          (oy * screen::w_post * screen::c_post);  // TODO: should be constexpr
  // Eigen requires long
  for (long y = 0; y < static_cast<long>(screen::h_post); ++y) {
    for (long x = 0; x < static_cast<long>(screen::w_post); ++x) {
      for (long c = 0; c < static_cast<long>(screen::c_post); ++c) {
        t(y, x, c) = bptr
              [((((y << screen::lgs) + oy) * screen::w + (x << screen::lgs) +
                 ox)
                << 2) +
               ((screen::c_post - 1) - c)];
      }
    }
  }
  CFRelease(data);
  return t;
}

#endif  // MAC_SCREENSHOT_HPP_