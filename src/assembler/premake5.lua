project "FalconASM"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    targetdir "../../lib/%{cfg.buildcfg}/%{cfg.system}/%{cfg.architecture}/"
    objdir "../../obj/%{cfg.buildcfg}/%{cfg.system}/%{cfg.architecture}/"

    files
    {
        "*.cpp"
    }

    filter "configurations:Debug"
        symbols "On"
        optimize "Off"

    filter "configurations:Release"
        symbols "Off"
        optimize "Full"

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
    targetdir "../../bin/%{cfg.buildcfg}/%{cfg.system}/%{cfg.architecture}/"
    objdir "../../obj/%{cfg.buildcfg}/%{cfg.system}/%{cfg.architecture}/"

    files
    {
        "test/*.cpp"
    }

    includedirs
    {
        "."
    }

    libdirs
    {
        "../../lib/%{cfg.buildcfg}/%{cfg.system}/%{cfg.architecture}/"
    }

    links
    {
        "FalconASM"
    }
    
    filter "configurations:Debug"
        symbols "On"
        optimize "Off"

    filter "configurations:Release"
        symbols "Off"
        optimize "Full"

    filter "platforms:Linux*"
        system "linux"

    filter "platforms:*32"
        architecture "x86"

    filter "platforms:*64"
        architecture "x86_64"
