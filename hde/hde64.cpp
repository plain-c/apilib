///////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * Hacker Disassembler Engine 64 C
 * Copyright (c) 2008-2009, Vyacheslav Patkov.
 * All rights reserved.
*/

///////////////////////////////////////////////////////////////////////////////////////////////////

#include "../include/mem.h"
#include "../include/lib.h"

#include "hde64.h"
#include "tbl64.h"

///////////////////////////////////////////////////////////////////////////////////////////////////

unsigned char apilib::hde64_disasm(const void* address, HDE64_CONTEXT* context)
{
  if(!address || !context) {
    return 0;
  }

  uint08_t x, c, *p = (uint08_t *)address, cflags, opcode, pref = 0;
  uint08_t m_mod, m_reg, m_rm, disp_size = 0, op64 = 0;
  const uint08_t *ht = hde64_table;

  apilib::memzero(context, sizeof(HDE64_CONTEXT));

  for (x = 16; x; x--)
    switch (c = *p++) {
    case 0xf3:
      context->p_rep = c;
      pref |= HDE64_PRE_F3;
      break;
    case 0xf2:
      context->p_rep = c;
      pref |= HDE64_PRE_F2;
      break;
    case 0xf0:
      context->p_lock = c;
      pref |= HDE64_PRE_LOCK;
      break;
    case 0x26: case 0x2e: case 0x36:
    case 0x3e: case 0x64: case 0x65:
      context->p_seg = c;
      pref |= HDE64_PRE_SEG;
      break;
    case 0x66:
      context->p_66 = c;
      pref |= HDE64_PRE_66;
      break;
    case 0x67:
      context->p_67 = c;
      pref |= HDE64_PRE_67;
      break;
    default:
      goto pref_done;
  }
pref_done:

  context->flags = (uint32_t)pref << 23;

  if (!pref)
    pref |= HDE64_PRE_NONE;

  if ((c & 0xf0) == 0x40) {
    context->flags |= HDE64_F_PREFIX_REX;
    if ((context->rex_w = (c & 0xf) >> 3) && (*p & 0xf8) == 0xb8)
      op64++;
    context->rex_r = (c & 7) >> 2;
    context->rex_x = (c & 3) >> 1;
    context->rex_b = c & 1;
    if (((c = *p++) & 0xf0) == 0x40) {
      opcode = c;
      goto error_opcode;
    }
  }

  if ((context->opcode = c) == 0x0f) {
    context->opcode2 = c = *p++;
    ht += HDE64_DELTA_OPCODES;
  } else if (c >= 0xa0 && c <= 0xa3) {
    op64++;
    if (pref & HDE64_PRE_67)
      pref |= HDE64_PRE_66;
    else
      pref &= ~HDE64_PRE_66;
  }

  opcode = c;
  cflags = ht[ht[opcode / 4] + (opcode % 4)];

  if (cflags == HDE64_C_ERROR) {
error_opcode:
    context->flags |= HDE64_F_ERROR | HDE64_F_ERROR_OPCODE;
    cflags = 0;
    if ((opcode & -3) == 0x24)
      cflags++;
  }

  x = 0;
  if (cflags & HDE64_C_GROUP) {
    uint16_t t;
    t = *(uint16_t *)(ht + (cflags & 0x7f));
    cflags = (uint08_t)t;
    x = (uint08_t)(t >> 8);
  }

  if (context->opcode2) {
    ht = hde64_table + HDE64_DELTA_PREFIXES;
    if (ht[ht[opcode / 4] + (opcode % 4)] & pref)
      context->flags |= HDE64_F_ERROR | HDE64_F_ERROR_OPCODE;
  }

  if (cflags & HDE64_C_MODRM) {
    context->flags |= HDE64_F_MODRM;
    context->modrm = c = *p++;
    context->modrm_mod = m_mod = c >> 6;
    context->modrm_rm = m_rm = c & 7;
    context->modrm_reg = m_reg = (c & 0x3f) >> 3;

    if (x && ((x << m_reg) & 0x80))
      context->flags |= HDE64_F_ERROR | HDE64_F_ERROR_OPCODE;

    if (!context->opcode2 && opcode >= 0xd9 && opcode <= 0xdf) {
      uint08_t t = opcode - 0xd9;
      if (m_mod == 3) {
        ht = hde64_table + HDE64_DELTA_FPU_MODRM + t*8;
        t = ht[m_reg] << m_rm;
      } else {
        ht = hde64_table + HDE64_DELTA_FPU_REG;
        t = ht[t] << m_reg;
      }
      if (t & 0x80)
        context->flags |= HDE64_F_ERROR | HDE64_F_ERROR_OPCODE;
    }

    if (pref & HDE64_PRE_LOCK) {
      if (m_mod == 3) {
        context->flags |= HDE64_F_ERROR | HDE64_F_ERROR_LOCK;
      } else {
        const uint08_t *table_end;
        uint08_t op = opcode;
        if (context->opcode2) {
          ht = hde64_table + HDE64_DELTA_OP2_LOCK_OK;
          table_end = ht + HDE64_DELTA_OP_ONLY_MEM - HDE64_DELTA_OP2_LOCK_OK;
        } else {
          ht = hde64_table + HDE64_DELTA_OP_LOCK_OK;
          table_end = ht + HDE64_DELTA_OP2_LOCK_OK - HDE64_DELTA_OP_LOCK_OK;
          op &= -2;
        }
        for (; ht != table_end; ht++)
          if (*ht++ == op) {
            if (!((*ht << m_reg) & 0x80))
              goto no_lock_error;
            else
              break;
          }
          context->flags |= HDE64_F_ERROR | HDE64_F_ERROR_LOCK;
no_lock_error:
          ;
      }
    }

    if (context->opcode2) {
      switch (opcode) {
      case 0x20: case 0x22:
        m_mod = 3;
        if (m_reg > 4 || m_reg == 1)
          goto error_operand;
        else
          goto no_error_operand;
      case 0x21: case 0x23:
        m_mod = 3;
        if (m_reg == 4 || m_reg == 5)
          goto error_operand;
        else
          goto no_error_operand;
      }
    } else {
      switch (opcode) {
      case 0x8c:
        if (m_reg > 5)
          goto error_operand;
        else
          goto no_error_operand;
      case 0x8e:
        if (m_reg == 1 || m_reg > 5)
          goto error_operand;
        else
          goto no_error_operand;
      }
    }

    if (m_mod == 3) {
      const uint08_t *table_end;
      if (context->opcode2) {
        ht = hde64_table + HDE64_DELTA_OP2_ONLY_MEM;
        table_end = ht + sizeof(hde64_table) - HDE64_DELTA_OP2_ONLY_MEM;
      } else {
        ht = hde64_table + HDE64_DELTA_OP_ONLY_MEM;
        table_end = ht + HDE64_DELTA_OP2_ONLY_MEM - HDE64_DELTA_OP_ONLY_MEM;
      }
      for (; ht != table_end; ht += 2)
        if (*ht++ == opcode) {
          if (*ht++ & pref && !((*ht << m_reg) & 0x80))
            goto error_operand;
          else
            break;
        }
        goto no_error_operand;
    } else if (context->opcode2) {
      switch (opcode) {
      case 0x50: case 0xd7: case 0xf7:
        if (pref & (HDE64_PRE_NONE | HDE64_PRE_66))
          goto error_operand;
        break;
      case 0xd6:
        if (pref & (HDE64_PRE_F2 | HDE64_PRE_F3))
          goto error_operand;
        break;
      case 0xc5:
        goto error_operand;
      }
      goto no_error_operand;
    } else
      goto no_error_operand;

error_operand:
    context->flags |= HDE64_F_ERROR | HDE64_F_ERROR_OPERAND;
no_error_operand:

    c = *p++;
    if (m_reg <= 1) {
      if (opcode == 0xf6)
        cflags |= HDE64_C_IMM8;
      else if (opcode == 0xf7)
        cflags |= HDE64_C_IMM_P66;
    }

    switch (m_mod) {
    case 0:
      if (pref & HDE64_PRE_67) {
        if (m_rm == 6)
          disp_size = 2;
      } else
        if (m_rm == 5)
          disp_size = 4;
      break;
    case 1:
      disp_size = 1;
      break;
    case 2:
      disp_size = 2;
      if (!(pref & HDE64_PRE_67))
        disp_size <<= 1;
    }

    if (m_mod != 3 && m_rm == 4) {
      context->flags |= HDE64_F_SIB;
      p++;
      context->sib = c;
      context->sib_scale = c >> 6;
      context->sib_index = (c & 0x3f) >> 3;
      if ((context->sib_base = c & 7) == 5 && !(m_mod & 1))
        disp_size = 4;
    }

    p--;
    switch (disp_size) {
    case 1:
      context->flags |= HDE64_F_DISP8;
      context->disp.disp8 = *p;
      break;
    case 2:
      context->flags |= HDE64_F_DISP16;
      context->disp.disp16 = *(uint16_t *)p;
      break;
    case 4:
      context->flags |= HDE64_F_DISP32;
      context->disp.disp32 = *(uint32_t *)p;
    }
    p += disp_size;
  } else if (pref & HDE64_PRE_LOCK)
    context->flags |= HDE64_F_ERROR | HDE64_F_ERROR_LOCK;

  if (cflags & HDE64_C_IMM_P66) {
    if (cflags & HDE64_C_REL32) {
      if (pref & HDE64_PRE_66) {
        context->flags |= HDE64_F_IMM16 | HDE64_F_RELATIVE;
        context->imm.imm16 = *(uint16_t *)p;
        p += 2;
        goto disasm_done;
      }
      goto rel32_ok;
    }
    if (op64) {
      context->flags |= HDE64_F_IMM64;
      context->imm.imm64 = *(uint64_t *)p;
      p += 8;
    } else if (!(pref & HDE64_PRE_66)) {
      context->flags |= HDE64_F_IMM32;
      context->imm.imm32 = *(uint32_t *)p;
      p += 4;
    } else
      goto imm16_ok;
  }


  if (cflags & HDE64_C_IMM16) {
imm16_ok:
    context->flags |= HDE64_F_IMM16;
    context->imm.imm16 = *(uint16_t *)p;
    p += 2;
  }
  if (cflags & HDE64_C_IMM8) {
    context->flags |= HDE64_F_IMM8;
    context->imm.imm8 = *p++;
  }

  if (cflags & HDE64_C_REL32) {
rel32_ok:
    context->flags |= HDE64_F_IMM32 | HDE64_F_RELATIVE;
    context->imm.imm32 = *(uint32_t *)p;
    p += 4;
  } else if (cflags & HDE64_C_REL8) {
    context->flags |= HDE64_F_IMM8 | HDE64_F_RELATIVE;
    context->imm.imm8 = *p++;
  }

disasm_done:

  if ((context->len = (uint08_t)(p-(uint08_t *)address)) > 15) {
    context->flags |= HDE64_F_ERROR | HDE64_F_ERROR_LENGTH;
    context->len = 15;
  }

  return context->len;
}

///////////////////////////////////////////////////////////////////////////////////////////////////