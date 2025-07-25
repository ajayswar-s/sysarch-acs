/** @file
 * Copyright (c) 2016-2018, 2021-2025, Arm Limited or its affiliates. All rights reserved.
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

#include "include/acs_val.h"
#include "include/acs_peripherals.h"
#include "include/acs_common.h"
#include "include/acs_pcie.h"

PERIPHERAL_INFO_TABLE  *g_peripheral_info_table;

/**
  @brief  Return the Index of the entry in the peripheral info table
          which matches the input type and the input instance number
          Instance number is '0' based
          1. Caller       - VAL
          2. Prerequisite - val_peripheral_create_info_table
  @param  type     - Type of peripheral whose index needs to be returned
  @param  instance - Instance number is '0' based.

  @result  Index of peripheral matching type and instance
**/

static uint32_t
val_peripheral_get_entry_index(uint32_t type, uint32_t instance)
{
  uint32_t  i = 0;

  while (g_peripheral_info_table->info[i].type != 0xFF) {
      if (type == PERIPHERAL_TYPE_NONE || g_peripheral_info_table->info[i].type == type) {
          if (instance == 0)
             return i;
          else
             instance--;

      }
      i++;
  }
  return 0xFFFF;
}

/**
  @brief  Single entry point to return all Peripheral related information.
          1. Caller       - Test Suite
          2. Prerequisite - val_peripheral_create_info_table
  @param  info_type - Type of peripheral whose index needs to be returned
  @param  instance  - id (0' based) for which the info has to be returned

  @result  64-bit data of peripheral matching type and instance
**/
uint64_t
val_peripheral_get_info(PERIPHERAL_INFO_e info_type, uint32_t instance)
{
  uint32_t i;

  if (g_peripheral_info_table == NULL)
      return 0;

  switch(info_type) {
      case NUM_USB:
          return g_peripheral_info_table->header.num_usb;
      case NUM_SATA:
          return g_peripheral_info_table->header.num_sata;
      case NUM_UART:
          return g_peripheral_info_table->header.num_uart;
      case NUM_ALL:
          return g_peripheral_info_table->header.num_all;
      case USB_BASE0:
          i = val_peripheral_get_entry_index(PERIPHERAL_TYPE_USB, instance);
          if (i != 0xFFFF)
              return g_peripheral_info_table->info[i].base0;
          break;
      case USB_FLAGS:
          i = val_peripheral_get_entry_index(PERIPHERAL_TYPE_USB, instance);
          if (i != 0xFFFF)
              return g_peripheral_info_table->info[i].flags;
          break;
      case USB_GSIV:
          i = val_peripheral_get_entry_index(PERIPHERAL_TYPE_USB, instance);
          if (i != 0xFFFF)
              return g_peripheral_info_table->info[i].irq;
          break;
      case USB_BDF:
          i = val_peripheral_get_entry_index(PERIPHERAL_TYPE_USB, instance);
          if (i != 0xFFFF)
              return g_peripheral_info_table->info[i].bdf;
          break;
      case USB_INTERFACE_TYPE:
          i = val_peripheral_get_entry_index(PERIPHERAL_TYPE_USB, instance);
          if (i != 0xFFFF)
              return g_peripheral_info_table->info[i].interface_type;
          break;
      case USB_PLATFORM_TYPE:
          i = val_peripheral_get_entry_index(PERIPHERAL_TYPE_USB, instance);
          if (i != 0xFFFF)
              return g_peripheral_info_table->info[i].platform_type;
          break;
      case SATA_BASE0:
          i = val_peripheral_get_entry_index(PERIPHERAL_TYPE_SATA, instance);
          if (i != 0xFFFF)
              return g_peripheral_info_table->info[i].base0;
          break;
      case SATA_BASE1:
          i = val_peripheral_get_entry_index(PERIPHERAL_TYPE_SATA, instance);
          if (i != 0xFFFF)
              return g_peripheral_info_table->info[i].base1;
          break;
      case SATA_FLAGS:
          i = val_peripheral_get_entry_index(PERIPHERAL_TYPE_SATA, instance);
          if (i != 0xFFFF)
              return g_peripheral_info_table->info[i].flags;
          break;
      case SATA_BDF:
          i = val_peripheral_get_entry_index(PERIPHERAL_TYPE_SATA, instance);
          if (i != 0xFFFF)
              return g_peripheral_info_table->info[i].bdf;
          break;
      case SATA_GSIV:
          i = val_peripheral_get_entry_index(PERIPHERAL_TYPE_SATA, instance);
          if (i != 0xFFFF)
              return g_peripheral_info_table->info[i].irq;
          break;
      case SATA_INTERFACE_TYPE:
          i = val_peripheral_get_entry_index(PERIPHERAL_TYPE_SATA, instance);
          if (i != 0xFFFF)
              return g_peripheral_info_table->info[i].interface_type;
          break;
      case SATA_PLATFORM_TYPE:
          i = val_peripheral_get_entry_index(PERIPHERAL_TYPE_SATA, instance);
          if (i != 0xFFFF)
              return g_peripheral_info_table->info[i].platform_type;
          break;
      case UART_BASE0:
          i = val_peripheral_get_entry_index(PERIPHERAL_TYPE_UART, instance);
          if (i != 0xFFFF)
              return g_peripheral_info_table->info[i].base0;
          break;
      case UART_WIDTH:
          i = val_peripheral_get_entry_index(PERIPHERAL_TYPE_UART, instance);
          if (i != 0xFFFF)
              return g_peripheral_info_table->info[i].width;
          break;
      case UART_GSIV:
          i = val_peripheral_get_entry_index(PERIPHERAL_TYPE_UART, instance);
          if (i != 0xFFFF)
              return g_peripheral_info_table->info[i].irq;
          break;
      case UART_FLAGS:
          i = val_peripheral_get_entry_index(PERIPHERAL_TYPE_UART, instance);
          if (i != 0xFFFF)
              return g_peripheral_info_table->info[i].flags;
          break;
      case ANY_BASE0:
          i = val_peripheral_get_entry_index(PERIPHERAL_TYPE_NONE, instance);
          if (i != 0xFFFF)
              return g_peripheral_info_table->info[i].base0;
          break;
      case UART_BAUDRATE:
          i = val_peripheral_get_entry_index(PERIPHERAL_TYPE_UART, instance);
          if (i != 0xFFFF)
              return g_peripheral_info_table->info[i].baud_rate;
          break;
      case UART_INTERFACE_TYPE:
          i = val_peripheral_get_entry_index(PERIPHERAL_TYPE_UART, instance);
          if (i != 0xFFFF)
              return g_peripheral_info_table->info[i].interface_type;
          break;
      case ANY_FLAGS:
          i = val_peripheral_get_entry_index (PERIPHERAL_TYPE_NONE, instance);
          if (i != 0xFFFF)
              return g_peripheral_info_table->info[i].flags;
          break;
      case ANY_GSIV:
          i = val_peripheral_get_entry_index (PERIPHERAL_TYPE_NONE, instance);
          if (i != 0xFFFF)
            return g_peripheral_info_table->info[i].irq;
          break;
      case ANY_BDF:
          i = val_peripheral_get_entry_index (PERIPHERAL_TYPE_NONE, instance);
          if (i != 0xFFFF)
              return g_peripheral_info_table->info[i].bdf;
          break;
      case MAX_PASIDS:
          i = val_peripheral_get_entry_index (PERIPHERAL_TYPE_NONE, instance);
          if (i != 0xFFFF)
              return g_peripheral_info_table->info[i].max_pasids;
          break;
      default:
          break;
  }
  return 0;
}

void
val_peripheral_dump_info(void)
{

  uint32_t bus, dev, func, seg = 0;
  uint32_t dev_bdf, ecam_index, num_ecam;
  uint32_t reg_value, base_cc;
  uint32_t dply = 0, ntwk = 0, strg = 0;
  uint32_t start_bus, end_bus;

  num_ecam = val_pcie_get_info(PCIE_INFO_NUM_ECAM, 0);
  if (num_ecam == 0)
  {
      val_print(ACS_PRINT_DEBUG, "\n No ECAM is present", 0);
      return;
  }

  for (ecam_index = 0; ecam_index < num_ecam; ecam_index++)
  {
      seg = val_pcie_get_info(PCIE_INFO_SEGMENT, ecam_index);
      start_bus = val_pcie_get_info(PCIE_INFO_START_BUS, ecam_index);
      end_bus = val_pcie_get_info(PCIE_INFO_END_BUS, ecam_index);

      for (bus = start_bus; bus <= end_bus; bus++)
      {
          if (pal_pcie_check_bus_valid(bus)) {
              val_print(ACS_PRINT_DEBUG,
               "       Bus 0x%x marked as invalid in Platform API...Skipping\n", bus);
              continue;
          }

          for (dev = 0; dev < PCIE_MAX_DEV; dev++)
          {
              for (func = 0; func < PCIE_MAX_FUNC; func++)
              {
                  dev_bdf = PCIE_CREATE_BDF(seg, bus, dev, func);
                  val_pcie_read_cfg(dev_bdf, TYPE01_VIDR, &reg_value);
                  if (reg_value == PCIE_UNKNOWN_RESPONSE)
                      continue;
                  val_pcie_read_cfg(dev_bdf, TYPE01_RIDR, &reg_value);
                  val_print(ACS_PRINT_DEBUG, "\n BDF is %x", dev_bdf);
                  val_print(ACS_PRINT_DEBUG, "\n Class code is %x", reg_value);
                  base_cc = reg_value >> TYPE01_BCC_SHIFT;
                  if (base_cc == CNTRL_CC)
                      ntwk++;
                  if (base_cc == DP_CNTRL_CC)
                      dply++;
                  if (base_cc == MAS_CC)
                      strg++;
                  else
                      continue;
              }
          }
      }
  }


  val_print(ACS_PRINT_DEBUG, " Peripheral: Num of Network ctrl      :    %d\n", ntwk);
  val_print(ACS_PRINT_DEBUG, " Peripheral: Num of Storage ctrl      :    %d\n", strg);
  val_print(ACS_PRINT_DEBUG, " Peripheral: Num of Display ctrl      :    %d\n", dply);

}

/*
 * val_create_peripheralinfo_table:
 *    Caller         Application layer.
 *    Prerequisite   Memory allocated and passed as argument.
 *    Description    This function will call PAL layer to fill all relevant peripheral
 *                   information into the g_peripheral_info_table pointer.
 */
/**
  @brief  This API calls PAL layer to fill all relevant peripheral
          information into the g_peripheral_info_table pointer
          1. Caller       - Application layer
          2. Prerequisite - Memory allocated and passed as argument
  @param  info_table - pointer to a memory where peripheral data is filled

  @result  None
**/

void
val_peripheral_create_info_table(uint64_t *peripheral_info_table)
{

  g_peripheral_info_table = (PERIPHERAL_INFO_TABLE *)peripheral_info_table;
  val_print(ACS_PRINT_INFO, " Creating PERIPHERAL INFO table\n", 0);

  pal_peripheral_create_info_table(g_peripheral_info_table);

  val_print(ACS_PRINT_TEST, " Peripheral: Num of USB controllers   :    %d\n",
    val_peripheral_get_info(NUM_USB, 0));
  val_print(ACS_PRINT_TEST, " Peripheral: Num of SATA controllers  :    %d\n",
    val_peripheral_get_info(NUM_SATA, 0));
  val_print(ACS_PRINT_TEST, " Peripheral: Num of UART controllers  :    %d\n",
    val_peripheral_get_info(NUM_UART, 0));
  val_peripheral_dump_info();

}


/**
  @brief  Free the memory allocated for Peripheral Info table

  @param  None

  @return None
 **/
void
val_peripheral_free_info_table(void)
{
    if (g_peripheral_info_table != NULL) {
        pal_mem_free_aligned((void *)g_peripheral_info_table);
        g_peripheral_info_table = NULL;
    }
    else {
      val_print(ACS_PRINT_ERR,
                  "\n WARNING: g_peripheral_info_table pointer is already NULL",
        0);
    }
}

void val_peripheral_uart_setup(void)
{
  pal_peripheral_uart_setup();
}
