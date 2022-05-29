#include <fstream>                // std::ofstream
#include <opencv2/imgcodecs.hpp>  // cv::imread

#include "os_screenshot.hpp"

int main() {
  WRITE_SCREENSHOT();
  cv::Mat im = cv::imread("out/screen.png", cv::IMREAD_COLOR);
  std::ofstream f("config/sizes.hpp");
  f << "#ifndef CONFIG_SIZES_HPP_\n#define CONFIG_SIZES_HPP_\n"
       "#include <stddef.h>\nnamespace screen {\n  "
       "constexpr size_t w = "
    << std::to_string(im.cols)
    << ";\n  "
       "constexpr size_t h = "
    << std::to_string(im.rows)
    << ";\n  "
       "constexpr size_t bytes = screen::h * screen::w * 3;\n  "
       //  "uint8_t *const data = new uint8_t[bytes];\n"
       "} // namespace screen\n#endif // CONFIG_SIZES_HPP_";
  f.close();
}