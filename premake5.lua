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
-- 		"Svarog/src/Compute",
-- 		"Svarog/src/vendor/imgui",
-- 		"Svarog/src/vendor/imgui/backends",
-- 		"Svarog/src/vendor/stb_image",
--	}
--
--
----	prebuildcommands { "cd shaders; ./to-hex-include" }
--
--	links { "glfw", "GLEW", "GL", "Svarog", "cmdlineargs", "cfitsio", "yaml-cpp"  }
--
--	filter "configurations:Debug"
--		-- defines...
--		symbols "On"
--		defines "SV_DEBUG"
--
--	filter "configurations:Release"
--		-- defines...
--		optimize "on"
--		defines "SV_RELEASE"
--
--	filter "configurations:Dist"
--		-- defines...
--		optimize "On"


project "rarog"
	location "./rarog"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	--buildoptions { "-pthread", "-v", "-ftime-report" }
	buildoptions { "-pthread"}
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("build/" .. outputdir  .. "/%{prj.name}")
	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
		--"%{prj.name}/**.h",
		--"%{prj.name}/**.cpp"
	}

	pchheader "svpch.h"

	includedirs
	{
 		"%{prj.name}/src",
 		"%{prj.name}/src/Panels",
 		"Svarog/src",
 		"Svarog/src/Layer",
 		"Svarog/src/Event",
 		"Svarog/src/Scene",
 		"Svarog/src/Renderer",
 		"Svarog/src/Compute",
 		"Svarog/vendor/imgui",
 		"Svarog/vendor/imgui/backends",
 		"Svarog/vendor/stb_image",
 		"Svarog/vendor/entt",
 		"Svarog/vendor/ImGuizmo",
	}


	prebuildcommands { "cd shaders; ./to-hex-include" }

	links { "glfw", "GLEW", "GL", "cmdlineargs", "cfitsio",
			"yaml-cpp",  "spdlog",  "pthread", "Svarog"  }

	filter "configurations:Debug"
		-- defines...
		symbols "On"
		defines "SV_DEBUG"
		defines "SPDLOG_COMPILED_LIB"

	filter "configurations:Release"
		-- defines...
		optimize "on"
		defines "SV_RELEASE"
		defines "SPDLOG_COMPILED_LIB"

	filter "configurations:Dist"
		-- defines...
		optimize "On"



--project "Sandbox"
--	location "./Sandbox"
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
-- 		"Svarog/src/Compute",
-- 		"Svarog/src/vendor/imgui",
-- 		"Svarog/src/vendor/imgui/backends",
-- 		"Svarog/src/vendor/stb_image",
--	}
--
--	--prebuildcommands { "cd shaders; ./to-hex-include" }
--	--prebuildcommands { "cd ./%{prj.name}/shaders; ./to-hex-include" }
--
--	links { "glfw", "GLEW", "GL", "Svarog", "cmdlineargs", "cfitsio", "yaml-cpp" }
--
--	filter "configurations:Debug"
--		-- defines...
--		symbols "On"
--		defines "SV_DEBUG"
--
--	filter "configurations:Release"
--		-- defines...
--		optimize "on"
--		defines "SV_RELEASE"
--
--	filter "configurations:Dist"
--		-- defines...
--		optimize "On"



project "Svarog"
	location "./Svarog"
	--kind "ConsoleApp"
	kind "StaticLib"

	language "C++"
	cppdialect "C++17"
	buildoptions { "-pthread" }

	--targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	--objdir ("build/" .. outputdir .. "/%{prj.name}")
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("build/" .. outputdir  .. "/%{prj.name}")

	pchheader "svpch.h"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/stb_image/**.h",
		"%{prj.name}/vendor/stb_image/**.cpp",
		"%{prj.name}/vendor/imgui/**.h",
		"%{prj.name}/vendor/imgui/**.cpp",
		"%{prj.name}/vendor/ImGuizmo/ImGuizmo.h",
		"%{prj.name}/vendor/ImGuizmo/ImGuizmo.cpp"
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
 		"%{prj.name}/src/Compute",
 		"%{prj.name}/vendor/imgui",
 		"%{prj.name}/vendor/imgui/backends",
 		"%{prj.name}/vendor/stb_image",
 		"%{prj.name}/vendor/spdlog",
 		"%{prj.name}/vendor/entt",
 		"%{prj.name}/vendor/ImGuizmo",
	}


	links { "glfw", "GLEW", "GL", "cfitsio", "yaml-cpp", "pthread", "spdlog" }

	filter "configurations:Debug"
		-- defines...
		symbols "On"
		defines "SV_DEBUG"
		defines "SPDLOG_COMPILED_LIB"

	filter "configurations:Release"
		-- defines...
		optimize "On"
		defines "SV_RELEASE"
		defines "SPDLOG_COMPILED_LIB"

	filter "configurations:Dist"
		-- defines...
		optimize "On"
		defines "SPDLOG_COMPILED_LIB"



