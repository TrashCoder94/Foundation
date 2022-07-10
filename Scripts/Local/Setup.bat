@echo off
pushd %~dp0\..\..\

echo "Generating project files for Foundation Code Generator"
call ThirdParty\premake\Binaries\premake5.exe vs2019 FoundationCodeGenerator\premake5.lua
if %errorlevel% neq 0 exit /b %errorlevel%

echo "Generating code files from Foundation Engine"
devenv Foundation.sln /rebuild Debug /project FoundationCodeGenerator
if %errorlevel% neq 0 exit /b %errorlevel%
call Binaries\Debug-windows-x86_64\FoundationCodeGenerator\FoundationCodeGenerator.exe Foundation\Source\
if %errorlevel% neq 0 exit /b %errorlevel%

echo "Generating assimp project using CMake"
cmake Foundation\ThirdParty\assimp\CMakeLists.txt -BFoundation\ThirdParty\assimp\ -G"Visual Studio 16 2019"
if %errorlevel% neq 0 exit /b %errorlevel%
devenv Foundation\ThirdParty\assimp\Assimp.sln /rebuild Debug /project ALL_BUILD
if %errorlevel% neq 0 exit /b %errorlevel%

echo "Generating Projects with Premake"
call ThirdParty\premake\Binaries\premake5.exe vs2019
devenv Foundation.sln /rebuild Debug /project Premake
if %errorlevel% neq 0 exit /b %errorlevel%

echo "Building Debug Configuration"
devenv Foundation.sln /rebuild Debug
if %errorlevel% neq 0 exit /b %errorlevel%

popd