-- Code generator project which will scan engine/project files
project "FoundationCodeGenerator"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("%{wks.location}/Binaries/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/Intermediate/" .. outputdir .. "/%{prj.name}")

	files
	{
		"Source/**.h",
		"Source/**.cpp",
		"ThirdParty/Reflection/**.h",
		"ThirdParty/Reflection/**.cpp"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}

	-- Additional include directories
	includedirs
	{
		"%{IncludeDir.Reflection}"
	}

	postbuildmessage "Copying ReflectCommon.cpp over to the engine!"
	postbuildcommands
	{
		"{COPY} %{prj.location}ThirdParty/Reflection/ReflectCommon.cpp %{wks.location}/Foundation/ThirdParty/Reflection/"
	}

	filter "files:ThirdParty/Reflection/**.cpp"
		flags { "NoPCH" }

	-- Configuration setup for Windows
	filter "system:windows"
		systemversion "latest"

	-- Debug configuration, turn debug symbols on
	filter "configurations:Debug"
		defines "FD_DEBUG"
		runtime "Debug"
		symbols "on"

	-- Release configuration, optimizations turned on
	filter "configurations:Release"
		defines "FD_RELEASE"
		runtime "Release"
		optimize "on"

	-- Distribution configuration, optimizations turned on
	filter "configurations:Distribution"
		defines "FD_DISTRIBUTION"
		runtime "Release"
		optimize "on"