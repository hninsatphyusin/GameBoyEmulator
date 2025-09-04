#include <cpu.h>
#include <instruction.h>

extern cpu_context ctx;

u16 reverse(u16 val) {
    //swap the first byte and the second byte 
    return (val & 0xFF00) >> 8 | (val & 0x00FF) << 8;
}

u16 read_reg(reg_type r) {
    switch (r) {
        case RT_A: return ctx.regs.a;
        case RT_F: return ctx.regs.f;
        case RT_B: return ctx.regs.b;
        case RT_C: return ctx.regs.c;
        case RT_D: return ctx.regs.d;
        case RT_E: return ctx.regs.e;
        case RT_H: return ctx.regs.h;
        case RT_L: return ctx.regs.l;
        case RT_AF: return (ctx.regs.a << 8) | ctx.regs.f;
        case RT_BC: return (ctx.regs.b << 8) | ctx.regs.c;
        case RT_DE: return (ctx.regs.d << 8) | ctx.regs.e;
        case RT_HL: return (ctx.regs.h << 8) | ctx.regs.l;
        case RT_SP: return ctx.regs.sp;
        case RT_PC: return ctx.regs.pc;
        default:
            printf("Unknown register type %d\n", r);
            exit(-6);
            return 0;
    }   
}

char *inst_name(in_type t) {
    return inst_lookup[t];
}