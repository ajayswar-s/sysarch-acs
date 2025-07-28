# Personal Computing Base System Architecture - Architecture Compliance Suite

## Personal Computing Base System Architecture
**PC Base System Architecture** (PC-BSA) specifies a standard hardware system architecture for Personal Computers (PCs) that are based on the Arm 64-bit Architecture. PC system software, for example operating systems, hypervisors, and firmware can rely on this standard system architecture. PC-BSA extends the requirements specified in the Arm BSA

For more information, download the [PC-BSA specification](https://developer.arm.com/documentation/den0151/latest).

## Release details
 - Code Quality: Beta
 - The tests are written for version 1.0 of the PC BSA specification.
 - For more details on tests implemented in this release, Please refer [PC-BSA Test Scenario Document](docs/arm_pc-bsa_architecture_compliance_test_scenario.pdf).

## PC BSA build steps - UEFI Shell application

### Prerequisites
Before starting the build, ensure that the following requirements are met.

- Any mainstream Linux based OS distribution running on a x86 or AArch64 machine.
- Install GCC-ARM 13.2 [toolchain](https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads).
- Install the build prerequisite packages to build EDK2.
Note: The details of the packages are beyond the scope of this document.

### 1. Build Steps

1.  git clone the commit 836942fbadb629050b866a8052e6af755bcdf623 of EDK2 tree
> git clone --recursive https://github.com/tianocore/edk2
2.  git clone the EDK2 port of libc
> git clone https://github.com/tianocore/edk2-libc edk2/edk2-libc
3. git clone bsa-acs
> git clone "ssh://$USER@ap-gerrit-1.ap01.arm.com:29418/avk/syscomp_bsa" edk2/ShellPkg/Application/bsa-acs
4. git clone sbsa-acs
> git clone "ssh://$USER@ap-gerrit-1.ap01.arm.com:29418/avk/syscomp_sbsa" edk2/ShellPkg/Application/sbsa-acs
5.  Add the following to the [LibraryClasses.common] section in edk2/ShellPkg/ShellPkg.dsc
> PCBsaValLib|ShellPkg/Application/bsa-acs/val/PCBsaValLib.inf

> PCBsaPalLib|ShellPkg/Application/bsa-acs/pal/uefi_acpi/PCBsaPalLib.inf
6.  Add the following to the [components] section of edk2/ShellPkg/ShellPkg.dsc
> ShellPkg/Application/bsa-acs/pc_bsa/uefi_app/PCBsaAcs.inf



## Limitations

 - Since this is a Alpha quality release, contains limited number of tests based on PC BSA Specification.

## License
PC BSA ACS is distributed under Apache v2.0 License.

## Feedback, contributions, and support

 - For feedback, use the GitHub Issue Tracker that is associated with this repository.
 - For support, send an email to "support-systemready-acs@arm.com" with details.
 - Arm licensees may contact Arm directly through their partner managers.
 - Arm welcomes code contributions through GitHub pull requests. See the GitHub documentation on how to raise pull requests.

--------------

*Copyright (c) 2024, Arm Limited and Contributors. All rights reserved.*
