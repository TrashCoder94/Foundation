include "./ThirdParty/premake/premake_customization/solution_items.lua"

-- Visual Studio Solution
workspace "Foundation"
	architecture "x86_64"
	startproject "FoundationEditor"
	
	configurations
	{
		"Debug",
		"Release",
		"Distribution"
	}

	solution_items
	{
		".editorconfig"
	}

	flags
	{
		"MultiProcessorCompile"
	}

	linkoptions 
	{ 
		"-IGNORE:4006" 
	}
	
-- Folder to put Binaries and Intermediate files into
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories
IncludeDir = {}
IncludeDir["ThirdParty"] = "%{wks.location}/Foundation/ThirdParty"
IncludeDir["spdlog"] = "%{wks.location}/Foundation/ThirdParty/spdlog/include"
IncludeDir["GLFW"] = "%{wks.location}/Foundation/ThirdParty/GLFW/include"
IncludeDir["Glad"] = "%{wks.location}/Foundation/ThirdParty/Glad/include"
IncludeDir["ImGui"] = "%{wks.location}/Foundation/ThirdParty/imgui"
IncludeDir["glm"] = "%{wks.location}/Foundation/ThirdParty/glm"
IncludeDir["stb_image"] = "%{wks.location}/Foundation/ThirdParty/stb_image"
IncludeDir["yaml_cpp"] = "%{wks.location}/Foundation/ThirdParty/yaml-cpp/include"
IncludeDir["Reflection"] = "%{wks.location}/FoundationCodeGenerator/ThirdParty/Reflection"
IncludeDir["asio"] = "%{wks.location}/Foundation/ThirdParty/NetworkFramework/ThirdParty/asio-1.18.0/include"
IncludeDir["NetworkFramework"] = "%{wks.location}/Foundation/ThirdParty/NetworkFramework/NetworkCommon/include"
IncludeDir["ImGuizmo"] = "%{wks.location}/Foundation/ThirdParty/ImGuizmo"
IncludeDir["json"] = "%{wks.location}/Foundation/ThirdParty/json/include"
IncludeDir["assimp"] = "%{wks.location}/Foundation/ThirdParty/assimp/include"

-- Other premakes to use
group "Dependencies"
	include "ThirdParty/premake"
	include "Foundation/ThirdParty/GLFW"
	include "Foundation/ThirdParty/Glad"
	include "Foundation/ThirdParty/imgui"
	include "Foundation/ThirdParty/yaml-cpp"
	include "Foundation/ThirdParty/NetworkFramework"
	include "Foundation/ThirdParty/assimp"
group ""

group "FoundationEngine"
	include "BuildMachineTest"
	include "FoundationCodeGenerator"
	include "Foundation"
	include "FoundationEditor"
	include "FoundationServer"
group ""

group "ExampleProjects"
	include "Examples/Sandbox"
group ""