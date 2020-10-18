project "FalconFCC"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    targetdir "../../lib/%{cfg.buildcfg}/%{cfg.platform}/"
    objdir "../../obj"

    files
    {
        "falconscript/*.hpp",
        "falconscript/*.cpp"
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
        system "windows"

    filter "platforms:Linux*"
        system "linux"

    filter "platforms:*32"
        architecture "x86"

    filter "platforms:*64"
        architecture "x86_64"

project "FalconFCCTest"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    targetname "fcctest"
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
        "FalconFCC",
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
        system "windows"

    filter "platforms:Linux*"
        system "linux"

    filter "platforms:*32"
        architecture "x86"

    filter "platforms:*64"
        architecture "x86_64"
