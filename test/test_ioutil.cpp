#include "molly/ioutil/tempfile.h"
#include "molly/os/exception.h"
#include "molly/os/file.h"
#include "molly/os/os.h"
#include <gtest/gtest.h>
#include <iostream>
#include <stdexcept>
#include <vector>

namespace ioutil = molly::ioutil;
namespace os = molly::os;

TEST(IOUtilTest, TempFile) {
  EXPECT_THROW({ os::File file = ioutil::temp_file("/_not_exists_", "foo"); },
               std::system_error);
  std::string dir = os::temp_dir();
  os::File file = ioutil::temp_file(dir, "ioutil_test");
  file.close();
  os::remove(file.name());
}
