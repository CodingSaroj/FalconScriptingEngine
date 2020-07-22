project "FalconVM"
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
        system "Linux"

    filter "platforms:*32"
        architecture "x86"

    filter "platforms:*64"
        architecture "x86_64"

project "FalconVMTest"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    targetname "vmtest"
    targetdir "../../bin/%{cfg.buildcfg}/%{cfg.system}/%{cfg.architecture}/"
    objdir "../../obj/%{cfg.buildcfg}/%{cfg.system}/%{cfg.architecture}/"

    files
    {
        "test/main.cpp"
    }

    libdirs
    {
        "../../lib/%{cfg.buildcfg}/%{cfg.system}/%{cfg.architecture}/"
    }

    links
    {
        "FalconVM"
    }

    filter "configurations:Debug"
        symbols "On"
        optimize "Off"

    filter "configurations:Release"
        symbols "Off"
        optimize "Full"

    filter "platforms:Linux*"
        system "Linux"

    filter "platforms:*32"
        architecture "x86"

    filter "platforms:*64"
        architecture "x86_64"
