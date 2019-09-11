# Create Unit Test executable
add_executable(AndGen_Engine_Tests)
set_target_properties(AndGen_Engine_Tests
					  PROPERTIES 
					  OUTPUT_NAME "AndGenEngineTests"
)

# Add include directories
target_include_directories(AndGen_Engine_Tests 
	# Google Test includes
	PRIVATE "${gtest_SOURCE_DIR}/include"
	# Google Mock includes
	PRIVATE "${gmock_SOURCE_DIR}/include"
	# CTPL Engine dependency
	PRIVATE "${CMAKE_CACHEFILE_DIR}/CTPL-src"
	# AndGen includes
	PUBLIC "${INCLUDE_DIR}"
	PRIVATE "${SOURCE_DIR}"
	PRIVATE "${CMAKE_CURRENT_LIST_DIR}"
)

# Add unit test source files within the Engine Tests directory
target_sources(AndGen_Engine_Tests 
	# Add main engine unit tests
	PRIVATE "${CMAKE_CURRENT_LIST_DIR}/CommandLineArgumentsTests.cpp"
	# Job system unit tests
	PRIVATE "${CMAKE_CURRENT_LIST_DIR}/Jobs/JobQueueTests.cpp"
	PRIVATE "${CMAKE_CURRENT_LIST_DIR}/Jobs/JobTests.cpp"
	# Add Concurrency unit tests
	#PRIVATE "${CMAKE_CURRENT_LIST_DIR}/Concurrency/ThreadPoolTests.cpp"
	PRIVATE "${CMAKE_CURRENT_LIST_DIR}/Concurrency/ThreadNotifierTests.cpp"
	PRIVATE "${CMAKE_CURRENT_LIST_DIR}/Concurrency/PooledThreadTests.cpp"
	# Tests suit main
	PUBLIC "${CMAKE_CURRENT_LIST_DIR}/main.cpp"
)

# Link unit test project to Google Test and Google Mock
# and AndGen Engine
target_link_libraries(AndGen_Engine_Tests gmock_main AndGen_Engine)

# Add AndGen Engine unit tests to CTest
add_test(NAME    AndGen_Engine_Tests 
		 COMMAND AndGen_Engine_Tests)
