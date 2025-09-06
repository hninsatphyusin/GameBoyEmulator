#include <common.h>
#include <cart.h>
#include <ram.h>
#include <cpu.h>
#include <io.h>
#include <ppu.h>
#include <dma.h>

//The Game Boy has a 16-bit address bus, which is used to address ROM, RAM, and I/O.

u8 bus_read(u16 address) {
    if (address < 0x8000) {
        //0000-7FFF: ROM
        return cart_read(address);
    } else if (address < 0xA000) {
        //Char/Map data
        return ppu_vram_read(address);
    } else if (address < 0xC000) {
        //Cartridge RAM 
        return cart_read(address);
    } else if (address < 0xE000) {
        //Ram bank 0-7, working ram 
        return wram_read(address);
    } else if (address < 0xFE00) {
        //reserved echo ram 
        return 0;
    } else if (address < 0xFEA0) {
        //object attribute memory (OAM) - sprite info etc
        if (dma_transferring()) {
            //during DMA transfer, reading OAM returns 0xFF
            return 0xFF;
        }
        return ppu_oam_read(address);
    } else if (address < 0xFF00) {
        //reserved unusable
        return 0;
    } else if (address < 0xFF80) {
        //i/o memory
        return io_read(address);
    } else if (address == 0xFFFF) {
        return cpu_get_ie_register();
    }

    //if memory register > 0xFFFF
    return hram_read(address); //read high ram, no IMPL tho
    
}
void bus_write(u16 address, u8 value) {
    if (address < 0x8000) {
        //0000-7FFF: ROM
        cart_write(address, value);
    } else if (address < 0xA000) {
        //Char/Map data
        ppu_vram_write(address, value);
    } else if (address < 0xC000) {
        //Cartridge RAM 
        cart_write(address, value);
    } else if (address < 0xE000) {
        //Ram bank 0-7, working ram 
        wram_write(address, value);
    } else if (address < 0xFE00) {
        //reserved echo ram 
        
    } else if (address < 0xFEA0) {
        //object attribute memory (OAM) - sprite info etc
        if (dma_transferring()) {
            return; //during DMA transfer, writes to OAM are ignored
        }
        
        ppu_oam_write(address, value);
        //NO_IMPL
    } else if (address < 0xFF00) {
        //reserved unusable
        
    } else if (address < 0xFF80) {
        //i/o memory
        return io_write(address, value);
        //NO_IMPL
    } else if (address == 0xFFFF) { //cpu set enable register
        cpu_set_ie_register(value);
    } else {
        hram_write(address, value);
    }
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