#include <cart.h>

typedef struct {
    char filename[1024];
    u32 rom_size;
    u8 *rom_data;
    rom_header *header;
} cart_context;


static const char *CARTRIDGE_TYPES[] = {
    "ROM ONLY",
    "MBC1",
    "MBC1+RAM",
    "MBC1+RAM+BATTERY",
    "0x04 ???",
    "MBC2",
    "MBC2+BATTERY",
    "0x07 ???",
    "ROM+RAM 1",
    "ROM+RAM+BATTERY 1",
    "0x0A ???",
    "MMM01",
    "MMM01+RAM",
    "MMM01+RAM+BATTERY",
    "0x0E ???",
    "MBC3+TIMER+BATTERY",
    "MBC3+TIMER+RAM+BATTERY 2",
    "MBC3",
    "MBC3+RAM 2",
    "MBC3+RAM+BATTERY 2",
    "0x14 ???",
    "0x15 ???",
    "0x16 ???",
    "0x17 ???",
    "0x18 ???",
    "MBC5",
    "MBC5+RAM",
    "MBC5+RAM+BATTERY",
    "MBC5+RUMBLE",
    "MBC5+RUMBLE+RAM",
    "MBC5+RUMBLE+RAM+BATTERY",
    "0x1F ???",
    "MBC6",
    "0x21 ???",
    "MBC7+SENSOR+RUMBLE+RAM+BATTERY",
};

static const char *LICENSEE_CODE[0xA5] = {
    [0x00] = "None",
    [0x01] = "Nintendo R&D1",
    [0x08] = "Capcom",
    [0x13] = "Electronic Arts",
    [0x18] = "Hudson Soft",
    [0x19] = "b-ai",
    [0x20] = "kss",
    [0x22] = "pow",
    [0x24] = "PCM Complete",
    [0x25] = "san-x",
    [0x28] = "Kemco Japan",
    [0x29] = "seta",
    [0x30] = "Viacom",
    [0x31] = "Nintendo",
    [0x32] = "Bandai",
    [0x33] = "Ocean/Acclaim",
    [0x34] = "Konami",
    [0x35] = "Hector",
    [0x37] = "Taito",
    [0x38] = "Hudson",
    [0x39] = "Banpresto",
    [0x41] = "Ubi Soft",
    [0x42] = "Atlus",
    [0x44] = "Malibu",
    [0x46] = "angel",
    [0x47] = "Bullet-Proof",
    [0x49] = "irem",
    [0x50] = "Absolute",
    [0x51] = "Acclaim",
    [0x52] = "Activision",
    [0x53] = "American sammy",
    [0x54] = "Konami",
    [0x55] = "Hi tech entertainment",
    [0x56] = "LJN",
    [0x57] = "Matchbox",
    [0x58] = "Mattel",
    [0x59] = "Milton Bradley",
    [0x60] = "Titus",
    [0x61] = "Virgin",
    [0x64] = "LucasArts",
    [0x67] = "Ocean",
    [0x69] = "Electronic Arts",
    [0x70] = "Infogrames",
    [0x71] = "Interplay",
    [0x72] = "Broderbund",
    [0x73] = "sculptured",
    [0x75] = "sci",
    [0x78] = "THQ",
    [0x79] = "Accolade",
    [0x80] = "misawa",
    [0x83] = "lozc",
    [0x86] = "Tokuma Shoten Intermedia",
    [0x87] = "Tsukuda Original",
    [0x91] = "Chunsoft",
    [0x92] = "Video system",
    [0x93] = "Ocean/Acclaim",
    [0x95] = "Varie",
    [0x96] = "Yonezawa/s’pal",
    [0x97] = "Kaneko",
    [0x99] = "Pack in soft",
    [0xA4] = "Konami (Yu-Gi-Oh!)"
};

static cart_context ctx; 

const char* get_licensee() {
    u16 code = ctx.header->licensee_code;
    if (code <= 0xA5) {
        return LICENSEE_CODE[code];
    } 
    return "UNKNOWN";
}

const char* get_cartridge_type() {
    u8 code = ctx.header->cartridge_type;
    return CARTRIDGE_TYPES[code];
}

const int get_rom_size() {
    /*
    according to pan docs, 
    rom size is given by 
    32 KiB * (1 << <value>)
    where <value> is the hexadecimal value
    */
    u8 size = ctx.header->rom_size;
    return 32 * (1 << size); //this is in KiB
}

const int get_ram_size() {
    int ram_size[] = {0, 0, 8, 32, 128, 64};
    u8 size = ctx.header->ram_size;
    return ram_size[size]; //in KiB 
}

bool load_cart(char *cart_filename) {
    snprintf(ctx.filename, 1024, "%s", cart_filename);
    FILE *fptr = fopen(cart_filename, "r");

    if (!fptr) {
        printf("Opening cartridge file failed...");
        return false;
    }

    //get file size
    fseek(fptr, 0, SEEK_END);
    ctx.rom_size = ftell(fptr);
    rewind(fptr);

    //load the whole rom
    ctx.rom_data = malloc(ctx.rom_size);
    fread(ctx.rom_data, ctx.rom_size, 1, fptr);
    fclose(fptr);

    //entry point of the header starts at 0x100;
    ctx.header = (rom_header *)(ctx.rom_data + 0x100);
    ctx.header->title[15] = 0x00; //make sure the ending is padding with 0x00

    printf("Title : %s\n", ctx.header->title);
    printf("Type : %2.2X (%s)\n", ctx.header->cartridge_type, get_cartridge_type());
    printf("ROM Size: %d KiB\n", get_rom_size());
    printf("RAM Size: %2.2X KiB\n", get_ram_size());
    printf("Licensee: %2.2X (%s)\n", ctx.header->licensee_code, get_licensee());
    printf("Rom Vers: %2.2X\n", ctx.header->mask_rom_version_number);


    /*
    checksum check algorithm according to pan docs
    uint8_t checksum = 0;
    for (unint16_t address = 0x0134; address <= 0x014C; address++) {
        checksum = checksum - rom[address] -1;
    }
    */
    u16 calculated = 0;
    for (u16 address = 0x0134; address <= 0x014C; address++) {
        calculated = calculated - ctx.rom_data[address] - 1;
    }

    //checksum is only 1 byte, but the caculated value can go up to 2 bytes so we just take the last two digits
    calculated = calculated & 0xFF;

    if (calculated == ctx.header->header_checksum) {
        printf("Checksum: %x (PASSED)\n", ctx.header->header_checksum);
        return true;
    }
    printf("Checksum: %x (FAILED)\n", ctx.header->header_checksum);
    return false;
} 


