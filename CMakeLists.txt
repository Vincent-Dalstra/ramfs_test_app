# The following lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.5)

# Minimal build
set(COMPONENTS main)

set(EXTRA_COMPONENT_DIRS "")

	# Set the components to include the tests for.
	# This can be overriden from CMake cache:
	# - when invoking CMake directly: cmake -D TEST_COMPONENTS="xxxxx" ..
	# - when using idf.py: idf.py -T xxxxx build
	#
	set(TEST_COMPONENTS "" CACHE STRING "List of components to test")

	list(APPEND EXTRA_COMPONENT_DIRS dev_managed_components/)
	list(APPEND TEST_COMPONENTS "ramfs")

	# Necessary when 'overriding main', or else the tests will not run
	list(APPEND COMPONENTS ${TEST_COMPONENTS})


include($ENV{IDF_PATH}/tools/cmake/project.cmake)

# Component dependency graph generation
# For details see: esp-idf/tools/cmake/depgraph.cmake
# NOTE: must be between include(project.cmake) and project(name) calls.
idf_build_set_property(__BUILD_COMPONENT_DEPGRAPH_ENABLED 1)

project(ramfs_test_app)
