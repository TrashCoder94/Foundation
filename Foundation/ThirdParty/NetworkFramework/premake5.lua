project "NetworkFramework"
	kind "StaticLib"
	language "C++"

	targetdir ("%{wks.location}/Binaries/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/Intermediate/" .. outputdir .. "/%{prj.name}")

	files
	{
		"NetworkCommon/src/**.cpp",
		"NetworkCommon/include/**.h"
	}

	includedirs
	{
		"NetworkCommon/include",
		"ThirdParty/asio-1.18.0/include"
	}

	filter "system:windows"
		systemversion "latest"
		cppdialect "C++17"
		staticruntime "on"
		
	filter "system:linux"
		pic "On"
		systemversion "latest"
		cppdialect "C++17"
		staticruntime "on"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"