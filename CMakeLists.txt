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

if (NOT DEFINED FALCON_BUILD_VM)
    set(FALCON_BUILD_VM ON)
endif()

if (NOT DEFINED FALCON_BUILD_ASSEMBLER)
    set(FALCON_BUILD_ASSEMBLER ON)
endif()

if (NOT DEFINED FALCON_BUILD_FALI)
    set(FALCON_BUILD_FALI ON)
endif()

if (NOT DEFINED FALCON_BUILD_FCC)
    set(FALCON_BUILD_FCC ON)
endif()

if(FALCON_BUILD_VM)
    message("Configuring VM...")
    add_subdirectory(src/vm)
endif()

if(FALCON_BUILD_ASSEMBLER)
    message("Configuring Assembler...")
    add_subdirectory(src/assembler)
endif()

if(FALCON_BUILD_FALI)
    message("Configuring FALI...")
    add_subdirectory(src/fali)
endif()

if(FALCON_BUILD_FCC)
    message("Configuring FCC...")
    add_subdirectory(src/fcc)
endif()
