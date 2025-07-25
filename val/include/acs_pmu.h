/** @file
 * Copyright (c) 2024-2025, Arm Limited or its affiliates. All rights reserved.
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

#ifndef __ACS_PMU_H__
#define __ACS_PMU_H__

/* EL2 Cycle Count Filter Enable */
#define NSH_EN    27
/* Cycle Counter Enable */
#define CC_EN     31
/* Long Cycle Counter Enable */
#define LCC_EN    06
/* Cycle Count Reset */
#define CCR_SET   02
/* Global Cycle Counter enable */
#define GCC_EN    00

#define PMCCFILTR_NSH_EN_BIT    27
#define PMCNTENSET_C_EN_BIT     31
#define PMCR_LC_EN_BIT          6
#define PMCR_C_RESET_BIT        2
#define PMCR_EN_BIT             0

uint64_t AA64ReadPmccntr(void);
uint64_t AA64ReadPmccfiltr(void);
uint64_t AA64ReadPmcntenset(void);
void AA64WritePmccntr(uint64_t WriteData);
void AA64WritePmccfiltr(uint64_t WriteData);
void AA64WritePmcntenset(uint64_t WriteData);

uint64_t val_pmu_get_info(PMU_INFO_e type, uint32_t node_index);
uint8_t  val_pmu_supports_dedicated_cycle_counter(uint32_t node_index);
uint32_t val_pmu_get_monitor_count(uint32_t node_index);
void     val_pmu_disable_all_monitors(uint32_t node_index);
void     val_pmu_enable_all_monitors(uint32_t node_index);
void     val_pmu_reset_all_monitors(uint32_t node_index);
uint32_t val_pmu_get_monitor_group_count(uint32_t node_index);
uint32_t val_pmu_get_max_monitor_size(uint32_t node_index);
uint32_t val_pmu_configure_monitor(uint32_t node_index, PMU_EVENT_TYPE_e event_type,
                                                                         uint32_t mon_inst);
void     val_pmu_enable_monitor(uint32_t node_index, uint32_t mon_inst);
void     val_pmu_disable_monitor(uint32_t node_index, uint32_t mon_inst);
uint64_t val_pmu_read_count(uint32_t node_index, uint32_t mon_inst);
uint32_t val_pmu_get_node_index(uint64_t prox_domain);
uint32_t val_pmu_implements_pmscr(uint32_t node_index);
uint32_t val_pmu_is_secure(uint32_t node_index);
uint32_t val_pmu_get_multi_traffic_support_interface(uint64_t *interface_acpiid,
                                                       uint32_t *num_traffic_type_support);
uint32_t val_pmu_get_index_acpiid(uint64_t interface_acpiid);
uint32_t val_generate_traffic(uint64_t interface_acpiid, uint32_t pmu_node_index,
                                     uint32_t mon_index, uint32_t eventid);
uint32_t val_pmu_check_monitor_count_value(uint64_t interface_acpiid, uint32_t count_value,
                                                                          uint32_t eventid);
void     val_pmu_set_node_coresight_complaint(uint32_t flag, uint32_t node_index);

uint32_t pmu001_entry(uint32_t num_pe);
uint32_t pmu002_entry(uint32_t num_pe);
uint32_t pmu003_entry(uint32_t num_pe);
uint32_t pmu004_entry(uint32_t num_pe);
uint32_t pmu005_entry(uint32_t num_pe);
uint32_t pmu006_entry(uint32_t num_pe);
uint32_t pmu007_entry(uint32_t num_pe);
uint32_t pmu009_entry(uint32_t num_pe);

#endif /*__ACS_PMU_H__ */
