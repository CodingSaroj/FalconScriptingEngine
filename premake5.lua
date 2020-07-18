workspace "Falcon"
    configurations
    {
        "Debug",
        "Release"
    }

    platforms
    {
        "Linux32",
        "Linux64"
    }

    include "src/vm/premake5.lua"
    include "src/assembler/premake5.lua"
