workspace "Falcon"
    configurations
    {
        "Debug",
        "Release"
    }

    platforms
    {
        "Win32",
        "Win64",
        "Linux32",
        "Linux64"
    }

    project "Falcon"
        kind "StaticLib"
        language "C++"
        toolset "gcc"
        targetdir "lib/%{cfg.buildcfg}/%{cfg.system}/%{cfg.architecture}"

        files
        {
            "src/*.hpp",
            "src/*.cpp"
        }

        filter "configurations:Debug"
            symbols "On"
            optimize "Off"

        filter "configurations:Release"
            optimize "Full"

        filter "platforms:Win32"
            architecture "x86"
            system "windows"
            gccprefix "i686-w64-mingw32-"

        filter "platforms:Win64"
            architecture "x86_64"
            system "windows"
            gccprefix "x86_64-w64-mingw32-"

        filter "platforms:Linux32"
            architecture "x86"
            system "linux"

        filter "platforms:Linux64"
            architecture "x86_64"
            system "linux"
