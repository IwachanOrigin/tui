
set(main_src
  main.cpp
  console.cpp
  console.h
  datehelper.h
)

add_executable(
  ${PROJECT_NAME}
  ${main_src}
)

target_link_libraries(
  ${PROJECT_NAME}
  ${CURSES_LIBRARIES}
)

