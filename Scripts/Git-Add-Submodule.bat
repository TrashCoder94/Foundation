@echo off
pushd %~dp0\..\

set /P repo=What git repo do you want to add to Foundation/ThirdParty (please reply with .git link not just the normal url)?
set /P folderName=What do you want to call the parent folder?

echo Adding %repo% to Foundation/ThirdParty/%folderName%/
git submodule add %repo% Foundation/ThirdParty/%folderName%/

popd
PAUSE