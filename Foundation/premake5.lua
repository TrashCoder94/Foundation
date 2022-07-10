-- The engine Visual Studio project
project "Foundation"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	-- Binaries and Intermediate folders
	targetdir ("%{wks.location}/Binaries/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/Intermediate/" .. outputdir .. "/%{prj.name}")

	-- Precompiled header files
	pchheader "fdpch.h"
	pchsource "Source/fdpch.cpp"

	-- Which files to include in our Visual Studio project
	files
	{
		"Source/**.h",
		"Source/**.cpp",
		"ThirdParty/stb_image/**.h",
		"ThirdParty/stb_image/**.cpp",
		"ThirdParty/glm/glm/**.hpp",
		"ThirdParty/glm/glm/**.inl",
		"ThirdParty/ImGuizmo/ImGuizmo.h",
		"ThirdParty/ImGuizmo/ImGuizmo.cpp",
		"ThirdParty/Reflection/ReflectCommon.cpp",
		"ThirdParty/Reflection/Reflect.cpp",
		"%{wks.location}/Intermediate/GeneratedCode/" .. outputdir .. "/%{prj.name}/*.cpp"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		"GLFW_INCLUDE_NONE"
	}

	-- Additional include directories
	includedirs
	{
		"Source",
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.yaml_cpp}",
		"%{IncludeDir.Reflection}",
		"%{IncludeDir.asio}",
		"%{IncludeDir.NetworkFramework}",
		"%{IncludeDir.ImGuizmo}",
		"%{IncludeDir.json}",
		"%{IncludeDir.assimp}"
	}
	
	-- Linking other projects
	links
	{
		"GLFW",
		"Glad",
		"ImGui",
		"yaml-cpp",
		"NetworkFramework",
		"opengl32.lib",
		"FoundationCodeGenerator"
	}
	
	postbuildmessage "Copying assets over to the FoundationEditor"
	postbuildcommands
	{
		"{COPY} %{prj.location}/Assets %{wks.location}/FoundationEditor/Assets"
	}

	filter "files:ThirdParty/ImGuizmo/**.cpp"
		flags { "NoPCH" }

	filter "files:ThirdParty/Reflection/ReflectCommon.cpp"
		flags { "NoPCH" }

	-- Configuration setup for Windows
	filter "system:windows"
		systemversion "latest"

	-- Debug configuration, turn debug symbols on
	filter "configurations:Debug"
		targetname "Foundation_Debug"
		defines "FD_DEBUG"
		runtime "Debug"
		symbols "on"
		links { "%{wks.location}/Foundation/ThirdParty/assimp/lib/Debug/assimp-vc142-mtd.lib", "%{wks.location}/Foundation/ThirdParty/assimp/contrib/zlib/Debug/zlibstaticd.lib" }

	-- Release configuration, optimizations turned on
	filter "configurations:Release"
		targetname "Foundation_Release"
		defines "FD_RELEASE"
		runtime "Release"
		optimize "on"
		links { "%{wks.location}/Foundation/ThirdParty/assimp/lib/Release/assimp-vc142-mt.lib", "%{wks.location}/Foundation/ThirdParty/assimp/contrib/zlib/Release/zlibstatic.lib" }

	-- Distribution configuration, optimizations turned on
	filter "configurations:Distribution"
		targetname "Foundation_Distribution"
		defines "FD_DISTRIBUTION"
		runtime "Release"
		optimize "on"
		links { "%{wks.location}/Foundation/ThirdParty/assimp/lib/Release/assimp-vc142-mt.lib", "%{wks.location}/Foundation/ThirdParty/assimp/contrib/zlib/Release/zlibstatic.lib" }