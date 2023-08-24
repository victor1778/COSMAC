workspace "COSMAC"
   architecture "x64"
   configurations { "Debug", "Release", "Dist" }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "COSMAC/vendor/GLFW/include"

include "COSMAC/vendor/GLFW"

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

	includedirs { "%{prj.name}/vendor/spdlog/include", "%{prj.name}/src", "%{IncludeDir.GLFW}" }

	links { "GLFW", "opengl32.lib", "dwmapi.lib" }

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"

		defines { "COSMAC_PLATFORM_WINDOWS", "COSMAC_BUILD_DLL" }

		postbuildcommands { "{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox"}

	filter "configurations:Debug"
		defines "COSMAC_DEBUG"
		symbols "on"

	filter "configurations:Release"
		defines "COSMAC_RELEASE"
		optimize "on"

	filter "configurations:Dist"
		defines "COSMAC_DIST"
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
		symbols "on"

	filter "configurations:Release"
		defines "COSMAC_RELEASE"
		optimize "on"

	filter "configurations:Dist"
		defines "COSMAC_DIST"
		optimize "on"