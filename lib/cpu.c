#include <cpu.h>
#include <instruction.h> 
#include <bus.h>
#include <emu.h>

cpu_context ctx;

void cpu_init() {
    ctx.regs.pc = 0x100;
    ctx.regs.a = 0x01;
}

static void fetch_instruction() {
    printf("Fetching instruction at PC: %04X\n", ctx.regs.pc);  
    ctx.curr_opcode = bus_read(ctx.regs.pc++); //read the instruction at curr address, then increment counter 
    ctx.curr_instr = getInstructionUsingOpcode(ctx.curr_opcode);
}

static void fetch_data() {
    //When register points to an address then u need to fetch the data from the data before doing some operation
    ctx.mem_dest = 0;
    ctx.is_dest_mem = false;

    if (ctx.curr_instr == NULL) {
        return;
    }

    //for different addressing modes
    switch (ctx.curr_instr->mode) {
        case AM_IMP: return;

        case AM_R:
            ctx.fetched_data = read_reg(ctx.curr_instr->reg_1);
            return;

        case AM_R_D8:
            ctx.fetched_data = bus_read(ctx.regs.pc);
            emu_cycles(1);
            ctx.regs.pc++;
            return;

        case AM_D16: {
            u16 low = bus_read(ctx.regs.pc);
            printf("Fetching D16 at PC: %04X\n", low);
            emu_cycles(1);
            u16 high = bus_read(ctx.regs.pc + 1);
            printf("Fetching D16 at PC: %04X\n", high);
            emu_cycles(1);
            ctx.fetched_data = (high << 8) | low;
            
            ctx.regs.pc += 2;
            return;
        }

        default:
            printf("Unknown addressing mode: %d\n", ctx.curr_instr->mode);
            ctx.regs.pc++;
            return;
    }

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

        printf("%04X: %7s (%02X %02X %02X) A: %02X B: %02X C: %02X\n", 
            pc, inst_name(ctx.curr_instr->type), ctx.curr_opcode, 
        bus_read(pc + 1), bus_read(pc + 2), ctx.regs.a, ctx.regs.b, ctx.regs.c);  
  
        
        if (ctx.curr_instr == NULL) {
            printf("Unknown Instruction! %02X\n", ctx.curr_opcode);
            exit(-7);
        }
        
        execute_instruction(); 
    } 
    return true;
}
