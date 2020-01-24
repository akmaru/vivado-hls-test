#ifndef VHLSTEST_IO_H
#define VHLSTEST_IO_H

#include <string>

namespace vhlstest {
namespace {

template <typename... Rest>
std::string format(const char *fmt, const Rest &... rest) {
  int length = snprintf(NULL, 0, fmt, rest...) +
               1;  // Explicit place for null termination
  std::vector<char> buf(length, 0);
  snprintf(&buf[0], length, fmt, rest...);
  std::string s(buf.begin(), std::find(buf.begin(), buf.end(), '\0'));
  return s;
}

}  // namespace
}  // namespace vhlstest

#endif  // VHLSTEST_IO_H
