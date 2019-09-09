# Create game engine library
add_library(AndGen_Engine STATIC)
set_target_properties(AndGen_Engine
					  PROPERTIES 
					  OUTPUT_NAME "AndGenEngine"
)

# Add CTPL dependency
target_include_directories(AndGen_Engine
	PRIVATE "${CMAKE_CACHEFILE_DIR}/CTPL-src"
)

# Add include directories
target_include_directories(AndGen_Engine 
	PUBLIC "${INCLUDE_DIR}"
	PRIVATE "${CMAKE_CURRENT_LIST_DIR}"
)

# Add source files to the build
target_sources(AndGen_Engine 
	# Add main engine source
	PUBLIC "${CMAKE_CURRENT_LIST_DIR}/CommandLineArguments.cpp"
	# Add Concurrency source files
	PRIVATE "${CMAKE_CURRENT_LIST_DIR}/Concurrency/PooledThread.cpp"
	# Add Job System source files
	PRIVATE "${CMAKE_CURRENT_LIST_DIR}/Jobs/Job.cpp"
	PRIVATE "${CMAKE_CURRENT_LIST_DIR}/Jobs/JobQueue.cpp"
	# Add Application main source
	PRIVATE "${CMAKE_CURRENT_LIST_DIR}/main.cpp"
)
