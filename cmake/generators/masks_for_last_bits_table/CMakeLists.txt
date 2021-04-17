cmake_minimum_required (VERSION 3.13)
project ("cmake_generator_masks_for_last_bits_table" "C")

include ("../common/toolchain.cmake")

add_executable ("main" "main.c")

add_custom_command (
  TARGET "main"
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E "copy"
  "$<TARGET_FILE:main>" "${CMAKE_CURRENT_BINARY_DIR}/result"
)
