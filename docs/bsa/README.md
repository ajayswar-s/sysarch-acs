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

1. Setup edk2 build directory

>	 git clone git clone --branch edk2-stable202505 --depth 1 git@github.com:tianocore/edk2.git<br>
>	 git clone https://github.com/tianocore/edk2-libc edk2/edk2-libc<br>
>	 cd edk2<br>
>	 git submodule update --init --recursive<br>

2. Download source files and apply edk2 patch
>	 git clone "ssh://ajas01@ap-gerrit-1.ap01.arm.com:29418/avk/sysarch-acs" ShellPkg/Application/sysarch-acs

3. Build bsa-acs UEFI app <br>
Note :  Install GCC-ARM 13.2 [toolchain](https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads)
>          export GCC49_AARCH64_PREFIX=<path to CC>arm-gnu-toolchain-13.2.Rel1-x86_64-aarch64-none-linux-gnu/bin/aarch64-none-linux-gnu-
>          export PACKAGES_PATH=`pwd`/edk2-libc
>          source edksetup.sh
>          make -C BaseTools/Source/C
>          source ShellPkg/Application/sysarch-acs/tools/scripts/acsbuild.sh pc_bsa

4. BSA EFI application path
- The EFI executable file is generated at <edk2-path>/Build/Shell/DEBUG_GCC49/AARCH64/PC_Bsa.efi


#### 2.2 Emulation environment with secondary storage
On an emulation environment with secondary storage, perform the following steps:

1. Create an image file which contains the '.efi' file. For example:
  - mkfs.vfat -C -n HD0 hda.img 2097152
  - sudo mount hda.img /mnt/acs/
  - sudo cp PC_Bsa.efi /mnt/acs
  - sudo umount /mnt/acs
Note: If /mnt/acs/ is not already created, you may need to create it using mkdir -p /mnt/acs/.

2. Load the image file to the secondary storage using a backdoor. The steps to load the image file are emulation environment-specific and beyond the scope of this document.
3. Boot the system to UEFI shell.
4. To determine the file system number of the secondary storage, execute 'map -r' command.
5. Type 'fs<x>' where '<x>' is replaced by the number determined in step 4.
6. To start the compliance tests, run the executable Bsa.efi with the appropriate parameters.
7. Copy the UART console output to a log file for analysis and certification

#### 2.3 Emulation environment without secondary storage

On an emulation platform where secondary storage is not available, perform the following steps:

1. Add the path to PC_bsa.efi file in the UEFI FD file.
2. Build UEFI image including the UEFI Shell.
3. Boot the system to UEFI shell.
4. Copy the UART console output to a log file for analysis and certification.

## PC BSA Linux Application

### 1.Build Steps
1. wget https://gitlab.arm.com/linux-arm/linux-acs/-/raw/master/acs-drv/files/build.sh
2. chmod +x build.sh
3. source build.sh

### 2. Build Output

The following output folder is created in __build__ folder:
(As part of build bsa and sbsa module and app also got created)
 - pcbsa_acs.ko
 - pcbsa_app

### 3. Loading the kernel module
Before the PC BSA ACS Linux application can be run, load the PC BSA ACS kernel module respectively using the insmod command.

```sh
shell> insmod pcbsa_acs.ko
```sh

### 3. Running PC BSA ACS

```sh
shell> ./pcbsa_app
```sh
### 4. PC BSA Linux Test Log View

```sh
shell> sudo dmesg | tail -500 # print last 500 kernel logs
```sh
After the run is complete, you can remove the PC BSA module from the system if it is no longer needed.

```sh
shell> sudo rmmod bsa_acs
shell> sudo rmmod sbsa_acs
```sh
- For information on the PC BSA Linux build parameters and limitation, see the README linux.

### Test suite execution on TC3 platform

## ✅ Test Suite Execution on TC3 (LSC23) FVP Platform

### 1. Follow TC3 Setup Guide

Begin by following the steps in the official [**LSC23 TC3 Setup Guide**](https://totalcompute.docs.arm.com/en/lsc23.1/totalcompute/lsc23/user-guide.html) to set up the TC3 platform and workspace.

---

### 2. Download the TC3 FVP Model

Refer to the official Arm Total Compute FVP download page:\
👉 [**TC3 Model – Arm Developer**](https://developer.arm.com/Tools%20and%20Software/Fixed%20Virtual%20Platforms/Total%20Compute%20FVPs)

---

### 3. Set Up Environment Variable

```bash
export MODEL=$PWD/FVP_TC3
```

---

### 4. Prepare ACS Disk Image

Follow the steps in [**Emulation environment with secondary-storage**](../README.md#22-emulation-environment-with-secondary-storage) to create a FAT-formatted `.img` file containing the `PC_bsa.efi` binary.

- Save the image as:
  ```bash
  acs.img
  ```

---

### 5. Update ACS Image Path in Model Run Script

Open the run script:

```bash
vi <TC_WORKSPACE>/run-scripts/common/run_model.sh
```

Find this line:

```bash
ACS_DISK_IMAGE="$DEPLOY_DIR/systemready_acs_live_image.img"
```

Replace it with:

```bash
ACS_DISK_IMAGE="$DEPLOY_DIR/acs.img"
```

---

### 6. Run the TC3 Model

Navigate to the TC3 run-script directory and launch the model:

```bash
cd <TC_WORKSPACE>/run-scripts/tc3
./run_model.sh -m $MODEL -d acs-test-suite
```

---

### 7. Run the PC BSA Test from UEFI Shell

- When the model boots, press **Esc** to enter the UEFI Boot Manager.
- Select **Built-in EFI Shell**
- Execute the following commands:

```shell
map -r          # Refresh and list file systems
fs0:            # Replace 0 with the correct disk number
PC_bsa.efi      # Launch the PC BSA test
```

---

### ✅ Notes

- Ensure the `.efi` binary is copied correctly into the image using the steps in the [secondary-storage setup section](../README.md#22-emulation-environment-with-secondary-storage).
- Capture UART output logs for test verification and reporting.



## Limitations

 - The PC BSA tests are distributed across different ACS components — including SCT tests, UEFI-based tests, and a Linux-based test. The Linux portion consists of a single test, which is part of the PC BSA app. This app can be built using the steps provided below; however, unlike BSA and SBSA, it does not have a prebuilt .img system-ready image.

Some of the PC BSA rules are also covered by SCT tests. Since the coverage is spread across different test types, you need to run all testsuites to ensure full validation. Additionally, a few test cases may require manual verification.

## License
PC BSA ACS is distributed under Apache v2.0 License.

## Feedback, contributions, and support

 - For feedback, use the GitHub Issue Tracker that is associated with this repository.
 - For support, send an email to "support-systemready-acs@arm.com" with details.
 - Arm licensees may contact Arm directly through their partner managers.
 - Arm welcomes code contributions through GitHub pull requests. See the GitHub documentation on how to raise pull requests.

--------------

*Copyright (c) 2025, Arm Limited and Contributors. All rights reserved.*

