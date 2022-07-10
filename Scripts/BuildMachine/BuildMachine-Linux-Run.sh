#!/bin/sh

echo "Running Debug Foundation BuildMachineTest executable!"
chmod +x Binaries/Debug-linux-x86_64/BuildMachineTest/BuildMachineTest
Binaries/Debug-linux-x86_64/BuildMachineTest/BuildMachineTest

echo "Running Release Foundation BuildMachineTest executable!"
chmod +x Binaries/Release-linux-x86_64/BuildMachineTest/BuildMachineTest
Binaries/Release-linux-x86_64/BuildMachineTest/BuildMachineTest

echo "Running Distribution Foundation BuildMachineTest executable!"
chmod +x Binaries/Distribution-linux-x86_64/BuildMachineTest/BuildMachineTest
Binaries/Distribution-linux-x86_64/BuildMachineTest/BuildMachineTest