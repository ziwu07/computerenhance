#ifndef INCLUDEcomputerenhancecommoncommon
#define INCLUDEcomputerenhancecommoncommon

#define _POSIX_C_SOURCE 200809L
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

typedef int8_t i8;
typedef uint8_t u8;
typedef char c8;
typedef uint16_t u16;
typedef int16_t i16;
typedef uint32_t u32;
typedef int32_t i32;
typedef uint64_t u64;
typedef int64_t i64;
#define internal static
#define global static

#define likely(x) __builtin_expect((x), 1)
#define unlikely(x) __builtin_expect((x), 0)

#define expect(expr, msg)                                                                                                                                                                              \
  do {                                                                                                                                                                                                 \
    if (unlikely(!(expr))) {                                                                                                                                                                           \
      fprintf(stderr, "Error at %s:%u, %s\n", __FILE__, __LINE__, msg);                                                                                                                                \
      exit(1);                                                                                                                                                                                         \
    }                                                                                                                                                                                                  \
  } while (0)

#define expect_errno(expr, msg)                                                                                                                                                                        \
  do {                                                                                                                                                                                                 \
    if (unlikely(!(expr))) {                                                                                                                                                                           \
      perror(msg);                                                                                                                                                                                     \
      exit(1);                                                                                                                                                                                         \
    }                                                                                                                                                                                                  \
  } while (0)

#define todo                                                                                                                                                                                           \
  do {                                                                                                                                                                                                 \
    fprintf(stderr, "Not implimented at %s:%u\n", __FILE__, __LINE__);                                                                                                                                 \
    exit(1);                                                                                                                                                                                           \
  } while (0)

#endif // INCLUDEcomputerenhancecommoncommon.h_
