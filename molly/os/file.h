#ifndef __FILE_H
#define __FILE_H

#include <string>
#include "os.h"


namespace molly {
namespace os {
class File {
public:
  File(int fd, std::string name) : _fd(fd), _name(name) {}
  // read no more than num_bytes into container *out. The container is assumed
  // to be contiguous, with element size equal to 1, and offer size(),
  // reserve(), and random access (e.g. std::vector<char>, std::string).
  template <class Container>
  std::int64_t read(Container &out, std::size_t num_bytes);

private:
  int _fd;
  std::string _name;
  std::int64_t readbyteOffset;
};

// open_file is the generalized open call; most users will use Open or Create
// instead. It opens the named file with specified flag and perm, if applicable.
// If successful, methods on the returned File can be used for I/O.
File *open_file(std::string name, int flag, file_mode perm);

File *open(std::string name);
}
}

#endif