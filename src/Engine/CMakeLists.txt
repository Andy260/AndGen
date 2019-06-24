# Create game engine library
add_library(AndGen_Engine STATIC)
set_target_properties(AndGen_Engine
					  PROPERTIES 
					  OUTPUT_NAME "AndGenEngine"
)

# Add include directories
target_include_directories(AndGen_Engine 
	PUBLIC "${INCLUDE_DIR}"
	PRIVATE "${CMAKE_CURRENT_LIST_DIR}"
)

# Add source files to the build
target_sources(AndGen_Engine 
	# Add application main source file to build
	PRIVATE "${CMAKE_CURRENT_LIST_DIR}/main.cpp"
)
