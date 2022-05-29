#if __APPLE__
#include "mac/screenshot.hpp"
#elif __linux__
#include "linux/screenshot.hpp"
#elif defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#error "Get a real computer"
#else
#error "Unrecognized OS"
#endif