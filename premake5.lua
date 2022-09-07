workspace "VolumeInspector"
    architecture "x64"
    configurations {"Debug", "Release"}

outputDir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "VolumeInspector"
    kind "ConsoleApp"
    language "C++"
    staticruntime "on"

    targetdir ("bin" .. outputDir .. "/%{prj.name}")
    objdir ("bin-int" .. outputDir .. "/%{prj.name}")

    libdirs {"%{prj.location}External/Lib"}

    filter "system.windows"
        cppdialect "C++17"
        systemversion "latest"

    filter "configurations.Debug"
        defines "DEBUG"
        symbols "on"

    filter "configurations.Release"
        kind "WindowedApp"
        defines "NDEBUG"
        optimize "on"
