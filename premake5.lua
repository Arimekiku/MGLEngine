workspace "RenderingEngine"
	architecture "x64"
	startproject "Application"

	configurations 
	{
		"Debug",
		"Release"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

includes = {}
includes["GLFW"] = "RenderingEngine/vendor/GLFW/include"
includes["spdlog"] = "RenderingEngine/vendor/spdlog/include"

include "RenderingEngine/vendor/GLFW"

project "RenderingEngine"
	location "RenderingEngine"
	kind "StaticLib"
	language "C++"
	staticruntime "off"

	targetdir ("binaries/" .. outputdir .. "/%{prj.name}")
	objdir ("intermediates/" .. outputdir .. "/%{prj.name}")

	files 
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs 
	{
		"%{prj.name}/src",
		"%{includes.spdlog}",
		"%{includes.GLFW}"
	}

	links 
	{
		"GLFW",
		"opengl32.lib",
		"dwmapi.lib"
	}

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"

	filter "configurations:Debug"
		defines "APP_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "APP_RELEASE"
		optimize "On"

project "Application"
	location "Application"
	kind "ConsoleApp"
	language "C++"
	staticruntime "off"

	targetdir ("binaries/" .. outputdir .. "/%{prj.name}")
	objdir ("intermediates/" .. outputdir .. "/%{prj.name}")

	files 
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs 
	{
		"RenderingEngine/src",
		"RenderingEngine/vendor",
		"%{includes.spdlog}",
		"%{includes.GLFW}"
	}

	links 
	{
		"RenderingEngine"
	}

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"

	filter "configurations:Debug"
		defines "APP_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "APP_RELEASE"
		optimize "On"