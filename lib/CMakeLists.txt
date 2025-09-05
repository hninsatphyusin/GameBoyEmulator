include_directories("/opt/homebrew/include/SDL2")
include_directories("/opt/homebrew/Cellar/sdl2/2.32.10/include/SDL2")
find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)

file(GLOB sources CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/lib/*.c")
file(GLOB headers CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/include/*.h")

add_library(emu STATIC ${sources} ${headers})

target_include_directories(emu PUBLIC ${PROJECT_SOURCE_DIR}/include ${SDL2_INCLUDE_DIRS})
target_link_libraries(emu PUBLIC ${SDL2_LIBRARIES} ${SDL2_TTF_LIBRARIES})



