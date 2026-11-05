#include "../common.h"

c8 *const reg_table1[] = {"bx+si", "bx+di", "bp+si", "bp+di", "si", "di", "bp", "bx"};
c8 *const instruction_table1[] = {"jo", "jno", "jb", "jnb", "je", "jne", "jbe", "ja", "js", "jns", "jp", "jnp", "jl", "jnl", "jle", "jg"};
c8 *const instruction_table2[] = {"loopnz", "loopz", "loop", "jcxz"};

enum Register {
  AX_,
  BX_,
  CX_,
  DX_,
  SP_,
  BP_,
  SI_,
  DI_,
  SS_,
  DS_,
  CS_,
  ES_,
  REGISTER_SIZE,
};

enum Reg {
  NONE = 0,
  AL,
  CL,
  DL,
  BL,
  AH,
  CH,
  DH,
  BH,
  AX,
  CX,
  DX,
  BX,
  SP,
  BP,
  SI,
  DI,
  ES,
  CS,
  SS,
  DS,
  REG_SIZE,
};

enum Reg reg_table0[] = {AL, CL, DL, BL, AH, CH, DH, BH, AX, CX, DX, BX, SP, BP, SI, DI};
enum Reg reg_table2[] = {ES, CS, SS, DS};

enum Instruction {
  UNKNOWN = 0,
  MOV,
  ADD,
  SUB,
  CMP,
};

enum Instruction instruction_table0[] = {ADD, 0, 0, 0, 0, SUB, 0, CMP};

c8 *instruction_to_name(enum Instruction inst) {
  c8 *const name_table[] = {0, "mov", "add", "sub", "cmp"};
  return name_table[inst];
}

c8 *register_to_name(enum Register reg) {
  c8 *const name_table[] = {"AX", "BX", "CX", "DX", "SP", "BP", "SI", "DI", "SS", "DS", "CS", "ES"};
  return name_table[reg];
}

c8 *reg_to_name(enum Reg reg) {
  c8 *const name_table[] = {0, "al", "cl", "dl", "bl", "ah", "ch", "dh", "bh", "ax", "cx", "dx", "bx", "sp", "bp", "si", "di", "es", "cs", "ss", "ds"};
  return name_table[reg];
}

void write_reg(u16 *registers, enum Reg reg, u16 val) {
  switch (reg) {
  case NONE: {
    expect(0, "?");
  } break;
  case AL: {
    registers[AX_] &= 0xff00;
    registers[AX_] |= val & 0xff;
  } break;
  case CL: {
    registers[CX_] &= 0xff00;
    registers[CX_] |= val & 0xff;
  } break;
  case DL: {
    registers[DX_] &= 0xff00;
    registers[DX_] |= val & 0xff;
  } break;
  case BL: {
    registers[BX_] &= 0xff00;
    registers[BX_] |= val & 0xff;
  } break;
  case AH: {
    registers[AX_] &= 0x00ff;
    registers[AX_] |= (val & 0xff) << 8;
  } break;
  case CH: {
    registers[CX_] &= 0x00ff;
    registers[CX_] |= (val & 0xff) << 8;
  } break;
  case DH: {
    registers[DX_] &= 0x00ff;
    registers[DX_] |= (val & 0xff) << 8;
  } break;
  case BH: {
    registers[BX_] &= 0x00ff;
    registers[BX_] |= (val & 0xff) << 8;
  } break;
  case AX: {
    registers[AX_] = val;
  } break;
  case CX: {
    registers[CX_] = val;
  } break;
  case DX: {
    registers[DX_] = val;
  } break;
  case BX: {
    registers[BX_] = val;
  } break;
  case SP: {
    registers[SP_] = val;
  } break;
  case BP: {
    registers[BP_] = val;
  } break;
  case SI: {
    registers[SI_] = val;
  } break;
  case DI: {
    registers[DI_] = val;
  } break;
  case ES: {
    registers[ES_] = val;
  } break;
  case CS: {
    registers[CS_] = val;
  } break;
  case SS: {
    registers[SS_] = val;
  } break;
  case DS: {
    registers[DS_] = val;
  } break;
  default: {
    expect(0, "?");
  } break;
  }
}

u16 read_reg(u16 *registers, enum Reg reg) {
  switch (reg) {
  case NONE: {
    expect(0, "?");
  } break;
  case AL: {
    return registers[AX_] & 0xff;
  } break;
  case CL: {
    return registers[CX_] & 0xff;
  } break;
  case DL: {
    return registers[DX_] & 0xff;
  } break;
  case BL: {
    return registers[BX_] & 0xff;
  } break;
  case AH: {
    return registers[AX_] >> 8;
  } break;
  case CH: {
    return registers[CX_] >> 8;
  } break;
  case DH: {
    return registers[DX_] >> 8;
  } break;
  case BH: {
    return registers[BX_] >> 8;
  } break;
  case AX: {
    return registers[AX_];
  } break;
  case CX: {
    return registers[CX_];
  } break;
  case DX: {
    return registers[DX_];
  } break;
  case BX: {
    return registers[BX_];
  } break;
  case SP: {
    return registers[SP_];
  } break;
  case BP: {
    return registers[BP_];
  } break;
  case SI: {
    return registers[SI_];
  } break;
  case DI: {
    return registers[DI_];
  } break;
  case ES: {
    return registers[ES_];
  } break;
  case CS: {
    return registers[CS_];
  } break;
  case SS: {
    return registers[SS_];
  } break;
  case DS: {
    return registers[DS_];
  } break;
  default: {
    expect(0, "?");
  } break;
  }
}

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
  u16 registers[REGISTER_SIZE] = {};
  u8 *prev_ptr;
  for (u8 *ptr = file; ptr < file + st.st_size;) {
    prev_ptr = ptr;
    if (bits_eq(ptr, 6, 0b100010) || (*ptr & 0b11000100) == 0b00000000 || (*ptr & 0b11111101) == 0b10001100) {
      // reg/mem to/from reg
      enum Instruction instruction;
      if (*ptr & 0x80) {
        instruction = MOV;
      } else {
        instruction = instruction_table0[(*ptr >> 3) & 0b111];
      }
      expect(instruction != 0, "unknown instruction");
      u32 d = (*ptr & 0b10) >> 1;
      u32 w = *ptr & 0b1;
      u32 segment = (*ptr >> 2) & 1;
      ptr++;
      u32 mod = (*ptr & 0b11000000) >> 6;
      u32 reg = (*ptr & 0b111000) >> 3;
      u32 rm = *ptr & 0b111;
      ptr++;
      enum Reg src_reg;
      if (segment) {
        // segment reg
        expect(!(reg >> 2), "?");
        src_reg = reg_table2[reg];
        w = 1;
      } else {
        src_reg = reg_table0[reg | w << 3];
      }
      if (mod == 0b11) {
        enum Reg dest_reg = reg_table0[rm | w << 3];
        if (d) {
          u32 tmp = src_reg;
          src_reg = dest_reg;
          dest_reg = tmp;
        }
        printf("%s %s, %s", instruction_to_name(instruction), reg_to_name(dest_reg), reg_to_name(src_reg));
        if (instruction == MOV) {
          u16 prev_val = read_reg(registers, dest_reg);
          write_reg(registers, dest_reg, read_reg(registers, src_reg));
          printf(" ; 0x%x -> %x", prev_val, read_reg(registers, dest_reg));
        }
      } else if (mod == 0b00) {
        if (rm == 0b110) {
          u32 disp = get_word(&ptr);
          if (d) {
            printf("%s %s, [%u]", instruction_to_name(instruction), reg_to_name(src_reg), disp);
          } else {
            printf("%s [%u], %s", instruction_to_name(instruction), disp, reg_to_name(src_reg));
          }
        } else {
          c8 *effective_addr_reg = reg_table1[rm];
          if (d) {
            printf("%s %s, [%s]", instruction_to_name(instruction), reg_to_name(src_reg), effective_addr_reg);
          } else {
            printf("%s [%s], %s", instruction_to_name(instruction), effective_addr_reg, reg_to_name(src_reg));
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
          printf("%s %s, [%s%c%u]", instruction_to_name(instruction), reg_to_name(src_reg), effective_addr_reg, sign, disp);
        } else {
          printf("%s [%s%c%u], %s", instruction_to_name(instruction), effective_addr_reg, sign, disp, reg_to_name(src_reg));
        }
      }
    } else if (bits_eq(ptr, 4, 0b1011)) {
      // imm to reg
      u32 w = (*ptr >> 3) & 1;
      u32 reg = *ptr & 0b111;
      ptr++;
      enum Reg dest_reg = reg_table0[reg | w << 3];
      u32 data = get_wbytes(&ptr, w);
      printf("mov %s, %u", reg_to_name(dest_reg), data);
      u16 prev_val = read_reg(registers, dest_reg);
      write_reg(registers, dest_reg, data);
      printf(" ; 0x%x -> %x", prev_val, read_reg(registers, dest_reg));
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
        printf("mov [%u], %s", addr, reg);
      } else {
        printf("mov %s, [%u]", reg, addr);
      }
    } else if (bits_eq(ptr, 6, 0b100000) || bits_eq(ptr, 7, 0b1100011)) {
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
      enum Instruction instruction;
      if ((*ptr & 0x40) == 0b0) {
        if (s && !w) {
          expect(0, "?");
        }
        ptr++;
        instruction = instruction_table0[((*ptr) >> 3) & 0b111];
      } else {
        ptr++;
        expect(!(*ptr & 0b111000), "ilegal instruction");
        instruction = MOV;
        s = 0;
      }
      u32 mod = *ptr >> 6;
      u32 rm = *ptr & 0b111;
      ptr++;
      c8 *size = w ? "word" : "byte";
      if (mod == 0b11) {
        // to reg
        enum Reg dest = reg_table0[rm | w << 3];
        u32 data = get_wbytes(&ptr, s ^ w);
        printf("%s %s, %u", instruction_to_name(instruction), reg_to_name(dest), data);
      } else if (mod == 0b00) {
        if (rm == 0b110) {
          u32 disp = get_word(&ptr);
          u32 data = get_wbytes(&ptr, s ^ w);
          printf("%s %s [%u], %s %u", instruction_to_name(instruction), size, disp, size, data);
        } else {
          c8 *effective_addr_reg = reg_table1[rm];
          u32 data = get_wbytes(&ptr, s ^ w);
          printf("%s %s [%s], %s %u", instruction_to_name(instruction), size, effective_addr_reg, size, data);
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
        printf("%s %s [%s%c%u], %s %u", instruction_to_name(instruction), size, effective_addr_reg, sign, disp, size, data);
      }
    } else if ((*ptr & 0b11000110) == 0b00000100) {
      // imm with a
      u32 w = *ptr & 1;
      enum Instruction instruction = instruction_table0[((*ptr) >> 3) & 0b111];
      ptr++;
      u32 data = get_wbytes(&ptr, w);
      printf("%s %s, %u", instruction_to_name(instruction), reg_to_name(w ? AX : AL), data);
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
        printf("%s $+%d", instruction, val);
      } else {
        printf("%s $%d", instruction, val);
      }
    } else {
      expect(0, "unknown instruction");
    }
    expect(prev_ptr != ptr, "mssing branch");
    puts("");
  }
  for (u32 i = 0; i < REGISTER_SIZE; ++i) {
    printf("%s: 0x%x\n", register_to_name(i), registers[i]);
  }
  return 0;
}
