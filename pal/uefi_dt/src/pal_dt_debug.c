/** @file
 * Copyright (c) 2021, 2023-2025, Arm Limited or its affiliates. All rights reserved.
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
#include <Library/UefiLib.h>
#include "include/pal_uefi.h"
#include "include/pal_dt.h"

/**
  @brief  This API is use to dump PE_INFO Table after filling from DT

  @param  PeTable  - Address where the PE information needs to be filled.

  @return  None
**/
VOID
dt_dump_pe_table(PE_INFO_TABLE *PeTable)
{
  UINT32 Index = 0;

  if (!PeTable) {
    acs_print(ACS_PRINT_ERR, L" PeTable ptr NULL\n");
    return;
  }

  acs_print(ACS_PRINT_DEBUG, L" ************PE TABLE DUMP************\n");
  acs_print(ACS_PRINT_DEBUG, L" NUM PE %d\n", PeTable->header.num_of_pe);

  while (Index < PeTable->header.num_of_pe) {
    acs_print(ACS_PRINT_DEBUG, L" PE NUM      :%x\n", PeTable->pe_info[Index].pe_num);
    acs_print(ACS_PRINT_DEBUG, L" MPIDR       :%llx\n", PeTable->pe_info[Index].mpidr);
//    acs_print(ACS_PRINT_DEBUG, L"    ATTR     :%x\n", PeTable->pe_info[Index].attr);
    acs_print(ACS_PRINT_DEBUG, L" PMU GSIV    :%x\n", PeTable->pe_info[Index].pmu_gsiv);
    acs_print(ACS_PRINT_DEBUG, L" GIC MAINT GSIV    :%x\n", PeTable->pe_info[Index].gmain_gsiv);
    Index++;
  }
  acs_print(ACS_PRINT_DEBUG, L" *************************************\n");
}

/**
  @brief  This API is use to dump GIC_INFO Table after filling from DT

  @param  GicTable  - Address where the GIC information needs to be filled.

  @return  None
**/
VOID
dt_dump_gic_table(GIC_INFO_TABLE *GicTable)
{
  UINT32 Index = 0;

  if (!GicTable) {
    acs_print(ACS_PRINT_ERR, L" GicTable ptr NULL\n");
    return;
  }

  acs_print(ACS_PRINT_DEBUG, L" ************GIC TABLE************\n");
  acs_print(ACS_PRINT_DEBUG, L" GIC version %d\n", GicTable->header.gic_version);
  acs_print(ACS_PRINT_DEBUG, L" GIC num D %d\n", GicTable->header.num_gicd);
  acs_print(ACS_PRINT_DEBUG, L" GIC num GICC RD %d\n", GicTable->header.num_gicc_rd);
  acs_print(ACS_PRINT_DEBUG, L" GIC num GICR RD %d\n", GicTable->header.num_gicr_rd);
//  acs_print(ACS_PRINT_DEBUG, L" GIC num ITS %d\n", GicTable->header.num_its);

  while (GicTable->gic_info[Index].type != 0xFF) {
    acs_print(ACS_PRINT_DEBUG, L" GIC TYPE     :%x\n", GicTable->gic_info[Index].type);
    acs_print(ACS_PRINT_DEBUG, L" BASE         :%x\n", GicTable->gic_info[Index].base);
    acs_print(ACS_PRINT_DEBUG, L" LEN          :%x\n", GicTable->gic_info[Index].length);
//    acs_print(ACS_PRINT_DEBUG, L"     ITS ID   :%x\n", GicTable->gic_info[Index].entry_id);
    Index++;
  }
  acs_print(ACS_PRINT_DEBUG, L" *************************************\n");
}

/**
  @brief  This API is use to dump WD_INFO Table after filling from DT

  @param  WdTable  - Address where the WD information needs to be filled.

  @return  None
**/
VOID
dt_dump_wd_table(WD_INFO_TABLE *WdTable)
{
  UINT32 Index = 0;

  if (!WdTable) {
    acs_print(ACS_PRINT_ERR, L" WdTable ptr NULL\n");
    return;
  }

  acs_print(ACS_PRINT_DEBUG, L" ************WD TABLE************\n");
  acs_print(ACS_PRINT_DEBUG, L" NUM WD %d\n", WdTable->header.num_wd);

  while (Index < WdTable->header.num_wd) {
    acs_print(ACS_PRINT_DEBUG, L" REFRESH BASE  :%x\n", WdTable->wd_info[Index].wd_refresh_base);
    acs_print(ACS_PRINT_DEBUG, L" CONTROL BASE  :%x\n", WdTable->wd_info[Index].wd_ctrl_base);
    acs_print(ACS_PRINT_DEBUG, L" GSIV          :%x\n", WdTable->wd_info[Index].wd_gsiv);
    acs_print(ACS_PRINT_DEBUG, L" FLAGS         :%x\n", WdTable->wd_info[Index].wd_flags);
    Index++;
  }
  acs_print(ACS_PRINT_DEBUG, L" *************************************\n");
}

/**
=======
  @brief  This API is use to dump PCIE_INFO Table after filling from DT

  @param  PcieTable  - Address where the PCIE information needs to be filled.

  @return  None
**/
VOID
dt_dump_pcie_table(PCIE_INFO_TABLE *PcieTable)
{
  UINT32 Index = 0;

  if (!PcieTable) {
    acs_print(ACS_PRINT_ERR, L" PcieTable ptr NULL\n");
    return;
  }

  acs_print(ACS_PRINT_DEBUG, L" ************PCIE TABLE************\n");
  acs_print(ACS_PRINT_DEBUG, L" NUM ECAM %d\n", PcieTable->num_entries);

  while (Index < PcieTable->num_entries) {
    acs_print(ACS_PRINT_DEBUG, L" ECAM BASE          :%x\n", PcieTable->block[Index].ecam_base);
    acs_print(ACS_PRINT_DEBUG, L" START BUS          :%x\n", PcieTable->block[Index].start_bus_num);
    acs_print(ACS_PRINT_DEBUG, L" END BUS            :%x\n", PcieTable->block[Index].end_bus_num);
//    acs_print(ACS_PRINT_DEBUG, L"      SEGMENT NUM   :%x\n", PcieTable->block[Index].segment_num);
    Index++;
  }
  acs_print(ACS_PRINT_DEBUG, L" *************************************\n");
}

/**
  @brief  This API is use to dump MEMORY_INFO Table after filling from DT

  @param  memoryInfoTable  - Address where the MEMORY information needs to be filled.

  @return  None
**/
VOID
dt_dump_memory_table(MEMORY_INFO_TABLE *memoryInfoTable)
{
  UINT32 Index = 0;

  if (!memoryInfoTable) {
    acs_print(ACS_PRINT_ERR, L" memoryInfoTable ptr NULL\n");
    return;
  }

  acs_print(ACS_PRINT_DEBUG, L" ************MEMORY TABLE************\n");
  acs_print(ACS_PRINT_DEBUG, L" dram base  :%x\n", memoryInfoTable->dram_base);
  acs_print(ACS_PRINT_DEBUG, L" dram size  :%x\n", memoryInfoTable->dram_size);

  while (memoryInfoTable->info[Index].type < 0x1004) {
    acs_print(ACS_PRINT_DEBUG, L" Type      :%x\n", memoryInfoTable->info[Index].type);
    acs_print(ACS_PRINT_DEBUG, L" PHY addr  :%x\n", memoryInfoTable->info[Index].phy_addr);
    acs_print(ACS_PRINT_DEBUG, L" VIRT addr :%x\n", memoryInfoTable->info[Index].virt_addr);
    acs_print(ACS_PRINT_DEBUG, L" size      :%x\n", memoryInfoTable->info[Index].size);
    acs_print(ACS_PRINT_DEBUG, L" flags     :%x\n", memoryInfoTable->info[Index].flags);
    Index++;
  }
  acs_print(ACS_PRINT_DEBUG, L" *************************************\n");
}

/**
  @brief  This API is use to dump TIMER_INFO Table after filling from DT

  @param  TimerTable  - Address where the TIMER information needs to be filled.

  @return  None
**/
VOID
dt_dump_timer_table(TIMER_INFO_TABLE *TimerTable)
{
  UINT32 Index = 0;

  if (!TimerTable) {
    acs_print(ACS_PRINT_ERR, L" TimerTable ptr NULL\n");
    return;
  }

  acs_print(ACS_PRINT_DEBUG, L" ************TIMER TABLE************\n");
  acs_print(ACS_PRINT_DEBUG, L" Num of system timers %d\n", TimerTable->header.num_platform_timer);
  acs_print(ACS_PRINT_DEBUG, L" s_el1_timer_flag %x\n", TimerTable->header.s_el1_timer_flag);
  acs_print(ACS_PRINT_DEBUG, L" ns_el1_timer_flag %x\n", TimerTable->header.ns_el1_timer_flag);
  acs_print(ACS_PRINT_DEBUG, L" el2_timer_flag %x\n", TimerTable->header.el2_timer_flag);
  acs_print(ACS_PRINT_DEBUG, L" el2_virt_timer_flag %x\n", TimerTable->header.el2_virt_timer_flag);
  acs_print(ACS_PRINT_DEBUG, L" s_el1_timer_gsiv %x\n", TimerTable->header.s_el1_timer_gsiv);
  acs_print(ACS_PRINT_DEBUG, L" ns_el1_timer_gsiv %x\n", TimerTable->header.ns_el1_timer_gsiv);
  acs_print(ACS_PRINT_DEBUG, L" el2_timer_gsiv %x\n", TimerTable->header.el2_timer_gsiv);
  acs_print(ACS_PRINT_DEBUG, L" virtual_timer_flag %x\n", TimerTable->header.virtual_timer_flag);
  acs_print(ACS_PRINT_DEBUG, L" virtual_timer_gsiv %x\n", TimerTable->header.virtual_timer_gsiv);
  acs_print(ACS_PRINT_DEBUG, L" el2_virt_timer_gsiv %x\n", TimerTable->header.el2_virt_timer_gsiv);
  acs_print(ACS_PRINT_DEBUG, L" CNTBase             %x\n", TimerTable->gt_info->block_cntl_base);

  while (Index < TimerTable->gt_info->timer_count) {
    acs_print(ACS_PRINT_DEBUG, L" Frame num   :%x\n", TimerTable->gt_info->frame_num[Index]);
    acs_print(ACS_PRINT_DEBUG, L" GtCntBase   :%x\n", TimerTable->gt_info->GtCntBase[Index]);
    acs_print(ACS_PRINT_DEBUG, L" GtCntEl0Base:%x\n", TimerTable->gt_info->GtCntEl0Base[Index]);
    acs_print(ACS_PRINT_DEBUG, L" gsiv        :%x\n", TimerTable->gt_info->gsiv[Index]);
    acs_print(ACS_PRINT_DEBUG, L" virt_gsiv   :%x\n", TimerTable->gt_info->virt_gsiv[Index]);
    acs_print(ACS_PRINT_DEBUG, L" flags       :%x\n", TimerTable->gt_info->flags[Index]);
    Index++;
  }
  acs_print(ACS_PRINT_DEBUG, L" *************************************\n");
}

/**
  @brief  This API is use to dump peripheralInfo Table after filling from DT

  @param  peripheralInfoTable  - Address where the WD information needs to be filled.

  @return  None
**/
VOID
dt_dump_peripheral_table(PERIPHERAL_INFO_TABLE *peripheralInfoTable)
{
  UINT32 Index = 0;

  if (!peripheralInfoTable) {
    acs_print(ACS_PRINT_ERR, L" peripheralInfoTable ptr NULL\n");
    return;
  }

  acs_print(ACS_PRINT_DEBUG, L" ************USB TABLE************\n");
  acs_print(ACS_PRINT_DEBUG, L" NUM USB %d\n", peripheralInfoTable->header.num_usb);

  while (Index < peripheralInfoTable->header.num_usb) {
    acs_print(ACS_PRINT_DEBUG, L" TYPE          :%x\n", peripheralInfoTable->info[Index].type);
    acs_print(ACS_PRINT_DEBUG, L" CONTROL BASE  :%x\n", peripheralInfoTable->info[Index].base0);
    acs_print(ACS_PRINT_DEBUG, L" GSIV          :%d\n", peripheralInfoTable->info[Index].irq);
    acs_print(ACS_PRINT_DEBUG, L" FLAGS         :%x\n", peripheralInfoTable->info[Index].flags);
    acs_print(ACS_PRINT_DEBUG, L" BDF           :%x\n", peripheralInfoTable->info[Index].bdf);
    Index++;
  }

  acs_print(ACS_PRINT_DEBUG, L" ************SATA TABLE************\n");
  acs_print(ACS_PRINT_DEBUG, L" NUM SATA %d\n", peripheralInfoTable->header.num_sata);

  while (Index < (peripheralInfoTable->header.num_sata + peripheralInfoTable->header.num_usb)) {
    acs_print(ACS_PRINT_DEBUG, L" TYPE          :%x\n", peripheralInfoTable->info[Index].type);
    acs_print(ACS_PRINT_DEBUG, L" CONTROL BASE  :%x\n", peripheralInfoTable->info[Index].base0);
    acs_print(ACS_PRINT_DEBUG, L" GSIV          :%d\n", peripheralInfoTable->info[Index].irq);
    acs_print(ACS_PRINT_DEBUG, L" FLAGS         :%x\n", peripheralInfoTable->info[Index].flags);
    acs_print(ACS_PRINT_DEBUG, L" BDF           :%x\n", peripheralInfoTable->info[Index].bdf);
    Index++;
  }

  acs_print(ACS_PRINT_DEBUG, L" ************UART TABLE************\n");
  acs_print(ACS_PRINT_DEBUG, L" NUM UART %d\n", peripheralInfoTable->header.num_uart);

  while (Index < (peripheralInfoTable->header.num_sata + peripheralInfoTable->header.num_usb +
      peripheralInfoTable->header.num_uart)) {
    acs_print(ACS_PRINT_DEBUG, L" TYPE          :%x\n", peripheralInfoTable->info[Index].type);
    acs_print(ACS_PRINT_DEBUG, L" CONTROL BASE  :%x\n", peripheralInfoTable->info[Index].base0);
    acs_print(ACS_PRINT_DEBUG, L" GSIV          :%d\n", peripheralInfoTable->info[Index].irq);
    acs_print(ACS_PRINT_DEBUG, L" FLAGS         :%x\n", peripheralInfoTable->info[Index].flags);
    Index++;
  }
  acs_print(ACS_PRINT_DEBUG, L" *************************************\n");
}
