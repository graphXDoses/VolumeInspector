project "VolumeInspector"
    language "C++"
    cppdialect "C++17"
    staticruntime "off"

    targetdir ("%{wks.location}/bin/" .. outputDir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputDir .. "/%{prj.name}")

    files
    {
        "src/**.h",
        "src/**.cpp"
    }

    includedirs
    {
        "%{wks.location}/MobiusEngine/External/spdlog/include",
        "%{wks.location}/MobiusEngine/External",
        "%{wks.location}/MobiusEngine/src",
        "%{IncludeDir.glm}"
    }

    links
    {
        "MobiusEngine"
    }

    filter "system:windows"
        systemversion "latest"

    filter "configurations:Debug"
        kind "ConsoleApp"
        defines "MBS_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        kind "WindowedApp"
        defines "MBS_RELEASE"
        runtime "Release"
        optimize "on"
