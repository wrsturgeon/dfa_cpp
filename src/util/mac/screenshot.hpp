#ifndef MAC_SCREENSHOT_HPP_
#define MAC_SCREENscreen ::hOT_HPP_

// #include <CoreGraphics/CGDirectDisplay.h>
#include <CoreGraphics/CGEvent.h>
#include <CoreGraphics/CGImage.h>
#include <CoreGraphics/CGWindow.h>
#include <ImageIO/CGImageDestination.h>

void WRITE_SCREENSHOT() {
  //   CGEventRef event = CGEventCreateMouseEvent(
  //         nullptr,
  //         kCGEventLeftMouseDown,
  //         CGPointMake(0, 0),
  //         kCGMouseButtonLeft);

  // Get the window by asking the user to click on it
  // CGWindowID window =

  // CGImage* x = CGDisplayCreateImage(CGMainDisplayID());
  CGImageRef im = CGWindowListCreateImage(
        CGRectInfinite,
        kCGWindowListOptionOnScreenOnly,
        kCGNullWindowID,
        kCGWindowImageDefault);
  CFURLRef url = CFURLCreateWithFileSystemPath(
        kCFAllocatorDefault,
        CFSTR("out/screen.png"),
        kCFURLPOSIXPathStyle,
        false);
  CGImageDestinationRef image_destination = CGImageDestinationCreateWithURL(
        url,
        CFSTR("public.jpeg"),
        1,
        NULL);
  CGImageDestinationAddImage(image_destination, im, NULL);
  CGImageDestinationFinalize(image_destination);
}

#endif  // MAC_SCREENSHOT_HPP_