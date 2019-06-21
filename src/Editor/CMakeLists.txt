# Create engine editor executable
add_executable(AndGen_Editor)

# Add include directories
target_include_directories(AndGen_Editor 
	PUBLIC "${INCLUDE_DIR}"
	PRIVATE "${CMAKE_CURRENT_LIST_DIR}"
	PRIVATE "${SOURCE_DIR}"
)

# Link Editor with Engine
target_link_libraries(AndGen_Editor AndGen_Engine)

# Add source files to the build
target_sources(AndGen_Editor 
	# Add application main source file to build
	PRIVATE "${CMAKE_CURRENT_LIST_DIR}/main.cpp"
)
