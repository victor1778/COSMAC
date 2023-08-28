workspace "COSMAC"
   architecture "x64"
   configurations { "Debug", "Release", "Dist" }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "COSMAC/vendor/GLFW/include"
IncludeDir["Glad"] = "COSMAC/vendor/Glad/include"

include "COSMAC/vendor/GLFW"
include "COSMAC/vendor/Glad"

project "COSMAC"
	location "COSMAC"
	kind "SharedLib"
	language "C++"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "cpch.h"
	pchsource "COSMAC/src/cpch.cpp"

	files { "%{prj.name}/src/**.h", "%{prj.name}/src/**.cpp" }

	includedirs { "%{prj.name}/vendor/spdlog/include", "%{prj.name}/src", "%{IncludeDir.GLFW}", "%{IncludeDir.Glad}" }

	links { "GLFW", "Glad", "opengl32.lib", "dwmapi.lib" }

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"

		defines { "COSMAC_PLATFORM_WINDOWS", "COSMAC_BUILD_DLL", "GLFW_INCLUDE_NONE" }

		postbuildcommands { "{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox"}

	filter "configurations:Debug"
		defines "COSMAC_DEBUG"
		buildoptions "/MDd"
		symbols "on"

	filter "configurations:Release"
		defines "COSMAC_RELEASE"
		buildoptions "/MD"
		optimize "on"

	filter "configurations:Dist"
		defines "COSMAC_DIST"
		buildoptions "/MD"
		optimize "on"


project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files { "%{prj.name}/src/**.h", "%{prj.name}/src/**.cpp" }

	includedirs { "COSMAC/vendor/spdlog/include", "COSMAC/src" }

	links { "COSMAC" }

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"

		defines { "COSMAC_PLATFORM_WINDOWS" }

	filter "configurations:Debug"
		defines "COSMAC_DEBUG"
		buildoptions "/MDd"
		symbols "on"

	filter "configurations:Release"
		defines "COSMAC_RELEASE"
		buildoptions "/MD"
		optimize "on"

	filter "configurations:Dist"
		defines "COSMAC_DIST"
		buildoptions "/MD"
		optimize "on"