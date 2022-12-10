newoption {
	trigger = "rarog-ini",
	value = "PATH",
	description = "path to Rarogs ini file"
}


project "rarog"
	location "./"
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
		--"%{prj.name}/**.h",
		--"%{prj.name}/**.cpp"
	}

-- 	pchheader "svpch.h"
-- 	pchheader "%{wks.location}/Svarog/svpch.h"

	includedirs
	{
 		"src",
 		"src/Panels",
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
 		"%{wks.location}/Svarog/vendor/spdlog/include/",
 		"%{wks.location}/Svarog/vendor/cppargs/src/",
		"%{wks.location}/Svarog/assets/fonts"
	}


 	prebuildcommands { "cd %{wks.location}/rarog/shaders; ./to-hex-include" }

	links { "glfw", "GLEW", "GL", "cfitsio",
			"yaml-cpp", "pthread", "Svarog"  }

-- 	local ini_file = "SV_CONFIG_INI_FILE=\"" .. _OPTIONS["rarog-ini"] .. "\""

	filter "configurations:Debug"
		-- defines...
		symbols "On"
		defines "SV_DEBUG"
		--defines "SPDLOG_COMPILED_LIB"
		if _OPTIONS["rarog-ini"] then
			local ini_file = "SV_CONFIG_INI_FILE=\"" .. _OPTIONS["rarog-ini"] .. "\""
			defines { ini_file }
		end

	filter "configurations:Release"
		-- defines...
		optimize "On"
		defines "SV_RELEASE"
		--defines "SPDLOG_COMPILED_LIB"
		if _OPTIONS["rarog-ini"] then
			local ini_file = "SV_CONFIG_INI_FILE=\"" .. _OPTIONS["rarog-ini"] .. "\""
			defines { ini_file }
		end

	filter "configurations:Dist"
		-- defines...
		optimize "On"

