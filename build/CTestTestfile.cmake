# CMake generated Testfile for 
# Source directory: /Users/hninsatphyusin/Documents/01 Projects/GameBoyEmulator
# Build directory: /Users/hninsatphyusin/Documents/01 Projects/GameBoyEmulator/build
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(check_gbe "/Users/hninsatphyusin/Documents/01 Projects/GameBoyEmulator/build/tests/check_gbe")
set_tests_properties(check_gbe PROPERTIES  _BACKTRACE_TRIPLES "/Users/hninsatphyusin/Documents/01 Projects/GameBoyEmulator/CMakeLists.txt;96;add_test;/Users/hninsatphyusin/Documents/01 Projects/GameBoyEmulator/CMakeLists.txt;0;")
subdirs("lib")
subdirs("gbemu")
subdirs("tests")
