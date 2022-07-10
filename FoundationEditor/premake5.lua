-- Editor project which uses the engine
project "FoundationEditor"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("%{wks.location}/Binaries/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/Intermediate/" .. outputdir .. "/%{prj.name}")

	links
	{
		"Foundation"
	}
	
	files
	{
		"Source/**.h",
		"Source/**.cpp",
		"Assets/Scripts/**.h",
		"Assets/Scripts/**.cpp"
	}

	includedirs
	{
		"%{wks.location}/Foundation/Source",
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.ThirdParty}",
		"%{IncludeDir.yaml_cpp}",
		"%{IncludeDir.Reflection}",
		"%{IncludeDir.asio}",
		"%{IncludeDir.NetworkFramework}",
		"%{IncludeDir.ImGuizmo}",
		"%{IncludeDir.assimp}",
		"Assets"
	}
	
	postbuildmessage "Copying Assets to binaries folder!"
	postbuildcommands
	{
		"{COPY} %{wks.location}/Foundation/Assets %{prj.location}/Assets",
		"{COPY} %{prj.location}Assets %{cfg.targetdir}/Assets"
	}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		defines "FD_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "FD_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Distribution"
		defines "FD_DISTRIBUTION"
		runtime "Release"
		optimize "on"