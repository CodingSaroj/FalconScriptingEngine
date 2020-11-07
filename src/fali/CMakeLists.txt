# Copyright (c) 2020 Saroj Kumar
# All rights reserved.
# 
# This file is licensed under the MIT License.
# See the "LICENSE" file at the root directory or https://mit-license.org for details.
cmake_minimum_required(VERSION 3.14)

set(CMAKE_CXX_STANDARD 17)

project(FalconFALI)

file(GLOB SRC_FILES "${CMAKE_SOURCE_DIR}/src/common/*.hpp" "${CMAKE_SOURCE_DIR}/src/common/*.cpp" "*.hpp" "*.cpp")

add_library(FalconFALI ${SRC_FILES})

set_target_properties(FalconFALI PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib/${CMAKE_BUILD_TYPE}/${CMAKE_SYSTEM_NAME}/${CMAKE_SYSTEM_PROCESSOR}"
)

target_precompile_headers(FalconFALI PUBLIC "${CMAKE_SOURCE_DIR}/pch/FalconPCH.hpp")
target_include_directories(FalconFALI PUBLIC ${INCLUDE_DIRS})

if (FALCON_BUILD_VM)
    project(FalconBin)
    
    add_executable(falcon "exec/falcon/main.cpp")
    
    set_target_properties(falcon PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE}/${CMAKE_SYSTEM_NAME}/${CMAKE_SYSTEM_PROCESSOR}"
    )
    
    target_include_directories(falcon PUBLIC ${INCLUDE_DIRS})
    target_link_directories(falcon PUBLIC ${LIB_DIRS})
    target_link_libraries(falcon FalconVM FalconFALI)
    
    project(FalconFLDB)
    
    add_executable(fldb "exec/debugger/main.cpp")
    
    set_target_properties(fldb PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE}/${CMAKE_SYSTEM_NAME}/${CMAKE_SYSTEM_PROCESSOR}"
    )
    
    target_include_directories(fldb PUBLIC ${INCLUDE_DIRS})
    target_link_directories(fldb PUBLIC ${LIB_DIRS})
    target_link_libraries(fldb FalconVM FalconFALI)
endif()
