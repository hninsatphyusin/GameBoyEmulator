#include <cpu.h>
#include <emu.h>
#include <bus.h>
#include <instructions.h>
#include <stack.h>


void cpu_set_flags(cpu_context *ctx, int8_t z, int8_t n, int8_t h, int8_t c) {
    if (z != -1) { BIT_SET(ctx->regs.f, 7, z); }
    if (n != -1) { BIT_SET(ctx->regs.f, 6, n); }
    if (h != -1) { BIT_SET(ctx->regs.f, 5, h); }
    if (c != -1) { BIT_SET(ctx->regs.f, 4, c); }
}

static void proc_none(cpu_context *ctx) {
    printf("INVALID Instruction!\n");
    exit(-7);
}

static void proc_nop(cpu_context *ctx) {
    //printf("Process Nope not implemented yet\n");
}

reg_type rt_lookup[] = {
    RT_B, 
    RT_C, 
    RT_D, 
    RT_E, 
    RT_H, 
    RT_L, 
    RT_HL,
    RT_A
};

static bool is16Bit(reg_type rt) {
    return rt >= RT_AF;
}

static void proc_ld(cpu_context *ctx) {
    if (ctx->is_dest_mem) {
        //for example LD (BC), A
        if (is16Bit(ctx->curr_instr->reg_2)) {
            emu_cycles(1);
            bus_write16(ctx->mem_dest, ctx->fetched_data);

        } else {
            bus_write(ctx->mem_dest, ctx->fetched_data);
        }
        return;
    }

    if (ctx->curr_instr->mode == AM_HL_SPR) {
        u8 hflag = (cpu_read_reg(ctx->curr_instr->reg_2) & 0xF) + (ctx->fetched_data & 0xF) >= 0x10;
        u8 cflag = (cpu_read_reg(ctx->curr_instr->reg_2) & 0xFF) + (ctx->fetched_data & 0xFF) >= 0x100;

        cpu_set_flags(ctx, 0, 0, hflag, cflag);
        cpu_set_reg(ctx->curr_instr->reg_1, cpu_read_reg(ctx->curr_instr->reg_2) + (char)ctx->fetched_data);
        return;
    }
    //main case is to set a register to a value 
    cpu_set_reg(ctx->curr_instr->reg_1, ctx->fetched_data);
}





reg_type decode_reg(u8 reg) {
    if (reg > 0b111) {
        return RT_NONE;
    }
    return rt_lookup[reg];
}


static void proc_cb(cpu_context *ctx) {
    u8 op = ctx->fetched_data;
    reg_type reg = decode_reg(op & 0b111); //last 3 bits contain the register
    u8 bit = (op >> 3) & 0b111; //next 3 bits contain the bit number
    u8 bit_op = (op >> 6) & 0b11; //first 2 bits contain the operation
    u8 reg_val = cpu_read_reg8(reg);

    emu_cycles(1);

    if (reg == RT_HL) {
        emu_cycles(2);
    }

    switch(bit_op) {
        case 1:
            //BIT
            cpu_set_flags(ctx, !(reg_val & (1 << bit)), 0, 1, -1);
            return;

        case 2:
            //RST
            reg_val &= ~(1 << bit);
            cpu_set_reg8(reg, reg_val);
            return;

        case 3:
            //SET
            reg_val |= (1 << bit);
            cpu_set_reg8(reg, reg_val);
            return;
    }

    //if the operation is 0, then its one of the rotate/shift oeprations
    bool flagC = CPU_FLAG_C;

    switch(bit) {
        case 0: {
            //RLC - Rotate Left Circular
            bool setC = false;
            u8 result = (reg_val << 1) & 0xFF;

            if ((reg_val & (1 << 7)) != 0) {
                result |= 1; // Set carry flag if bit 7 was set
                setC = true;
            }

            cpu_set_reg8(reg, result);
            cpu_set_flags(ctx, result == 0, 0, 0, setC);
        } return;

        case 1: {
            // RRC - Rotate Right Circular
            u8 old = reg_val;
            reg_val >>= 1;
            reg_val |= (old << 7);

            cpu_set_reg8(reg, reg_val);
            cpu_set_flags(ctx, !reg_val, 0, 0, old & 1);
        } return;

        case 2: {
            //RL - Rotate Left 
            u8 old = reg_val;
            reg_val <<= 1; //same thing as reg_val = reg_val << 1;
            reg_val |= flagC;

            cpu_set_reg8(reg, reg_val);
            cpu_set_flags(ctx, !reg_val, 0, 0, !!(old & 0x80));
        } return;

        case 3: {
            //RR
            u8 old = reg_val;
            reg_val >>= 1;

            reg_val |= (flagC << 7);

            cpu_set_reg8(reg, reg_val);
            cpu_set_flags(ctx, !reg_val, 0, 0, old & 1);
        } return;

        case 4: {
            //SLA
            u8 old = reg_val;
            reg_val <<= 1;

            cpu_set_reg8(reg, reg_val);
            cpu_set_flags(ctx, !reg_val, 0, 0, !!(old & 0x80));
        } return;

        case 5: {
            //SRA
            u8 u = (int8_t)reg_val >> 1;
            cpu_set_reg8(reg, u);
            cpu_set_flags(ctx, !u, 0, 0, reg_val & 1);
        } return;

        case 6: {
            //SWAP
            reg_val = ((reg_val & 0xF0) >> 4) | ((reg_val & 0xF) << 4);
            cpu_set_reg8(reg, reg_val);
            cpu_set_flags(ctx, reg_val == 0, 0, 0, 0);
        } return;

        case 7: {
            //SRL
            u8 u = reg_val >> 1;
            cpu_set_reg8(reg, u);
            cpu_set_flags(ctx, !u, 0, 0, reg_val & 1);
        } return;
    }

    fprintf(stderr, "ERROR: INVALID CB: %02X", op);
    NO_IMPL
}

static void proc_rlca(cpu_context *ctx) {
    u8 u = ctx->regs.a;
    bool c = (u >> 7) & 1;
    u = (u << 1) | c;
    ctx->regs.a = u;

    cpu_set_flags(ctx, 0, 0, 0, c);
}

static void proc_rrca(cpu_context *ctx) {
    u8 b = ctx->regs.a & 1;
    ctx->regs.a >>= 1;
    ctx->regs.a |= (b << 7);

    cpu_set_flags(ctx, 0, 0, 0, b);
}


static void proc_rla(cpu_context *ctx) {
    u8 u = ctx->regs.a;
    u8 cf = CPU_FLAG_C;
    u8 c = (u >> 7) & 1;

    ctx->regs.a = (u << 1) | cf;
    cpu_set_flags(ctx, 0, 0, 0, c);
}

static void proc_stop(cpu_context *ctx) {
    fprintf(stderr, "STOPPING!\n");
    NO_IMPL
}

static void proc_daa(cpu_context *ctx) {
    u8 u = 0;
    int fc = 0;

    if (CPU_FLAG_H || (!CPU_FLAG_N && (ctx->regs.a & 0xF) > 9)) {
        u = 6;
    }

    if (CPU_FLAG_C || (!CPU_FLAG_N && ctx->regs.a > 0x99)) {
        u |= 0x60;
        fc = 1;
    }

    ctx->regs.a += CPU_FLAG_N ? -u : u;

    cpu_set_flags(ctx, ctx->regs.a == 0, -1, 0, fc);
}

static void proc_cpl(cpu_context *ctx) {
    ctx->regs.a = ~ctx->regs.a;
    cpu_set_flags(ctx, -1, 1, 1, -1);
}

static void proc_scf(cpu_context *ctx) {
    cpu_set_flags(ctx, -1, 0, 0, 1);
}

static void proc_ccf(cpu_context *ctx) {
    cpu_set_flags(ctx, -1, 0, 0, CPU_FLAG_C ^ 1);
}

static void proc_halt(cpu_context *ctx) {
    ctx->halted = true;
}

static void proc_rra(cpu_context *ctx) {
    u8 carry = CPU_FLAG_C;
    u8 new_c = ctx->regs.a & 1;

    ctx->regs.a >>= 1;
    ctx->regs.a |= (carry << 7);

    cpu_set_flags(ctx, 0, 0, 0, new_c);
}

static void proc_and(cpu_context *ctx) {
    ctx->regs.a &= ctx->fetched_data;
    cpu_set_flags(ctx, ctx->regs.a == 0, 0, 1, 0);
}

static void proc_xor(cpu_context *ctx) {
    ctx->regs.a ^= ctx->fetched_data & 0xFF;
    cpu_set_flags(ctx, ctx->regs.a == 0, 0, 0, 0);
}

static void proc_or(cpu_context *ctx) {
    ctx->regs.a |= ctx->fetched_data & 0xFF;
    cpu_set_flags(ctx, ctx->regs.a == 0, 0, 0, 0);
}

static void proc_cp(cpu_context *ctx) {
    int n = (int)ctx->regs.a - (int)ctx->fetched_data;
    int h = ((int)ctx->regs.a&0x0F) - ((int)ctx->fetched_data&0x0F) < 0;
    int c = n < 0;

    cpu_set_flags(ctx, n == 0, 1, h, c);
}

static bool check_cond(cpu_context *ctx) {
    bool z = CPU_FLAG_Z;
    bool c = CPU_FLAG_C;

    switch(ctx->curr_instr->cond) {
        case CT_NONE: return true;
        case CT_C: return c; 
        case CT_NC: return !c;
        case CT_Z: return z;
        case CT_NZ: return !z;
    }
    return false;
}

static void goto_addr(cpu_context *ctx, u16 addr, bool pushpc) {
    if (check_cond(ctx)) {
        if (pushpc) {
            emu_cycles(2);
            stack_push16(ctx->regs.pc);
        }
        ctx->regs.pc = addr;
        emu_cycles(1);
    }
}

static void proc_jp(cpu_context *ctx) {
    goto_addr(ctx, ctx->fetched_data, false);
}

static void proc_ret(cpu_context *ctx) {
    if (ctx->curr_instr->cond != CT_NONE) {
        emu_cycles(1);
    }

    if (check_cond(ctx)) {
        u16 lo = stack_pop();
        emu_cycles(1);
        u16 hi = stack_pop();
        emu_cycles(1);

        u16 value = (hi << 8) | lo;
        ctx->regs.pc = value;

        emu_cycles(1);
    }
}

static void proc_reti(cpu_context *ctx) {
    ctx->int_master_enabled = true;
    proc_ret(ctx);
}


static void proc_call(cpu_context *ctx) {
    goto_addr(ctx, ctx->fetched_data, true);
}

static void proc_rst(cpu_context *ctx) {
    goto_addr(ctx, ctx->curr_instr->param, true);
}

static void proc_jr(cpu_context *ctx) {
    char offset = (char)(ctx->fetched_data & 0xFF);
    u16 addr = ctx->regs.pc + offset;
    goto_addr(ctx, addr, false);
}

static void proc_di(cpu_context *ctx) {
    ctx->int_master_enabled = false;
}

static void proc_ei(cpu_context *ctx) {
    ctx->enabling_ime = true;
}


static void proc_ldh(cpu_context *ctx) {
    //used to transfer data between register A and high ram
    if (ctx->curr_instr->reg_1 == RT_A) {
        //its to load the value from memory address into register A
        cpu_set_reg(ctx->curr_instr->reg_1, bus_read(0xFF00 | ctx->fetched_data));
    } else {
        //stores the value of register A into memory address (the memory addres is 0xFF00 + addr)
        bus_write(ctx->mem_dest, ctx->regs.a);
    }
    emu_cycles(1);
}

static void proc_pop(cpu_context *ctx) {
    u16 lo = stack_pop();
    emu_cycles(1);
    u16 hi = stack_pop();
    emu_cycles(1);

    u16 value = (hi << 8) | lo;

    cpu_set_reg(ctx->curr_instr->reg_1, value);

    if (ctx->curr_instr->reg_1 == RT_AF) {
        cpu_set_reg(ctx->curr_instr->reg_1, value & 0xFFF0); //lower nibble of F is always 0
    }
}

static void proc_push(cpu_context *ctx) {
    u16 hi = (cpu_read_reg(ctx->curr_instr->reg_1) >> 8) & 0xFF;
    emu_cycles(1);
    stack_push(hi);

    u16 lo = cpu_read_reg(ctx->curr_instr->reg_1) & 0xFF;
    emu_cycles(1);
    stack_push(lo);
    emu_cycles(1);
}

static void proc_inc(cpu_context *ctx) {
    u16 val = cpu_read_reg(ctx->curr_instr->reg_1) + 1;

    if (is16Bit(ctx->curr_instr->reg_1)) {
        emu_cycles(1);
    }   

    if (ctx->curr_instr->reg_1 == RT_HL && ctx->curr_instr->mode == AM_MR) { //if the value is a memory address
        val = bus_read(cpu_read_reg(RT_HL)) + 1;
        val &= 0xFF;
        bus_write(cpu_read_reg(RT_HL), val);
    } else { //normal register
        cpu_set_reg(ctx->curr_instr->reg_1, val);
        val = cpu_read_reg(ctx->curr_instr->reg_1);
    }

    if ((ctx->curr_opcode & 0x03) == 0x03) {
        return; //no flags affected
    }
    //set the z, h and c flags
    cpu_set_flags(ctx, val == 0, 0, (val & 0x0F) == 0, -1); 
}

static void proc_dec(cpu_context *ctx) {
    u16 val = cpu_read_reg(ctx->curr_instr->reg_1) - 1;

    if (is16Bit(ctx->curr_instr->reg_1)) {
        emu_cycles(1);
    }   

    if (ctx->curr_instr->reg_1 == RT_HL && ctx->curr_instr->mode == AM_MR) { //if the value is a memory address
        val = bus_read(cpu_read_reg(RT_HL)) - 1;
        val &= 0xFF;
        bus_write(cpu_read_reg(RT_HL), val);
    } else { //normal register
        cpu_set_reg(ctx->curr_instr->reg_1, val);
        val = cpu_read_reg(ctx->curr_instr->reg_1);
    }

    if ((ctx->curr_opcode & 0x0B) == 0x0B) {
        return; //no flags affected
    }
    //set the z, h and c flags
    cpu_set_flags(ctx, val == 0, 1, (val & 0x0F) == 0x0F, -1); 
}

static void proc_add(cpu_context *ctx) {
    u32 val = cpu_read_reg(ctx->curr_instr->reg_1) + ctx->fetched_data;

    bool is_16bit = is16Bit(ctx->curr_instr->reg_1);

    if (is_16bit) {
        emu_cycles(1);
    }

    if (ctx->curr_instr->reg_1 == RT_SP) {
        val = cpu_read_reg(ctx->curr_instr->reg_1) + (char)ctx->fetched_data;
    }

    int z = (val & 0xFF) == 0;
    int h = (cpu_read_reg(ctx->curr_instr->reg_1) & 0xF) + (ctx->fetched_data & 0xF) >= 0x10;
    int c = (int)(cpu_read_reg(ctx->curr_instr->reg_1) & 0xFF) + (int)(ctx->fetched_data & 0xFF) >= 0x100;

    if (is_16bit) {
        z = -1;
        h = (cpu_read_reg(ctx->curr_instr->reg_1) & 0xFFF) + (ctx->fetched_data & 0xFFF) >= 0x1000;
        u32 n = ((u32)cpu_read_reg(ctx->curr_instr->reg_1)) + ((u32)ctx->fetched_data);
        c = n >= 0x10000;
    }

    if (ctx->curr_instr->reg_1 == RT_SP) {
        z = 0;
        h = (cpu_read_reg(ctx->curr_instr->reg_1) & 0xF) + (ctx->fetched_data & 0xF) >= 0x10;
        c = (int)(cpu_read_reg(ctx->curr_instr->reg_1) & 0xFF) + (int)(ctx->fetched_data & 0xFF) >= 0x100;
    }

    cpu_set_reg(ctx->curr_instr->reg_1, val & 0xFFFF);
    cpu_set_flags(ctx, z, 0, h, c);
}


static void proc_adc(cpu_context *ctx) {
    u16 u = ctx->fetched_data;
    u16 a = ctx->regs.a;
    u16 c = CPU_FLAG_C;

    ctx->regs.a = (a + u + c) & 0xFF;

    cpu_set_flags(ctx, ctx->regs.a == 0, 0, (a & 0xF) + (u & 0xF) + c > 0xF, a + u + c > 0xFF);
}

static void proc_sub(cpu_context *ctx) {
    u16 val = cpu_read_reg(ctx->curr_instr->reg_1) - ctx->fetched_data;

    int z = val == 0;
    int h = ((int)cpu_read_reg(ctx->curr_instr->reg_1) & 0xF) - ((int)ctx->fetched_data & 0xF) < 0;
    int c = ((int)cpu_read_reg(ctx->curr_instr->reg_1)) - ((int)ctx->fetched_data) < 0;

    cpu_set_reg(ctx->curr_instr->reg_1, val);
    cpu_set_flags(ctx, z, 1, h, c);
}

static void proc_sbc(cpu_context *ctx) {
    u8 val = ctx->fetched_data + CPU_FLAG_C;

    int z = cpu_read_reg(ctx->curr_instr->reg_1) - val == 0;

    int h = ((int)cpu_read_reg(ctx->curr_instr->reg_1) & 0xF) 
        - ((int)ctx->fetched_data & 0xF) - ((int)CPU_FLAG_C) < 0;
    int c = ((int)cpu_read_reg(ctx->curr_instr->reg_1)) 
        - ((int)ctx->fetched_data) - ((int)CPU_FLAG_C) < 0;

    cpu_set_reg(ctx->curr_instr->reg_1, cpu_read_reg(ctx->curr_instr->reg_1) - val);
    cpu_set_flags(ctx, z, 1, h, c);
}


static IN_PROC processors[] = {
    [IN_NONE] = proc_none, //IN_NONE
    [IN_NOP] = proc_nop,
    [IN_LD] = proc_ld, //load instruction
    [IN_JP] = proc_jp,
    [IN_DI] = proc_di, //disable interrupts
    [IN_XOR] = proc_xor, 
    [IN_LDH] = proc_ldh, 
    [IN_POP] = proc_pop, 
    [IN_PUSH] = proc_push, 
    [IN_JR] = proc_jr, 
    [IN_CALL] = proc_call, 
    [IN_RET] = proc_ret, 
    [IN_RETI] = proc_reti,  
    [IN_INC] = proc_inc,
    [IN_DEC] = proc_dec, 
    [IN_ADD] = proc_add, 
    [IN_ADC] = proc_adc, 
    [IN_SUB] = proc_sub,
    [IN_SBC] = proc_sbc, 
    [IN_AND] = proc_and,
    [IN_OR] = proc_or,
    [IN_CP] = proc_cp, 
    [IN_CB] = proc_cb,
    [IN_RLCA] = proc_rlca,
    [IN_RRCA] = proc_rrca,
    [IN_RLA] = proc_rla,
    [IN_STOP] = proc_stop,
    [IN_DAA] = proc_daa,
    [IN_CPL] = proc_cpl,
    [IN_SCF] = proc_scf,
    [IN_CCF] = proc_ccf,
    [IN_HALT] = proc_halt,
    [IN_RRA] = proc_rra, 
    [IN_EI] = proc_ei, 
    [IN_RST] = proc_rst
};


IN_PROC inst_get_processor(in_type type) {
    return processors[type];
}