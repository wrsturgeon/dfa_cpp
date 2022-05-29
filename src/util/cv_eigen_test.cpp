#include <stdint.h>

#include <iostream>
#include <opencv2/core/mat.hpp>

#include "config/sizes.hpp"
#include "eigen/unsupported/Eigen/CXX11/Tensor"

typedef Eigen::TensorFixedSize<
      uint8_t,
      Eigen::Sizes<screen::h, screen::w, 3>,
      Eigen::StorageOptions::RowMajor>
      poop;

// int main() {
//   uint8_t* ptr = new uint8_t[screen::bytes];
//   Eigen::TensorMap<poop, Eigen::StorageOptions::RowMajor> t(ptr,
//   Eigen::Sizes<screen::h, screen::w, 3>()); cv::Mat m(
//         screen::h,
//         screen::w,
//         CV_8UC3,
//         static_cast<void*>(const_cast<uint8_t*>(t.data())));
//   std::cout << static_cast<void*>(t.data())
//             << " =?= " << static_cast<void*>(m.data) << std::endl;
//   // Hey! They're the same!
// }

int main() {
  uint8_t* ptr = new uint8_t[screen::bytes];
  Eigen::TensorMap<poop, Eigen::StorageOptions::RowMajor> t(
        ptr,
        Eigen::Sizes<screen::h, screen::w, 3>());
  cv::Mat m(screen::h, screen::w, CV_8UC3, ptr);
  std::cout << static_cast<void*>(t.data())
            << " =?= " << static_cast<void*>(m.data) << std::endl;
  // Hey! They're the same!
}

// NOTE: Screenshots are literally just too big for stack allocation
// You'll get a segfault WITHOUT warning and WITHOUT running anything
//   But it will compile! What the fucking fuck
// Instead, allocate on the heap ONCE, and pass the same pointer each time