# Original build system by Barry, Benny, and mxaddict. 
# Build system modified for MARS by Gameplay.Solutions.
# Copyright (C) 2018 Gameplay.Solutions
#
# Copyright (C) 2014 Barry Deeney
# Copyright (C) 2014 Benny Bobaganoosh
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

################
# CMAKE RULES! #
################
# Huge thanks to mxaddict for setting up the CMake build system!

# we need features that are 2.6 >= dependent
cmake_minimum_required(VERSION 2.6)

# lets name the project
project(MARS)

# add the -c and -Wall flags
if(MSVC)
	add_definitions(
		-c
		-W4
	)
else()
	add_definitions(
		-c
		-Wall
		-msse2
	)
endif()

if ( CMAKE_BUILD_TYPE STREQUAL "" )
	# CMake defaults to leaving CMAKE_BUILD_TYPE empty. This screws up
	# differentiation between debug and release builds.
	set(
		CMAKE_BUILD_TYPE
			"Debug"
		CACHE STRING
			"Choose the type of build, options are: None (CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) \"Debug\" \"Release\" \"RelWithDebInfo\" \"MinSizeRel\"."
		FORCE
	)
endif ( CMAKE_BUILD_TYPE STREQUAL "" )

if ( CMAKE_BUILD_TYPE STREQUAL "Release" )
add_definitions( -O2 )
endif ( CMAKE_BUILD_TYPE STREQUAL "Release" )

# Lets LOAD app our headers!
file(GLOB_RECURSE HDRS
	${MARS_SOURCE_DIR}/Source/*.h
	${MARS_SOURCE_DIR}/Source/*.hpp
)

# Lets LOAD app our sources!
file(GLOB_RECURSE SRCS
	${MARS_SOURCE_DIR}/Source/*.cpp
	${MARS_SOURCE_DIR}/Source/*.c
)

# Define the executable
add_executable(MARS ${HDRS} ${SRCS})

# We need a CMAKE_DIR with some code to find external dependencies
SET(MARS_CMAKE_DIR "${MARS_SOURCE_DIR}/cmake")

#######################################
# LOOK for the packages that we need! #
#######################################

# OpenGL
find_package(OpenGL REQUIRED)

# GLEW
INCLUDE(${MARS_CMAKE_DIR}/FindGLEW.cmake)

# SDL2
INCLUDE(${MARS_CMAKE_DIR}/FindSDL2.cmake)

# ASSIMP
INCLUDE(${MARS_CMAKE_DIR}/FindASSIMP.cmake)

# Define the include DIRs
include_directories(
	${MARS_SOURCE_DIR}/headers
	${MARS_SOURCE_DIR}/sources
	${MARS_SOURCE_DIR}/Source
	${MARS_SOURCE_DIR}/Thirdparty
	${OPENGL_INCLUDE_DIRS}
	${GLEW_INCLUDE_DIRS}
	${SDL2_INCLUDE_DIRS}
	${ASSIMP_INCLUDE_DIRS}
)

# Define the link libraries
target_link_libraries( MARS
	${OPENGL_LIBRARIES}
	${GLEW_LIBRARIES}
	${SDL2_LIBRARIES}
	${ASSIMP_LIBRARIES}
)

if(WIN32)
	string(REPLACE "/" "\\" source_path_windows "${MARS_SOURCE_DIR}/Resources")
	string(REPLACE "/" "\\" build_path_windows "${MARS_BINARY_DIR}/Resources")
	execute_process(COMMAND cmd.exe /c mklink /J "${build_path_windows}" "${source_path_windows}" RESULT_VARIABLE exitcode)
else()
	execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink ${MARS_SOURCE_DIR}/Resources ${MARS_BINARY_DIR}/Resources RESULT_VARIABLE exitcode)
endif()

if(NOT ${exitcode} EQUAL 0)
	MESSAGE("SYMLINKING FAILED: ${exitcode}")
	MESSAGE("FALLING BACK TO COPYING")
endif()

#Copy file if the build resource folder contains an older version. This is ignored when the symlink was succesful because the files are already the newest version.
file(GLOB_RECURSE RESOURCES RELATIVE ${MARS_SOURCE_DIR}/Resources/ ${MARS_SOURCE_DIR}/Resources/*.*)
foreach(file IN LISTS RESOURCES)
	configure_file(${MARS_SOURCE_DIR}/Resources/${file} ${MARS_BINARY_DIR}/Resources/${file} COPYONLY)
endforeach()

#Create virtual folders to make it look nicer in VS
if(MSVC_IDE)
	foreach(source IN LISTS SRCS HDRS)
		get_filename_component(source_path "${source}" PATH)
		string(REPLACE "${MARS_SOURCE_DIR}" "" relative_source_path "${source_path}")
		string(REPLACE "/" "\\" source_path_msvc "${relative_source_path}")
		source_group("${source_path_msvc}" FILES "${source}")
	endforeach()
endif()
