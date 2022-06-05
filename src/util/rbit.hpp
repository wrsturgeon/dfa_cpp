#ifndef UTIL_RBIT_HPP_
#define UTIL_RBIT_HPP_

#include <stdint.h>

#include "xoshiro.hpp"

bool rbit() {
  static uint64_t b = 0, r;
  if (b <<= 1) return r & b;  // lol rhythm & blues amirite
  return (r = xoshiro::next()) & (b = 1);
}

#endif  // UTIL_RBIT_HPP_