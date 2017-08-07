#ifndef __FILE_H
#define __FILE_H

#include "exception.h"
#include "os.h"
#include <fcntl.h>
#include <string>
#include <unistd.h>

namespace molly {
namespace os {
class File {
public:
  // Creates an empty File object, for late initialization.
  File() noexcept;

  // Open and create a file object. Throws on error.
  File(std::string name, int flag = O_RDONLY, int mode_ = 0666);
  File(int fd, const std::string &name, bool ownsFd = false) noexcept;

  ~File();

  int fd() const { return fd_; }
  std::string name() const { return name_; }

  // dupcate file descriptor and return File that owns it.
  File dup() const;

  file_info stat();

  // If we own the file descriptor, close the file and throw on error.
  // Otherwise, do nothing.
  void close();

  // Movable
  //   File(File &&) noexcept;
  //   File &operator=(File &&);

  // read no more than num_bytes into container *out. The container is assumed
  // to be contiguous, with element size equal to 1, and offer size(),
  // reserve(), and random access (e.g. std::vector<char>, std::string).
  template <class Container> std::int64_t read(Container &out, std::size_t num_bytes);
  template <class Container> std::int64_t read_at(Container &buf, std::ptrdiff_t offset);

  template <class Container> std::int64_t write(Container &out);
  template <class Container> std::int64_t write_at(Container &buf, std::ptrdiff_t offset);

private:
  bool closeNoThrow();
  int release() noexcept;

  int fd_;
  bool ownsFd_;
  std::string name_;
  std::int64_t readbyteOffset;
  std::int64_t writebyteOffset;
};

template <class Container> std::int64_t File::read(Container &buf, std::size_t num_bytes) {
  int num_to_read = (num_bytes < buf.capacity()) ? num_bytes : buf.capacity();
  ssize_t n = ::read(fd_, &buf[0], num_to_read);
  if (n < 0) {
    throw IOReadException(name_, errno);
  }
  readbyteOffset += n;
  return n;
}

template <class Container> std::int64_t File::read_at(Container &buf, std::ptrdiff_t offset) {
  readbyteOffset = offset;
  int n = 0;
  int remaining = buf.capacity();
  while (remaining > 0) {
    int m = ::pread(fd_, &buf[n], remaining, offset);
    if (m == -1) {
      throw std::system_error(errno, std::system_category(), "pread failed");
    } else if (m == 0) {
      break;
    }
    n += m;
    remaining = buf.capacity() - n;
    offset += m;
  }
  readbyteOffset = offset;
  return n;
}

template <class Container> std::int64_t File::write(Container &buf) {
  ssize_t n = ::write(fd_, &buf[0], buf.capacity());
  if (n == -1) {
    throw std::system_error(errno, std::system_category(), "write failed");
  }
  writebyteOffset += n;
  return n;
}

template <class Container> std::int64_t File::write_at(Container &buf, std::ptrdiff_t offset) {
  int n = 0;
  writebyteOffset = offset;
  int remaining = buf.size();
  while (remaining > 0) {
    int m = ::pwrite(fd_, &buf[n], remaining, offset);
    if (m == -1) {
      throw std::system_error(errno, std::system_category(), "pwrite failed");
    } else if (m == 0) {
      break;
    }
    n += m;
    remaining = buf.size() - n;
    offset += m;
  }
  writebyteOffset = offset;
  return n;
}
/*
// open_file is the generalized open call; most users will use Open or Create
// instead. It opens the named file with specified flag and perm, if applicable.
// If successful, methods on the returned File can be used for I/O.
File *open_file(std::string name, int flag, file_mode perm);

File *open(std::string name);
*/
std::uint32_t mode(int i);
}
}

#endif