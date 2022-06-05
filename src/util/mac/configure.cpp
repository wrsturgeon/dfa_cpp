#include <CoreGraphics/CGDirectDisplay.h>

#include <fstream>

// TODO: make more robust to non-8b machines

int main() {
  CGImageRef im = CGWindowListCreateImage(
        CGRectInfinite,
        kCGWindowListOptionOnScreenOnly,
        kCGNullWindowID,
        kCGWindowImageDefault);
  size_t w = CGImageGetWidth(im);
  size_t h = CGImageGetHeight(im);
  size_t b = CGImageGetBitsPerPixel(im);
  CGImageRelease(im);
  static constexpr uint8_t c_post = 3;  // channels post-CGImage (into Eigen)
  static constexpr uint8_t lgs = 3;     // log-stride
  static constexpr size_t stride_mask = (1 << lgs) - 1;
  if ((w & stride_mask) || (h & stride_mask))
    throw std::runtime_error("Screen size not divisible by stride");
  std::ofstream f("config.hpp");
  f << "#ifndef CONFIG_HPP_\n"
       "#define CONFIG_HPP_\n"
       "#include \"../eigen/unsupported/Eigen/CXX11/Tensor\"\n"
       "namespace screen {\n  static constexpr size_t w = "
    << w << ";\n  static constexpr size_t h = " << h << ";\n"
    << "static constexpr size_t b = " << b << ";\n"
    << "static constexpr size_t w_post = w >> 3;\n"
    << "static constexpr size_t h_post = h >> 3;\n"
    << "static constexpr size_t c_post = " << +c_post << ";\n"
    << "static constexpr size_t n_post = c_post * w_post * h_post;\n"
    << "static constexpr uint8_t lgs = " << +lgs << ";\n"
    << "static constexpr size_t stride = 1 << lgs;\n"
    << "static constexpr size_t bstride = stride * b >> 3;\n"
    << "static constexpr size_t stride_mask = stride - 1;\n"
    << "typedef Eigen::Sizes<h_post, w_post, c_post> Size;\n"
    << "typedef Eigen::TensorFixedSize<uint8_t, Size> type;\n"
    << "} // namespace screen\n#endif // CONFIG_HPP_\n";
  f.close();
}