# CMAKE generated file: DO NOT EDIT!
# Generated by CMake Version 3.31
cmake_policy(SET CMP0009 NEW)

# headers at lib/CMakeLists.txt:5 (file)
file(GLOB NEW_GLOB LIST_DIRECTORIES true "/home/hnin/Documents/Projects/GameBoyEmulator/include/*.h")
set(OLD_GLOB
  "/home/hnin/Documents/Projects/GameBoyEmulator/include/bus.h"
  "/home/hnin/Documents/Projects/GameBoyEmulator/include/cart.h"
  "/home/hnin/Documents/Projects/GameBoyEmulator/include/common.h"
  "/home/hnin/Documents/Projects/GameBoyEmulator/include/cpu.h"
  "/home/hnin/Documents/Projects/GameBoyEmulator/include/emu.h"
  "/home/hnin/Documents/Projects/GameBoyEmulator/include/ppu.h"
  "/home/hnin/Documents/Projects/GameBoyEmulator/include/timer.h"
  )
if(NOT "${NEW_GLOB}" STREQUAL "${OLD_GLOB}")
  message("-- GLOB mismatch!")
  file(TOUCH_NOCREATE "/home/hnin/Documents/Projects/GameBoyEmulator/build/CMakeFiles/cmake.verify_globs")
endif()

# sources at lib/CMakeLists.txt:3 (file)
file(GLOB NEW_GLOB LIST_DIRECTORIES true "/home/hnin/Documents/Projects/GameBoyEmulator/lib/*.c")
set(OLD_GLOB
  "/home/hnin/Documents/Projects/GameBoyEmulator/lib/bus.c"
  "/home/hnin/Documents/Projects/GameBoyEmulator/lib/cart.c"
  "/home/hnin/Documents/Projects/GameBoyEmulator/lib/cpu.c"
  "/home/hnin/Documents/Projects/GameBoyEmulator/lib/emu.c"
  "/home/hnin/Documents/Projects/GameBoyEmulator/lib/ppu.c"
  "/home/hnin/Documents/Projects/GameBoyEmulator/lib/timer.c"
  )
if(NOT "${NEW_GLOB}" STREQUAL "${OLD_GLOB}")
  message("-- GLOB mismatch!")
  file(TOUCH_NOCREATE "/home/hnin/Documents/Projects/GameBoyEmulator/build/CMakeFiles/cmake.verify_globs")
endif()
