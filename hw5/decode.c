#include "../common.h"

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
  REG_NONE = 0,
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

enum EffectiveAddressReg {
  DISPLACEMENT_NONE = 0,
  BX_SI,
  BX_DI,
  BP_SI,
  BP_DI,
  SI_0,
  DI_0,
  BP_0,
  BX_0,
  DISPLACEMENT_SIZE,
};

const enum EffectiveAddressReg displacement_table[] = {BX_SI, BX_DI, BP_SI, BP_DI, SI_0, DI_0, BP_0, BX_0};

internal inline c8 *effective_address_reg_to_name(enum EffectiveAddressReg displacement) {
  c8 *const name_table[] = {0, "bx+si", "bx+di", "bp+si", "bp+di", "si", "di", "bp", "bx"};
  return name_table[displacement];
}

enum OperandType {
  OPERAND_TYPE_NOT_SET = 0,
  OPERAND_TYPE_NONE,
  IMMEDIATE,
  REG,
  EFFECTIVEADDRESS,
};

struct Operand {
  enum OperandType type;
  union {
    // When type == IMMEDIATE
    u16 imm;
    // When type == REG
    enum Reg reg;
    // When type == EFFECTIVEADDRESS
    struct {
      enum EffectiveAddressReg reg;
      i16 displacement;
    } effective_address;
  };
};

const enum Reg reg_table0[] = {AL, CL, DL, BL, AH, CH, DH, BH, AX, CX, DX, BX, SP, BP, SI, DI};
const enum Reg reg_table2[] = {ES, CS, SS, DS};

enum Instruction {
  UNKNOWN = 0,
  MOV,
  ADD,
  SUB,
  CMP,
  JO,
  JNO,
  JB,
  JNB,
  JE,
  JNE,
  JBE,
  JA,
  JS,
  JNS,
  JP,
  JNP,
  JL,
  JNL,
  JLE,
  JG,
  LOOPNZ,
  LOOPZ,
  LOOP,
  JCXZ,
};

const enum Instruction instruction_table1[] = {JO, JNO, JB, JNB, JE, JNE, JBE, JA, JS, JNS, JP, JNP, JL, JNL, JLE, JG};
const enum Instruction instruction_table2[] = {LOOPNZ, LOOPZ, LOOP, JCXZ};

const enum Instruction instruction_table0[] = {ADD, 0, 0, 0, 0, SUB, 0, CMP};

struct Flags {
  u32 zero;
  u32 sign;
};

internal inline c8 *instruction_to_name(enum Instruction inst) {
  c8 *const name_table[] = {0, "mov", "add", "sub", "cmp", "jo", "jno", "jb", "jnb", "je", "jne", "jbe", "ja", "js", "jns", "jp", "jnp", "jl", "jnl", "jle", "jg", "loopnz", "loopz", "loop", "jcxz"};
  return name_table[inst];
}

internal inline c8 *register_to_name(enum Register reg) {
  c8 *const name_table[] = {"AX", "BX", "CX", "DX", "SP", "BP", "SI", "DI", "SS", "DS", "CS", "ES"};
  return name_table[reg];
}

internal inline c8 *reg_to_name(enum Reg reg) {
  c8 *const name_table[] = {0, "al", "cl", "dl", "bl", "ah", "ch", "dh", "bh", "ax", "cx", "dx", "bx", "sp", "bp", "si", "di", "es", "cs", "ss", "ds"};
  return name_table[reg];
}

internal void write_reg(u16 *registers, enum Reg reg, u16 val) {
  switch (reg) {
  case REG_NONE: {
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

internal u16 read_reg(u16 *registers, enum Reg reg) {
  switch (reg) {
  case REG_NONE: {
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

internal inline u32 bits_eq(u8 *byte, u32 num, u8 val) {
  u8 mask = (~0) ^ ((1 << (8 - num)) - 1);
  return (*byte & mask) == (val << (8 - num));
}

internal inline u32 get_word(u8 **ptr) {
  u32 data;
  data = **ptr;
  (*ptr)++;
  data |= (**ptr) << 8;
  (*ptr)++;
  return data;
}

internal inline u32 get_byte(u8 **ptr) {
  u32 data;
  data = **ptr;
  (*ptr)++;
  return data;
}

internal inline u32 get_wbytes(u8 **ptr, u32 w) {
  return w ? get_word(ptr) : get_byte(ptr);
}

internal inline void setflags(const enum Instruction instruction, struct Flags *flags, u16 prev_val, u16 new_val) {
  (void)prev_val;
  switch (instruction) {
  case ADD:
  case SUB:
  case CMP: {
    flags->zero = new_val == 0 ? 1 : 0;
    flags->sign = new_val & 0x8000 ? 1 : 0;
  } break;
  default: {
    todo;
  } break;
  }
}

internal u16 read_operand_value(struct Operand operand, u16 *registers) {
  switch (operand.type) {
  case OPERAND_TYPE_NOT_SET: {
    return 0;
  }
  case OPERAND_TYPE_NONE: {
    return 0;
  } break;
  case IMMEDIATE: {
    return operand.imm;
  } break;
  case REG: {
    return read_reg(registers, operand.reg);
  } break;
  case EFFECTIVEADDRESS: {
    todo;
  } break;
  }
}

internal void write_operand_value(struct Operand operand, u16 *registers, u16 value) {
  switch (operand.type) {
  case OPERAND_TYPE_NOT_SET: {
    expect(0, "?");
  }
  case OPERAND_TYPE_NONE: {
    return;
  } break;
  case IMMEDIATE: {
    expect(0, "?");
  } break;
  case REG: {
    write_reg(registers, operand.reg, value);
  } break;
  case EFFECTIVEADDRESS: {
    todo;
  } break;
  }
}

internal void execute_instruction(enum Instruction instruction, struct Operand src, struct Operand dest, u16 *registers, struct Flags *flags) {
  (void)flags;
  u16 prev_val = read_operand_value(dest, registers);
  u16 new_val;
  u32 val_changed = 0;
  u32 flags_changed = 0;
  switch (instruction) {
  case UNKNOWN: {
    expect(0, "?");
  } break;
  case MOV: {
    new_val = read_operand_value(src, registers);
    write_operand_value(dest, registers, new_val);
    val_changed = 1;
  } break;
  case ADD: {
    u16 val = read_operand_value(src, registers);
    new_val = val + prev_val;
    setflags(ADD, flags, prev_val, new_val);
    write_operand_value(dest, registers, new_val);
    val_changed = 1;
    flags_changed = 1;
  } break;
  case SUB: {
    u16 val = read_operand_value(src, registers);
    new_val = prev_val - val;
    setflags(SUB, flags, prev_val, new_val);
    write_operand_value(dest, registers, new_val);
    val_changed = 1;
    flags_changed = 1;
  } break;
  case CMP: {
    u16 val = read_operand_value(src, registers);
    setflags(CMP, flags, prev_val, val);
    flags_changed = 1;
  } break;
  case JO: {
    todo;
  } break;
  case JNO: {
    todo;
  } break;
  case JB: {
    todo;
  } break;
  case JNB: {
    todo;
  } break;
  case JE: {
    todo;
  } break;
  case JNE: {
    todo;
  } break;
  case JBE: {
    todo;
  } break;
  case JA: {
    todo;
  } break;
  case JS: {
    todo;
  } break;
  case JNS: {
    todo;
  } break;
  case JP: {
    todo;
  } break;
  case JNP: {
    todo;
  } break;
  case JL: {
    todo;
  } break;
  case JNL: {
    todo;
  } break;
  case JLE: {
    todo;
  } break;
  case JG: {
    todo;
  } break;
  case LOOPNZ: {
    todo;
  } break;
  case LOOPZ: {
    todo;
  } break;
  case LOOP: {
    todo;
  } break;
  case JCXZ: {
    todo;
  } break;
  }
  if (val_changed) {
    printf("\t; 0x%.4x -> 0x%.4x", prev_val, new_val);
  }
  if (flags_changed) {
    if (!val_changed) {
      printf("\t;                 ");
    }
    printf("  Flags: %c%c", flags->zero ? 'Z' : '-', flags->sign ? 'S' : '-');
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
  u16 registers[REGISTER_SIZE] = {};
  struct Flags flags = {0};
  u8 *prev_ptr;
  struct Operand src;
  struct Operand dest;
  enum Instruction instruction;
  for (u8 *ptr = file; ptr < file + st.st_size;) {
    prev_ptr = ptr;
    src.type = OPERAND_TYPE_NOT_SET;
    dest.type = OPERAND_TYPE_NOT_SET;
    instruction = UNKNOWN;
    if (bits_eq(ptr, 6, 0b100010) || (*ptr & 0b11000100) == 0b00000000 || (*ptr & 0b11111101) == 0b10001100) {
      // reg/mem to/from reg
      if (*ptr & 0x80) {
        instruction = MOV;
      } else {
        instruction = instruction_table0[(*ptr >> 3) & 0b111];
      }
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
        src.type = REG;
        src.reg = src_reg;
        dest.type = REG;
        dest.reg = dest_reg;
      } else if (mod == 0b00) {
        if (rm == 0b110) {
          u32 disp = get_word(&ptr);
          if (d) {
            printf("%s %s, [%u]", instruction_to_name(instruction), reg_to_name(src_reg), disp);
          } else {
            printf("%s [%u], %s", instruction_to_name(instruction), disp, reg_to_name(src_reg));
          }
        } else {
          enum EffectiveAddressReg effective_addr_reg = displacement_table[rm];
          if (d) {
            printf("%s %s, [%s]", instruction_to_name(instruction), reg_to_name(src_reg), effective_address_reg_to_name(effective_addr_reg));
          } else {
            printf("%s [%s], %s", instruction_to_name(instruction), effective_address_reg_to_name(effective_addr_reg), reg_to_name(src_reg));
          }
        }
      } else {
        enum EffectiveAddressReg effective_addr_reg = displacement_table[rm];
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
          printf("%s %s, [%s%c%u]", instruction_to_name(instruction), reg_to_name(src_reg), effective_address_reg_to_name(effective_addr_reg), sign, disp);
        } else {
          printf("%s [%s%c%u], %s", instruction_to_name(instruction), effective_address_reg_to_name(effective_addr_reg), sign, disp, reg_to_name(src_reg));
        }
      }
    } else if (bits_eq(ptr, 4, 0b1011)) {
      // imm to reg
      u32 w = (*ptr >> 3) & 1;
      u32 reg = *ptr & 0b111;
      ptr++;
      enum Reg dest_reg = reg_table0[reg | w << 3];
      instruction = MOV;
      u32 data = get_wbytes(&ptr, w);
      printf("mov %s, %u", reg_to_name(dest_reg), data);
      src.type = IMMEDIATE;
      src.imm = data;
      dest.type = REG;
      dest.reg = dest_reg;
    } else if (bits_eq(ptr, 6, 0b101000)) {
      // mem to a or a to mem
      u32 w = *ptr & 1;
      u32 d = (*ptr >> 1) & 1;
      ptr++;
      instruction = MOV;
      u32 addr = get_word(&ptr);
      enum Reg reg;
      if (w) {
        reg = AX;
      } else {
        reg = AL;
      }
      if (d) {
        printf("mov [%u], %s", addr, reg_to_name(reg));
      } else {
        printf("mov %s, [%u]", reg_to_name(reg), addr);
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
        enum Reg dest_reg = reg_table0[rm | w << 3];
        u32 data = get_wbytes(&ptr, s ^ w);
        src.type = IMMEDIATE;
        src.imm = data;
        dest.type = REG;
        dest.reg = dest_reg;
        printf("%s %s, %u", instruction_to_name(instruction), reg_to_name(dest_reg), data);
      } else if (mod == 0b00) {
        if (rm == 0b110) {
          u32 disp = get_word(&ptr);
          u32 data = get_wbytes(&ptr, s ^ w);
          printf("%s %s [%u], %s %u", instruction_to_name(instruction), size, disp, size, data);
        } else {
          enum EffectiveAddressReg effective_addr_reg = displacement_table[rm];
          u32 data = get_wbytes(&ptr, s ^ w);
          printf("%s %s [%s], %s %u", instruction_to_name(instruction), size, effective_address_reg_to_name(effective_addr_reg), size, data);
        }
      } else {
        enum EffectiveAddressReg effective_addr_reg = displacement_table[rm];
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
        printf("%s %s [%s%c%u], %s %u", instruction_to_name(instruction), size, effective_address_reg_to_name(effective_addr_reg), sign, disp, size, data);
      }
    } else if ((*ptr & 0b11000110) == 0b00000100) {
      // imm with a
      u32 w = *ptr & 1;
      instruction = instruction_table0[((*ptr) >> 3) & 0b111];
      ptr++;
      u32 data = get_wbytes(&ptr, w);
      enum Reg dest_reg = w ? AX : AL;
      src.type = IMMEDIATE;
      src.imm = data;
      dest.type = REG;
      dest.reg = dest_reg;
      printf("%s %s, %u", instruction_to_name(instruction), reg_to_name(dest_reg), data);
    } else if (bits_eq(ptr, 4, 0b0111) || bits_eq(ptr, 6, 0b111000)) {
      if (*ptr & 0x80) {
        instruction = instruction_table2[*ptr & 0b11];
      } else {
        instruction = instruction_table1[*ptr & 0b1111];
      }
      ptr++;
      u32 byte = get_byte(&ptr);
      i8 val = ((i8)byte) + 2;
      if (val >= 0) {
        printf("%s $+%d", instruction_to_name(instruction), val);
      } else {
        printf("%s $%d", instruction_to_name(instruction), val);
      }
    } else {
      expect(0, "unknown instruction");
    }
    expect(prev_ptr != ptr, "mssing branch");
    expect(src.type != OPERAND_TYPE_NOT_SET, "missing src");
    expect(dest.type != OPERAND_TYPE_NOT_SET, "missing dest");
    expect(instruction != UNKNOWN, "missing instruction");
    execute_instruction(instruction, src, dest, registers, &flags);
    puts("");
  }
  puts("\n; Registers:");
  for (u32 i = 0; i < REGISTER_SIZE; ++i) {
    printf("; %s: 0x%.4x\n", register_to_name(i), registers[i]);
  }
  printf("; Flags: %c%c\n", flags.zero ? 'Z' : '-', flags.sign ? 'S' : '-');
  return 0;
}
