#ifndef MODEL_U_NET_HPP_
#define MODEL_U_NET_HPP_

#include "../../config.hpp"
#include "../util/compiletime.hpp"

// TODO: "Frozen" mode with weights known at compile time.
//   Or would that even provide much benefit?

////////////////////////////////////////////////////////////////////////////////
// Implementation concepts
// U-Net with parameterizable kernels like StyleGAN
//   So convolutions can function as an _offset_ at various scales
//   Maybe for only, say, half the channels? And the others are recognition
// Remember though that if we're going at a constant speed,
//   distance from the vanishing point scales 1/t with time,
//   which is NOT linear over a given interval
////////////////////////////////////////////////////////////////////////////////

// Based on Imagen's Efficient U-Net (https://arxiv.org/pdf/2205.11487, p. 20)
// See https://github.com/lucidrains/imagen-pytorch (3rd-party implementation)
class UNet {
 private:
  // Internal data type
  typedef uint16_t dtype;
  // Internal dimensionality
  static constexpr uint16_t ndim(256);
  // Input channels
  static constexpr uint16_t cin(3);
  // Output channels
  static constexpr uint16_t cout(1);
  // Scale residual adds by 1/sqrt2
  static constexpr dtype rscale(0.7071067811865475);

 public:
  screen::type operator()(screen::type const& x) const;
};

#endif  // MODEL_U_NET_HPP_