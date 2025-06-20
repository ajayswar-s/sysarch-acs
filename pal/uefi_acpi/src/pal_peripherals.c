/** @file
 * Copyright (c) 2016-2018, 2020-2025, Arm Limited or its affiliates. All rights reserved.
 * SPDX-License-Identifier : Apache-2.0

 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
**/

#include <Uefi.h>
#include <PiDxe.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DxeServicesTableLib.h>

#include <Protocol/AcpiTable.h>
#include "Include/IndustryStandard/Acpi61.h"
#include "Include/IndustryStandard/SerialPortConsoleRedirectionTable.h"

#include "include/pal_uefi.h"
#include "include/pcie_enum.h"
#include "../include/platform_override.h"

#define USB_CLASSCODE   0x0C0300
#define SATA_CLASSCODE  0x010600
#define BAR0            0
#define BAR1            1
#define BAR2            2

UINT32 spcr_baudrate_id[] = {0, 0, 0, 9600, 19200, 0, 57600, 115200};
UINT64
pal_get_spcr_ptr();

/**
  @brief  This API fills in the PERIPHERAL_INFO_TABLE with information about peripherals
          in the system. This is achieved by parsing the ACPI - SPCR table and PCIe config space.

  @param  peripheralInfoTable  - Address where the Peripheral information needs to be filled.

  @return  None
**/
VOID
pal_peripheral_create_info_table(PERIPHERAL_INFO_TABLE *peripheralInfoTable)
{
  UINT32   DeviceBdf = 0;
  UINT32   StartBdf  = 0;
  UINT32   bar_index = 0;
  PERIPHERAL_INFO_BLOCK *per_info = NULL;
  EFI_ACPI_SERIAL_PORT_CONSOLE_REDIRECTION_TABLE *spcr = NULL;

  if (peripheralInfoTable == NULL) {
    acs_print(ACS_PRINT_ERR,
              L" Input Peripheral Table Pointer is NULL. Cannot create Peripheral INFO\n");
    return;
  }

  per_info = peripheralInfoTable->info;

  peripheralInfoTable->header.num_usb = 0;
  peripheralInfoTable->header.num_sata = 0;
  peripheralInfoTable->header.num_uart = 0;
  peripheralInfoTable->header.num_all = 0;
  per_info->base0 = 0;

  /* check for any USB Controllers */
  do {

       DeviceBdf = palPcieGetBdf(USB_CLASSCODE, StartBdf);
       if (DeviceBdf != 0) {
          per_info->type  = PERIPHERAL_TYPE_USB;
          for (bar_index = 0; bar_index < TYPE0_MAX_BARS; bar_index++)
          {
              per_info->base0 = palPcieGetBase(DeviceBdf, bar_index);
              if (per_info->base0 != 0)
                  break;
          }
          per_info->bdf   = DeviceBdf;
          per_info->platform_type = PLATFORM_TYPE_ACPI;
          acs_print(ACS_PRINT_INFO, L"  Found a USB controller %4x\n",
                                                            per_info->base0);
          peripheralInfoTable->header.num_usb++;
          peripheralInfoTable->header.num_all++;
          per_info++;
       }
       StartBdf = incrementBusDev(DeviceBdf);

  } while (DeviceBdf != 0);

  StartBdf = 0;
  /* check for any SATA Controllers */
  do {

       DeviceBdf = palPcieGetBdf(SATA_CLASSCODE, StartBdf);
       if (DeviceBdf != 0) {
          per_info->type  = PERIPHERAL_TYPE_SATA;
          for (bar_index = 0; bar_index < TYPE0_MAX_BARS; bar_index++)
          {
              per_info->base0 = palPcieGetBase(DeviceBdf, bar_index);
              if (per_info->base0 != 0)
                  break;
          }
          per_info->bdf   = DeviceBdf;
          per_info->platform_type = PLATFORM_TYPE_ACPI;
          acs_print(ACS_PRINT_INFO, L"  Found a SATA controller %4x\n",
                                                            per_info->base0);
          peripheralInfoTable->header.num_sata++;
          peripheralInfoTable->header.num_all++;
          per_info++;
       }
       //Increment and check if we have more controllers
       StartBdf = incrementBusDev(DeviceBdf);

  } while (DeviceBdf != 0);

  /* Search for a SPCR table in the system to get the UART details */
  spcr = (EFI_ACPI_SERIAL_PORT_CONSOLE_REDIRECTION_TABLE *)pal_get_spcr_ptr();

  if (spcr) {
    peripheralInfoTable->header.num_uart++;
    peripheralInfoTable->header.num_all++;
    per_info->base0 = spcr->BaseAddress.Address;
    per_info->width = 1 << (spcr->BaseAddress.AccessSize + 2);  // Convert GAS to 8/16/32
    per_info->irq   = spcr->GlobalSystemInterrupt;
    per_info->type  = PERIPHERAL_TYPE_UART;
    per_info->bdf   = 0;

    if (spcr->BaudRate < 8)
        per_info->baud_rate = spcr_baudrate_id[spcr->BaudRate];
    else
         per_info->baud_rate = 0;

    per_info->interface_type = spcr->InterfaceType;
    per_info++;
  }
  else {
    acs_print(ACS_PRINT_DEBUG, L"  WARNING:SPCR acpi table not found\n", 0);
    acs_print(ACS_PRINT_DEBUG, L"  UEFI console setting must be set to serial\n", 0);
  }

  if (PLATFORM_GENERIC_UART_BASE) {
    peripheralInfoTable->header.num_uart++;
    peripheralInfoTable->header.num_all++;
    per_info->base0 = PLATFORM_GENERIC_UART_BASE;
    per_info->irq   = PLATFORM_GENERIC_UART_INTID;
    per_info->type  = PERIPHERAL_TYPE_UART;
    per_info++;
  }

  per_info->type = 0xFF; //indicate end of table

}


/**
  @brief  Check if the memory type is reserved for UEFI

  @param  EFI_MEMORY_TYPE  - Type of UEFI memory.

  @return  true   if memory reserved for UEFI usage
           false  otherwise
**/
BOOLEAN
IsUefiMemory(EFI_MEMORY_TYPE type)
{

  switch(type) {
    case  EfiReservedMemoryType:
    case  EfiLoaderCode:
    case  EfiLoaderData:
    case  EfiBootServicesCode:
    case  EfiBootServicesData:
    case  EfiRuntimeServicesCode:
    case  EfiRuntimeServicesData:
    case  EfiACPIReclaimMemory:
    case  EfiACPIMemoryNVS:
      return TRUE;
    default:
      return FALSE;
  }

}

/**
  @brief  Check if the memory type is normal

  @param  EFI_MEMORY_TYPE  - Type of UEFI memory.

  @return  true   if memory is normal
           false  otherwise
**/
BOOLEAN
IsNormalMemory(EFI_MEMORY_TYPE type)
{

  switch(type) {
    case EfiConventionalMemory:
      return TRUE;
    default:
      return FALSE;
  }

}

/**
  @brief  Check if the memory type is device

  @param  EFI_MEMORY_TYPE  - Type of UEFI memory.

  @return  true   if memory is device
           false  otherwise
**/
BOOLEAN
IsDeviceMemory(EFI_MEMORY_TYPE type)
{

  switch(type) {
    case  EfiMemoryMappedIO:
    case  EfiMemoryMappedIOPortSpace:
      return TRUE;
    default:
      return FALSE;
  }
}

/**
  @brief  Check if the memory type is persistent

  @param  EFI_MEMORY_TYPE  - Type of UEFI memory.

  @return  true   if memory is persistent
           false  otherwise
**/
BOOLEAN
IsPersistentMemory(EFI_MEMORY_TYPE type)
{

  switch(type) {
    case  EfiPersistentMemory:
      return TRUE;
    default:
      return FALSE;
  }
}


/**
  @brief  This API fills in the MEMORY_INFO_TABLE with information about memory in the
          system. This is achieved by parsing the UEFI memory map.

  @param  memory_info_table Address where the memory info table is created

  @return  None
**/
VOID
pal_memory_create_info_table(MEMORY_INFO_TABLE *memoryInfoTable)
{

  UINTN                 MemoryMapSize;
  EFI_MEMORY_DESCRIPTOR *MemoryMap = NULL;
  EFI_MEMORY_DESCRIPTOR *MemoryMapPtr = NULL;
  EFI_PHYSICAL_ADDRESS  Address;
  UINTN                 MapKey;
  UINTN                 DescriptorSize;
  UINT32                DescriptorVersion;
  UINTN                 Pages;
  EFI_STATUS            Status;
  UINT32                Index, i = 0;

  if (memoryInfoTable == NULL) {
    acs_print(ACS_PRINT_ERR, L" Input Memory Table Pointer is NULL. Cannot create Memory INFO\n");
    return;
  }

// Retrieve the UEFI Memory Map

  MemoryMap = NULL;
  MemoryMapSize = 0;
  Status = gBS->GetMemoryMap (&MemoryMapSize, MemoryMap, &MapKey, &DescriptorSize, &DescriptorVersion);
  if (Status == EFI_BUFFER_TOO_SMALL) {
    // The UEFI specification advises to allocate more memory for the MemoryMap buffer between successive
    // calls to GetMemoryMap(), since allocation of the new buffer may potentially increase memory map size.
    Pages = EFI_SIZE_TO_PAGES (MemoryMapSize) + 1;
    gBS->AllocatePages (AllocateAnyPages, EfiBootServicesData, Pages, &Address);
    MemoryMap = (EFI_MEMORY_DESCRIPTOR *)Address;
    if (MemoryMap == NULL) {
      Status = EFI_OUT_OF_RESOURCES;
      return;
    }
    Status = gBS->GetMemoryMap (&MemoryMapSize, MemoryMap, &MapKey, &DescriptorSize, &DescriptorVersion);
  }

  // Go through the list and add the reserved region to the Device Tree
  if (!EFI_ERROR (Status)) {
    MemoryMapPtr = MemoryMap;
    for (Index = 0; Index < (MemoryMapSize / DescriptorSize); Index++) {
          acs_print(ACS_PRINT_INFO, L"  Reserved region of type %d [0x%lX, 0x%lX]\n",
            MemoryMapPtr->Type, (UINTN)MemoryMapPtr->PhysicalStart,
            (UINTN)(MemoryMapPtr->PhysicalStart + MemoryMapPtr->NumberOfPages * EFI_PAGE_SIZE));
      if (IsUefiMemory ((EFI_MEMORY_TYPE)MemoryMapPtr->Type)) {
        memoryInfoTable->info[i].type      = MEMORY_TYPE_RESERVED;
      } else {
        if (IsNormalMemory ((EFI_MEMORY_TYPE)MemoryMapPtr->Type)) {
          memoryInfoTable->info[i].type      = MEMORY_TYPE_NORMAL;
        } else {
          if (IsDeviceMemory ((EFI_MEMORY_TYPE)MemoryMapPtr->Type)) {
            memoryInfoTable->info[i].type      = MEMORY_TYPE_DEVICE;
          } else {
            if (IsPersistentMemory ((EFI_MEMORY_TYPE)MemoryMapPtr->Type)) {
              memoryInfoTable->info[i].type      = MEMORY_TYPE_PERSISTENT;
            } else {
                memoryInfoTable->info[i].type      = MEMORY_TYPE_NOT_POPULATED;
            }
          }
        }
      }
      memoryInfoTable->info[i].phy_addr  = MemoryMapPtr->PhysicalStart;
      memoryInfoTable->info[i].virt_addr = MemoryMapPtr->VirtualStart;
      memoryInfoTable->info[i].size      = (MemoryMapPtr->NumberOfPages * EFI_PAGE_SIZE);
      i++;
      if (i >= MEM_INFO_TBL_MAX_ENTRY) {
        acs_print(ACS_PRINT_DEBUG, L"  Memory Info tbl limit exceeded, Skipping remaining\n", 0);
        break;
      }

      MemoryMapPtr = (EFI_MEMORY_DESCRIPTOR*)((UINTN)MemoryMapPtr + DescriptorSize);
    }
    memoryInfoTable->info[i].type      = MEMORY_TYPE_LAST_ENTRY;
  }

}

/**
  @brief Maps the physical memory region into the virtual address space

  @param ptr Pointer to physical memory region
  @param size Size
  @param attr Attributes

  @return Pointer to mapped virtual address space
**/
UINT64
pal_memory_ioremap(VOID *ptr, UINT32 size, UINT32 attr)
{


  return (UINT64)ptr;
}

/**
  @brief Removes the physical memory to virtual address space mapping

  @param ptr Pointer to mapped space

  @return None
**/
VOID
pal_memory_unmap(VOID *ptr)
{

  return;
}

/**
  @brief  Return the address of unpopulated memory of requested
          instance from the GCD memory map.

  @param  addr      - Address of the unpopulated memory
          instance  - Instance of memory

  @return  EFI_STATUS
**/
UINT64
pal_memory_get_unpopulated_addr(UINT64 *addr, UINT32 instance)
{
  EFI_STATUS                        Status;
  EFI_GCD_MEMORY_SPACE_DESCRIPTOR  *MemorySpaceMap = NULL;
  UINT32                            Index;
  UINTN                             NumberOfDescriptors;
  UINT32                            Memory_instance = 0;

  /* Get the Global Coherency Domain Memory Space map table */
  Status = gDS->GetMemorySpaceMap(&NumberOfDescriptors, &MemorySpaceMap);
  if (Status != EFI_SUCCESS)
  {
    acs_print(ACS_PRINT_ERR, L" Failed to get GCD memory with error: %x\n", Status);
    if (Status == EFI_NO_MAPPING)
    {
        return MEM_MAP_NO_MEM;
    }

    return MEM_MAP_FAILURE;
  }

  for (Index = 0; Index < NumberOfDescriptors; Index++, MemorySpaceMap++)
  {
    if (MemorySpaceMap->GcdMemoryType == EfiGcdMemoryTypeNonExistent)
    {
      if (Memory_instance == instance)
      {
        *addr = MemorySpaceMap->BaseAddress;
        if (*addr == 0)
          continue;

        acs_print(ACS_PRINT_INFO, L" Unpopulated region with base address 0x%lX found\n", *addr);
        return MEM_MAP_SUCCESS;
      }

      Memory_instance++;
    }
  }

  return PCIE_NO_MAPPING;
}

/**
  @brief  Platform specific code for UART initialisation

  @param   None
  @return  None
**/
VOID
pal_peripheral_uart_setup(VOID)
{
  return;
}
