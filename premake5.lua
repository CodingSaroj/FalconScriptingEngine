-- Copyright (c) 2020 Saroj Kumar
-- All rights reserved.
-- 
-- This file is licensed under the MIT License.
-- See the "LICENSE" file at the root directory or https://mit-license.org for details.
workspace "Falcon"
    configurations
    {
        "Debug",
        "Release"
    }

    platforms
    {
        "Windows32",
        "Windows64",
        "Linux32",
        "Linux64"
    }

    IncludeDirs =
    {
        ".",
        "%{wks.location}/src/",
        "%{wks.location}/pch/"
    }

    LibDirs =
    {
        "%{wks.location}/lib/%{cfg.buildcfg}/%{cfg.platform}/"
    }

    include "src/vm/premake5.lua"
    include "src/assembler/premake5.lua"
    include "src/fali/premake5.lua"
    include "src/fcc/premake5.lua"
