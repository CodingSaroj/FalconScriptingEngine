# Copyright (c) 2020 Saroj Kumar
# All rights reserved.
# 
# This file is licensed under the MIT License.
# See the "LICENSE" file at the root directory or https://mit-license.org for details.
cmake_minimum_required(VERSION 3.14)

get_filename_component(ROOT_DIR ${CMAKE_SOURCE_DIR} REALPATH BASE_DIR ".")

set(INCLUDE_DIRS "${CMAKE_SOURCE_DIR}/src;" "${CMAKE_SOURCE_DIR}/pch")
set(LIB_DIRS "${CMAKE_SOURCE_DIR}/lib/${CMAKE_BUILD_TYPE}/${CMAKE_SYSTEM_NAME}_${CMAKE_SYSTEM_PROCESSOR}/")

project(Falcon)

add_subdirectory(src/vm)
add_subdirectory(src/assembler)
add_subdirectory(src/fali)
add_subdirectory(src/fcc)
