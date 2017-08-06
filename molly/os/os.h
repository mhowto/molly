#ifndef __OS_H
#define __OS_H

#include <chrono>
#include <fstream>
#include <string>
#include <sys/stat.h>

typedef struct stat posix_stat_;

namespace molly {
namespace os {
enum file_mode {
  ModeNothing = 0,
  ModeDir = 1 << (32 - 1 - 0),         // d: is a directory
  ModeAppend = 1 << (32 - 1 - 1),      // a: append-only
  ModeExclusive = 1 << (32 - 1 - 2),   // l: exclusive use
  ModeTemporary = 1 << (32 - 1 - 3),   // T: temporary file (not backed up)
  ModeSymlink = 1 << (32 - 1 - 4),     // L: symbolic link
  ModeDevice = 1 << (32 - 1 - 5),      // D: device file
  ModeNamedPipe = 1 << (32 - 1 - 6),   // p: named pipe (FIFO)
  ModeSocket = 1 << (32 - 1 - 7),      // S: Unix domain socket
  ModeSetuid = 1 << (32 - 1 - 8),      // u: setuid
  ModeSetgid = 1 << (32 - 1 - 9),      // g: setgid
  ModeCharDevice = 1 << (32 - 1 - 10), // c: unix character device, when ModeDevice is set
  ModeSticky = 1 << (32 - 1 - 11),     // t: sticky

  // Mask for the type bits. For regular files, none will be set.
  ModeType = ModeDir | ModeSymlink | ModeNamedPipe | ModeSocket | ModeDevice,

  ModePerm = 0777, // Unix permission bits
};

struct file_info {
  std::string name;
  std::uint64_t size;
  file_mode mode;
  std::chrono::time_point<std::chrono::system_clock> mod_time;
  bool is_dir;
};

std::string getenv(std::string key);

std::string temp_dir();

// removes the named file or directory.
void remove(const std::string &name);

struct file_info stat(std::string name);

void fill_file_stat_from_sys(std::string name, const posix_stat_ &stat_buf, struct file_info &fs);
}
}

#endif