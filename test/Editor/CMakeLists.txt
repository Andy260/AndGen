# Create Unit Test executable
add_executable(AndGen_Editor_Tests)

# Add include directories
target_include_directories(AndGen_Editor_Tests 
	# Google Test includes
	PRIVATE "${gtest_SOURCE_DIR}/include"
	# Google Mock includes
	PRIVATE "${gmock_SOURCE_DIR}/include"
	# AndGen includes
	PUBLIC "${INCLUDE_DIR}"
	PRIVATE "${SOURCE_DIR}"
	PRIVATE "${CMAKE_CURRENT_LIST_DIR}"
)

# Add unit test source files within the Engine Tests directory
target_sources(AndGen_Editor_Tests 
	# Tests suit main
	PUBLIC "${CMAKE_CURRENT_LIST_DIR}/main.cpp"
)

# Link unit test project to Google Test and Google Mock
# and AndGen Editor
target_link_libraries(AndGen_Editor_Tests gmock_main AndGen_Editor)

# Add Editor unit tests to ctest
add_test(NAME    AndGen_Editor_Tests 
		 COMMAND AndGen_Editor_Tests)
