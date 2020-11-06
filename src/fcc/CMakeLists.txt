# Copyright (c) 2020 Saroj Kumar
# All rights reserved.
# 
# This file is licensed under the MIT License.
# See the "LICENSE" file at the root directory or https://mit-license.org for details.
cmake_minimum_required(VERSION 3.14)

set(CMAKE_CXX_STANDARD 17)

project(FalconFCC)

file(GLOB SRC_FILES "falconscript/*.hpp" "falconscript/*.cpp")

add_library(FalconFCC ${SRC_FILES})

set_target_properties(FalconFCC PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib/${CMAKE_BUILD_TYPE}/${CMAKE_SYSTEM_NAME}/${CMAKE_SYSTEM_PROCESSOR}"
)

target_precompile_headers(FalconFCC PUBLIC "${CMAKE_SOURCE_DIR}/pch/FalconPCH.hpp")
target_include_directories(FalconFCC PUBLIC ${INCLUDE_DIRS})

project(FalconFCCTest)

add_executable(fcctest "test/main.cpp")

set_target_properties(fcctest PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE}/${CMAKE_SYSTEM_NAME}/${CMAKE_SYSTEM_PROCESSOR}"
)

target_include_directories(fcctest PUBLIC ${INCLUDE_DIRS})
target_link_directories(fcctest PUBLIC ${LIB_DIRS})
target_link_libraries(fcctest FalconVM FalconFALI FalconFCC)
