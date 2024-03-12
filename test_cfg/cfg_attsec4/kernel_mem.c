/* kernel_mem.c */
#include "kernel/kernel_int.h"
#include "kernel_cfg.h"

#if !(TKERNEL_PRID == 0x000bU && (TKERNEL_PRVER & 0xf000U) == 0x3000U)
#error The kernel does not match this configuration file.
#endif

/*
 *  Include Directives
 */

#ifndef OMIT_TARGET_MEMCFG
#ifdef TOPPERS_ML_AUTO
#endif
#endif
#include "target_timer.h"
#if TNUM_PRCID >= 2
#endif
#if TNUM_PRCID >= 3
#endif
#if TNUM_PRCID >= 4
#endif
#if TNUM_PRCID >= 2
#endif
#if TNUM_PRCID >= 3
#endif
#if TNUM_PRCID >= 4
#endif
#ifdef TOPPERS_SUPPORT_OVRHDR
#if TNUM_PRCID >= 2
#endif
#if TNUM_PRCID >= 3
#endif
#if TNUM_PRCID >= 4
#endif
#endif
#include "target_ipi.h"
#if TNUM_PRCID >= 2
#endif
#if TNUM_PRCID >= 3
#endif
#if TNUM_PRCID >= 4
#endif
#if TNUM_PRCID >= 2
#endif
#if TNUM_PRCID >= 3
#endif
#if TNUM_PRCID >= 4
#endif
#if TNUM_PRCID >= 2
#endif
#if TNUM_PRCID >= 3
#endif
#if TNUM_PRCID >= 4
#endif
#if TNUM_PRCID >= 2
#endif
#if TNUM_PRCID >= 3
#endif
#if TNUM_PRCID >= 4
#endif
#include "cfg_common.h"

/*
 *  Memory Object Initialization Information
 */

extern char __start_mo_text_kernel[];
extern char __start_mo_rodata_kernel[];
extern char __start_mo_text_kernel_A7[];
extern char __aend_mo_text_kernel_A7[];
extern char __start_mo_text_shared[];
extern char __aend_mo_text_shared[];
extern char __start_mo_RAM_text_kernel[];
extern char __start_mo_RAM_rodata_kernel[];
extern char __start_mo_RAM_text_kernel_A7[];
extern char __aend_mo_RAM_text_kernel_A7[];
extern char __start_mo_rwdata_kernel_A4[];
extern char __start_mo_rwdata_kernel[];
extern char __start_mo_rwdata_kernel_A6[];
extern char __start_mo_rwdata_kernel_A2[];
extern char __aend_mo_rwdata_kernel_A2[];

const uint_t _kernel_tnum_meminib = 16U;

void *const _kernel_memtop_table[16] = {
	0,
	__start_mo_text_kernel /* 0x01000000 */,
	__start_mo_rodata_kernel /* 0x0100b000 */,
	__start_mo_text_kernel_A7 /* 0x0100c000 */,
	__aend_mo_text_kernel_A7 /* 0x0100d000 */,
	__start_mo_text_shared /* 0x0100e000 */,
	__aend_mo_text_shared /* 0x0100f000 */,
	__start_mo_RAM_text_kernel /* 0x01800000 */,
	__start_mo_RAM_rodata_kernel /* 0x01801000 */,
	__start_mo_RAM_text_kernel_A7 /* 0x01802000 */,
	__aend_mo_RAM_text_kernel_A7 /* 0x01803000 */,
	__start_mo_rwdata_kernel_A4 /* 0x01804000 */,
	__start_mo_rwdata_kernel /* 0x01805000 */,
	__start_mo_rwdata_kernel_A6 /* 0x0180b000 */,
	__start_mo_rwdata_kernel_A2 /* 0x0180c000 */,
	__aend_mo_rwdata_kernel_A2 /* 0x0180d000 */
};

const MEMINIB _kernel_meminib_table[16] = {
	{ TA_NOEXS, 0U, 0U, 0U },
	{ TA_TEXTSEC, 0U, TACP_KERNEL, TACP_KERNEL },
	{ TA_RODATASEC, 0U, TACP_KERNEL, TACP_KERNEL },
	{ TA_NOREAD|TA_NOWRITE|TA_EXEC, 0U, TACP_KERNEL, TACP_KERNEL },
	{ TA_NOEXS, 0U, 0U, 0U },
	{ TA_TEXTSEC, 0U, TACP_SHARED, TACP_SHARED },
	{ TA_NOEXS, 0U, 0U, 0U },
	{ 0x5, 0U, TACP_KERNEL, TACP_KERNEL },
	{ 0x1, 0U, TACP_KERNEL, TACP_KERNEL },
	{ 0x7, 0U, TACP_KERNEL, TACP_KERNEL },
	{ TA_NOEXS, 0U, 0U, 0U },
	{ 0x4, TACP_KERNEL, TACP_KERNEL, TACP_KERNEL },
	{ 0x0, TACP_KERNEL, TACP_KERNEL, TACP_KERNEL },
	{ 0x6, TACP_KERNEL, TACP_KERNEL, TACP_KERNEL },
	{ 0x2, TACP_KERNEL, TACP_KERNEL, TACP_KERNEL },
	{ TA_NOEXS, 0U, 0U, 0U }
};

/*
 *  Data Section Initialization Information
 */

extern char __start_RAM_text_kernel_Ad[];
extern char __end_RAM_text_kernel_Ad[];
extern char __start_RAM_itext_kernel_Ad[];
extern char __start_RAM_rodata_kernel_A9[];
extern char __end_RAM_rodata_kernel_A9[];
extern char __start_RAM_irodata_kernel_A9[];
extern char __start_RAM_text_kernel_Af[];
extern char __end_RAM_text_kernel_Af[];
extern char __start_RAM_itext_kernel_Af[];
extern char __start_RAM_linkonly_kernel_Ab[];
extern char __end_RAM_linkonly_kernel_Ab[];
extern char __start_RAM_ilinkonly_kernel_Ab[];
extern char __start_data_kernel_Ac[];
extern char __end_data_kernel_Ac[];
extern char __start_idata_kernel_Ac[];
extern char __start_data_kernel[];
extern char __end_data_kernel[];
extern char __start_idata_kernel[];
extern char __start_data_kernel_Ae[];
extern char __end_data_kernel_Ae[];
extern char __start_idata_kernel_Ae[];
extern char __start_data_kernel_Aa[];
extern char __end_data_kernel_Aa[];
extern char __start_idata_kernel_Aa[];

const uint_t _kernel_tnum_datasec = 8U;

const DATASECINIB _kernel_datasecinib_table[8] = {
	{ __start_RAM_text_kernel_Ad, __end_RAM_text_kernel_Ad, __start_RAM_itext_kernel_Ad },
	{ __start_RAM_rodata_kernel_A9, __end_RAM_rodata_kernel_A9, __start_RAM_irodata_kernel_A9 },
	{ __start_RAM_text_kernel_Af, __end_RAM_text_kernel_Af, __start_RAM_itext_kernel_Af },
	{ __start_RAM_linkonly_kernel_Ab, __end_RAM_linkonly_kernel_Ab, __start_RAM_ilinkonly_kernel_Ab },
	{ __start_data_kernel_Ac, __end_data_kernel_Ac, __start_idata_kernel_Ac },
	{ __start_data_kernel, __end_data_kernel, __start_idata_kernel },
	{ __start_data_kernel_Ae, __end_data_kernel_Ae, __start_idata_kernel_Ae },
	{ __start_data_kernel_Aa, __end_data_kernel_Aa, __start_idata_kernel_Aa }
};

/*
 *  BSS Section Initialization Information
 */

extern char __start_RAM_text_kernel_A15[];
extern char __end_RAM_text_kernel_A15[];
extern char __start_RAM_rodata_kernel_A11[];
extern char __end_RAM_rodata_kernel_A11[];
extern char __start_RAM_text_kernel_A17[];
extern char __end_RAM_text_kernel_A17[];
extern char __start_RAM_linkonly_kernel_A13[];
extern char __end_RAM_linkonly_kernel_A13[];
extern char __start_bss_kernel_A14[];
extern char __end_bss_kernel_A14[];
extern char __start_bss_kernel[];
extern char __end_bss_kernel[];
extern char __start_bss_kernel_A16[];
extern char __end_bss_kernel_A16[];
extern char __start_bss_kernel_A12[];
extern char __end_bss_kernel_A12[];

const uint_t _kernel_tnum_bsssec = 8U;

const BSSSECINIB _kernel_bsssecinib_table[8] = {
	{ __start_RAM_text_kernel_A15, __end_RAM_text_kernel_A15 },
	{ __start_RAM_rodata_kernel_A11, __end_RAM_rodata_kernel_A11 },
	{ __start_RAM_text_kernel_A17, __end_RAM_text_kernel_A17 },
	{ __start_RAM_linkonly_kernel_A13, __end_RAM_linkonly_kernel_A13 },
	{ __start_bss_kernel_A14, __end_bss_kernel_A14 },
	{ __start_bss_kernel, __end_bss_kernel },
	{ __start_bss_kernel_A16, __end_bss_kernel_A16 },
	{ __start_bss_kernel_A12, __end_bss_kernel_A12 }
};

