
set(main_src
  main.cpp
)

add_executable(
  ${PROJECT_NAME}
  ${main_src}
)

target_link_libraries(
  ${PROJECT_NAME}
  ${CURSES_LIBRARIES}
  ${TUI_LIB}
)

