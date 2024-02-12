
set(main_src
  console.cpp
  console.h
  datehelper.h
)

add_library(
  ${TUI_LIB}
  ${main_src}
)

target_link_libraries(
  ${TUI_LIB}
  ${CURSES_LIBRARIES}
)

