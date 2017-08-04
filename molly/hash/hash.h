#ifndef __HASH_H
#define __HASH_H

#include <string>

namespace molly {
namespace hash {

const std::uint32_t FNV_32_HASH_START = 2166136261UL;
const std::uint64_t FNV_64_HASH_START = 14695981039346656037ULL;
const std::uint64_t FNVA_64_HASH_START = 14695981039346656037ULL;

inline std::uint32_t fnv32_buf(const void *buf, size_t n, std::uint32_t hash = FNV_32_HASH_START) {
  // forcing signed char, since other platforms can use unsigned
  const signed char *char_buf = reinterpret_cast<const signed char *>(buf);

  for (size_t i = 0; i < n; ++i) {
    hash += (hash << 1) + (hash << 4) + (hash << 7) + (hash << 8) + (hash << 24);
    hash ^= char_buf[i];
  }

  return hash;
}

inline std::uint32_t fnv32(const std::string &str, std::uint32_t hash = FNV_32_HASH_START) {
  return fnv32_buf(str.data(), str.size(), hash);
}

inline std::uint32_t fnv32(const char *buf, std::uint32_t hash = FNV_32_HASH_START) {
  // forcing signed char, since other platforms can use unsigned
  const signed char *s = reinterpret_cast<const signed char *>(buf);

  for (; *s; ++s) {
    hash += (hash << 1) + (hash << 4) + (hash << 7) + (hash << 8) + (hash << 24);
    hash ^= *s;
  }
  return hash;
}

inline uint64_t fnv64(const char *buf, uint64_t hash = FNV_64_HASH_START) {
  // forcing signed char, since other platforms can use unsigned
  const signed char *s = reinterpret_cast<const signed char *>(buf);

  for (; *s; ++s) {
    hash += (hash << 1) + (hash << 4) + (hash << 5) + (hash << 7) + (hash << 8) + (hash << 40);
    hash ^= *s;
  }
  return hash;
}

inline uint64_t fnv64_buf(const void *buf, size_t n, uint64_t hash = FNV_64_HASH_START) {
  // forcing signed char, since other platforms can use unsigned
  const signed char *char_buf = reinterpret_cast<const signed char *>(buf);

  for (size_t i = 0; i < n; ++i) {
    hash += (hash << 1) + (hash << 4) + (hash << 5) + (hash << 7) + (hash << 8) + (hash << 40);
    hash ^= char_buf[i];
  }
  return hash;
}

inline uint64_t fnv64(const std::string &str, uint64_t hash = FNV_64_HASH_START) {
  return fnv64_buf(str.data(), str.size(), hash);
}

inline std::uint64_t fnva64_buf(const void *buf, size_t n, std::uint64_t hash = FNVA_64_HASH_START) {
  const std::uint8_t *char_buf = reinterpret_cast<const std::uint8_t *>(buf);

  for (size_t i = 0; i < n; i++) {
    hash ^= char_buf[i];
    hash += (hash << 1) + (hash << 4) + (hash << 5) + (hash << 7) + (hash << 8) + (hash << 40);
  }
  return hash;
}

inline std::uint64_t fnva64(const std::string &str, std::uint64_t hash = FNVA_64_HASH_START) {
  return fnva64_buf(str.data(), str.size(), hash);
}
}
}

#endif
