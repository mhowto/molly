#ifndef __OS_ERROR_H
#define __OS_ERROR_H

#include <cstring>
#include <error.h>
#include <stdexcept>
#include <string>

namespace molly {
namespace os {

struct PathException : public std::runtime_error {
  PathException(std::string op, std::string path, int _errno = ENOENT)
      : std::runtime_error(op + " " + path + ": " + ::strerror(_errno)) {}
};
}
}

#endif