newoption {
	trigger = "meshlc-ini",
	value = "PATH",
	description = "path to meshlc ini file"
}



project "meshlc"
	location "./meshlc"
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
 		"%{wks.location}/Svarog/vendor/cppargs/src",
		"%{wks.location}/Svarog/assets/fonts"
	}


 	prebuildcommands { "cd %{wks.location}/meshlc/shaders; ./to-hex-include" }


	links { "glfw", "GLEW", "GL", "cfitsio",
			"yaml-cpp",  "spdlog", "fmt",  "pthread", "Svarog"  }

	filter "configurations:Debug"
		-- defines...
		symbols "On"
		defines "SV_DEBUG"
		defines "SPDLOG_COMPILED_LIB"
		if _OPTIONS["meshlc-ini"] then
			local ini_file = "SV_CONFIG_INI_FILE=\"" .. _OPTIONS["meshlc-ini"] .. "\""
			defines { ini_file }
		end

	filter "configurations:Release"
		-- defines...
		optimize "On"
		defines "SV_RELEASE"
		defines "SPDLOG_COMPILED_LIB"
		if _OPTIONS["meshlc-ini"] then
			local ini_file = "SV_CONFIG_INI_FILE=\"" .. _OPTIONS["meshlc-ini"] .. "\""
			defines { ini_file }
		end

	filter "configurations:Dist"
		-- defines...
		optimize "On"


