@echo off
pushd %~dp0\..\

@set MESA_GL_VERSION_OVERRIDE=4.5
@set MESA_GLSL_VERSION_OVERRIDE=410

set BinariesDirectory=Binaries\Debug-windows-x86_64\BuildMachineTest\

cd %BinariesDirectory%
echo "Running Debug Foundation BuildMachineTest executable!"
call BuildMachineTest.exe
if %errorlevel% neq 0 exit /b %errorlevel%

:: echo "Running Release Foundation BuildMachineTest executable!"
:: call Binaries\Release-windows-x86_64\BuildMachineTest\BuildMachineTest.exe
:: if %errorlevel% neq 0 exit /b %errorlevel%

:: echo "Running Distribution Foundation BuildMachineTest executable!"
:: call Binaries\Distribution-windows-x86_64\BuildMachineTest\BuildMachineTest.exe
:: if %errorlevel% neq 0 exit /b %errorlevel%

popd