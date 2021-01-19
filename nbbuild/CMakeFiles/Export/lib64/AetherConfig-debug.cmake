#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "aether" for configuration "Debug"
set_property(TARGET aether APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(aether PROPERTIES
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib64/aether/libaether.so"
  IMPORTED_SONAME_DEBUG "libaether.so"
  )

list(APPEND _IMPORT_CHECK_TARGETS aether )
list(APPEND _IMPORT_CHECK_FILES_FOR_aether "${_IMPORT_PREFIX}/lib64/aether/libaether.so" )

# Import target "aehttp" for configuration "Debug"
set_property(TARGET aehttp APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(aehttp PROPERTIES
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib64/http/libaehttp.so"
  IMPORTED_SONAME_DEBUG "libaehttp.so"
  )

list(APPEND _IMPORT_CHECK_TARGETS aehttp )
list(APPEND _IMPORT_CHECK_FILES_FOR_aehttp "${_IMPORT_PREFIX}/lib64/http/libaehttp.so" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
