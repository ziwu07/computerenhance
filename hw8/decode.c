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
  EFFECTIVE_ADDRESS_REG_NONE = 0,
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
  u32 wide;
  union {
    // When type == IMMEDIATE
    u16 imm;
    // When type == REG
    enum Reg reg;
    // When type == EFFECTIVEADDRESS
    struct {
      enum EffectiveAddressReg reg;
      u16 displacement;
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
  u32 carry;
  u32 parity;
  u32 auxiliary;
  u32 overflow;
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

internal inline u32 bits_eq(u8 byte, u32 num, u8 val) {
  u8 mask = (u8)((0xff) ^ ((1 << (8 - num)) - 1));
  return (byte & mask) == (val << (8 - num));
}

internal inline u16 get_word(u8 *memory, u16 *ip) {
  u16 data;
  data = memory[*ip];
  (*ip)++;
  data |= memory[*ip] << 8;
  (*ip)++;
  return data;
}

internal inline i8 get_byte(u8 *memory, u16 *ip) {
  i8 data;
  data = (i8)memory[*ip];
  (*ip)++;
  return data;
}

internal inline u16 get_wbytes(u8 *memory, u16 *ip, u32 w) {
  return w ? get_word(memory, ip) : (u16)((i16)get_byte(memory, ip));
}

internal inline u16 compute_effective_address(enum EffectiveAddressReg regs, u16 disp, u16 *registers) {
  switch (regs) {
  case EFFECTIVE_ADDRESS_REG_NONE: {
    return disp;
  } break;
  case BX_SI: {
    return registers[BX_] + registers[SI_] + disp;
  } break;
  case BX_DI: {
    return registers[BX_] + registers[DI_] + disp;
  } break;
  case BP_SI: {
    return registers[BP_] + registers[SI_] + disp;
  } break;
  case BP_DI: {
    return registers[BP_] + registers[DI_] + disp;
  } break;
  case SI_0: {
    return registers[SI_] + disp;
  } break;
  case DI_0: {
    return registers[DI_] + disp;
  } break;
  case BP_0: {
    return registers[BP_] + disp;
  } break;
  case BX_0: {
    return registers[BX_] + disp;
  } break;
  default: {
    expect(0, "?");
  } break;
  }
}

internal u16 read_operand_value(struct Operand operand, u16 *registers, u8 *memory) {
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
    u16 idx = compute_effective_address(operand.effective_address.reg, operand.effective_address.displacement, registers);
    if (operand.wide) {
      return (u16)(((u16)memory[idx]) | (((u16)memory[(u16)(idx + 1)]) << 8));
    } else {
      return memory[idx];
    }
  } break;
  }
}

internal void write_operand_value(struct Operand operand, u16 *registers, u8 *memory, u16 value) {
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
    u16 idx = compute_effective_address(operand.effective_address.reg, operand.effective_address.displacement, registers);
    if (operand.wide) {
      memory[idx] = (u8)value;
      memory[(u16)(idx + 1)] = value >> 8;
    } else {
      memory[idx] = (u8)value;
    }
  } break;
  }
}

internal void execute_instruction(enum Instruction instruction, struct Operand src, struct Operand dest, u16 *registers, struct Flags *flags, u16 prev_ip, u16 *ip, u8 *memory) {
  (void)flags;
  u16 dest_val = read_operand_value(dest, registers, memory);
  u16 src_val = read_operand_value(src, registers, memory);
  u16 new_val;
  u32 val_changed = 0;
  u32 flags_changed = 0;
  switch (instruction) {
  case UNKNOWN: {
    expect(0, "?");
  } break;
  case MOV: {
    new_val = src_val;
    write_operand_value(dest, registers, memory, src_val);
    val_changed = 1;
  } break;
  case ADD: {
    u32 res = src_val + dest_val;
    new_val = (u16)res;
    flags->zero = new_val == 0 ? 1 : 0;
    flags->sign = new_val & 0x8000 ? 1 : 0;
    flags->carry = res & 0x10000 ? 1 : 0;
    flags->parity = __builtin_popcount(new_val & 0xff) % 2 == 0 ? 1 : 0;
    flags->auxiliary = ((u32)(src_val & 0xf) + (u32)(dest_val & 0xf)) & 0x10 ? 1 : 0;
    flags->overflow = (dest_val & 0x8000) ^ (src_val & 0x8000) ? 0 : ((new_val & 0x8000) ^ (src_val & 0x8000) ? 1 : 0);
    write_operand_value(dest, registers, memory, new_val);
    val_changed = 1;
    flags_changed = 1;
  } break;
  case SUB: {
    u32 res = dest_val - src_val;
    new_val = (u16)res;
    flags->zero = new_val == 0 ? 1 : 0;
    flags->sign = new_val & 0x8000 ? 1 : 0;
    flags->carry = res & 0x10000 ? 1 : 0;
    flags->parity = __builtin_popcount(new_val & 0xff) % 2 == 0 ? 1 : 0;
    flags->auxiliary = ((u32)(dest_val & 0xf) - (u32)(src_val & 0xf)) & 0x10 ? 1 : 0;
    /*
     * 0 - 0 = x no
     * 0 - 1 = 1 yes
     * 0 - 1 = 0 no
     * 1 - 0 = 0 yes
     * 1 - 0 = 1 no
     * 1 - 1 = x no
     */
    flags->overflow = (dest_val & 0x8000) ^ (src_val & 0x8000) ? ((new_val & 0x8000) ^ (src_val & 0x8000) ? 0 : 1) : 0;
    write_operand_value(dest, registers, memory, new_val);
    val_changed = 1;
    flags_changed = 1;
  } break;
  case CMP: {
    u32 res = dest_val - src_val;
    u16 test_val = (u16)res;
    flags->zero = test_val == 0 ? 1 : 0;
    flags->sign = test_val & 0x8000 ? 1 : 0;
    flags->carry = res & 0x10000 ? 1 : 0;
    flags->parity = __builtin_popcount(test_val & 0xff) % 2 == 0 ? 1 : 0;
    flags->auxiliary = ((u32)(dest_val & 0xf) - (u32)(src_val & 0xf)) & 0x10 ? 1 : 0;
    flags->overflow = (dest_val & 0x8000) ^ (src_val & 0x8000) ? ((test_val & 0x8000) ^ (src_val & 0x8000) ? 0 : 1) : 0;
    flags_changed = 1;
  } break;
  case JO: {
    if (flags->overflow) {
      (*ip) += src_val;
    }
  } break;
  case JNO: {
    if (!flags->overflow) {
      (*ip) += src_val;
    }
  } break;
  case JB: {
    if (flags->carry) {
      (*ip) += src_val;
    }
  } break;
  case JNB: {
    if (!flags->carry) {
      (*ip) += src_val;
    }
  } break;
  case JE: {
    if (flags->zero) {
      (*ip) += src_val;
    }
  } break;
  case JNE: {
    if (!flags->zero) {
      (*ip) += src_val;
    }
  } break;
  case JBE: {
    if (flags->carry || flags->zero) {
      (*ip) += src_val;
    }
  } break;
  case JA: {
    if (!(flags->carry || flags->zero)) {
      (*ip) += src_val;
    }
  } break;
  case JS: {
    if (flags->sign) {
      (*ip) += src_val;
    }
  } break;
  case JNS: {
    if (!flags->sign) {
      (*ip) += src_val;
    }
  } break;
  case JP: {
    if (flags->parity) {
      (*ip) += src_val;
    }
  } break;
  case JNP: {
    if (!flags->parity) {
      (*ip) += src_val;
    }
  } break;
  case JL: {
    if ((flags->sign ^ flags->overflow) == 1) {
      (*ip) += src_val;
    }
  } break;
  case JNL: {
    if ((flags->sign ^ flags->overflow) == 0) {
      (*ip) += src_val;
    }
  } break;
  case JLE: {
    if (((flags->sign ^ flags->overflow) | flags->zero) == 1) {
      (*ip) += src_val;
    }
  } break;
  case JG: {
    if (((flags->sign ^ flags->overflow) | flags->zero) == 0) {
      (*ip) += src_val;
    }
  } break;
  case LOOPNZ: {
    registers[CX_] -= 1;
    if (registers[CX_] != 0 && !flags->zero) {
      (*ip) += src_val;
    }
  } break;
  case LOOPZ: {
    registers[CX_] -= 1;
    if (registers[CX_] != 0 && flags->zero) {
      (*ip) += src_val;
    }
  } break;
  case LOOP: {
    registers[CX_] -= 1;
    if (registers[CX_] != 0) {
      (*ip) += src_val;
    }
  } break;
  case JCXZ: {
    if (registers[CX_] == 0) {
      (*ip) += src_val;
    }
  } break;
  }
  printf("\t; IP: %.4x -> %.4x", prev_ip, *ip);
  if (val_changed) {
    printf("  0x%.4x -> 0x%.4x", dest_val, new_val);
  }
  if (flags_changed) {
    if (!val_changed) {
      printf("                  ");
    }
    printf("  Flags: %c%c%c%c%c%c", flags->zero ? 'Z' : '-', flags->sign ? 'S' : '-', flags->carry ? 'C' : '-', flags->parity ? 'P' : '-', flags->auxiliary ? 'A' : '-', flags->overflow ? 'O' : '-');
  }
}

internal inline void swap_if_d(u32 d, struct Operand *op0, struct Operand *op1) {
  if (d) {
    struct Operand tmp = *op0;
    *op0 = *op1;
    *op1 = tmp;
  }
}

int main(int argc, char *argv[]) {
  expect(argc == 2, "num args");
  i32 fd = open(argv[1], O_RDONLY);
  expect_errno(fd != -1, "open");
  struct stat st;
  fstat(fd, &st);
  alignas(1 << 16) u8 memory[1 << 16];
  {
    i64 ret = read(fd, memory, (u64)st.st_size);
    expect_errno(ret != -1, "read file");
    expect(ret == st.st_size, "read size");
  }
  puts("bits 16");
  u16 ip;
  u16 prev_ip;
  u16 registers[REGISTER_SIZE] = {};
  struct Flags flags = {0};
  struct Operand src;
  struct Operand dest;
  enum Instruction instruction;
  for (ip = 0; ip < st.st_size;) {
    prev_ip = ip;
    src.type = OPERAND_TYPE_NOT_SET;
    src.wide = 2;
    dest.type = OPERAND_TYPE_NOT_SET;
    dest.wide = 2;
    instruction = UNKNOWN;
    if (bits_eq(memory[ip], 6, 0b100010) || (memory[ip] & 0b11000100) == 0b00000000 || (memory[ip] & 0b11111101) == 0b10001100) {
      // reg/mem to/from reg
      if (memory[ip] & 0x80) {
        instruction = MOV;
      } else {
        instruction = instruction_table0[(memory[ip] >> 3) & 0b111];
      }
      u32 d = (memory[ip] & 0b10) >> 1;
      u32 w = memory[ip] & 0b1;
      u32 segment = (memory[ip] >> 2) & 1;
      ip++;
      u32 mod = (memory[ip] & 0b11000000) >> 6;
      u32 reg = (memory[ip] & 0b111000) >> 3;
      u32 rm = memory[ip] & 0b111;
      ip++;
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
        src.wide = w;
        dest.type = REG;
        dest.reg = dest_reg;
        dest.wide = w;
      } else if (mod == 0b00) {
        if (rm == 0b110) {
          u16 disp = get_word(memory, &ip);
          if (d) {
            printf("%s %s, [%u]", instruction_to_name(instruction), reg_to_name(src_reg), disp);
          } else {
            printf("%s [%u], %s", instruction_to_name(instruction), disp, reg_to_name(src_reg));
          }
          src.type = REG;
          src.reg = src_reg;
          src.wide = w;
          dest.type = EFFECTIVEADDRESS;
          dest.effective_address.reg = EFFECTIVE_ADDRESS_REG_NONE;
          dest.effective_address.displacement = disp;
          dest.wide = w;
          swap_if_d(d, &src, &dest);
        } else {
          enum EffectiveAddressReg effective_addr_reg = displacement_table[rm];
          if (d) {
            printf("%s %s, [%s]", instruction_to_name(instruction), reg_to_name(src_reg), effective_address_reg_to_name(effective_addr_reg));
          } else {
            printf("%s [%s], %s", instruction_to_name(instruction), effective_address_reg_to_name(effective_addr_reg), reg_to_name(src_reg));
          }
          src.type = REG;
          src.reg = src_reg;
          src.wide = w;
          dest.type = EFFECTIVEADDRESS;
          dest.effective_address.reg = effective_addr_reg;
          dest.effective_address.displacement = 0;
          dest.wide = w;
          swap_if_d(d, &src, &dest);
        }
      } else {
        enum EffectiveAddressReg effective_addr_reg = displacement_table[rm];
        u16 disp = get_wbytes(memory, &ip, mod == 0b10);
        if (d) {
          printf("%s %s, [%s%+d]", instruction_to_name(instruction), reg_to_name(src_reg), effective_address_reg_to_name(effective_addr_reg), disp);
        } else {
          printf("%s [%s%+d], %s", instruction_to_name(instruction), effective_address_reg_to_name(effective_addr_reg), disp, reg_to_name(src_reg));
        }
        src.type = REG;
        src.reg = src_reg;
        src.wide = w;
        dest.type = EFFECTIVEADDRESS;
        dest.effective_address.reg = effective_addr_reg;
        dest.effective_address.displacement = disp;
        dest.wide = w;
        swap_if_d(d, &src, &dest);
      }
    } else if (bits_eq(memory[ip], 4, 0b1011)) {
      // imm to reg
      u32 w = (memory[ip] >> 3) & 1;
      u32 reg = memory[ip] & 0b111;
      ip++;
      enum Reg dest_reg = reg_table0[reg | w << 3];
      instruction = MOV;
      u16 data = get_wbytes(memory, &ip, w);
      printf("mov %s, %d", reg_to_name(dest_reg), data);
      src.type = IMMEDIATE;
      src.imm = data;
      src.wide = w;
      dest.type = REG;
      dest.reg = dest_reg;
      dest.wide = w;
    } else if (bits_eq(memory[ip], 6, 0b101000)) {
      // mem to a or a to mem
      u32 w = memory[ip] & 1;
      u32 d = (memory[ip] >> 1) & 1;
      ip++;
      instruction = MOV;
      u16 addr = (u16)get_word(memory, &ip);
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
      src.type = EFFECTIVEADDRESS;
      src.effective_address.reg = EFFECTIVE_ADDRESS_REG_NONE;
      src.effective_address.displacement = addr;
      src.wide = w;
      dest.type = REG;
      dest.reg = reg;
      dest.wide = w;
      swap_if_d(d, &src, &dest);
    } else if (bits_eq(memory[ip], 6, 0b100000) || bits_eq(memory[ip], 7, 0b1100011)) {
      // imm to reg/mem
      u32 w = memory[ip] & 1;
      u32 s = (memory[ip] >> 1) & 1;
      // sw
      // 11
      // 1 byte, word
      // 10
      // ?
      // 01
      // 2 byte, word
      // 00
      // 1 byte, byte
      if ((memory[ip] & 0x40) == 0b0) {
        if (s && !w) {
          expect(0, "?");
        }
        ip++;
        instruction = instruction_table0[(memory[ip] >> 3) & 0b111];
      } else {
        ip++;
        expect(!(memory[ip] & 0b111000), "ilegal instruction");
        instruction = MOV;
        s = 0;
      }
      u32 mod = memory[ip] >> 6;
      u32 rm = memory[ip] & 0b111;
      ip++;
      c8 *size = w ? "word" : "byte";
      if (mod == 0b11) {
        // to reg
        enum Reg dest_reg = reg_table0[rm | w << 3];
        u16 data = get_wbytes(memory, &ip, s ^ w);
        printf("%s %s, %u", instruction_to_name(instruction), reg_to_name(dest_reg), data);
        src.type = IMMEDIATE;
        src.imm = data;
        src.wide = w;
        dest.type = REG;
        dest.reg = dest_reg;
        dest.wide = w;
      } else if (mod == 0b00) {
        u16 data;
        if (rm == 0b110) {
          u16 disp = get_word(memory, &ip);
          data = get_wbytes(memory, &ip, s ^ w);
          printf("%s [%u], %s %u", instruction_to_name(instruction), disp, size, data);
          dest.effective_address.reg = EFFECTIVE_ADDRESS_REG_NONE;
          dest.effective_address.displacement = disp;
        } else {
          enum EffectiveAddressReg effective_addr_reg = displacement_table[rm];
          data = get_wbytes(memory, &ip, s ^ w);
          printf("%s [%s], %s %u", instruction_to_name(instruction), effective_address_reg_to_name(effective_addr_reg), size, data);
          dest.effective_address.reg = effective_addr_reg;
          dest.effective_address.displacement = 0;
        }
        src.type = IMMEDIATE;
        src.imm = data;
        src.wide = w;
        dest.type = EFFECTIVEADDRESS;
        dest.wide = w;
      } else {
        enum EffectiveAddressReg effective_addr_reg = displacement_table[rm];
        u16 disp = get_wbytes(memory, &ip, mod == 0b10);
        u16 data = get_wbytes(memory, &ip, s ^ w);
        printf("%s [%s%+d], %s %u", instruction_to_name(instruction), effective_address_reg_to_name(effective_addr_reg), disp, size, data);
        src.type = IMMEDIATE;
        src.imm = data;
        src.wide = w;
        dest.type = EFFECTIVEADDRESS;
        dest.effective_address.reg = effective_addr_reg;
        dest.effective_address.displacement = disp;
        dest.wide = w;
      }
    } else if ((memory[ip] & 0b11000110) == 0b00000100) {
      // imm with a
      u32 w = memory[ip] & 1;
      instruction = instruction_table0[(memory[ip] >> 3) & 0b111];
      ip++;
      u16 data = get_wbytes(memory, &ip, w);
      enum Reg dest_reg = w ? AX : AL;
      printf("%s %s, %u", instruction_to_name(instruction), reg_to_name(dest_reg), data);
      src.type = IMMEDIATE;
      src.imm = data;
      dest.type = REG;
      dest.reg = dest_reg;
    } else if (bits_eq(memory[ip], 4, 0b0111) || bits_eq(memory[ip], 6, 0b111000)) {
      if (memory[ip] & 0x80) {
        instruction = instruction_table2[memory[ip] & 0b11];
      } else {
        instruction = instruction_table1[memory[ip] & 0b1111];
      }
      ip++;
      i8 val = get_byte(memory, &ip);
      printf("%s $%+d", instruction_to_name(instruction), val + 2);
      src.type = IMMEDIATE;
      src.imm = (u16)((i16)val);
      src.wide = 0;
      dest.type = OPERAND_TYPE_NONE;
      dest.wide = 0;
    } else {
      expect(0, "unknown instruction");
    }
    expect(src.type != OPERAND_TYPE_NOT_SET, "missing src");
    expect(dest.type != OPERAND_TYPE_NOT_SET, "missing dest");
    expect(src.wide != 2, "missing wide flag on src");
    expect(dest.wide != 2, "missing wide flag on dest");
    expect(instruction != UNKNOWN, "missing instruction");
    execute_instruction(instruction, src, dest, registers, &flags, prev_ip, &ip, memory);
    puts("");
  }
  puts("\n; Registers:");
  for (u32 i = 0; i < REGISTER_SIZE; ++i) {
    printf("; %s: 0x%.4x\n", register_to_name(i), registers[i]);
  }
  printf("; IP: 0x%.4x\n", ip);
  printf("; Flags: %c%c%c%c%c%c\n", flags.zero ? 'Z' : '-', flags.sign ? 'S' : '-', flags.carry ? 'C' : '-', flags.parity ? 'P' : '-', flags.auxiliary ? 'A' : '-', flags.overflow ? 'O' : '-');
  i32 out_fd = open("test.data", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
  if (out_fd > 0) {
    write(out_fd, memory, sizeof(memory));
  }
  return 0;
}
