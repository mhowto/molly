#ifndef __OS_ERROR_H
#define __OS_ERROR_H

#include <cstring>
#include <errno.h>
#include <stdexcept>
#include <string>

namespace molly {
namespace os {

struct PathException : public std::runtime_error {
  PathException(std::string op, std::string path, int _errno = ENOENT)
      : std::runtime_error(op + " " + path + ": " + ::strerror(_errno)) {}
};

struct IOReadException : public std::runtime_error {
  IOReadException(std::string name, int _errno)
      : std::runtime_error("read " + name + ": " + ::strerror(_errno)) {}
};
}
}

#endif