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


const uint_t _kernel_tnum_meminib = 11U;

void *const _kernel_memtop_table[11] = {
	0,
	(void *)(0x01000000) /* 0x01000000 */,
	(void *)(((char *)(0x01000000)) + (0x00200000)) /* 0x01200000 */,
	(void *)(0x01800000) /* 0x01800000 */,
	(void *)(((char *)(0x01800000)) + (0x00200000)) /* 0x01a00000 */,
	(void *)(0xf0000000) /* 0xf0000000 */,
	(void *)(0xf1000000) /* 0xf1000000 */,
	(void *)(0xf2000000) /* 0xf2000000 */,
	(void *)(((char *)(0xf2000000)) + (STACK_SIZE)) /* 0xf2001000 */,
	(void *)(0xf3000000) /* 0xf3000000 */,
	(void *)(((char *)(0xf3000000)) + (0x1000000)) /* 0xf4000000 */
};

const MEMINIB _kernel_meminib_table[11] = {
	{ TA_NOEXS, 0U, 0U, 0U },
	{ TA_ATTMEM|(TA_NOWRITE|TA_EXEC), 0U, TACP_KERNEL, TACP_KERNEL },
	{ TA_NOEXS, 0U, 0U, 0U },
	{ TA_ATTMEM|(TA_NULL), TACP_KERNEL, TACP_KERNEL, TACP_KERNEL },
	{ TA_NOEXS, 0U, 0U, 0U },
	{ TA_ATTMEM|(TA_NULL), TACP_KERNEL, TACP_KERNEL, TACP_KERNEL },
	{ TA_ATTMEM|(TA_NULL), TACP(DOM1), TACP(DOM1), TACP(DOM1) },
	{ TA_USTACK|(TA_NOINITSEC), TACP(DOM1), TACP(DOM1), TACP(DOM1) },
	{ TA_NOEXS, 0U, 0U, 0U },
	{ TA_ATTMEM|(TA_NULL), TACP_KERNEL, TACP_KERNEL, TACP_KERNEL },
	{ TA_NOEXS, 0U, 0U, 0U }
};

