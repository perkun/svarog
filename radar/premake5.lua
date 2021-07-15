project "radar"
	location "./radar"
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
	}

	pchheader "svpch.h"

	includedirs
	{
 		"%{prj.name}/src",
 		"Svarog/src",
 		"Svarog/src/Layer",
 		"Svarog/src/Event",
 		"Svarog/src/Scene",
 		"Svarog/src/Renderer",
 		"Svarog/src/Compute",
 		"Svarog/src/Observations",
 		"Svarog/vendor/imgui",
 		"Svarog/vendor/imgui/backends",
 		"Svarog/vendor/stb_image",
 		"Svarog/vendor/entt",
 		"Svarog/vendor/ImGuizmo",
		"Svarog/assets/fonts"
	}


-- 	prebuildcommands { "cd shaders; ./to-hex-include" }

	links { "glfw", "GLEW", "GL", "cfitsio",
			"yaml-cpp",  "spdlog", "fmt",  "pthread", "Svarog"  }

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


