workspace "Svarog"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

--outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
outputdir = "%{cfg.buildcfg}"


--project "stargate"
--	location "./stargate"
--	kind "ConsoleApp"
--	language "C++"
--	cppdialect "C++17"
--	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
--	objdir ("build/" .. outputdir  .. "/%{prj.name}")
--	files
--	{
--		--"%{prj.name}/src/**.h",
--		--"%{prj.name}/src/**.cpp"
--		"%{prj.name}/**.h",
--		"%{prj.name}/**.cpp"
--	}
--
--	includedirs
--	{
-- 		"%{prj.name}",
-- 		"Svarog/src",
-- 		"Svarog/src/Layer",
-- 		"Svarog/src/Event",
-- 		"Svarog/src/Scene",
-- 		"Svarog/src/Renderer",
-- 		"Svarog/src/vendor/imgui",
-- 		"Svarog/src/vendor/imgui/backends",
-- 		"Svarog/src/vendor/stb_image",
--	}
--
--
--	links { "glfw", "GLEW", "GL", "Svarog", "cmdlineargs", "cfitsio" }
--
--	filter "configurations:Debug"
--		-- defines...
--		symbols "On"
--
--	filter "configurations:Release"
--		-- defines...
--		optimize "on"
--
--	filter "configurations:Dist"
--		-- defines...
--		optimize "On"


project "Sandbox"
	location "./Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("build/" .. outputdir  .. "/%{prj.name}")
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
 		"Svarog/src/Renderer",
 		"Svarog/src/vendor/imgui",
 		"Svarog/src/vendor/imgui/backends",
 		"Svarog/src/vendor/stb_image",
	}

	prebuildcommands { "cd shaders; ./to-hex-include" }
	--prebuildcommands { "cd ./%{prj.name}/shaders; ./to-hex-include" }

	links { "glfw", "GLEW", "GL", "Svarog", "cmdlineargs", "cfitsio", "yaml-cpp" }

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
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("build/" .. outputdir  .. "/%{prj.name}")

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
 		"%{prj.name}/src/Renderer",
 		"%{prj.name}/src/vendor/imgui",
 		"%{prj.name}/src/vendor/imgui/backends",
 		"%{prj.name}/src/vendor/stb_image",
	}


	links { "glfw", "GLEW", "GL", "cfitsio", "yaml-cpp" }

	filter "configurations:Debug"
		-- defines...
		symbols "On"
		defines "SV_DEBUG"

	filter "configurations:Release"
		-- defines...
		optimize "On"
		defines "SV_RELEASE"

	filter "configurations:Dist"
		-- defines...
		optimize "On"



