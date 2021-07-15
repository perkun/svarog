newoption {
	trigger = "fonts-path",
	value = "PATH",
	description = "path to fonts folder"
}

project "Svarog"
	location "./Svarog"
	--kind "ConsoleApp"
	kind "StaticLib"

	language "C++"
	cppdialect "C++17"
	buildoptions { "-pthread" }

	--targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	--objdir ("build/" .. outputdir .. "/%{prj.name}")
	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/build/" .. outputdir  .. "/%{prj.name}")

	pchheader "src/svpch.h"

	files
	{
		"src/**.h",
		"src/**.cpp",
		"vendor/stb_image/**.h",
		"vendor/stb_image/**.cpp",
		"vendor/imgui/**.h",
		"vendor/imgui/**.cpp",
		"vendor/ImGuizmo/ImGuizmo.h",
		"vendor/ImGuizmo/ImGuizmo.cpp",
		"vendor/cppargs/**.h",
		"vendor/cppargs/**.cpp",
		--"src/**.h",
		--"src/**.cpp"
	}

	includedirs
	{
 		"src",
 		"src/Layer",
 		"src/Event",
 		"src/Scene",
 		"src/Renderer",
 		"src/Compute",
 		"src/Observations",
 		"vendor/imgui",
 		"vendor/imgui/backends",
 		"vendor/stb_image",
 		"vendor/spdlog",
 		"vendor/entt",
 		"vendor/ImGuizmo",
 		"vendor/cppargs",
	}



	links { "glfw", "GLEW", "GL", "cfitsio", "yaml-cpp", "pthread", "spdlog" }

	if not _OPTIONS["fonts-path"] then
		_OPTIONS["fonts-path"] = _WORKING_DIR .. "/Svarog/assets/fonts"
	end
	local fonts_path = _OPTIONS["fonts-path"]
	local fonts_def = "SV_CONFIG_FONT_PATH=\"" .. fonts_path .. "\""

	printf("%s\n", fonts_def)

	filter "configurations:Debug"
		-- defines...
		symbols "On"
		defines "SV_DEBUG"
		defines "SPDLOG_COMPILED_LIB"
		defines { fonts_def }

	filter "configurations:Release"
		-- defines...
		optimize "On"
		defines "SV_RELEASE"
		defines "SPDLOG_COMPILED_LIB"
		defines { fonts_def }

	filter "configurations:Dist"
		-- defines...
		optimize "On"
		defines "SPDLOG_COMPILED_LIB"
