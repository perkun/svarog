workspace "Svarog"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Sandbox"
	location "./Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	targetdir ("bin/")
	objdir ("build/%{prj.name}/%{cfg.buildcfg}")
	files
	{
		--"%{prj.name}/src/**.h",
		--"%{prj.name}/src/**.cpp"
		"%{prj.name}/**.h",
		"%{prj.name}/**.cpp"
	}

	includedirs
	{
 		"%{prj.name}",
 		"Svarog/src",
 		"Svarog/src/Layer",
 		"Svarog/src/Event",
 		"Svarog/src/Scene",
 		"Svarog/src/vendor/imgui",
 		"Svarog/src/vendor/imgui/backends",
 		"Svarog/src/vendor/stb_image",
	}


	links { "glfw", "GLEW", "GL", "Svarog" }

	filter "configurations:Debug"
		-- defines...
		symbols "On"

	filter "configurations:Release"
		-- defines...
		optimize "on"

	filter "configurations:Dist"
		-- defines...
		optimize "On"


project "Svarog"
	location "./Svarog"
	--kind "ConsoleApp"
	kind "StaticLib"

	language "C++"
	cppdialect "C++17"

	--targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	--objdir ("build/" .. outputdir .. "/%{prj.name}")
	targetdir ("bin/")
	objdir ("build/%{prj.name}/%{cfg.buildcfg}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
		--"src/**.h",
		--"src/**.cpp"
	}

	includedirs
	{
 		"%{prj.name}/src",
 		"%{prj.name}/src/Layer",
 		"%{prj.name}/src/Event",
 		"%{prj.name}/src/Scene",
 		"%{prj.name}/src/vendor/imgui",
 		"%{prj.name}/src/vendor/imgui/backends",
 		"%{prj.name}/src/vendor/stb_image",
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



