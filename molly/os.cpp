#include "os.h"
#include "syscall.h"
#include <cstring>
#include <experimental/filesystem>
#include <stdexcept>
#include <cstdlib>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

namespace fs = std::experimental::filesystem;
namespace os = molly::os;

// basename removes trailing slashes and the leading directory name from path name
std::string basename(std::string name) {
  int i = name.size() - 1;
  // Remove trailing slashes
  for (; i > 0 && name[i] == '/'; i--) {
    name = name.substr(0, i);
  }
  // Remove leading directory name
  for (i--; i >= 0; i--) {
    if (name[i] == '/') {
      name = name.substr(i+1);
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

struct os::file_info os::stat(std::string name) { 
  struct stat stat_buf;
  int r = ::stat(name.c_str(), &stat_buf);
  if (r != 0) {
    char err_info[255];
    sprintf(err_info, "stat failed: %s", std::strerror(errno));
    throw std::runtime_error(err_info);
  }
  struct file_info fs;
  fs.name = basename(name);
  fs.size = stat_buf.st_size;
  //fs.mod_time = stat_buf.st_mtim;
  fs.mode = file_mode(stat_buf.st_mode & 0777);
  return fs;
}