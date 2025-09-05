#include <timer.h>
#include <interrupts.h>

static timer_context ctx = {0};

void timer_init() {
    ctx.div = 0xAC00; //initial values
}

void timer_tick() {
    u16 prev_div = ctx.div;

    ctx.div++;

    bool timer_update = false;

    switch(ctx.tac & (0b11)) {
        case 0b00:
            timer_update = (prev_div & (1 << 9)) && (!(ctx.div & (1 << 9))); //divide by 1024
            break;
        case 0b01:
            timer_update = (prev_div & (1 << 3)) && (!(ctx.div & (1 << 3))); //divide by 16
            break;
        case 0b10:
            timer_update = (prev_div & (1 << 5)) && (!(ctx.div & (1 << 5))); //divide by 64
            break;
        case 0b11:
            timer_update = (prev_div & (1 << 7)) && (!(ctx.div & (1 << 7))); //divide by 256
            break;
        default:
            break;
    }

    if (timer_update && ctx.tac & (1 <<2)) {
        ctx.tima++;

        if (ctx.tima == 0xFF) {
            ctx.tima = ctx.tma;

            cpu_request_interrupt(IT_TIMER);
        }
    }
}


void timer_write(u16 address, u8 value) {
    switch(address) {
        case 0xFF04: //DIV
            ctx.div = 0;
            break;
        case 0xFF05: //TIMA
            ctx.tima = value;
            break;
        case 0xFF06: //TMA
            ctx.tma = value;
            break;
        case 0xFF07: //TAC
            ctx.tac = value & 0b111; //only the first three bits are used
            break;
        default:
            printf("UNSUPPORTED timer_write(%04X, %02X)\n", address, value);
            break;  
    }
}

u8 timer_read(u16 address) {
    switch(address) {
        case 0xFF04: //DIV
            return (ctx.div >> 8); //we only care about the top byte;
        case 0xFF05: //TIMA
            return ctx.tima;
        case 0xFF06: //TMA
            return ctx.tma;
        case 0xFF07: //TAC
            return ctx.tac; //the first three bits are used, the rest are always 1
        default:
            printf("UNSUPPORTED timer_read(%04X)\n", address);
            return 0;  
    }   
}

timer_context *timer_get_context() {
    return &ctx;
}