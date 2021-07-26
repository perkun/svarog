project "zadanie_testowe"
	location "./zadanie_testowe"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	--buildoptions { "-pthread", "-v", "-ftime-report" }
	buildoptions { "-pthread"}
	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/build/" .. outputdir  .. "/%{prj.name}")
	files
	{
		"src/**.h",
		"src/**.cpp"
	}

-- 	pchheader "svpch.h"
-- 	pchheader "%{wks.location}/Svarog/svpch.h"

	includedirs
	{
 		"src",
 		"%{wks.location}/Svarog/src",
 		"%{wks.location}/Svarog/src/Layer",
 		"%{wks.location}/Svarog/src/Event",
 		"%{wks.location}/Svarog/src/Scene",
 		"%{wks.location}/Svarog/src/Renderer",
 		"%{wks.location}/Svarog/src/Compute",
 		"%{wks.location}/Svarog/src/Observations",
 		"%{wks.location}/Svarog/vendor/imgui",
 		"%{wks.location}/Svarog/vendor/imgui/backends",
 		"%{wks.location}/Svarog/vendor/stb_image",
 		"%{wks.location}/Svarog/vendor/entt",
 		"%{wks.location}/Svarog/vendor/ImGuizmo",
 		"%{wks.location}/Svarog/vendor/spdlog/include/spdlog",
 		"%{wks.location}/Svarog/vendor/cppargs",
		"%{wks.location}/Svarog/assets/fonts"
	}


 	prebuildcommands { "cd %{wks.location}/meshview/shaders; ./to-hex-include" }

	links { "glfw", "GLEW", "GL",  "cfitsio",
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


