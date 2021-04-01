function (cmake_check_bignum_library BIGNUM_LIBRARY)
  set (BIGNUM_LIBRARIES "gmp" "tommath")

  # Empty string will be used for bignum library selection.
  if (NOT BIGNUM_LIBRARY IN_LIST BIGNUM_LIBRARIES AND NOT BIGNUM_LIBRARY STREQUAL "")
    message (FATAL_ERROR "Invalid bignum library: ${BIGNUM_LIBRARY}.")
  endif ()

  include (CheckGMP)
  include (CheckTomMath)

  if (BIGNUM_LIBRARY STREQUAL "gmp")
    cmake_check_gmp (REQUIRED)
  elseif (BIGNUM_LIBRARY STREQUAL "tommath")
    cmake_check_tommath (REQUIRED)
  else ()
    # Bignum library selection.

    foreach (BIGNUM_LIBRARY_VARIANT ${BIGNUM_LIBRARIES})
      if (BIGNUM_LIBRARY_VARIANT STREQUAL "gmp")
        cmake_check_gmp (OPTIONAL)

        if (CMAKE_GMP_WORKS)
          set (BIGNUM_LIBRARY "gmp")
          break ()
        endif ()

      elseif (BIGNUM_LIBRARY_VARIANT STREQUAL "tommath")
        cmake_check_tommath (OPTIONAL)

        if (CMAKE_TOMMATH_WORKS)
          set (BIGNUM_LIBRARY "tommath")
          break ()
        endif ()
      endif ()
    endforeach ()

    if (BIGNUM_LIBRARY STREQUAL "")
      message (FATAL_ERROR "Selection of bignum library failed")
    endif ()
  endif ()

  # Setting required environment variables for bignum library.
  if (BIGNUM_LIBRARY STREQUAL "gmp")
    set (BIGNUM_LIBRARY_INCLUDE_PATH ${CMAKE_GMP_INCLUDE_PATH})
    set (BIGNUM_LIBRARY_SHARED_PATH ${CMAKE_GMP_SHARED_LIBRARY_PATH})
    set (BIGNUM_LIBRARY_STATIC_PATH ${CMAKE_GMP_STATIC_LIBRARY_PATH})
  elseif (BIGNUM_LIBRARY STREQUAL "tommath")
    set (BIGNUM_LIBRARY_INCLUDE_PATH ${CMAKE_TOMMATH_INCLUDE_PATH})
    set (BIGNUM_LIBRARY_SHARED_PATH ${CMAKE_TOMMATH_SHARED_LIBRARY_PATH})
    set (BIGNUM_LIBRARY_STATIC_PATH ${CMAKE_TOMMATH_STATIC_LIBRARY_PATH})
  endif ()

  set (
    CMAKE_BIGNUM_LIBRARY ${BIGNUM_LIBRARY}
    CACHE STRING "Bignum library"
  )
  set (
    CMAKE_BIGNUM_LIBRARY_INCLUDE_PATH ${BIGNUM_LIBRARY_INCLUDE_PATH}
    CACHE STRING "Bignum library include path"
  )
  set (
    CMAKE_BIGNUM_LIBRARY_SHARED_PATH ${BIGNUM_LIBRARY_SHARED_PATH}
    CACHE STRING "Bignum library shared path"
  )
  set (
    CMAKE_BIGNUM_LIBRARY_STATIC_PATH ${BIGNUM_LIBRARY_STATIC_PATH}
    CACHE STRING "Bignum library static path"
  )

  mark_as_advanced (
    CMAKE_BIGNUM_LIBRARY
    CMAKE_BIGNUM_LIBRARY_INCLUDE_PATH
    CMAKE_BIGNUM_LIBRARY_SHARED_PATH
    CMAKE_BIGNUM_LIBRARY_STATIC_PATH
  )
endfunction ()
