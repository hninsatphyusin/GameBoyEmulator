#pragma once

#include <common.h>

typedef struct {
    u8 entry_point[4];
    u8 logo[0x30];
    char title[16];
    u16 licensee_code;
    u8 sgb_flag;
    u8 cartridge_type;
    u8 rom_size;
    u8 ram_size;
    u8 destination_code;
    u8 old_licensee_code;
    u8 mask_rom_version_number;
    u8 header_checksum;
    u16 global_checksum;
} rom_header; 


bool load_cart(char *cart_filename);