#include <common.h>
#include <cart.h>

//The Game Boy has a 16-bit address bus, which is used to address ROM, RAM, and I/O.

u8 bus_read(u16 address) {
    if (address < 0x8000) {
        //0000-7FFF: ROM
        return cart_read(address);
    }
    NO_IMPL
}
void bus_write(u16 address, u8 value) {
    if (address < 0x8000) {
        //0000-7FFF: ROM
        cart_write(address, value);
        return;
    }
    NO_IMPL
}
