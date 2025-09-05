#include <cpu.h>
#include <instruction.h> 
#include <bus.h>
#include <emu.h>

extern cpu_context ctx;

void cpu_init() {
    ctx.regs.pc = 0x100;
    ctx.regs.a = 0x01;
}

static void fetch_instruction() {
    printf("Fetching instruction at PC: %04X\n", ctx.regs.pc);  
    ctx.curr_opcode = bus_read(ctx.regs.pc++); //read the instruction at curr address, then increment counter 
    ctx.curr_instr = getInstructionUsingOpcode(ctx.curr_opcode);
}

static void execute_instruction() { 
    IN_PROC proc = inst_get_processor(ctx.curr_instr->type);

    if (!proc) {
        NO_IMPL 
    }

    proc(&ctx);
}

bool cpu_step() {
    if (!ctx.halted) {
        u16 pc = ctx.regs.pc;
        fetch_instruction();
        fetch_data();

        char flags[16];
        sprintf(flags, "%c%c%c%c",
            ctx.regs.f & (1 << 7) ? 'Z' : '-', 
            ctx.regs.f & (1 << 6) ? 'N' : '-',
            ctx.regs.f & (1 << 5) ? 'H' : '-', 
            ctx.regs.f & (1 << 4) ? 'C' : '-'
        );

        printf("Tick - %08lX %04X: %7s (%02X %02X %02X) A: %02X BC: %02X%02X DE: %02X%02X HL: %02X%02X F: %s\n", 
            emu_get_context()->ticks, pc, inst_name(ctx.curr_instr->type), ctx.curr_opcode, 
            bus_read(pc + 1), bus_read(pc + 2), ctx.regs.a, ctx.regs.b, ctx.regs.c, ctx.regs.d, ctx.regs.e, ctx.regs.h, ctx.regs.l, flags);  
  
        
        if (ctx.curr_instr == NULL) {
            printf("Unknown Instruction! %02X\n", ctx.curr_opcode);
            exit(-7);
        }
        
        execute_instruction(); 
    } 
    return true;
}

u8 cpu_get_ie_register() {
    return ctx.ie_register;
}

void cpu_set_ie_register(u8 n) {
    ctx.ie_register = n;
}
