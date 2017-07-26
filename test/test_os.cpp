#include "molly/os/exception.h"
#include "molly/os/file.h"
#include "molly/os/os.h"
#include <gtest/gtest.h>
#include <iostream>
#include <stdexcept>
#include <vector>

namespace os = molly::os;

struct sys_dir {
  std::string name;
  std::vector<std::string> files;
};

struct sys_dir sysdir = {
    /*.name*/ "/etc",
    /*.files*/ {"group", "hosts", "passwd"},
};

std::int64_t size(std::string name) {
  os::File file(name);
  int readOnceBytes = 100;
  std::string buf;
  buf.resize(100);
  std::int64_t len = 0;
  for (;;) {
    int n = file.read(buf, readOnceBytes);
    if (n == 0) {
      break;
    }
    len += n;
  }
  return len;
}

TEST(OSTest, StatFunc) {
  std::string sfdir = sysdir.name;
  std::string sfname = sysdir.files[0];

  std::string path = sfdir + "/" + sfname;
  os::file_info file_info = os::stat(path);
  ASSERT_EQ(file_info.name, sfname);

  try {
    std::int64_t file_size = size(path);
    ASSERT_EQ(file_info.size, file_size);
  } catch (os::IOReadException &e) {
    FAIL() << e.what();
  }
}