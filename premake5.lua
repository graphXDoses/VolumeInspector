include "Dependencies.lua"

workspace "VolumeInspector"
    architecture "x64"
    startproject "VolumeInspector"

    configurations
    {
        "Debug", "Release"
    }

    flags
	{
		"MultiProcessorCompile"
	}

outputDir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Dependencies"
        include "MobiusEngine/External/IncludeDirs/GLFW"
        include "MobiusEngine/External/IncludeDirs/Glad"
        include "MobiusEngine/External/imgui"
group ""

include "MobiusEngine"
include "VolumeInspector"
