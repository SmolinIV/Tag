cmake_minimum_required(VERSION 3.8)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_TOOLCHAIN_FILE "C:\\vcpkg\\scripts\\buildsystems\\vcpkg.cmake")
set(PROJECT_NAME "Tag")
set(SUBSYSTEM_LINKER_OPTIONS "-mwindows")

project(${PROJECT_NAME})

#set (SFML_DIR "C:\\vcpkg\\installed\\x64-windows\\share\\sfml")
find_package(SFML 2.5.1 REQUIRED graphics network audio window system)

add_executable( ${PROJECT_NAME} TAG.cpp
Player.cpp
bot.cpp
Board.cpp
Cube.cpp
game_time.cpp
Menu.cpp
Tablet.cpp)

set_target_properties(${PROJECT_NAME}
PROPERTIES CXX_STANDARD 17
CXX_STANDARD_REQUIRED ON
)


#include_directories( C:\\vcpkg\\installed\\x64-windows\\include)
FILE(
    COPY "${CMAKE_CURRENT_SOURCE_DIR}/png" "${CMAKE_CURRENT_SOURCE_DIR}/font"
    DESTINATION "${CMAKE_CURRENT_BINARY_DIR}"
)

target_link_libraries(${PROJECT_NAME}
 PRIVATE 
 sfml-graphics
 sfml-audio
 sfml-network
 sfml-window
 sfml-system)

#set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake_modules")

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR
CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
target_compile_options(${PROJECT_NAME} PRIVATE
-g
-Wall
-Werror
-pedantic
-mwindows
)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
target_compile_options(${PROJECT_NAME} PRIVATE
/EHsc
/W4
)
endif()

