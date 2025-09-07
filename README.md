# Game Boy Emulator

A C-based Game Boy emulator built following the "Low Level Dude" YouTube tutorial series. This project implements the core components of the original Game Boy hardware to run classic Game Boy ROMs.

## 🎯 Project Overview

This emulator was developed by following the comprehensive YouTube tutorial series by Low Level Dude:
[Game Boy Emulator Tutorial Playlist](https://www.youtube.com/playlist?list=PLVxiWMqQvhg_yk4qy2cSC3457wZJga_e5)

The tutorial provided an excellent foundation for understanding emulator development, CPU architecture, and low-level programming concepts.

## 🏗️ Architecture Overview

The emulator is structured into modular components, each handling specific aspects of the Game Boy hardware:

### Core Components

| File | Description |
|------|-------------|
| **`main.c`** | Entry point and main execution loop |
| **`emu.c`** | Main emulator context and coordination |
| **`bus.c`** | Memory bus implementation for address routing |
| **`cart.c`** | Cartridge loading and Memory Bank Controller (MBC) handling |

### CPU Implementation
| File | Description |
|------|-------------|
| **`cpu.c`** | CPU core implementation and register management |
| **`cpu_fetch.c`** | Instruction fetching and decoding |
| **`cpu_proc.c`** | Instruction execution and processing |
| **`cpu_util.c`** | CPU utility functions and helpers |
| **`instructions.c`** | Complete Game Boy instruction set implementation |
| **`stack.c`** | Stack operations (push/pop) |

### Graphics (PPU - Picture Processing Unit)
| File | Description |
|------|-------------|
| **`ppu.c`** | Main PPU implementation and context management |
| **`ppu_sm.c`** | PPU state machine (OAM, XFER, HBLANK, VBLANK modes) |
| **`ppu_pipeline.c`** | Pixel rendering pipeline and FIFO implementation |
| **`lcd.c`** | LCD controller and display registers |

### I/O and Hardware
| File | Description |
|------|-------------|
| **`io.c`** | I/O register handling (timers, serial, audio, etc.) |
| **`timer.c`** | Game Boy timer implementation |
| **`gamepad.c`** | Joypad input handling |
| **`dma.c`** | Direct Memory Access for OAM transfers |
| **`interrupts.c`** | Interrupt handling system |

### Memory Management
| File | Description |
|------|-------------|
| **`ram.c`** | Work RAM and High RAM implementation |

### User Interface
| File | Description |
|------|-------------|
| **`ui.c`** | SDL-based display rendering and input handling |
| **`dbg.c`** | Debug utilities and logging |

## 🎮 Controls

| Keyboard | Game Boy |
|----------|----------|
| Arrow Keys | D-Pad |
| X | A Button |
| Z | B Button |
| Enter | Start |
| Tab | Select |

## 🚀 Building and Running

### Prerequisites
- CMake
- SDL2 and SDL2_ttf libraries
- C compiler (GCC or Clang)

### macOS (Homebrew)
```bash
brew install sdl2 sdl2_ttf cmake
```

### Building
```bash
mkdir build
cd build
cmake ..
make
```

### Running
```bash
./gbemu/gbemu path/to/rom.gb
```

## 📁 Project Structure

```
GameBoyEmulator/
├── CMakeLists.txt          # Build configuration
├── README.md              # This file
├── include/               # Header files
├── lib/                   # Implementation files
├── gbemu/                 # Main executable
├── tests/                 # Test suite
├── roms/                  # Game Boy ROM files
└── build/                 # Build output directory
```

## ✨ Current Features

- ✅ Complete Game Boy CPU instruction set
- ✅ Memory management and cartridge loading
- ✅ PPU with pixel-perfect rendering pipeline
- ✅ Joypad input support
- ✅ Timer and interrupt systems
- ✅ Basic I/O register implementation
- ✅ Debug window showing tile data
- ✅ SDL-based graphics output

## 🐛 Known Issues

### Display Corruption
Currently experiencing display corruption issues where:
- Graphics appear as vertical lines or distorted patterns
- May be related to PPU timing or pixel pipeline issues
- Debug investigation ongoing

## 🗓️ Roadmap

### Priority 1: Display Bug Fixes
- [ ] Investigate and fix PPU rendering pipeline issues
- [ ] Verify pixel FIFO implementation
- [ ] Debug color palette handling
- [ ] Ensure proper LCD timing and synchronization

### Priority 2: Audio Support
- [ ] Implement Game Boy APU (Audio Processing Unit)
- [ ] Add sound channel 1 (pulse with sweep)
- [ ] Add sound channel 2 (pulse)
- [ ] Add sound channel 3 (wave)
- [ ] Add sound channel 4 (noise)
- [ ] Implement audio mixing and SDL audio output

### Future Enhancements
- [ ] Save state functionality
- [ ] Game Boy Color support
- [ ] Performance optimizations
- [ ] Additional debug features
- [ ] Configuration options

## 🎓 Learning Outcomes

This project provided valuable experience in:
- Low-level programming and bit manipulation
- CPU architecture and instruction set implementation
- Graphics programming and rendering pipelines
- Memory management and hardware emulation
- C programming and project organization
- CMake build system
- Git version control

## 📚 Resources

- [Game Boy CPU Manual](http://marc.rawer.de/Gameboy/Docs/GBCPUman.pdf)
- [Pan Docs - Game Boy Technical Reference](https://gbdev.io/pandocs/)
- [Low Level Dude YouTube Channel](https://www.youtube.com/@LowLevelGameDev)

## 🤝 Contributing

This is primarily a learning project, but suggestions and improvements are welcome! Please feel free to:
- Report bugs
- Suggest improvements
- Share resources

## 📄 License

This project is for educational purposes. Game Boy is a trademark of Nintendo.

---

*Built with passion for retro gaming and low-level programming! 🎮*