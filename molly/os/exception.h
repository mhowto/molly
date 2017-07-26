#ifndef __OS_ERROR_H
#define __OS_ERROR_H

#include <cstring>
#include <errno.h>
#include <stdexcept>
#include <string>
#include <system_error>

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

struct AlreadyExistsException : public std::runtime_error {
  AlreadyExistsException(std::string name)
      : std::runtime_error(name + " already exists") {}
};

struct DirNotEmptyException : public std::runtime_error {
  DirNotEmptyException(std::string name)
      : std::runtime_error(name + " is not empty") {}
};

// Helper to throw std::system_error
[[noreturn]] inline void throwSystemErrorExplicit(int err, const char *msg) {
  throw std::system_error(err, std::system_category(), msg);
}

template<class... Args>[[noreturn]] void throwSystemError(int err,
                                                          Args &&... args) {
  throwSystemErrorExplicit(err, "haha");
}

// Helper to throw std::system_error from errno and components of a string
template <class... Args>[[noreturn]] void throwSystemError(Args &&... args) {
  throwSystemErrorExplicit(errno, std::forward<Args>(args)...);
}

// Check a traditional Unix return code (-1 and sets errno on error), throw on
// error.
template <class... Args> void checkUnixError(int ret, Args &&... args) {
  if (ret == -1) {
    throwSystemError(std::forward<Args>(args)...);
  }
}
}
}

#endif