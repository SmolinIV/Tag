cmake_minimum_required(VERSION 3.8)


set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PROJECT_NAME "TAG")

project(${PROJECT_NAME})

set(CMAKE_PREFIX_PATH "C:/VCPKG/installed/x64-windows/include")

add_executable( ${PROJECT_NAME} TAG.cpp
Player.cpp
bot.cpp
Board.cpp
Cube.cpp
game_time.cpp
Menu.cpp
Tablet.cpp)

find_package(SFML 2.5.1 COMPONENTS network audio graphics window system)

target_link_libraries(${PROJECT_NAME} PRIVATE sfml-graphics sfml-audio sfml-network sfml-window sfml-system)

target_include_directories(${PROJECT_NAME} PRIVATE ${SFML_INCLUDE_DIRS})

include_directories(
  ${CMAKE_SOURCE_DIR}/png 
  ${CMAKE_SOURCE_DIR}/font
)

target_include_directories(${PROJECT_NAME} PRIVATE 
  ${SFML_INCLUDE_DIRS} 
  include
)

target_link_libraries(${PROJECT_NAME}  PRIVATE ${SFML_LIBRARIES})

set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON)

message(STATUS ${PROJECT_SOURCE_DIR})

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR
CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
target_compile_options(TAG PRIVATE
-g
-Wall
-Werror
-pedantic
)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
target_compile_options(TAG PRIVATE
/EHsc
/W4
)
endif()

