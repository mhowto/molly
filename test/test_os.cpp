#include "molly/os.h"
#include <gtest/gtest.h>

namespace os = molly::os;

TEST(OSTest, StatFunc) {
    os::file_info file_info = os::stat("/etc/hosts");
    ASSERT_EQ(file_info.name, "hosts");
}