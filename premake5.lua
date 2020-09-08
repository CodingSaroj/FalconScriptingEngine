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
        "%{wks.location}/src/"
    }

    LibDirs =
    {
        "%{wks.location}/lib/%{cfg.buildcfg}/%{cfg.platform}/"
    }

    include "src/vm/premake5.lua"
    include "src/assembler/premake5.lua"
    include "src/fali/premake5.lua"
