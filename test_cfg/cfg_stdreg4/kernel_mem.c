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

extern char __start_mo_text_shared[];
extern char __start_mo_rodata_shared[];
extern char __aend_mo_rodata_shared[];
extern char __start_mo_rwdata_shared[];
extern char __aend_mo_rwdata_shared[];
extern char __start_mo_ROM1_text_kernel[];
extern char __start_mo_ROM1_rodata_kernel[];
extern char __aend_mo_ROM1_rodata_kernel[];
extern char __start_mo_RAM1_rwdata_kernel[];
extern char __aend_mo_RAM1_rwdata_kernel[];
extern char __start_mo_ROM2_text_DOM1[];
extern char __start_mo_ROM2_rodata_DOM1[];
extern char __aend_mo_ROM2_rodata_DOM1[];
extern char __start_ustack_TASK5[];
extern char __start_mo_RAM2_rwdata_DOM1[];
extern char __aend_mo_RAM2_rwdata_DOM1[];

const uint_t _kernel_tnum_meminib = 17U;

void *const _kernel_memtop_table[17] = {
	0,
	__start_mo_text_shared /* 0x01000000 */,
	__start_mo_rodata_shared /* 0x01001000 */,
	__aend_mo_rodata_shared /* 0x01002000 */,
	__start_mo_rwdata_shared /* 0x01800000 */,
	__aend_mo_rwdata_shared /* 0x01801000 */,
	__start_mo_ROM1_text_kernel /* 0x02000000 */,
	__start_mo_ROM1_rodata_kernel /* 0x0200b000 */,
	__aend_mo_ROM1_rodata_kernel /* 0x0200c000 */,
	__start_mo_RAM1_rwdata_kernel /* 0x02800000 */,
	__aend_mo_RAM1_rwdata_kernel /* 0x02810000 */,
	__start_mo_ROM2_text_DOM1 /* 0x03000000 */,
	__start_mo_ROM2_rodata_DOM1 /* 0x03001000 */,
	__aend_mo_ROM2_rodata_DOM1 /* 0x03002000 */,
	__start_ustack_TASK5 /* 0x03800000 */,
	__start_mo_RAM2_rwdata_DOM1 /* 0x03801000 */,
	__aend_mo_RAM2_rwdata_DOM1 /* 0x03802000 */
};

const MEMINIB _kernel_meminib_table[17] = {
	{ TA_NOEXS, 0U, 0U, 0U },
	{ TA_TEXTSEC, 0U, TACP_SHARED, TACP_SHARED },
	{ TA_RODATASEC, 0U, TACP_SHARED, TACP_SHARED },
	{ TA_NOEXS, 0U, 0U, 0U },
	{ 0x0, TACP_SHARED, TACP_SHARED, TACP_SHARED },
	{ TA_NOEXS, 0U, 0U, 0U },
	{ TA_TEXTSEC, 0U, TACP_KERNEL, TACP_KERNEL },
	{ TA_RODATASEC, 0U, TACP_KERNEL, TACP_KERNEL },
	{ TA_NOEXS, 0U, 0U, 0U },
	{ 0x0, TACP_KERNEL, TACP_KERNEL, TACP_KERNEL },
	{ TA_NOEXS, 0U, 0U, 0U },
	{ TA_TEXTSEC, 0U, TACP(DOM1), TACP(DOM1) },
	{ TA_RODATASEC, 0U, TACP(DOM1), TACP(DOM1) },
	{ TA_NOEXS, 0U, 0U, 0U },
	{ TA_USTACK|(TA_NOINITSEC), TACP(DOM1), TACP(DOM1), TACP(DOM1) },
	{ 0x0, TACP(DOM1), TACP(DOM1), TACP(DOM1) },
	{ TA_NOEXS, 0U, 0U, 0U }
};

/*
 *  Data Section Initialization Information
 */

extern char __start_data_shared[];
extern char __end_data_shared[];
extern char __start_idata_shared[];
extern char __start_RAM1_data_kernel[];
extern char __end_RAM1_data_kernel[];
extern char __start_RAM1_idata_kernel[];
extern char __start_RAM2_data_DOM1[];
extern char __end_RAM2_data_DOM1[];
extern char __start_RAM2_idata_DOM1[];

const uint_t _kernel_tnum_datasec = 3U;

const DATASECINIB _kernel_datasecinib_table[3] = {
	{ __start_data_shared, __end_data_shared, __start_idata_shared },
	{ __start_RAM1_data_kernel, __end_RAM1_data_kernel, __start_RAM1_idata_kernel },
	{ __start_RAM2_data_DOM1, __end_RAM2_data_DOM1, __start_RAM2_idata_DOM1 }
};

/*
 *  BSS Section Initialization Information
 */

extern char __start_bss_shared[];
extern char __end_bss_shared[];
extern char __start_RAM1_bss_kernel[];
extern char __end_RAM1_bss_kernel[];
extern char __start_RAM2_bss_DOM1[];
extern char __end_RAM2_bss_DOM1[];

const uint_t _kernel_tnum_bsssec = 3U;

const BSSSECINIB _kernel_bsssecinib_table[3] = {
	{ __start_bss_shared, __end_bss_shared },
	{ __start_RAM1_bss_kernel, __end_RAM1_bss_kernel },
	{ __start_RAM2_bss_DOM1, __end_RAM2_bss_DOM1 }
};

