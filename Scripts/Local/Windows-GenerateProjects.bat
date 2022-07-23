@echo off
pushd %~dp0\..\..\

echo "Generating project files for Foundation Code Generator"
call ThirdParty\premake\Binaries\premake5.exe vs2019 FoundationCodeGenerator\premake5.lua
if %errorlevel% neq 0 exit /b %errorlevel%

echo "Generating debug code files from Foundation Engine"
devenv Foundation.sln /rebuild Debug /project FoundationCodeGenerator
if %errorlevel% neq 0 exit /b %errorlevel%
call Binaries\Debug-windows-x86_64\FoundationCodeGenerator\FoundationCodeGenerator.exe Foundation\Source\
if %errorlevel% neq 0 exit /b %errorlevel%

echo "Generating release code files from Foundation Engine"
devenv Foundation.sln /rebuild Release /project FoundationCodeGenerator
if %errorlevel% neq 0 exit /b %errorlevel%
call Binaries\Release-windows-x86_64\FoundationCodeGenerator\FoundationCodeGenerator.exe Foundation\Source\
if %errorlevel% neq 0 exit /b %errorlevel%

echo "Generating distribution code files from Foundation Engine"
devenv Foundation.sln /rebuild Distribution /project FoundationCodeGenerator
if %errorlevel% neq 0 exit /b %errorlevel%
call Binaries\Distribution-windows-x86_64\FoundationCodeGenerator\FoundationCodeGenerator.exe Foundation\Source\
if %errorlevel% neq 0 exit /b %errorlevel%

echo "Generating Projects with Premake"
call ThirdParty\premake\Binaries\premake5.exe vs2019
devenv Foundation.sln /rebuild Debug /project Premake
if %errorlevel% neq 0 exit /b %errorlevel%

popd