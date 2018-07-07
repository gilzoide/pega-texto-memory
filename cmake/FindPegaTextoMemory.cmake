# FindPegaTextoMemory
# -------------------
# Locate pega-texto-memory library. This module defines:
#
#   PEGA_TEXTO_MEMORY_FOUND         - true if pega-texto-memory library was found
#   PEGA_TEXTO_MEMORY_LIBRARIES     - pega-texto-memory library
#   PEGA_TEXTO_MEMORY_INCLUDE_DIRS  - where to find pega-texto-memory.h
#   PEGA_TEXTO_MEMORY_VERSION       - the version of pega-texto-memory found

find_path(PEGA_TEXTO_MEMORY_INCLUDE_DIRS
	pega-texto-memory.h
	PATH_SUFFIXES pega-texto-memory/include)

find_library(PEGA_TEXTO_MEMORY_LIBRARIES
	NAMES pega-texto-memory pega-texto-memory2 pega-texto-memory1
	PATH_SUFFIXES pega-texto-memory/lib)

# find library version in header file
if(PEGA_TEXTO_MEMORY_INCLUDE_DIRS)
	file(STRINGS "${PEGA_TEXTO_MEMORY_INCLUDE_DIRS}/pega-texto-memory.h" _version_line REGEX "PT_MEMORY_VERSION")
	string(REGEX MATCH "[0-9]+\\.[0-9]+\\.[0-9]+" PEGA_TEXTO_MEMORY_VERSION ${_version_line})
endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(PegaTextoMemory
	REQUIRED_VARS PEGA_TEXTO_MEMORY_LIBRARIES PEGA_TEXTO_MEMORY_INCLUDE_DIRS
	VERSION_VAR PEGA_TEXTO_MEMORY_VERSION)

mark_as_advanced(PEGA_TEXTO_MEMORY_LIBRARIES PEGA_TEXTO_MEMORY_INCLUDE_DIRS)
