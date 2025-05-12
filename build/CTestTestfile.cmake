# CMake generated Testfile for 
# Source directory: /home/hnin/Documents/Projects/GameBoyEmulator
# Build directory: /home/hnin/Documents/Projects/GameBoyEmulator/build
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(check_gbe "/home/hnin/Documents/Projects/GameBoyEmulator/build/tests/check_gbe")
set_tests_properties(check_gbe PROPERTIES  _BACKTRACE_TRIPLES "/home/hnin/Documents/Projects/GameBoyEmulator/CMakeLists.txt;96;add_test;/home/hnin/Documents/Projects/GameBoyEmulator/CMakeLists.txt;0;")
subdirs("lib")
subdirs("gbemu")
subdirs("tests")
