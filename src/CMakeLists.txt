include_directories(${JUDY_INCLUDE_DIRS} ${PEGA_TEXTO_INCLUDE_DIRS})
link_libraries(${JUDY_LIBRARIES} ${PEGA_TEXTO_LIBRARIES})

# Version check
file(STRINGS "pega-texto-memory.h" _version_line REGEX "PT_MEMORY_VERSION")
string(REGEX MATCH "[0-9]+\\.[0-9]+\\.[0-9]+" PT_MEMORY_VERSION ${_version_line})
if(NOT PT_MEMORY_VERSION VERSION_EQUAL ${PROJECT_VERSION})
	message(SEND_ERROR "Version from CMakeLists.txt doesn't match the one in pega-texto-memory.h")
endif()

add_library(pega-texto-memory pega-texto-memory.c)
set_target_properties(pega-texto-memory PROPERTIES
	VERSION "${PROJECT_VERSION}"
	SOVERSION "${PROJECT_SOVERSION}")

if(COMPILE_EXAMPLE)
	add_executable(test main.c)
	target_link_libraries(test pega-texto-memory)
endif()

if(UNIX AND BUILD_SHARED_LIBS AND LINK_SONAME_FILE)
	if(APPLE)
		set(lib-ext dylib)
	else()
		set(lib-ext so)
	endif()
	set(link-name "libpega-texto-memory${PROJECT_VERSION_MAJOR}.${lib-ext}")
	add_custom_command(TARGET pega-texto-memory POST_BUILD COMMAND ${CMAKE_COMMAND} -E create_symlink $<TARGET_SONAME_FILE_NAME:pega-texto-memory> ${link-name})
	install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${link-name} DESTINATION lib)
endif()

install(TARGETS pega-texto-memory
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib)
install(FILES "pega-texto-memory.h" DESTINATION include)
