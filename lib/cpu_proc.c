#include <cpu.h>
#include <emu.h>
#include <instruction.h>

static void proc_none(cpu_context *ctx) {
    printf("INVALID Instruction!\n");
    exit(-7);
}

static void proc_ld(cpu_context *ctx) {
    printf("Process Load not implemented yet\n");
}

static void proc_nop(cpu_context *ctx) {
    printf("Process Nope not implemented yet\n");
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

static void proc_jp(cpu_context *ctx) {
    if (check_cond(ctx)) {
        ctx->regs.pc = ctx->fetched_data;
        printf("Jumping to %04X\n", ctx->fetched_data);
        emu_cycles(1);
    }
}

static void proc_di(cpu_context *ctx) {
    ctx->int_master_enabled = false;
}

void cpu_set_flags(cpu_context *ctx, bool z, bool n, bool h, bool c) {
    if (z != -1) { BIT_SET(ctx->regs.f, 7, z); }
    if (n != -1) { BIT_SET(ctx->regs.f, 6, n); }
    if (h != -1) { BIT_SET(ctx->regs.f, 5, h); }
    if (c != -1) { BIT_SET(ctx->regs.f, 4, c); }
}

static void proc_xor(cpu_context *ctx) {
    ctx->regs.a ^= ctx->fetched_data & 0xFF;
    cpu_set_flags(ctx, ctx->regs.a, 0, 0, 0);
}


static IN_PROC processors[] = {
    [IN_NONE] = proc_none, //IN_NONE
    [IN_NOP] = proc_nop,
    [IN_LD] = proc_ld, //load instruction
    [IN_JP] = proc_jp,
    [IN_DI] = proc_di, //disable interrupts
    [IN_XOR] = proc_xor
};

IN_PROC inst_get_processor(in_type type) {
    return processors[type];
}