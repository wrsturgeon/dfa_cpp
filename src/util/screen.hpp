#ifndef UTIL_SCREEN_HPP_
#define UTIL_SCREEN_HPP_

#include <opencv2/core/hal/interface.h>  // CV_8UC3

#include <iostream>
#include <opencv2/core/mat.hpp>   // cv::Mat
#include <opencv2/imgcodecs.hpp>  // cv::imread
#include <stdexcept>

#include "Eigen/CXX11/Tensor"
#include "config/sizes.hpp"
#include "os_screenshot.hpp"

namespace screen {

typedef Eigen::TensorFixedSize<
      uint8_t,
      Eigen::Sizes<screen::h, screen::w, 3>,
      Eigen::StorageOptions::RowMajor>
      type;

screen::type read() {
  // TODO: These first 2 lines are fucking *horribly* inefficient

  // Write the contents of the screen to out/screen.png
  WRITE_SCREENSHOT();
  // Read that file
  cv::Mat s = cv::imread("out/screen.png", cv::IMREAD_COLOR);

  // Perform some basic checks
  if (s.empty()) throw std::runtime_error("Couldn't read out/screen.png");
  if (s.cols != screen::w || s.rows != screen::h)
    throw std::runtime_error("Non-screen size");
  if (s.type() != CV_8UC3)
    throw std::runtime_error(
          "Image type (" + std::to_string(s.type()) + ") not 8UC3 (" +
          std::to_string(CV_8UC3) + ")");

  // Pass to Eigen
  Eigen::TensorMap<screen::type, Eigen::StorageOptions::RowMajor> d(
        s.data,
        Eigen::Sizes<screen::h, screen::w, 3>());
  return d;
}

}  // namespace screen

#endif  // UTIL_SCREEN_HPP_