#ifndef __FILE_H
#define __FILE_H

#include "error.h"
#include "os.h"
#include <fcntl.h>
#include <string>
#include <unistd.h>

namespace molly {
namespace os {
class File {
public:
  // Open and create a file object. Throws on error.
  File(std::string name, int flag = O_RDONLY, file_mode perm = ModeNothing);
  File(int fd, std::string name) : fd_(fd), name_(name) {}

  ~File();

  int fd() const { return fd_; }

  // Movable
  //   File(File &&) noexcept;
  //   File &operator=(File &&);

  // read no more than num_bytes into container *out. The container is assumed
  // to be contiguous, with element size equal to 1, and offer size(),
  // reserve(), and random access (e.g. std::vector<char>, std::string).
  template <class Container>
  std::int64_t read(Container &out, std::size_t num_bytes);

private:
  bool closeNoThrow();
  int release() noexcept;

  int fd_;
  std::string name_;
  std::int64_t readbyteOffset;
};

template <class Container>
std::int64_t File::read(Container &buf, std::size_t num_bytes) {
  int num_to_read = (num_bytes < buf.capacity()) ? num_bytes : buf.capacity();
  ssize_t n = ::read(fd_, &buf[0], num_to_read);
  if (n < 0) {
    throw IOReadException(name_, errno);
  }
  readbyteOffset += n;
  return n;
}

/*
// open_file is the generalized open call; most users will use Open or Create
// instead. It opens the named file with specified flag and perm, if applicable.
// If successful, methods on the returned File can be used for I/O.
File *open_file(std::string name, int flag, file_mode perm);

File *open(std::string name);
*/
}
}

#endif