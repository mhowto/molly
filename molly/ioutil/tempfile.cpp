#include "tempfile.h"
#include "molly/path/filepath/filepath.h"
#include <random>

namespace os = molly::os;
namespace filepath = molly::path::filepath;

std::string next_suffix() {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(0, std::numeric_limits<int>::max());
  int r = dis(gen);
  r = r * 1664525 + 1013904223; // constants from Numerical Recipes
  return std::to_string(r);
}

namespace molly {
namespace ioutil {

os::File temp_file(std::string dir, std::string prefix) {
  if (dir == "") {
    dir = os::temp_dir();
  }

  int nconflict = 0;
  for (int i = 0; i < 10000; i++) {
    std::string name = filepath::join(dir, prefix + next_suffix());
    try {
      os::File file(name, O_RDWR | O_CREAT | O_EXCL, 0600);
      return file.dup();
    } catch (std::system_error &e) {
      if (e.code().value() == EEXIST) {
        nconflict++;
        continue;
      } else {
        throw e;
      }
    }
  }
  throw std::system_error();
}

std::string temp_dir(std::string dir, std::string prefix) { return "unimplemented"; }
}
}
