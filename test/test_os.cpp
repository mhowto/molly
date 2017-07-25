#include "molly/os/os.h"
#include <gtest/gtest.h>
#include <iostream>
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
  // os::open
}

TEST(OSTest, StatFunc) {
  std::string sfdir = sysdir.name;
  std::string sfname = sysdir.files[0];

  std::string path = sfdir + "/" + sfname;
  os::file_info file_info = os::stat(path);
  ASSERT_EQ(file_info.name, sfname);

//   std::int64_t s = size(path);
}