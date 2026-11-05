#include "../common.h"

c8 *const reg_table0[] = {"al", "cl", "dl", "bl", "ah", "ch", "dh", "bh", "ax", "cx", "dx", "bx", "sp", "bp", "si", "di"};
c8 *const reg_table1[] = {"bx+si", "bx+di", "bp+si", "bp+di", "si", "di", "bp", "bx"};

u32 bits_eq(u8 *byte, u32 num, u8 val) {
  u8 mask = (~0) ^ ((1 << (8 - num)) - 1);
  return (*byte & mask) == (val << (8 - num));
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
  u32 increment = 0;
  for (u8 *ptr = file; ptr < file + st.st_size; ptr += increment) {
    increment = 0;
    if (bits_eq(ptr, 6, 0b100010)) {
      // reg/mem to/from reg
      u32 d = (*ptr & 0b10) >> 1;
      u32 w = *ptr & 0b1;
      u32 mod = (ptr[1] & 0b11000000) >> 6;
      u32 reg = (ptr[1] & 0b111000) >> 3;
      u32 rm = ptr[1] & 0b111;
      c8 *src = reg_table0[reg | w << 3];
      if (mod == 0b11) {
        c8 *dest = reg_table0[rm | w << 3];
        if (d) {
          c8 *tmp = src;
          src = dest;
          dest = tmp;
        }
        printf("mov %s, %s\n", dest, src);
        increment = 2;
      } else if (mod == 0b00) {
        if (rm == 0b110) {
          u32 disp = ptr[2] | (ptr[3] << 8);
          increment = 4;
          if (d) {
            printf("mov %s, [%u]\n", src, disp);
          } else {
            printf("mov [%u], %s\n", disp, src);
          }
        } else {
          increment = 2;
          c8 *effective_addr_reg = reg_table1[rm];
          if (d) {
            printf("mov %s, [%s]\n", src, effective_addr_reg);
          } else {
            printf("mov [%s], %s\n", effective_addr_reg, src);
          }
        }
      } else {
        c8 *effective_addr_reg = reg_table1[rm];
        u32 disp;
        u8 sign = '+';
        if (mod == 0b10) {
          // mem 16-bit
          disp = ptr[2] | (ptr[3] << 8);
          increment = 4;
        } else {
          // mem 8-bit
          disp = ptr[2];
          if (disp & (1 << 7)) {
            sign = '-';
            disp = 0x100 - disp;
          }
          increment = 3;
        }
        if (d) {
          printf("mov %s, [%s%c%u]\n", src, effective_addr_reg, sign, disp);
        } else {
          printf("mov [%s%c%u], %s\n", effective_addr_reg, sign, disp, src);
        }
      }
    } else if (bits_eq(ptr, 7, 0b1100011)) {
      // imm to reg/mem
      expect(!(ptr[1] & 0b111000), "ilegal instruction");
      u32 w = *ptr & 1;
      u32 mod = ptr[1] >> 6;
      u32 rm = ptr[1] & 0b111;
      c8 *size = w ? "word" : "byte";
      if (mod == 0b11) {
        // to reg
        c8 *dest = reg_table0[rm | w << 3];
        u32 data;
        if (w) {
          data = ptr[2] | (ptr[3] << 8);
          increment = 4;
        } else {
          data = ptr[2];
          increment = 3;
        }
        printf("mov %s, %u\n", dest, data);
      } else if (mod == 0b00) {
        if (rm == 0b110) {
          u32 disp = ptr[2] | (ptr[3] << 8);
          u32 data;
          if (w) {
            data = ptr[4] | (ptr[5] << 8);
            increment = 6;
          } else {
            data = ptr[4];
            increment = 5;
          }
          printf("mov [%u], %s %u\n", disp, size, data);
        } else {
          c8 *effective_addr_reg = reg_table1[rm];
          u32 data;
          if (w) {
            data = ptr[2] | (ptr[3] << 8);
            increment = 4;
          } else {
            data = ptr[2];
            increment = 3;
          }
          printf("mov [%s], %s %u\n", effective_addr_reg, size, data);
        }
      } else {
        c8 *effective_addr_reg = reg_table1[rm];
        u32 disp;
        u8 sign = '+';
        if (mod == 0b10) {
          disp = ptr[2] | (ptr[3] << 8);
          increment = 4;
        } else {
          disp = ptr[2];
          if (disp & (1 << 7)) {
            sign = '-';
            disp = 0x100 - disp;
          }
          increment = 3;
        }
        u32 data;
        if (w) {
          data = ptr[4] | (ptr[5] << 8);
          increment += 2;
        } else {
          data = ptr[4];
          increment++;
        }
        printf("mov [%s%c%u], %s %u\n", effective_addr_reg, sign, disp, size, data);
      }
    } else if (bits_eq(ptr, 4, 0b1011)) {
      // imm to reg
      u32 w = (*ptr >> 3) & 1;
      u32 reg = *ptr & 0b111;
      c8 *dest = reg_table0[reg | w << 3];
      u32 data;
      if (w) {
        data = ptr[1] | (ptr[2] << 8);
        increment = 3;
      } else {
        data = ptr[1];
        increment = 2;
      }
      printf("mov %s, %u\n", dest, data);
    } else if (bits_eq(ptr, 6, 0b101000)) {
      // mem to a or a to mem
      u32 w = *ptr & 1;
      u32 d = (*ptr >> 1) & 1;
      increment = 3;
      u32 addr = ptr[1] | (ptr[2] << 8);
      c8 *reg;
      if (w) {
        reg = "ax";
      } else {
        reg = "al";
      }
      if (d) {
        printf("mov [%u], %s\n", addr, reg);
      } else {
        printf("mov %s, [%u]\n", reg, addr);
      }
    } else {
      expect(0, "unknown instruction");
    }
    expect(increment != 0, "mssing branch");
  }
  return 0;
}
