workspace "Svarog"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Svarog"
	location "./"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"

	--targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	--objdir ("build/" .. outputdir .. "/%{prj.name}")
	targetdir ("bin/")
	objdir ("build/")

	files
	{
		--"%{prj.name}/src/**.h",
		--"%{prj.name}/src/**.cpp"
		"src/**.h",
		"src/**.cpp"
	}

	includedirs
	{
-- 		"%{prj.name}/src",
-- 		"%{prj.name}/src/Layer",
-- 		"%{prj.name}/src/Event",
-- 		"%{prj.name}/src/Scene",
-- 		"%{prj.name}/src/vendor/imgui",
-- 		"%{prj.name}/src/vendor/imgui/backends",
-- 		"%{prj.name}/src/vendor/stb_image",
 		"src",
 		"src/Layer",
 		"src/Event",
 		"src/Scene",
 		"src/vendor/imgui",
 		"src/vendor/imgui/backends",
 		"src/vendor/stb_image",
	}


	links { "glfw", "GLEW", "GL" }

	filter "configurations:Debug"
		-- defines...
		symbols "On"

	filter "configurations:Release"
		-- defines...
		optimize "on"

	filter "configurations:Dist"
		-- defines...
		optimize "On"



