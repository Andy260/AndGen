# CMakeList.txt : Primary CMake file for AndGen

cmake_minimum_required (VERSION 3.1 FATAL_ERROR)

# Set-up project
project(AndGen CXX C)
# Specify and require C++17 with no extensions
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set Project version project version
set(VERSION_MAJOR 0)
set(VERSION_MINOR 0)
set(VERSION_PATCH 0)

#--------------------------------------------------------------------
# Build Options
#--------------------------------------------------------------------
option(BUILD_ENGINE_TESTS "Build Engine Tests" FALSE)
option(BUILD_EDITOR_TESTS "Build Editor Tests" FALSE)
#--------------------------------------------------------------------

#--------------------------------------------------------------------
# Project Folder Defines
#--------------------------------------------------------------------
# Define common CMake utility files
set(UTILITY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/CMake")
# Define dependency directory
set(DEPENDENCY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/deps")
# Define project include directory
set(INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
# Define project source directory
set(SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
# Define project tests directory
set(TEST_DIR "${CMAKE_CURRENT_SOURCE_DIR}/test")
#--------------------------------------------------------------------

#--------------------------------------------------------------------
# CMake Modules
#--------------------------------------------------------------------
include(${UTILITY_DIR}/DownloadProject.cmake)
#--------------------------------------------------------------------

#--------------------------------------------------------------------
# Project dependencies
#--------------------------------------------------------------------
include("${UTILITY_DIR}/CTPL.cmake")
#--------------------------------------------------------------------

#--------------------------------------------------------------------
# Project source
#--------------------------------------------------------------------
# Add source sub-directories
add_subdirectory("${SOURCE_DIR}/Engine")
add_subdirectory("${SOURCE_DIR}/Editor")
#--------------------------------------------------------------------

#--------------------------------------------------------------------
# Project Tests
#--------------------------------------------------------------------
# Include Google Test and Google Mock
if (BUILD_ENGINE_TESTS OR BUILD_EDITOR_TESTS)
	include(CTest)
	include("${UTILITY_DIR}/GoogleTest.cmake")
endif()

# AndGen Engine Tests
if(BUILD_ENGINE_TESTS)
	add_subdirectory("${TEST_DIR}/Engine")
endif()

# AndGen Edutor Tests
if(BUILD_EDITOR_TESTS)
	add_subdirectory("${TEST_DIR}/Editor")
endif()
#--------------------------------------------------------------------
