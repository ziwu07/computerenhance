#include "../common.h"

c8 *reg_name(u32 reg, u32 w) {
  switch ((w << 3) | reg) {
  case 0: {
    return "al";
  } break;
  case 1: {
    return "cl";
  } break;
  case 2: {
    return "dl";
  } break;
  case 3: {
    return "bl";
  } break;
  case 4: {
    return "ah";
  } break;
  case 5: {
    return "ch";
  } break;
  case 6: {
    return "dh";
  } break;
  case 7: {
    return "bh";
  } break;
  case 8: {
    return "ax";
  } break;
  case 9: {
    return "cx";
  } break;
  case 10: {
    return "dx";
  } break;
  case 11: {
    return "bx";
  } break;
  case 12: {
    return "sp";
  } break;
  case 13: {
    return "bp";
  } break;
  case 14: {
    return "si";
  } break;
  case 15: {
    return "di";
  } break;
  default: {
    expect(0, "?");
  } break;
  }
}

int main(int argc, char *argv[]) {
  expect(argc == 2, "num args");
  i32 fd = open(argv[1], O_RDONLY);
  expect_errno(fd != -1, "open");
  struct stat st;
  fstat(fd, &st);
  u8 *file = mmap(0, st.st_size, PROT_READ, MAP_SHARED, fd, 0);
  expect_errno(file != MAP_FAILED, "MAP_FAILED");
  puts("bits 16");
  for (u8 *ptr = file; ptr < file + st.st_size; ptr += 2) {
    expect((*ptr & 0b11111100) == 0b10001000, "unknown instruction");
    u32 d = (*ptr & 0b10) >> 1;
    u32 w = *ptr & 0b1;
    u32 mod = (ptr[1] & 0b11000000) >> 6;
    u32 reg = (ptr[1] & 0b111000) >> 3;
    u32 rm = ptr[1] & 0b111;
    expect(mod == 0b11, "unknown mov type");
    c8 *src = reg_name(reg, w);
    c8 *dest = reg_name(rm, w);
    if (d) {
      c8 *tmp = src;
      src = dest;
      dest = tmp;
    }
    printf("mov %s, %s\n", dest, src);
  }
  return 0;
}
