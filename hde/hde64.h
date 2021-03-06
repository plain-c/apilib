#ifndef hde64_h__
#define hde64_h__

///////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * Hacker Disassembler Engine 64 C
 * Copyright (c) 2008-2009, Vyacheslav Patkov.
 * All rights reserved.
*/

///////////////////////////////////////////////////////////////////////////////////////////////////

namespace apilib
{
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//

  /**
   * ����� ���������� � ��������� HDE64.
  */
  enum HDE64_F
  {
    HDE64_F_MODRM          = 0x00000001,
    HDE64_F_SIB            = 0x00000002,
    HDE64_F_IMM8           = 0x00000004,
    HDE64_F_IMM16          = 0x00000008,
    HDE64_F_IMM32          = 0x00000010,
    HDE64_F_IMM64          = 0x00000020,
    HDE64_F_DISP8          = 0x00000040,
    HDE64_F_DISP16         = 0x00000080,
    HDE64_F_DISP32         = 0x00000100,
    HDE64_F_RELATIVE       = 0x00000200,
    HDE64_F_ERROR          = 0x00001000,
    HDE64_F_ERROR_OPCODE   = 0x00002000,
    HDE64_F_ERROR_LENGTH   = 0x00004000,
    HDE64_F_ERROR_LOCK     = 0x00008000,
    HDE64_F_ERROR_OPERAND  = 0x00010000,
    HDE64_F_PREFIX_REPNZ   = 0x01000000,
    HDE64_F_PREFIX_REPX    = 0x02000000,
    HDE64_F_PREFIX_REP     = 0x03000000,
    HDE64_F_PREFIX_66      = 0x04000000,
    HDE64_F_PREFIX_67      = 0x08000000,
    HDE64_F_PREFIX_LOCK    = 0x10000000,
    HDE64_F_PREFIX_SEG     = 0x20000000,
    HDE64_F_PREFIX_REX     = 0x40000000,
    HDE64_F_PREFIX_ANY     = 0x7f000000,
  };

  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//

  /**
   * �������� HDE64.
  */
  struct HDE64_CONTEXT
  {
    uint08_t len;
    uint08_t p_rep;
    uint08_t p_lock;
    uint08_t p_seg;
    uint08_t p_66;
    uint08_t p_67;
    uint08_t rex;
    uint08_t rex_w;
    uint08_t rex_r;
    uint08_t rex_x;
    uint08_t rex_b;
    uint08_t opcode;
    uint08_t opcode2;
    uint08_t modrm;
    uint08_t modrm_mod;
    uint08_t modrm_reg;
    uint08_t modrm_rm;
    uint08_t sib;
    uint08_t sib_scale;
    uint08_t sib_index;
    uint08_t sib_base;
    union {
      uint08_t imm8;
      uint16_t imm16;
      uint32_t imm32;
      uint64_t imm64;
    } imm;
    union {
      uint08_t disp8;
      uint16_t disp16;
      uint32_t disp32;
    } disp;
    uint32_t flags;    //����� HDE64_F.
  };

  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//

  /**
   * ������������������� ���������� � ����� �������� �� ����� � ������ ��������.
   *
   * @param address  ����� ����������.
   * @param context  �������� ���������� (������ �������� ����������).
   *
   * @return  � ������ ������ - ����� ����������,
   *          � ������ ������ - 0 ��� ������� ����� HDE64_F_ERROR.
  */
  unsigned char hde64_disasm(const void* address, HDE64_CONTEXT* context);

  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
};

///////////////////////////////////////////////////////////////////////////////////////////////////

#endif // hde64_h__