# Personal Computing Base System Architecture - Architecture Compliance Suite

## Personal Computing Base System Architecture
**PC Base System Architecture** (PC-BSA) specifies a standard hardware system architecture for Personal Computers (PCs) that are based on the Arm 64-bit Architecture. PC system software, for example operating systems, hypervisors, and firmware can rely on this standard system architecture. PC-BSA extends the requirements specified in the Arm BSA

For more information, download the [PC-BSA specification](https://developer.arm.com/documentation/den0151/latest).

## Release details
 - Code Quality: Alpha
 - The tests are written for version 1.0 of the PC BSA specification.
 - For more details on tests implemented in this release, Please refer [PC-BSA Test Scenario Document](docs/arm_pc-bsa_architecture_compliance_test_scenario.pdf).

## GitHub branch
  - To pick up the release version of the code, checkout the corresponding tag from the main branch.
  - To get the latest version of the code with bug fixes and new features, use the main branch.

## ACS build steps - UEFI Shell application

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

### 1.1 On Linux build environment, perform the following steps:
- On x86 machine
> wget https://developer.arm.com/-/media/Files/downloads/gnu/13.2.rel1/binrel/arm-gnu-toolchain-13.2.rel1-x86_64-aarch64-none-linux-gnu.tar.xz

> tar -xf arm-gnu-toolchain-13.2.rel1-x86_64-aarch64-none-linux-gnu.tar.xz

> export GCC49_AARCH64_PREFIX= GCC 13.2 toolchain path pointing to arm-gnu-toolchain-13.2.rel1-x86_64-aarch64-none-linux-gnu/bin/aarch64-none-linux-gnu-

- On AArch64 machine
> export GCC49_AARCH64_PREFIX=/usr/bin

- From inside the edk2 directory in console

> export PACKAGES_PATH=$PWD/edk2-libc

> source edksetup.sh

> make -C BaseTools/Source/C

> source ShellPkg/Application/bsa-acs/tools/scripts/acsbuild.sh ENABLE_PCBSA
### 2. Build output

The EFI executable file is generated at <edk2_path>/Build/Shell/DEBUG_GCC49/AARCH64/PC_Bsa.efi

### 3. Execution Steps

The execution of the compliance suite varies depending on the test environment.
These steps assume that the test suite is invoked through the ACS UEFI shell application.

#### Firmware Dependencies
- If the system supports LPI’s (Interrupt ID > 8192), then Firmware should have support for installing handler for LPI interrupts.
    - If you are using edk2, change the ArmGic driver in the ArmPkg to obtain support for installing handler for LPI’s.
    - Add the following in edk2/ArmPkg/Drivers/ArmGic/GicV3/ArmGicV3Dxe.c
>        - After [#define ARM_GIC_DEFAULT_PRIORITY  0x80]
>          +#define ARM_GIC_MAX_NUM_INTERRUPT 16384
>        - Change this in GicV3DxeInitialize Function.
>          -mGicNumInterrupts      = ArmGicGetMaxNumInterrupts (mGicDistributorBase);
>          +mGicNumInterrupts      = ARM_GIC_MAX_NUM_INTERRUPT;

### 3.1 On Silicon
On a system where a USB port is available and functional, perform the following steps:

1. Copy 'PC_Bsa.efi' to a USB Flash drive. Path for 'PC_Bsa.efi' is present in step 2.
2. Plug in the USB Flash drive to one of the functional USB ports on the system.
3. Boot the system to UEFI shell.
4. To determine the file system number of the plugged in USB drive, execute 'map -r' command.
5. Type 'fsx' where 'x' is replaced by the number determined in step 4.
6. To start the compliance tests, run the executable PC_Bsa.efi with the appropriate parameters.
   For details on the parameters, refer to [arm BSA User Guide Document](../docs/arm_bsa_architecture_compliance_user_guide.pdf)
> shell> PC_Bsa.efi
7. Copy the UART console output to a log file for analysis and certification.


### 3.2 Emulation environment with secondary storage
On an emulation environment with secondary storage, perform the following steps:

1. Create an image file which contains the 'PC_Bsa.efi' file. For Example:
  - mkfs.vfat -C -n HD0 \<name of image\>.img 2097152.
    Here 2097152 is the size of the image.
  - sudo mount -o rw,loop=/dev/loop0,uid=`whoami`,gid=`whoami` \<name of image\>.img /mnt/pcbsa.
    If loop0 is busy, specify the loop that is free
  - cp  "\<path to application\>/PC_Bsa.efi" /mnt/pcbsa/
  - sudo umount /mnt/pcbsa
2. Load the image file to the secondary storage using a backdoor.
   The steps followed to load the image file are emulation environment-specific and beyond the scope of this document.
3. Boot the system to UEFI shell.
4. To determine the file system number of the secondary storage, execute 'map -r' command.
5. Type 'fsx' where 'x' is replaced by the number determined in step 4.
6. To start the compliance tests, run the executable PC_Bsa.efi with the appropriate parameters.
   For details on the parameters, see the [arm BSA User Guide Document](../docs/arm_bsa_architecture_compliance_user_guide.pdf)
> shell> PC_Bsa.efi
7. Copy the UART console output to a log file for analysis and certification.


### 3.3 Emulation environment without secondary storage
On an emulation platform where secondary storage is not available, perform the following steps:

1. Add the path to 'PC_Bsa.efi' file in the UEFI FD file.
2. Build UEFI image including the UEFI Shell.
3. Boot the system to UEFI shell.
4. Run the executable 'PC_Bsa.efi' to start the compliance tests.
   For details about the parameters, see the [arm BSA User Guide Document](../docs/arm_bsa_architecture_compliance_user_guide.pdf).
> shell> PC_Bsa.efi
5. Copy the UART console output to a log file for analysis and certification.

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
