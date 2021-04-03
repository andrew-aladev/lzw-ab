if (DEFINED CMAKE_ASCIIDOC_FOUND)
  return ()
endif ()

set (MESSAGE_PREFIX "Status of Asciidoc")

find_program (CMAKE_ASCIIDOC_BINARY "asciidoc.py" "asciidoc")
if (CMAKE_ASCIIDOC_BINARY)
  message (STATUS "${MESSAGE_PREFIX} - main binary path: ${CMAKE_ASCIIDOC_BINARY}")
else ()
  message (STATUS "${MESSAGE_PREFIX} - main binary path not found")
endif ()

find_program (CMAKE_ASCIIDOC_A2X_BINARY "a2x.py" "a2x")
if (CMAKE_ASCIIDOC_BINARY)
  message (STATUS "${MESSAGE_PREFIX} - a2x binary path: ${CMAKE_ASCIIDOC_A2X_BINARY}")
else ()
  message (STATUS "${MESSAGE_PREFIX} - a2x binary path not found")
endif ()

if (CMAKE_ASCIIDOC_BINARY AND CMAKE_ASCIIDOC_A2X_BINARY)
  set (CMAKE_ASCIIDOC_FOUND true)
else ()
  set (CMAKE_ASCIIDOC_FOUND false)
endif ()

set (
  CMAKE_ASCIIDOC_FOUND ${CMAKE_ASCIIDOC_FOUND}
  CACHE BOOL "Asciidoc found"
)

mark_as_advanced (
  CMAKE_ASCIIDOC_FOUND
  CMAKE_ASCIIDOC_BINARY
  CMAKE_ASCIIDOC_A2X_BINARY
)

if (NOT CMAKE_ASCIIDOC_FOUND AND Asciidoc_FIND_REQUIRED)
  message (FATAL_ERROR "${MESSAGE_PREFIX} - is required")
endif ()
