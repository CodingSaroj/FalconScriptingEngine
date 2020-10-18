project "FalconASM"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    targetdir "../../lib/%{cfg.buildcfg}/%{cfg.platform}/"
    objdir "../../obj"

    pchheader "../../pch/FalconPCH.hpp"
    pchsource "../../pch/FalconPCH.cpp"

    files
    {
        "../common/*.cpp",
        "*.cpp"
    }
    
    includedirs
    {
        IncludeDirs
    }

    libdirs
    {
        LibDirs
    }

    filter "configurations:Debug"
        symbols "On"
        optimize "Off"

        defines
        {
            "DEBUG"
        }

    filter "configurations:Release"
        symbols "Off"
        optimize "Full"

    filter "platforms:Windows*"
        system "Linux"

    filter "platforms:Linux*"
        system "linux"

    filter "platforms:*32"
        architecture "x86"

    filter "platforms:*64"
        architecture "x86_64"

project "FalconASMBin"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    targetname "fasm"
    targetdir "../../bin/%{cfg.buildcfg}/%{cfg.platform}/"
    objdir "../../obj"

    files
    {
        "exec/*.cpp"
    }

    includedirs
    {
        IncludeDirs
    }

    libdirs
    {
        LibDirs
    }

    links
    {
        "FalconASM"
    }
    
    filter "configurations:Debug"
        symbols "On"
        optimize "Off"

        defines
        {
            "DEBUG"
        }

    filter "configurations:Release"
        symbols "Off"
        optimize "Full"

    filter "platforms:Windows*"
        system "Linux"

    filter "platforms:Linux*"
        system "linux"

    filter "platforms:*32"
        architecture "x86"

    filter "platforms:*64"
        architecture "x86_64"
