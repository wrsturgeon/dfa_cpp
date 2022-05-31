#include <CoreGraphics/CGImage.h>
#include <ImageIO/CGImageDestination.h>

#include <iostream>

#include "../config/sizes.hpp"

namespace screen {  // Declaration, no matter what OS (manually included)
extern screen::type shot();
}

int main() {
  screen::type cm;
  Eigen::TensorFixedSize<uint8_t, screen::Size, Eigen::RowMajor> rm;
  CGColorSpaceRef cs = CGColorSpaceCreateDeviceRGB();
  CFDataRef data;
  CGDataProviderRef prov;
  CGImageRef im;
  CFStringRef cfstr;
  CFURLRef url;
  CGImageDestinationRef dst;

  for (uint8_t i = 0; ++i;) {
    cm = screen::shot();  // Column-major
    // Again, unfortunately I think TensorMap<TensorFixedSize> is broken
    for (long y = 0; y < static_cast<long>(screen::h_post); ++y) {
      for (long x = 0; x < static_cast<long>(screen::w_post); ++x) {
        for (long c = 0; c < static_cast<long>(screen::c_post); ++c) {
          rm(y, x, c) = cm(y, x, c);
        }
      }
    }

    data = CFDataCreate(nullptr, rm.data(), rm.size());
    prov = CGDataProviderCreateWithCFData(data);
    im = CGImageCreate(
          screen::w_post,
          screen::h_post,
          8,
          screen::c_post << 3,  // << 3 === * 8
          screen::w_post * screen::c_post,
          cs,
          kCGBitmapByteOrderDefault,
          prov,
          nullptr,
          false,
          kCGRenderingIntentDefault);
    cfstr = CFStringCreateWithCString(
          kCFAllocatorDefault,
          (std::string("out/screen_") + std::to_string(+i) + ".png").c_str(),
          kCFStringEncodingUTF8);
    url = CFURLCreateWithFileSystemPath(
          kCFAllocatorDefault,
          cfstr,
          kCFURLPOSIXPathStyle,
          false);
    if (!(dst = CGImageDestinationCreateWithURL(
                url,
                CFSTR("public.png"),
                1,
                nullptr)))
      throw std::runtime_error("Could not create image destination");
    CGImageDestinationAddImage(dst, im, nullptr);
    if (!CGImageDestinationFinalize(dst)) throw std::runtime_error("poop lol");
    CFRelease(url);
    CGDataProviderRelease(prov);
    CFRelease(data);
    CGColorSpaceRelease(cs);
    CFRelease(dst);
    CGImageRelease(im);
  }
}