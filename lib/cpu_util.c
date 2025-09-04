#include <cpu.h>
#include <instruction.h>

cpu_context ctx;

u16 reverse(u16 val) {
    //swap the first byte and the second byte 
    return (val & 0xFF00) >> 8 | (val & 0x00FF) << 8;
}

u16 cpu_read_reg(reg_type r) {
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

void cpu_set_reg(reg_type rt, u16 val) {
    switch (rt) {
        case RT_A: ctx.regs.a = val & 0xFF; break;
        case RT_F: ctx.regs.f = val & 0xFF; break; 
        case RT_B: ctx.regs.b = val & 0xFF; break; 
        case RT_C: ctx.regs.c = val & 0xFF; break; 
        case RT_D: ctx.regs.d = val & 0xFF; break; 
        case RT_E: ctx.regs.e = val & 0xFF; break; 
        case RT_H: ctx.regs.h = val & 0xFF; break; 
        case RT_L: ctx.regs.l = val & 0xFF; break;
        case RT_AF: *((u16 *)&ctx.regs.a) = reverse(val); break;
        case RT_BC: *((u16 *)&ctx.regs.b) = reverse(val); break;
        case RT_DE: *((u16 *)&ctx.regs.d) = reverse(val); break;
        case RT_HL: {
            *((u16 *)&ctx.regs.h) = reverse(val);
            break;
        }   
        case RT_PC: ctx.regs.pc = val; break;
        case RT_SP: ctx.regs.sp = val; break;
        default:
            printf("Unknown register type %d\n", rt);
            exit(-6);
            break;            
    }
}