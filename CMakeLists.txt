cmake_minimum_required(VERSION 2.6)

set(PROJECT_VERSION_MAJOR 0)
set(PROJECT_VERSION_MINOR 1)
set(PROJECT_VERSION_PATCH 0)
set(PROJECT_VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}")
set(PROJECT_SOVERSION ${PROJECT_VERSION_MAJOR})

if(${CMAKE_VERSION} VERSION_GREATER 3.0)
	cmake_policy(SET CMP0048 NEW)
	project(pega-texto-memory VERSION ${PROJECT_VERSION})
else()
	project(pega-texto-memory)
endif()

set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wall -Wno-parentheses")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O2 -DNDEBUG")
set(CMAKE_C_FLAGS_RELMINSIZE "${CMAKE_C_FLAGS_RELMINSIZE} -Os -DNDEBUG")
add_definitions(-DPROJECT_VERSION="${PROJECT_VERSION}")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")
find_package(Judy REQUIRED)
find_package(PegaTexto 2 REQUIRED)

# Build stuff
option(BUILD_SHARED_LIBS "Build as a shared library" ON)
if(UNIX)
	option(LINK_SONAME_FILE "Create an additional symlink and pkg-config script for library based on it's SONAME" ON)
endif()
add_subdirectory(src)
# add_subdirectory(pkg-config)

# # Examples
# option(COMPILE_EXAMPLES "Compile the examples" OFF)
# if(COMPILE_EXAMPLES)
	# add_subdirectory(examples)
# endif()

# CPack packaging
include(InstallRequiredSystemLibraries)
set(CPACK_GENERATOR "STGZ;TGZ;TZ")
set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_SOURCE_DIR}/README.md")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Memory usage statistics for the pega-texto PEG runtime engine")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE")
set(CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}-${PROJECT_VERSION}")
set(CPACK_PACKAGE_NAME ${PROJECT_NAME})
set(CPACK_PACKAGE_CONTACT "gilzoide@gmail.com")
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
include(CPack)
