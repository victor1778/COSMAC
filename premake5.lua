workspace "COSMAC"
   architecture "x64"
   startproject "Sandbox"
   configurations { "Debug", "Release", "Dist" }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "COSMAC/vendor/GLFW/include"
IncludeDir["Glad"] = "COSMAC/vendor/Glad/include"
IncludeDir["ImGui"] = "COSMAC/vendor/imgui"

group "Dependencies"
	include "COSMAC/vendor/GLFW"
	include "COSMAC/vendor/Glad"
	include "COSMAC/vendor/imgui"

group ""

project "COSMAC"
	location "COSMAC"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "cpch.h"
	pchsource "COSMAC/src/cpch.cpp"

	files { "%{prj.name}/src/**.h", "%{prj.name}/src/**.cpp" }

	defines { "_CRT_SECURE_NO_WARNINGS" }

	includedirs { "%{prj.name}/vendor/spdlog/include", "%{prj.name}/src", "%{IncludeDir.GLFW}", "%{IncludeDir.Glad}", "%{IncludeDir.ImGui}" }

	links { "GLFW", "Glad", "ImGui", "opengl32.lib", "dwmapi.lib" }

	filter "system:windows"
		systemversion "latest"

		defines { "COSMAC_PLATFORM_WINDOWS", "COSMAC_BUILD_DLL", "GLFW_INCLUDE_NONE" }

		postbuildcommands { "{COPY} %{cfg.buildtarget.relpath} \"../bin/" .. outputdir .. "/Sandbox/\""}

	filter "configurations:Debug"
		defines "COSMAC_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "COSMAC_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "COSMAC_DIST"
		runtime "Release"
		optimize "on"


project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files { "%{prj.name}/src/**.h", "%{prj.name}/src/**.cpp" }

	includedirs { "COSMAC/vendor/spdlog/include", "COSMAC/src", "COSMAC/vendor" }

	links { "COSMAC" }

	filter "system:windows"
		systemversion "latest"

		defines { "COSMAC_PLATFORM_WINDOWS" }

	filter "configurations:Debug"
		defines "COSMAC_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "COSMAC_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "COSMAC_DIST"
		runtime "Release"
		optimize "on"