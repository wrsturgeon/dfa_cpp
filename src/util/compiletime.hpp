#ifndef UTIL_COMPILETIME_HPP_
#define UTIL_COMPILETIME_HPP_

// https://artificial-mind.net/blog/2020/11/14/cpp17-consteval
template <auto V>
static constexpr auto compiletime = V;

#endif  // UTIL_COMPILETIME_HPP_