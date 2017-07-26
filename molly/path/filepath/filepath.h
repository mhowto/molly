#ifndef __PATH_FILEPATH_H
#define __PATH_FILEPATH_H

#include <string>

namespace molly {
namespace path {
namespace filepath {
template <typename T> std::string join(T v) { return v; }

template <typename T, typename... Args>
// std::string join(const std::string &first, Args... args) {
std::string join(T first, Args... args) {
  if (sizeof...(args) == 0) {
    return first;
  }
  return first + "/" + join(args...);
}
}
}
}

#endif