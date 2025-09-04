#include <common.h>
#include <cart.h>

//The Game Boy has a 16-bit address bus, which is used to address ROM, RAM, and I/O.

u8 bus_read(u16 address) {
    if (address < 0x8000) {
        //0000-7FFF: ROM
        return cart_read(address);
    }
    printf("UNSUPPORTED Bus read (%04X)\n", address);
}
void bus_write(u16 address, u8 value) {
    if (address < 0x8000) {
        //0000-7FFF: ROM
        return cart_write(address, value);
    }
    printf("UNSUPPORTED Bus write (%04X)\n", address);
}
u16 bus_read16(u16 address) {
    u16 lo = bus_read(address);
    u16 hi = bus_read(address + 1);
    return lo | (hi << 8);
}

void bus_write16(u16 address, u16 value) {
    bus_write(address + 1, (value >> 8) & 0xFF);
    bus_write(address, value & 0xFF);
}