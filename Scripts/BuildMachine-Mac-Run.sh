#!/bin/sh

echo "Running Debug Foundation BuildMachineTest executable!"
chmod +x Binaries/Debug-mac-x86_64/BuildMachineTest/BuildMachineTest
Binaries/Debug-mac-x86_64/BuildMachineTest/BuildMachineTest

echo "Running Release Foundation BuildMachineTest executable!"
chmod +x Binaries/Release-mac-x86_64/BuildMachineTest/BuildMachineTest
Binaries/Release-mac-x86_64/BuildMachineTest/BuildMachineTest

echo "Running Distribution Foundation BuildMachineTest executable!"
chmod +x Binaries/Distribution-linux-x86_64/BuildMachineTest/BuildMachineTest
Binaries/Distribution-mac-x86_64/BuildMachineTest/BuildMachineTest