#include "file.h"
#include <cstring>
#include <iostream>

namespace molly {
namespace os {

/*
File *open(std::string name) { return open_file(name, O_RDONLY, ModeNothing); }

File *open_file(std::string name, int flag, file_mode perm) {
  if (name == "") {
    throw PathException("open", name, ENOENT);
  }

  int fd = ::open(name.c_str(), flag | O_CLOEXEC, mode(perm));
  if (fd < 0) {
    char err_info[255];
    sprintf(err_info, "open file failed: %s\n", std::strerror(errno));
    throw std::runtime_error(err_info);
  }
  // TODO: open_dir

  return new File(fd, name);
}
*/

std::uint32_t mode(file_mode i) {
  std::uint32_t o = 0;
  o |= i;
  if (i & ModeSetuid) {
    o |= S_ISUID;
  }
  if (i & ModeSetgid) {
    o |= S_ISGID;
  }
  if (i & ModeSticky) {
    o |= S_ISVTX;
  }
  return o;
}

File::File(std::string name, int flag, file_mode perm) {
  fd_ = ::open(name.c_str(), flag | O_CLOEXEC, mode(perm));
  name_ = name;
  if (fd_ < 0) {
    char err_info[255];
    sprintf(err_info, "open file failed: %s\n", std::strerror(errno));
    throw std::runtime_error(err_info);
  }
}

File::~File() {
  int fd = fd_;
  if (!closeNoThrow() && errno == EBADF) { // ignore most errors
    std::cerr << "closing fd " << fd << ", it may already have been closed. "
                                        "Another time, this might close the "
                                        "wrong FD.";
  }
}

bool File::closeNoThrow() {
  int r = ::close(fd_);
  release();
  return r == 0;
}

int File::release() noexcept {
  int released = fd_;
  fd_ = -1;
  // ownsFd_ = false;
  return released;
}
}
}