find_package (Asciidoc REQUIRED)

function (add_man_target NAME MANDIR)
  set (SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/${NAME}.txt")
  set (RESULT "${CMAKE_CURRENT_BINARY_DIR}/${NAME}")

  add_custom_command (
    OUTPUT  ${RESULT}
    COMMAND ${CMAKE_ASCIIDOC_A2X_BINARY}
      --doctype=manpage
      --format=manpage
      --destination-dir="${CMAKE_CURRENT_BINARY_DIR}"
      ${SOURCE}
    DEPENDS ${SOURCE}
  )

  set (TARGET "${LZWS_TARGET}-${NAME}")
  add_custom_target (${TARGET} ALL DEPENDS ${RESULT})

  install (
    FILES ${RESULT}
    DESTINATION "${CMAKE_INSTALL_MANDIR}/${MANDIR}"
  )
endfunction ()

add_man_target ("lzws.1" "man1")

add_subdirectory ("file")
add_subdirectory ("generic")
add_subdirectory ("string")
