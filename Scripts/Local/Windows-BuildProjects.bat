@echo off
pushd %~dp0\..\..\

echo "Building Debug Configuration"
devenv Foundation.sln /rebuild Debug
if %errorlevel% neq 0 exit /b %errorlevel%

popd
PAUSE