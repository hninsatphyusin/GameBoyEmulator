#include <cpu.h>
#include <bus.h>
#include <emu.h>

cpu_context ctx;

void fetch_data() {
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
            ctx.fetched_data = cpu_read_reg(ctx.curr_instr->reg_1);
            return;

        case AM_R_R:
            ctx.fetched_data = cpu_read_reg(ctx.curr_instr->reg_2);
            return;

        case AM_R_D8:
            ctx.fetched_data = bus_read(ctx.regs.pc);
            emu_cycles(1);
            ctx.regs.pc++;
            return;
        
        case AM_R_D16: 
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

        case AM_MR_R:
            ctx.fetched_data = cpu_read_reg(ctx.curr_instr->reg_2);
            ctx.mem_dest = cpu_read_reg(ctx.curr_instr->reg_1);
            ctx.is_dest_mem = true;

            if (ctx.curr_instr->reg_1 == RT_C) {
                ctx.mem_dest |= 0xFF00;
            }
            return;

        case AM_R_MR: {
            //reading from memory address to register
            u16 addr = cpu_read_reg(ctx.curr_instr->reg_2);

            if (ctx.curr_instr->reg_1 == RT_C) {
                addr |= 0xFF00;
            }
            ctx.fetched_data = bus_read(addr);
            emu_cycles(1);
            return;
        }
        case AM_R_HLI: {
            ctx.fetched_data = bus_read(cpu_read_reg(ctx.curr_instr->reg_2));
            emu_cycles(1);
            cpu_set_reg(RT_HL, cpu_read_reg(RT_HL) + 1);
            return;
        }

        case AM_R_HLD: {
            ctx.fetched_data = bus_read(cpu_read_reg(ctx.curr_instr->reg_2));
            emu_cycles(1);
            cpu_set_reg(RT_HL, cpu_read_reg(RT_HL) - 1);
            return;
        }

        case AM_HLI_R: {
            ctx.fetched_data = cpu_read_reg(ctx.curr_instr->reg_2);
            ctx.fetched_data = cpu_read_reg(ctx.curr_instr->reg_1);
            ctx.is_dest_mem = true;
            cpu_set_reg(RT_HL, cpu_read_reg(RT_HL) + 1);
        }

        case AM_HLD_R: {
            ctx.fetched_data = cpu_read_reg(ctx.curr_instr->reg_2);
            ctx.fetched_data = cpu_read_reg(ctx.curr_instr->reg_1);
            ctx.is_dest_mem = true;
            cpu_set_reg(RT_HL, cpu_read_reg(RT_HL) - 1);
        }

        case AM_R_A8: {
            ctx.fetched_data = bus_read(ctx.regs.pc);
            emu_cycles(1);
            ctx.regs.pc++;
        }

        case AM_A8_R: {
            ctx.mem_dest = bus_read(ctx.regs.pc) | 0xFF00;
            ctx.is_dest_mem = true;
            emu_cycles(1);
            ctx.regs.pc++;
            return;
        }

        case AM_HL_SPR: {
            ctx.fetched_data = bus_read(ctx.regs.pc);
            emu_cycles(1);
            ctx.regs.pc++;
            return;
        }

        case AM_D8: {
            ctx.fetched_data = bus_read(ctx.regs.pc);
            emu_cycles(1);
            ctx.regs.pc++;
            return;
        }

        case AM_A16_R:
        case AM_D16_R: {
            u16 low = bus_read(ctx.regs.pc);
            printf("Fetching D16 at PC: %04X\n", low);
            emu_cycles(1);
            u16 high = bus_read(ctx.regs.pc + 1);
            printf("Fetching D16 at PC: %04X\n", high);
            emu_cycles(1);
            ctx.mem_dest = (high << 8) | low;
            ctx.is_dest_mem = true;

            
            ctx.regs.pc += 2;
            ctx.fetched_data = cpu_read_reg(ctx.curr_instr->reg_2);
            return;
        }

        case AM_MR_D8: {
            ctx.fetched_data = bus_read(ctx.regs.pc);
            emu_cycles(1);
            ctx.regs.pc++;
            ctx.mem_dest = cpu_read_reg(ctx.curr_instr->reg_1);
            ctx.is_dest_mem = true;
            return;
        }

        case AM_MR: {
            ctx.mem_dest = cpu_read_reg(ctx.curr_instr->reg_1);
            ctx.is_dest_mem = true;
            ctx.fetched_data = bus_read(cpu_read_reg(ctx.curr_instr->reg_1));
            emu_cycles(1);
            return;
        }

        case AM_R_A16: {
            u16 low = bus_read(ctx.regs.pc);
            printf("Fetching D16 at PC: %04X\n", low);
            emu_cycles(1);
            u16 high = bus_read(ctx.regs.pc + 1);
            printf("Fetching D16 at PC: %04X\n", high);
            emu_cycles(1);
            u16 addr = (high << 8) | low;
            
            ctx.regs.pc += 2;

            ctx.fetched_data = bus_read(addr);
            emu_cycles(1);
            return;
        }

        default:
            printf("Unknown addressing mode: %d\n", ctx.curr_instr->mode);
            ctx.regs.pc++;
            return;
    }
}