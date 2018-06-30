include_directories(${JUDY_INCLUDE_DIRS})
link_libraries(${JUDY_LIBRARIES})

add_library(pega-texto-memory pega-texto-memory.c)
set_target_properties(pega-texto-memory PROPERTIES
	VERSION "${PROJECT_VERSION}"
	SOVERSION "${PROJECT_SOVERSION}")

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
