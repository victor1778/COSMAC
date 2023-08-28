project "Glad"
	kind "StaticLib"
	language "C"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"include/glad/glad.h",
		"include/KHR/khrplatform.h",
		"src/glad.c"
	}

	includedirs { "include" }

	filter "system:linux"
		pic "on"
		systemversion "latest"

	filter "system:macosx"
		pic "on"

	filter "system:windows"
		staticruntime "on"
		systemversion "latest"

	filter { "system:windows", "configurations:Release" }
		buildoptions "/MT"
