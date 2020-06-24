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

    project "Falcon"
        kind "StaticLib"
        language "C++"
        cppdialect "C++17"
        targetdir "lib/%{cfg.buildcfg}/%{cfg.system}/%{cfg.architecture}"

        files
        {
            "src/*.cpp"
        }

        filter "configurations:Debug"
            symbols "On"
            optimize "Off"

        filter "configurations:Release"
            symbols "Off"
            optimize "Full"

        filter "platforms:Linux32"
            architecture "x86"
            system "linux"

        filter "platforms:Linux64"
            architecture "x86_64"
            system "linux"
