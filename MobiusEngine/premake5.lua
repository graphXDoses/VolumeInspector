project "MobiusEngine"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "off"

    targetdir ("%{wks.location}/bin/" .. outputDir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputDir .. "/%{prj.name}")

    pchheader "ngnpch.h"
    pchsource "src/ngnpch.cpp"

    files
    {
        "src/**.h",
        "src/**.cpp",
        "External/stb_image/**.h",
        "External/stb_image/**.cpp",
        "External/glm/glm/**.hpp",
        "External/glm/glm/**.inl"
    }

    defines
    {
        "_CRT_SECURE_NO_WARNINGS",
        "GLFW_INCLUDE_NONE"
    }

    includedirs
    {
        "src",
        "External/spdlog/include",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.Glad}",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.stb_image}"
    }

    links
    {
        "GLFW",
        "Glad",
        "ImGui",
        "opengl32.lib"
    }

    -- flags { "NoPCH" }

    filter "system:windows"
        systemversion "latest"

        defines
        {
        }

    filter "configurations:Debug"
        defines "MBS_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "MBS_RELEASE"
        runtime "Release"
        optimize "on"
