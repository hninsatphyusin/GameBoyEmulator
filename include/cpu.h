#pragma once

#include <common.h>
#include <instructions.h>
/* 
cpu structure: 
AF - Accumulator & Flags
BC - BC 
DE - DE
HL - HL 
SP - Stack Pointer
PC - Program Counter
*/
typedef struct {
    u8 a; 
    u8 f;
    u8 b; 
    u8 c; 
    u8 d; 
    u8 e; 
    u8 h; 
    u8 l; 
    u16 sp;
    u16 pc;

} cpu_registers;

typedef struct {
    cpu_registers regs;
    u8 curr_opcode; 
    u16 fetched_data;
    u16 mem_dest;
    bool is_dest_mem;
    instruction* curr_instr;
    bool halted; 
    bool stepping; //for debugging purpose
    bool int_master_enabled;
    u8 ie_register;
    bool enabling_ime;
    u8 int_flags;
} cpu_context;

void cpu_init();
bool cpu_step();

u16 cpu_read_reg(reg_type r);
u16 reverse(u16 val);

typedef void (*IN_PROC)(cpu_context *);
IN_PROC inst_get_processor(in_type type);

#define CPU_FLAG_Z BIT(ctx->regs.f, 7)
#define CPU_FLAG_N BIT(ctx->regs.f, 6)
#define CPU_FLAG_H BIT(ctx->regs.f, 5)
#define CPU_FLAG_C BIT(ctx->regs.f, 4)


void cpu_set_reg(reg_type rt, u16 val);
void fetch_data();

u8 cpu_get_ie_register();
void cpu_set_ie_register(u8 n);

cpu_registers *cpu_get_regs();

void cpu_set_reg8(reg_type r, u8 val);
u8 cpu_read_reg8(reg_type r);


u8 cpu_get_int_flags();
void cpu_set_int_flags(u8 value);