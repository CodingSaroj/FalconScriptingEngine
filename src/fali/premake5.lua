project "FalconFALI"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    targetdir "../../lib/%{cfg.buildcfg}/%{cfg.platform}/"
    objdir "../../obj"

    pchheader "../../pch/FalconPCH.hpp"
    pchsource "../../pch/FalconPCH.cpp"

    files
    {
        "optimizations/*.hpp",
        "optimizations/*.cpp",
        "*.hpp",
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
        system "Linux"

    filter "platforms:*32"
        architecture "x86"

    filter "platforms:*64"
        architecture "x86_64"

project "FalconBin"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    targetname "falcon"
    targetdir "../../bin/%{cfg.buildcfg}/%{cfg.platform}/"
    objdir "../../obj"

    files
    {
        "exec/falcon/main.cpp"
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
        "FalconFALI",
        "FalconVM"
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
        system "Linux"

    filter "platforms:*32"
        architecture "x86"

    filter "platforms:*64"
        architecture "x86_64"


project "FalconFLDB"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    targetname "fldb"
    targetdir "../../bin/%{cfg.buildcfg}/%{cfg.platform}/"
    objdir "../../obj"

    files
    {
        "exec/debugger/main.cpp"
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
        "FalconFALI",
        "FalconVM"
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
        system "Linux"

    filter "platforms:*32"
        architecture "x86"

    filter "platforms:*64"
        architecture "x86_64"

project "FalconFALITest"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    targetname "falitest"
    targetdir "../../bin/%{cfg.buildcfg}/%{cfg.platform}/"
    objdir "../../obj"

    files
    {
        "test/main.cpp"
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
        "FalconFALI",
        "FalconVM"
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
        system "Linux"

    filter "platforms:*32"
        architecture "x86"

    filter "platforms:*64"
        architecture "x86_64"
