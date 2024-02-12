
cmake_minimum_required(VERSION 3.10)

# set the project name
project(01_outfileScroll CXX)

# output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
# output compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
add_definitions(-DWIN32_LEAN_AND_MEAN -DNOMINMAX -DUNICODE -D_UNICODE)

# Visual StudioのfilteringをON
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Keep the auto-generated files together in the Visual Studio source tree.
# Because by default it it added to Source Files in the Visual Studio source tree.
# This is very hard to see.
set_property(GLOBAL PROPERTY AUTOGEN_TARGETS_FOLDER AutoGenFiles)
set_property(GLOBAL PROPERTY AUTOGEN_SOURCE_GROUP AutoGenFiles)

if (WIN32)
  set(CURSES_PATH ${CMAKE_CURRENT_SOURCE_DIR}/_assets/PDCurses)
  set(CURSES_INCLUDE_DIRS ${CURSES_PATH}/include)
  set(CURSES_LIBRARIES ${CURSES_PATH}/lib/pdcurses.lib)
else()
  # linux
  find_package(Curses REQUIRED)
endif()

include_directories(${CURSES_INCLUDE_DIRS})

add_subdirectory(main)
