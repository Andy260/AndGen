# Create engine editor targets
add_library(AndGen_Editor STATIC)
set_target_properties(AndGen_Editor
					  PROPERTIES 
					  OUTPUT_NAME "AndGenEditor"
)
add_executable(AndGen_Editor_Exe)
set_target_properties(AndGen_Editor_Exe
					  PROPERTIES 
					  OUTPUT_NAME "AndGenEditor"
)

# Add include directories
target_include_directories(AndGen_Editor 
	PUBLIC "${INCLUDE_DIR}"
	PRIVATE "${CMAKE_CURRENT_LIST_DIR}"
	PRIVATE "${SOURCE_DIR}"
)

# Link Editor with Engine
target_link_libraries(AndGen_Editor AndGen_Engine)
target_link_libraries(AndGen_Editor_Exe AndGen_Editor AndGen_Engine)

# Add source files to the build
target_sources(AndGen_Editor
	PUBLIC "${CMAKE_CURRENT_LIST_DIR}/EditorApplication.cpp"
)
target_sources(AndGen_Editor_Exe
	# Add application main source file to build
	PRIVATE "${CMAKE_CURRENT_LIST_DIR}/main.cpp"
)
