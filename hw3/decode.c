#include "../common.h"

c8 *const reg_table0[] = {"al", "cl", "dl", "bl", "ah", "ch", "dh", "bh", "ax", "cx", "dx", "bx", "sp", "bp", "si", "di"};
c8 *const reg_table1[] = {"bx+si", "bx+di", "bp+si", "bp+di", "si", "di", "bp", "bx"};
c8 *const instruction_table0[] = {"add", 0, 0, 0, 0, "sub", 0, "cmp"};
c8 *const instruction_table1[] = {"jo", "jno", "jb", "jnb", "je", "jne", "jbe", "ja", "js", "jns", "jp", "jnp", "jl", "jnl", "jle", "jg"};
c8 *const instruction_table2[] = {"loopnz", "loopz", "loop", "jcxz"};

u32 bits_eq(u8 *byte, u32 num, u8 val) {
  u8 mask = (~0) ^ ((1 << (8 - num)) - 1);
  return (*byte & mask) == (val << (8 - num));
}

u32 get_word(u8 **ptr) {
  u32 data;
  data = **ptr;
  (*ptr)++;
  data |= (**ptr) << 8;
  (*ptr)++;
  return data;
}

u32 get_byte(u8 **ptr) {
  u32 data;
  data = **ptr;
  (*ptr)++;
  return data;
}

u32 get_wbytes(u8 **ptr, u32 w) {
  return w ? get_word(ptr) : get_byte(ptr);
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
  u8 *prev_ptr;
  for (u8 *ptr = file; ptr < file + st.st_size;) {
    prev_ptr = ptr;
    if (bits_eq(ptr, 6, 0b100010) || (*ptr & 0b11000100) == 0b00000000) {
      // reg/mem to/from reg
      c8 *instruction;
      if (*ptr & 0x80) {
        instruction = "mov";
      } else {
        instruction = instruction_table0[(*ptr >> 3) & 0b111];
      }
      expect(instruction != 0, "unknown instruction");
      u32 d = (*ptr & 0b10) >> 1;
      u32 w = *ptr & 0b1;
      ptr++;
      u32 mod = (*ptr & 0b11000000) >> 6;
      u32 reg = (*ptr & 0b111000) >> 3;
      u32 rm = *ptr & 0b111;
      ptr++;
      c8 *src = reg_table0[reg | w << 3];
      if (mod == 0b11) {
        c8 *dest = reg_table0[rm | w << 3];
        if (d) {
          c8 *tmp = src;
          src = dest;
          dest = tmp;
        }
        printf("%s %s, %s\n", instruction, dest, src);
      } else if (mod == 0b00) {
        if (rm == 0b110) {
          u32 disp = get_word(&ptr);
          if (d) {
            printf("%s %s, [%u]\n", instruction, src, disp);
          } else {
            printf("%s [%u], %s\n", instruction, disp, src);
          }
        } else {
          c8 *effective_addr_reg = reg_table1[rm];
          if (d) {
            printf("%s %s, [%s]\n", instruction, src, effective_addr_reg);
          } else {
            printf("%s [%s], %s\n", instruction, effective_addr_reg, src);
          }
        }
      } else {
        c8 *effective_addr_reg = reg_table1[rm];
        u32 disp;
        u8 sign = '+';
        if (mod == 0b10) {
          // mem 16-bit
          disp = get_word(&ptr);
        } else {
          // mem 8-bit
          disp = get_byte(&ptr);
          if (disp & (1 << 7)) {
            sign = '-';
            disp = 0x100 - disp;
          }
        }
        if (d) {
          printf("%s %s, [%s%c%u]\n", instruction, src, effective_addr_reg, sign, disp);
        } else {
          printf("%s [%s%c%u], %s\n", instruction, effective_addr_reg, sign, disp, src);
        }
      }
    } else if (bits_eq(ptr, 7, 0b1100011)) {
      // imm to reg/mem
      u32 w = *ptr & 1;
      ptr++;
      expect(!(*ptr & 0b111000), "ilegal instruction");
      u32 mod = *ptr >> 6;
      u32 rm = *ptr & 0b111;
      ptr++;
      c8 *size = w ? "word" : "byte";
      if (mod == 0b11) {
        // to reg
        c8 *dest = reg_table0[rm | w << 3];
        u32 data = get_wbytes(&ptr, w);
        printf("mov %s, %u\n", dest, data);
      } else if (mod == 0b00) {
        if (rm == 0b110) {
          u32 disp = get_word(&ptr);
          u32 data = get_wbytes(&ptr, w);
          printf("mov [%u], %s %u\n", disp, size, data);
        } else {
          c8 *effective_addr_reg = reg_table1[rm];
          u32 data = get_wbytes(&ptr, w);
          printf("mov [%s], %s %u\n", effective_addr_reg, size, data);
        }
      } else {
        c8 *effective_addr_reg = reg_table1[rm];
        u32 disp;
        u8 sign = '+';
        if (mod == 0b10) {
          disp = get_word(&ptr);
        } else {
          disp = get_byte(&ptr);
          if (disp & (1 << 7)) {
            sign = '-';
            disp = 0x100 - disp;
          }
        }
        u32 data = get_wbytes(&ptr, w);
        printf("mov [%s%c%u], %s %u\n", effective_addr_reg, sign, disp, size, data);
      }
    } else if (bits_eq(ptr, 4, 0b1011)) {
      // imm to reg
      u32 w = (*ptr >> 3) & 1;
      u32 reg = *ptr & 0b111;
      ptr++;
      c8 *dest = reg_table0[reg | w << 3];
      u32 data = get_wbytes(&ptr, w);
      printf("mov %s, %u\n", dest, data);
    } else if (bits_eq(ptr, 6, 0b101000)) {
      // mem to a or a to mem
      u32 w = *ptr & 1;
      u32 d = (*ptr >> 1) & 1;
      ptr++;
      u32 addr = get_word(&ptr);
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
    } else if (bits_eq(ptr, 6, 0b100000)) {
      // imm to reg/mem
      u32 w = *ptr & 1;
      u32 s = (*ptr >> 1) & 1;
      // sw
      // 11
      // 1 byte, word
      // 10
      // ?
      // 01
      // 2 byte, word
      // 00
      // 1 byte, byte
      if (s && !w) {
        expect(0, "?");
      }
      ptr++;
      c8 *instruction = instruction_table0[((*ptr) >> 3) & 0b111];
      u32 mod = *ptr >> 6;
      u32 rm = *ptr & 0b111;
      ptr++;
      c8 *size = w ? "word" : "byte";
      if (mod == 0b11) {
        // to reg
        c8 *dest = reg_table0[rm | w << 3];
        u32 data = get_wbytes(&ptr, s ^ w);
        printf("%s %s, %u\n", instruction, dest, data);
      } else if (mod == 0b00) {
        if (rm == 0b110) {
          u32 disp = get_word(&ptr);
          u32 data = get_wbytes(&ptr, s ^ w);
          printf("%s %s [%u], %s %u\n", instruction, size, disp, size, data);
        } else {
          c8 *effective_addr_reg = reg_table1[rm];
          u32 data = get_wbytes(&ptr, s ^ w);
          printf("%s %s [%s], %s %u\n", instruction, size, effective_addr_reg, size, data);
        }
      } else {
        c8 *effective_addr_reg = reg_table1[rm];
        u32 disp;
        u8 sign = '+';
        if (mod == 0b10) {
          disp = get_word(&ptr);
        } else {
          disp = get_byte(&ptr);
          if (disp & (1 << 7)) {
            sign = '-';
            disp = 0x100 - disp;
          }
        }
        u32 data = get_wbytes(&ptr, s ^ w);
        printf("%s %s [%s%c%u], %s %u\n", instruction, size, effective_addr_reg, sign, disp, size, data);
      }
    } else if ((*ptr & 0b11000110) == 0b00000100) {
      // imm with a
      u32 w = *ptr & 1;
      c8 *instruction = instruction_table0[((*ptr) >> 3) & 0b111];
      ptr++;
      u32 data = get_wbytes(&ptr, w);
      printf("%s %s, %u\n", instruction, w ? "ax" : "al", data);
    } else if (bits_eq(ptr, 4, 0b0111) || bits_eq(ptr, 6, 0b111000)) {
      c8 *instruction;
      if (*ptr & 0x80) {
        instruction = instruction_table2[*ptr & 0b11];
      } else {
        instruction = instruction_table1[*ptr & 0b1111];
      }
      ptr++;
      u32 byte = get_byte(&ptr);
      i8 val = ((i8)byte) + 2;
      if (val >= 0) {
        printf("%s $+%d\n", instruction, val);
      } else {
        printf("%s $%d\n", instruction, val);
      }
    } else {
      expect(0, "unknown instruction");
    }
    expect(prev_ptr != ptr, "mssing branch");
  }
  return 0;
}
