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

std::uint32_t mode(int i) {
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

File::File() noexcept : fd_(-1), ownsFd_(false) {}

File::File(int fd, const std::string &name, bool ownsFd) noexcept
    : fd_(fd), ownsFd_(ownsFd), name_(name) {
  if (fd < -1) {
    std::cerr << "fd must be -1 or non-negative" << std::endl;
  }

  if (fd == -1 && ownsFd) {
    std::cerr << "cannot own -1" << std::endl;
  }
}

File::File(std::string name, int flag, int mode_) {
  fd_ = ::open(name.c_str(), flag | O_CLOEXEC, mode(mode_));
  name_ = name;
  if (fd_ < 0) {
    char err_info[255];
    sprintf(err_info, "open %s failed", name.c_str());
    throw std::system_error(errno, std::system_category(), err_info);
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

void File::close() {
  if (!closeNoThrow()) {
    throw std::system_error(errno, std::system_category(), "close() failed");
  }
}

File File::dup() const {
  if (fd_ != -1) {
    int fd = ::dup(fd_);
    if (fd == -1) {
      throw std::system_error(errno, std::system_category(), "dup() failed");
    }

    return File(fd, name_, true);
  }

  return File();
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