# Copyright (c) 2020 Saroj Kumar
# All rights reserved.
# 
# This file is licensed under the MIT License.
# See the "LICENSE" file at the root directory or https://mit-license.org for details.
cmake_minimum_required(VERSION 3.14)

set(CMAKE_CXX_STANDARD 17)

project(FalconVM)

file(GLOB SRC_FILES "${CMAKE_SOURCE_DIR}/src/common/*.hpp" "${CMAKE_SOURCE_DIR}/src/common/*.cpp" "*.hpp" "*.cpp")

add_library(FalconVM ${SRC_FILES})

set_target_properties(FalconVM PROPERTIES ARCHIVE_OUTPUT_DIRECTORY
    "${CMAKE_SOURCE_DIR}/lib/${CMAKE_BUILD_TYPE}/${CMAKE_SYSTEM_NAME}/${CMAKE_SYSTEM_PROCESSOR}")

target_precompile_headers(FalconVM PUBLIC "${CMAKE_SOURCE_DIR}/pch/FalconPCH.hpp")
target_include_directories(FalconVM PUBLIC ${INCLUDE_DIRS})
