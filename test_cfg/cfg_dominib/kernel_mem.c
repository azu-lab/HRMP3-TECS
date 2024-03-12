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
 *  Protection Domain Management Functions
 */

const ID _kernel_tmax_domid = (TMIN_DOMID + TNUM_DOMID - 1);

extern SCHEDCB _kernel_schedcb_kernel_prc1;
extern SCHEDCB _kernel_schedcb_idle_prc1;
extern SCHEDCB _kernel_schedcb_DOM1_prc1;
extern SCHEDCB _kernel_schedcb_kernel_prc2;
extern SCHEDCB _kernel_schedcb_idle_prc2;
extern SCHEDCB _kernel_schedcb_DOM1_prc2;

extern TMEVTN _kernel_tmevt_heap_kernel_prc1[];
extern TMEVTN _kernel_tmevt_heap_idle_prc1[];
extern TMEVTN _kernel_tmevt_heap_DOM1_prc1[];
extern TMEVTN _kernel_tmevt_heap_kernel_prc2[];
extern TMEVTN _kernel_tmevt_heap_idle_prc2[];
extern TMEVTN _kernel_tmevt_heap_DOM1_prc2[];
extern TMEVTN _kernel_tmevt_heap_kernel_prc3[];
extern TMEVTN _kernel_tmevt_heap_kernel_prc4[];

const DOMINIB _kernel_dominib_kernel = { TACP_KERNEL, { &_kernel_schedcb_kernel_prc1, &_kernel_schedcb_kernel_prc2, NULL, NULL }, { _kernel_tmevt_heap_kernel_prc1, _kernel_tmevt_heap_kernel_prc2, _kernel_tmevt_heap_kernel_prc3, _kernel_tmevt_heap_kernel_prc4 }, { TACP_KERNEL, TACP_KERNEL, TACP_KERNEL, TACP_KERNEL }};

const DOMINIB _kernel_dominib_table[TNUM_DOMID] = {
	{ TACP(DOM1), { &_kernel_schedcb_DOM1_prc1, &_kernel_schedcb_DOM1_prc2, NULL, NULL }, { _kernel_tmevt_heap_DOM1_prc1, _kernel_tmevt_heap_DOM1_prc2, NULL, NULL }, { TACP(DOM1), TACP(DOM1), TACP_KERNEL, TACP(DOM1) } },
	{ TACP(DOM2), { NULL, NULL, NULL, NULL }, { NULL, NULL, NULL, NULL }, { TACP(DOM2), TACP(DOM2), TACP_KERNEL, TACP(DOM2) } },
	{ TACP(DOM3), { &_kernel_schedcb_idle_prc1, &_kernel_schedcb_idle_prc2, NULL, NULL }, { _kernel_tmevt_heap_idle_prc1, _kernel_tmevt_heap_idle_prc2, NULL, NULL }, { TACP(DOM3), TACP(DOM3), TACP_KERNEL, TACP(DOM3) } }
};

/*
 *  Memory Object Initialization Information
 */

extern char __start_mo_text_kernel[];
extern char __start_mo_rodata_kernel[];
extern char __start_mo_text_shared[];
extern char __aend_mo_text_shared[];
extern char __start_mo_rwdata_kernel[];
extern char __start_ustack_TASK3[];
extern char __start_ustack_TASK5[];
extern char __start_ustack_TASK4[];
extern char __start_ustack_TASK6[];
extern char __aend_ustack_TASK6[];

const uint_t _kernel_tnum_meminib = 11U;

void *const _kernel_memtop_table[11] = {
	0,
	__start_mo_text_kernel /* 0x01000000 */,
	__start_mo_rodata_kernel /* 0x0100b000 */,
	__start_mo_text_shared /* 0x0100c000 */,
	__aend_mo_text_shared /* 0x0100d000 */,
	__start_mo_rwdata_kernel /* 0x01800000 */,
	__start_ustack_TASK3 /* 0x01809000 */,
	__start_ustack_TASK5 /* 0x0180a000 */,
	__start_ustack_TASK4 /* 0x0180b000 */,
	__start_ustack_TASK6 /* 0x0180c000 */,
	__aend_ustack_TASK6 /* 0x0180d000 */
};

const MEMINIB _kernel_meminib_table[11] = {
	{ TA_NOEXS, 0U, 0U, 0U },
	{ TA_TEXTSEC, 0U, TACP_KERNEL, TACP_KERNEL },
	{ TA_RODATASEC, 0U, TACP_KERNEL, TACP_KERNEL },
	{ TA_TEXTSEC, 0U, TACP_SHARED, TACP_SHARED },
	{ TA_NOEXS, 0U, 0U, 0U },
	{ 0x0, TACP_KERNEL, TACP_KERNEL, TACP_KERNEL },
	{ TA_USTACK|(TA_NOINITSEC), TACP(DOM1), TACP(DOM1), TACP(DOM1) },
	{ TA_USTACK|(TA_NOINITSEC), TACP(DOM3), TACP(DOM3), TACP(DOM3) },
	{ TA_USTACK|(TA_NOINITSEC), TACP(DOM1), TACP(DOM1), TACP(DOM1) },
	{ TA_USTACK|(TA_NOINITSEC), TACP(DOM3), TACP(DOM3), TACP(DOM3) },
	{ TA_NOEXS, 0U, 0U, 0U }
};

/*
 *  Data Section Initialization Information
 */

const uint_t _kernel_tnum_datasec = 0U;

TOPPERS_EMPTY_LABEL(const DATASECINIB, _kernel_datasecinib_table);

/*
 *  BSS Section Initialization Information
 */

extern char __start_bss_kernel[];
extern char __end_bss_kernel[];

const uint_t _kernel_tnum_bsssec = 1U;

const BSSSECINIB _kernel_bsssecinib_table[1] = {
	{ __start_bss_kernel, __end_bss_kernel }
};

