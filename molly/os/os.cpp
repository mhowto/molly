#include "os.h"
#include "syscall.h"
#include <cstdlib>
#include <cstring>
#include <experimental/filesystem>
#include <stdexcept>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

namespace fs = std::experimental::filesystem;
namespace os = molly::os;

// basename removes trailing slashes and the leading directory name from path
// name
std::string basename(std::string name) {
  int i = name.size() - 1;
  // Remove trailing slashes
  for (; i > 0 && name[i] == '/'; i--) {
    name = name.substr(0, i);
  }
  // Remove leading directory name
  for (i--; i >= 0; i--) {
    if (name[i] == '/') {
      name = name.substr(i + 1);
      break;
    }
  }
  return name;
}

std::string os::temp_dir() {
  std::string dir = getenv("TMPDIR");
  if (dir == "") {
    fs::path path = fs::temp_directory_path();
    return path.string();
  }
  return dir;
}

std::string os::getenv(std::string key) {
  const char *env_p = std::getenv(key.c_str());
  if (!env_p) {
    return "";
  }
  std::string env(env_p);
  delete[] env_p;
  return env;
}

void os::remove(const std::string &name) {
  int r = ::unlink(name.c_str());
  if (r == 0) {
    return;
  }
  r = ::rmdir(name.c_str());
  if (r == -1) {
    std::string err_info = std::string("remove file ") + name + " failed: unlink() or rmdir() returned -1";
    throw std::system_error(errno, std::system_category(), err_info);
  }
}

void os::fill_file_stat_from_sys(std::string name, const posix_stat_ &stat_buf, struct file_info &fs) {
  fs.name = basename(name);
  fs.size = stat_buf.st_size;
  fs.mod_time = std::chrono::system_clock::from_time_t(stat_buf.st_mtime);

  fs.mode = file_mode(stat_buf.st_mode & 0777);
  switch (stat_buf.st_mode & S_IFMT) {
  case S_IFBLK:
    fs.mode = static_cast<file_mode>(fs.mode | ModeDevice);
    break;
  case S_IFCHR:
    fs.mode = static_cast<file_mode>(fs.mode | ModeDevice | ModeCharDevice);
    break;
  case S_IFDIR:
    fs.mode = static_cast<file_mode>(fs.mode | ModeDir);
    break;
  case S_IFIFO:
    fs.mode = static_cast<file_mode>(fs.mode | ModeNamedPipe);
    break;
  case S_IFLNK:
    fs.mode = static_cast<file_mode>(fs.mode | ModeSymlink);
    break;
  case S_IFREG:
    // nothing to do
    break;
  case S_IFSOCK:
    fs.mode = static_cast<file_mode>(fs.mode | ModeSocket);
    break;
  }
  if (stat_buf.st_mode & S_ISGID) {
    fs.mode = static_cast<file_mode>(fs.mode | ModeSetgid);
  }
  if (stat_buf.st_mode & S_ISUID) {
    fs.mode = static_cast<file_mode>(fs.mode | ModeSetuid);
  }
  if (stat_buf.st_mode & S_ISVTX) {
    fs.mode = static_cast<file_mode>(fs.mode | ModeSticky);
  }
}

struct os::file_info os::stat(std::string name) {
  posix_stat_ stat_buf;
  int r = ::stat(name.c_str(), &stat_buf);
  if (r != 0) {
    throw std::system_error(errno, std::system_category(), "stat failed");
  }
  struct file_info fs;
  fill_file_stat_from_sys(name, stat_buf, fs);

  return fs;
}