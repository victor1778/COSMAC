workspace "COSMAC"
   architecture "x64"
   configurations { "Debug", "Release", "Dist" }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "COSMAC"
	location "COSMAC"
	kind "SharedLib"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files { "%{prj.name}/src/**.h", "%{prj.name}/src/**.cpp" }

	includedirs { "%{prj.name}/vendor/spdlog/include", "%{prj.name}/src" }

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines { "COSMAC_PLATFORM_WINDOWS", "COSMAC_BUILD_DLL" }

		postbuildcommands { "{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox"}

	filter "configurations:Debug"
		defines "COSMAC_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "COSMAC_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "COSMAC_DIST"
		optimize "On"


project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files { "%{prj.name}/src/**.h", "%{prj.name}/src/**.cpp" }

	includedirs { "COSMAC/vendor/spdlog/include", "COSMAC/src" }

	links { "COSMAC" }

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines { "COSMAC_PLATFORM_WINDOWS" }

	filter "configurations:Debug"
		defines "COSMAC_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "COSMAC_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "COSMAC_DIST"
		optimize "On"