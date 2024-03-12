/* kernel_cfg.c */
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
 *  Processor Management Functions
 */

bool_t	_kernel_kerflg_table[TNUM_PRCID];

static PCB _kernel_pcb_prc1 __attribute__((section(".kernel_data_CLS_PRC1"),nocommon));
static PCB _kernel_pcb_prc2 __attribute__((section(".kernel_data_CLS_PRC2"),nocommon));
static PCB _kernel_pcb_prc3 __attribute__((section(".kernel_data_CLS_PRC3"),nocommon));
static PCB _kernel_pcb_prc4 __attribute__((section(".kernel_data_CLS_PRC4"),nocommon));

PCB	*const _kernel_p_pcb_table[TNUM_PRCID] = {
	&_kernel_pcb_prc1,
	&_kernel_pcb_prc2,
	&_kernel_pcb_prc3,
	&_kernel_pcb_prc4
};

/*
 *  Task Management Functions
 */

const ID _kernel_tmax_tskid = (TMIN_TSKID + TNUM_TSKID - 1);

static STK_T _kernel_sstack_TASK1[COUNT_STK_T(STACK_SIZE)] __attribute__((section(".system_stack_CLS_PRC1"),nocommon));
static STK_T _kernel_sstack_TASK2[COUNT_STK_T(STACK_SIZE)] __attribute__((section(".system_stack_CLS_PRC2"),nocommon));
static STK_T _kernel_sstack_TASK3[COUNT_STK_T(DEFAULT_SSTKSZ)] __attribute__((section(".system_stack_CLS_PRC1"),nocommon));
static STK_T _kernel_ustack_TASK3[COUNT_STK_T(4096)] __attribute__((section(".ustack_TASK3"),nocommon));
static STK_T _kernel_sstack_TASK4[COUNT_STK_T(DEFAULT_SSTKSZ)] __attribute__((section(".system_stack_CLS_PRC2"),nocommon));
static STK_T _kernel_ustack_TASK4[COUNT_STK_T(4096)] __attribute__((section(".ustack_TASK4"),nocommon));
static STK_T _kernel_sstack_TASK5[COUNT_STK_T(DEFAULT_SSTKSZ)] __attribute__((section(".system_stack_CLS_PRC1"),nocommon));
static STK_T _kernel_ustack_TASK5[COUNT_STK_T(4096)] __attribute__((section(".ustack_TASK5"),nocommon));
static STK_T _kernel_sstack_TASK6[COUNT_STK_T(DEFAULT_SSTKSZ)] __attribute__((section(".system_stack_CLS_PRC2"),nocommon));
static STK_T _kernel_ustack_TASK6[COUNT_STK_T(4096)] __attribute__((section(".ustack_TASK6"),nocommon));
const TINIB _kernel_tinib_table[TNUM_TSKID] = {
	{ (TDOM_KERNEL), (TA_ACT), (EXINF)(1), (TASK)(task1), INT_PRIORITY(MID_PRIORITY), ROUND_STK_T(STACK_SIZE), _kernel_sstack_TASK1, 0, NULL, { TACP_KERNEL, TACP_KERNEL, TACP_KERNEL, TACP_KERNEL }, 1, 0x1U },
	{ (TDOM_KERNEL), (TA_ACT), (EXINF)(2), (TASK)(task2), INT_PRIORITY(MID_PRIORITY), ROUND_STK_T(STACK_SIZE), _kernel_sstack_TASK2, 0, NULL, { TACP_KERNEL, TACP_KERNEL, TACP_KERNEL, TACP_KERNEL }, 2, 0x2U },
	{ (DOM1), (TA_ACT), (EXINF)(3), (TASK)(task3), INT_PRIORITY(MID_PRIORITY), ROUND_STK_T(DEFAULT_SSTKSZ), _kernel_sstack_TASK3, ROUND_STK_T(4096), _kernel_ustack_TASK3, { TACP(DOM1), TACP(DOM1), TACP(DOM1), TACP(DOM1) }, 1, 0x1U },
	{ (DOM1), (TA_ACT), (EXINF)(4), (TASK)(task4), INT_PRIORITY(MID_PRIORITY), ROUND_STK_T(DEFAULT_SSTKSZ), _kernel_sstack_TASK4, ROUND_STK_T(4096), _kernel_ustack_TASK4, { TACP(DOM1), TACP(DOM1), TACP(DOM1), TACP(DOM1) }, 2, 0x2U },
	{ (DOM3), (TA_ACT), (EXINF)(5), (TASK)(task5), INT_PRIORITY(MID_PRIORITY), ROUND_STK_T(DEFAULT_SSTKSZ), _kernel_sstack_TASK5, ROUND_STK_T(4096), _kernel_ustack_TASK5, { TACP(DOM3), TACP(DOM3), TACP(DOM3), TACP(DOM3) }, 1, 0x1U },
	{ (DOM3), (TA_ACT), (EXINF)(6), (TASK)(task6), INT_PRIORITY(MID_PRIORITY), ROUND_STK_T(DEFAULT_SSTKSZ), _kernel_sstack_TASK6, ROUND_STK_T(4096), _kernel_ustack_TASK6, { TACP(DOM3), TACP(DOM3), TACP(DOM3), TACP(DOM3) }, 2, 0x2U }
};

static TCB _kernel_tcb_TASK1 __attribute__((section(".kernel_data_CLS_PRC1"),nocommon));
static TCB _kernel_tcb_TASK2 __attribute__((section(".kernel_data_CLS_PRC2"),nocommon));
static TCB _kernel_tcb_TASK3 __attribute__((section(".kernel_data_CLS_PRC1"),nocommon));
static TCB _kernel_tcb_TASK4 __attribute__((section(".kernel_data_CLS_PRC2"),nocommon));
static TCB _kernel_tcb_TASK5 __attribute__((section(".kernel_data_CLS_PRC1"),nocommon));
static TCB _kernel_tcb_TASK6 __attribute__((section(".kernel_data_CLS_PRC2"),nocommon));

TCB	*const _kernel_p_tcb_table[TNUM_TSKID] = {
	&_kernel_tcb_TASK1,
	&_kernel_tcb_TASK2,
	&_kernel_tcb_TASK3,
	&_kernel_tcb_TASK4,
	&_kernel_tcb_TASK5,
	&_kernel_tcb_TASK6
};

const ID _kernel_torder_table[TNUM_TSKID] = { 
	TASK1, TASK2, TASK3, TASK4, TASK5, TASK6
};

/*
 *  Semaphore Functions
 */

const ID _kernel_tmax_semid = (TMIN_SEMID + TNUM_SEMID - 1);

TOPPERS_EMPTY_LABEL(const SEMINIB, _kernel_seminib_table);
TOPPERS_EMPTY_LABEL(SEMCB *const, _kernel_p_semcb_table);

/*
 *  Eventflag Functions
 */

const ID _kernel_tmax_flgid = (TMIN_FLGID + TNUM_FLGID - 1);

TOPPERS_EMPTY_LABEL(const FLGINIB, _kernel_flginib_table);
TOPPERS_EMPTY_LABEL(FLGCB *const, _kernel_p_flgcb_table);

/*
 *  Dataqueue Functions
 */

const ID _kernel_tmax_dtqid = (TMIN_DTQID + TNUM_DTQID - 1);

TOPPERS_EMPTY_LABEL(const DTQINIB, _kernel_dtqinib_table);
TOPPERS_EMPTY_LABEL(DTQCB *const, _kernel_p_dtqcb_table);

/*
 *  Priority Dataqueue Functions
 */

const ID _kernel_tmax_pdqid = (TMIN_PDQID + TNUM_PDQID - 1);

TOPPERS_EMPTY_LABEL(const PDQINIB, _kernel_pdqinib_table);
TOPPERS_EMPTY_LABEL(PDQCB *const, _kernel_p_pdqcb_table);

/*
 *  Mutex Functions
 */

const ID _kernel_tmax_mtxid = (TMIN_MTXID + TNUM_MTXID - 1);

TOPPERS_EMPTY_LABEL(const MTXINIB, _kernel_mtxinib_table);
TOPPERS_EMPTY_LABEL(MTXCB *const, _kernel_p_mtxcb_table);

/*
 *  Message Buffer Functions
 */

const ID _kernel_tmax_mbfid = (TMIN_MBFID + TNUM_MBFID - 1);

TOPPERS_EMPTY_LABEL(const MBFINIB, _kernel_mbfinib_table);
TOPPERS_EMPTY_LABEL(MBFCB *const, _kernel_p_mbfcb_table);

/*
 *  SpinLock Functions
 */

const ID _kernel_tmax_spnid = (TMIN_SPNID + TNUM_SPNID - 1);

TOPPERS_EMPTY_LABEL(const SPNINIB, _kernel_spninib_table);
/*
 *  Fixed-sized Memorypool Functions
 */

const ID _kernel_tmax_mpfid = (TMIN_MPFID + TNUM_MPFID - 1);

TOPPERS_EMPTY_LABEL(const MPFINIB, _kernel_mpfinib_table);
TOPPERS_EMPTY_LABEL(MPFCB *const, _kernel_p_mpfcb_table);

/*
 *  Cyclic Notification Functions
 */

const ID _kernel_tmax_cycid = (TMIN_CYCID + TNUM_CYCID - 1);

TOPPERS_EMPTY_LABEL(const CYCINIB, _kernel_cycinib_table);
TOPPERS_EMPTY_LABEL(CYCCB *const, _kernel_p_cyccb_table);

/*
 *  Alarm Notification Functions
 */

const ID _kernel_tmax_almid = (TMIN_ALMID + TNUM_ALMID - 1);

static void _kernel_almhdr_ALM1(EXINF exinf) __attribute__((section(".kernel_code_CLS_PRC1")));

static void
_kernel_almhdr_ALM1(EXINF exinf)
{
	(void) act_tsk((ID) exinf);
}

static void _kernel_almhdr_ALM2(EXINF exinf) __attribute__((section(".kernel_code_CLS_PRC2")));

static void
_kernel_almhdr_ALM2(EXINF exinf)
{
	(void) act_tsk((ID) exinf);
}

static void _kernel_almhdr_ALM3(EXINF exinf) __attribute__((section(".kernel_code_CLS_PRC1")));

static void
_kernel_almhdr_ALM3(EXINF exinf)
{
	(void) act_tsk((ID) exinf);
}

static void _kernel_almhdr_ALM4(EXINF exinf) __attribute__((section(".kernel_code_CLS_PRC2")));

static void
_kernel_almhdr_ALM4(EXINF exinf)
{
	(void) act_tsk((ID) exinf);
}

static void _kernel_almhdr_ALM5(EXINF exinf) __attribute__((section(".kernel_code_CLS_PRC1")));

static void
_kernel_almhdr_ALM5(EXINF exinf)
{
	(void) act_tsk((ID) exinf);
}

static void _kernel_almhdr_ALM6(EXINF exinf) __attribute__((section(".kernel_code_CLS_PRC2")));

static void
_kernel_almhdr_ALM6(EXINF exinf)
{
	(void) act_tsk((ID) exinf);
}

const ALMINIB _kernel_alminib_table[TNUM_ALMID] = {
	{ (TA_NULL), (intptr_t)(TASK1), _kernel_almhdr_ALM1, { TACP_KERNEL, TACP_KERNEL, TACP_KERNEL, TACP_KERNEL }, &_kernel_dominib_kernel, 1, 0x1 },
	{ (TA_NULL), (intptr_t)(TASK2), _kernel_almhdr_ALM2, { TACP_KERNEL, TACP_KERNEL, TACP_KERNEL, TACP_KERNEL }, &_kernel_dominib_kernel, 2, 0x2 },
	{ (TA_NULL), (intptr_t)(TASK3), _kernel_almhdr_ALM3, { TACP(DOM1), TACP(DOM1), TACP(DOM1), TACP(DOM1) }, &(_kernel_dominib_table[INDEX_DOM(DOM1)]), 1, 0x1 },
	{ (TA_NULL), (intptr_t)(TASK4), _kernel_almhdr_ALM4, { TACP(DOM1), TACP(DOM1), TACP(DOM1), TACP(DOM1) }, &(_kernel_dominib_table[INDEX_DOM(DOM1)]), 2, 0x2 },
	{ (TA_NULL), (intptr_t)(TASK5), _kernel_almhdr_ALM5, { TACP(DOM3), TACP(DOM3), TACP(DOM3), TACP(DOM3) }, &(_kernel_dominib_table[INDEX_DOM(DOM3)]), 1, 0x1 },
	{ (TA_NULL), (intptr_t)(TASK6), _kernel_almhdr_ALM6, { TACP(DOM3), TACP(DOM3), TACP(DOM3), TACP(DOM3) }, &(_kernel_dominib_table[INDEX_DOM(DOM3)]), 2, 0x2 }
};

static ALMCB _kernel_almcb_ALM1 __attribute__((section(".kernel_data_CLS_PRC1"),nocommon));
static ALMCB _kernel_almcb_ALM2 __attribute__((section(".kernel_data_CLS_PRC2"),nocommon));
static ALMCB _kernel_almcb_ALM3 __attribute__((section(".kernel_data_CLS_PRC1"),nocommon));
static ALMCB _kernel_almcb_ALM4 __attribute__((section(".kernel_data_CLS_PRC2"),nocommon));
static ALMCB _kernel_almcb_ALM5 __attribute__((section(".kernel_data_CLS_PRC1"),nocommon));
static ALMCB _kernel_almcb_ALM6 __attribute__((section(".kernel_data_CLS_PRC2"),nocommon));

ALMCB	*const _kernel_p_almcb_table[TNUM_ALMID] = {
	&_kernel_almcb_ALM1,
	&_kernel_almcb_ALM2,
	&_kernel_almcb_ALM3,
	&_kernel_almcb_ALM4,
	&_kernel_almcb_ALM5,
	&_kernel_almcb_ALM6
};

/*
 *  Interrupt Management Functions
 */

#ifndef LOG_ISR_ENTER
#define LOG_ISR_ENTER(isrid)
#endif /* LOG_ISR_ENTER */

#ifndef LOG_ISR_LEAVE
#define LOG_ISR_LEAVE(isrid)
#endif /* LOG_ISR_LEAVE */

#define TNUM_DEF_INHNO	24
const uint_t _kernel_tnum_def_inhno = TNUM_DEF_INHNO;

INTHDR_ENTRY(INHNO_HRT_PRC1, 65536, _kernel_target_hrt_handler)
INTHDR_ENTRY(INHNO_HRT_PRC2, 131072, _kernel_target_hrt_handler)
INTHDR_ENTRY(INHNO_HRT_PRC3, 196608, _kernel_target_hrt_handler)
INTHDR_ENTRY(INHNO_HRT_PRC4, 262144, _kernel_target_hrt_handler)
INTHDR_ENTRY(INHNO_TWDTIMER_PRC1, 65537, _kernel_target_twdtimer_handler)
INTHDR_ENTRY(INHNO_TWDTIMER_PRC2, 131073, _kernel_target_twdtimer_handler)
INTHDR_ENTRY(INHNO_TWDTIMER_PRC3, 196609, _kernel_target_twdtimer_handler)
INTHDR_ENTRY(INHNO_TWDTIMER_PRC4, 262145, _kernel_target_twdtimer_handler)
INTHDR_ENTRY(INHNO_IPI_DISPATCH_PRC1, 65540, _kernel_dispatch_handler)
INTHDR_ENTRY(INHNO_IPI_DISPATCH_PRC2, 131076, _kernel_dispatch_handler)
INTHDR_ENTRY(INHNO_IPI_DISPATCH_PRC3, 196612, _kernel_dispatch_handler)
INTHDR_ENTRY(INHNO_IPI_DISPATCH_PRC4, 262148, _kernel_dispatch_handler)
INTHDR_ENTRY(INHNO_IPI_EXT_KER_PRC1, 65541, _kernel_ext_ker_handler)
INTHDR_ENTRY(INHNO_IPI_EXT_KER_PRC2, 131077, _kernel_ext_ker_handler)
INTHDR_ENTRY(INHNO_IPI_EXT_KER_PRC3, 196613, _kernel_ext_ker_handler)
INTHDR_ENTRY(INHNO_IPI_EXT_KER_PRC4, 262149, _kernel_ext_ker_handler)
INTHDR_ENTRY(INHNO_IPI_SET_HRT_EVT_PRC1, 65542, _kernel_set_hrt_event_handler)
INTHDR_ENTRY(INHNO_IPI_SET_HRT_EVT_PRC2, 131078, _kernel_set_hrt_event_handler)
INTHDR_ENTRY(INHNO_IPI_SET_HRT_EVT_PRC3, 196614, _kernel_set_hrt_event_handler)
INTHDR_ENTRY(INHNO_IPI_SET_HRT_EVT_PRC4, 262150, _kernel_set_hrt_event_handler)
INTHDR_ENTRY(INHNO_IPI_START_SCYC_PRC1, 65543, _kernel_start_scyc_handler)
INTHDR_ENTRY(INHNO_IPI_START_SCYC_PRC2, 131079, _kernel_start_scyc_handler)
INTHDR_ENTRY(INHNO_IPI_START_SCYC_PRC3, 196615, _kernel_start_scyc_handler)
INTHDR_ENTRY(INHNO_IPI_START_SCYC_PRC4, 262151, _kernel_start_scyc_handler)

const INHINIB _kernel_inhinib_table[TNUM_DEF_INHNO] = {
	{ (INHNO_HRT_PRC1), (TA_NULL), (FP)(INT_ENTRY(INHNO_HRT_PRC1, _kernel_target_hrt_handler)), 1 },
	{ (INHNO_HRT_PRC2), (TA_NULL), (FP)(INT_ENTRY(INHNO_HRT_PRC2, _kernel_target_hrt_handler)), 2 },
	{ (INHNO_HRT_PRC3), (TA_NULL), (FP)(INT_ENTRY(INHNO_HRT_PRC3, _kernel_target_hrt_handler)), 3 },
	{ (INHNO_HRT_PRC4), (TA_NULL), (FP)(INT_ENTRY(INHNO_HRT_PRC4, _kernel_target_hrt_handler)), 4 },
	{ (INHNO_TWDTIMER_PRC1), (TA_NULL), (FP)(INT_ENTRY(INHNO_TWDTIMER_PRC1, _kernel_target_twdtimer_handler)), 1 },
	{ (INHNO_TWDTIMER_PRC2), (TA_NULL), (FP)(INT_ENTRY(INHNO_TWDTIMER_PRC2, _kernel_target_twdtimer_handler)), 2 },
	{ (INHNO_TWDTIMER_PRC3), (TA_NULL), (FP)(INT_ENTRY(INHNO_TWDTIMER_PRC3, _kernel_target_twdtimer_handler)), 3 },
	{ (INHNO_TWDTIMER_PRC4), (TA_NULL), (FP)(INT_ENTRY(INHNO_TWDTIMER_PRC4, _kernel_target_twdtimer_handler)), 4 },
	{ (INHNO_IPI_DISPATCH_PRC1), (TA_NULL), (FP)(INT_ENTRY(INHNO_IPI_DISPATCH_PRC1, _kernel_dispatch_handler)), 1 },
	{ (INHNO_IPI_DISPATCH_PRC2), (TA_NULL), (FP)(INT_ENTRY(INHNO_IPI_DISPATCH_PRC2, _kernel_dispatch_handler)), 2 },
	{ (INHNO_IPI_DISPATCH_PRC3), (TA_NULL), (FP)(INT_ENTRY(INHNO_IPI_DISPATCH_PRC3, _kernel_dispatch_handler)), 3 },
	{ (INHNO_IPI_DISPATCH_PRC4), (TA_NULL), (FP)(INT_ENTRY(INHNO_IPI_DISPATCH_PRC4, _kernel_dispatch_handler)), 4 },
	{ (INHNO_IPI_EXT_KER_PRC1), (TA_NULL), (FP)(INT_ENTRY(INHNO_IPI_EXT_KER_PRC1, _kernel_ext_ker_handler)), 1 },
	{ (INHNO_IPI_EXT_KER_PRC2), (TA_NULL), (FP)(INT_ENTRY(INHNO_IPI_EXT_KER_PRC2, _kernel_ext_ker_handler)), 2 },
	{ (INHNO_IPI_EXT_KER_PRC3), (TA_NULL), (FP)(INT_ENTRY(INHNO_IPI_EXT_KER_PRC3, _kernel_ext_ker_handler)), 3 },
	{ (INHNO_IPI_EXT_KER_PRC4), (TA_NULL), (FP)(INT_ENTRY(INHNO_IPI_EXT_KER_PRC4, _kernel_ext_ker_handler)), 4 },
	{ (INHNO_IPI_SET_HRT_EVT_PRC1), (TA_NULL), (FP)(INT_ENTRY(INHNO_IPI_SET_HRT_EVT_PRC1, _kernel_set_hrt_event_handler)), 1 },
	{ (INHNO_IPI_SET_HRT_EVT_PRC2), (TA_NULL), (FP)(INT_ENTRY(INHNO_IPI_SET_HRT_EVT_PRC2, _kernel_set_hrt_event_handler)), 2 },
	{ (INHNO_IPI_SET_HRT_EVT_PRC3), (TA_NULL), (FP)(INT_ENTRY(INHNO_IPI_SET_HRT_EVT_PRC3, _kernel_set_hrt_event_handler)), 3 },
	{ (INHNO_IPI_SET_HRT_EVT_PRC4), (TA_NULL), (FP)(INT_ENTRY(INHNO_IPI_SET_HRT_EVT_PRC4, _kernel_set_hrt_event_handler)), 4 },
	{ (INHNO_IPI_START_SCYC_PRC1), (TA_NULL), (FP)(INT_ENTRY(INHNO_IPI_START_SCYC_PRC1, _kernel_start_scyc_handler)), 1 },
	{ (INHNO_IPI_START_SCYC_PRC2), (TA_NULL), (FP)(INT_ENTRY(INHNO_IPI_START_SCYC_PRC2, _kernel_start_scyc_handler)), 2 },
	{ (INHNO_IPI_START_SCYC_PRC3), (TA_NULL), (FP)(INT_ENTRY(INHNO_IPI_START_SCYC_PRC3, _kernel_start_scyc_handler)), 3 },
	{ (INHNO_IPI_START_SCYC_PRC4), (TA_NULL), (FP)(INT_ENTRY(INHNO_IPI_START_SCYC_PRC4, _kernel_start_scyc_handler)), 4 }
};

#define TNUM_CFG_INTNO	24
const uint_t _kernel_tnum_cfg_intno = TNUM_CFG_INTNO;

const INTINIB _kernel_intinib_table[TNUM_CFG_INTNO] = {
	{ (INTNO_HRT_PRC1), (TA_ENAINT | INTATR_HRT), (INTPRI_HRT), 1, 0x1U },
	{ (INTNO_HRT_PRC2), (TA_ENAINT|INTATR_HRT), (INTPRI_HRT), 2, 0x2U },
	{ (INTNO_HRT_PRC3), (TA_ENAINT|INTATR_HRT), (INTPRI_HRT), 3, 0x4U },
	{ (INTNO_HRT_PRC4), (TA_ENAINT|INTATR_HRT), (INTPRI_HRT), 4, 0x8U },
	{ (INTNO_TWDTIMER_PRC1), (TA_ENAINT|INTATR_TWDTIMER), (INTPRI_TWDTIMER), 1, 0x1U },
	{ (INTNO_TWDTIMER_PRC2), (TA_ENAINT|INTATR_TWDTIMER), (INTPRI_TWDTIMER), 2, 0x2U },
	{ (INTNO_TWDTIMER_PRC3), (TA_ENAINT|INTATR_TWDTIMER), (INTPRI_TWDTIMER), 3, 0x4U },
	{ (INTNO_TWDTIMER_PRC4), (TA_ENAINT|INTATR_TWDTIMER), (INTPRI_TWDTIMER), 4, 0x8U },
	{ (INTNO_IPI_DISPATCH_PRC1), (TA_ENAINT), (INTPRI_IPI_DISPATCH_PRC1), 1, 0x1U },
	{ (INTNO_IPI_DISPATCH_PRC2), (TA_ENAINT), (INTPRI_IPI_DISPATCH_PRC2), 2, 0x2U },
	{ (INTNO_IPI_DISPATCH_PRC3), (TA_ENAINT), (INTPRI_IPI_DISPATCH_PRC3), 3, 0x4U },
	{ (INTNO_IPI_DISPATCH_PRC4), (TA_ENAINT), (INTPRI_IPI_DISPATCH_PRC4), 4, 0x8U },
	{ (INTNO_IPI_EXT_KER_PRC1), (TA_ENAINT), (INTPRI_IPI_EXT_KER_PRC1), 1, 0x1U },
	{ (INTNO_IPI_EXT_KER_PRC2), (TA_ENAINT), (INTPRI_IPI_EXT_KER_PRC2), 2, 0x2U },
	{ (INTNO_IPI_EXT_KER_PRC3), (TA_ENAINT), (INTPRI_IPI_EXT_KER_PRC3), 3, 0x4U },
	{ (INTNO_IPI_EXT_KER_PRC4), (TA_ENAINT), (INTPRI_IPI_EXT_KER_PRC4), 4, 0x8U },
	{ (INTNO_IPI_SET_HRT_EVT_PRC1), (TA_ENAINT), (INTPRI_IPI_SET_HRT_EVT_PRC1), 1, 0x1U },
	{ (INTNO_IPI_SET_HRT_EVT_PRC2), (TA_ENAINT), (INTPRI_IPI_SET_HRT_EVT_PRC2), 2, 0x2U },
	{ (INTNO_IPI_SET_HRT_EVT_PRC3), (TA_ENAINT), (INTPRI_IPI_SET_HRT_EVT_PRC3), 3, 0x4U },
	{ (INTNO_IPI_SET_HRT_EVT_PRC4), (TA_ENAINT), (INTPRI_IPI_SET_HRT_EVT_PRC4), 4, 0x8U },
	{ (INTNO_IPI_START_SCYC_PRC1), (TA_ENAINT), (INTPRI_IPI_START_SCYC_PRC1), 1, 0x1U },
	{ (INTNO_IPI_START_SCYC_PRC2), (TA_ENAINT), (INTPRI_IPI_START_SCYC_PRC2), 2, 0x2U },
	{ (INTNO_IPI_START_SCYC_PRC3), (TA_ENAINT), (INTPRI_IPI_START_SCYC_PRC3), 3, 0x4U },
	{ (INTNO_IPI_START_SCYC_PRC4), (TA_ENAINT), (INTPRI_IPI_START_SCYC_PRC4), 4, 0x8U }
};

/*
 *  CPU Exception Management Functions
 */

#define TNUM_DEF_EXCNO	0
const uint_t _kernel_tnum_def_excno = TNUM_DEF_EXCNO;

TOPPERS_EMPTY_LABEL(const EXCINIB, _kernel_excinib_table);

/*
 *  Extended Service Calls
 */

const FN _kernel_tmax_fncd = TMAX_FNCD;

TOPPERS_EMPTY_LABEL(const SVCINIB, _kernel_svcinib_table);

/*
 *  Access Permission Vectors for System States
 */

const ACVCT _kernel_sysstat1_acvct = { TACP_KERNEL, TACP_KERNEL, TACP_KERNEL, TACP_KERNEL };
const ACVCT _kernel_sysstat2_acvct = { TACP_KERNEL, TACP_KERNEL, TACP_KERNEL, TACP_KERNEL };

/*
 *  Stack Area for Non-task Context
 */

static STK_T _kernel_istack_prc1[COUNT_STK_T(DEFAULT_ISTKSZ)] __attribute__((section(".system_stack_CLS_PRC1"),nocommon));
static STK_T _kernel_istack_prc2[COUNT_STK_T(DEFAULT_ISTKSZ)] __attribute__((section(".system_stack_CLS_PRC2"),nocommon));
static STK_T _kernel_istack_prc3[COUNT_STK_T(DEFAULT_ISTKSZ)] __attribute__((section(".system_stack_CLS_PRC3"),nocommon));
static STK_T _kernel_istack_prc4[COUNT_STK_T(DEFAULT_ISTKSZ)] __attribute__((section(".system_stack_CLS_PRC4"),nocommon));

const size_t _kernel_istksz_table[TNUM_PRCID] = {
	ROUND_STK_T(DEFAULT_ISTKSZ),
	ROUND_STK_T(DEFAULT_ISTKSZ),
	ROUND_STK_T(DEFAULT_ISTKSZ),
	ROUND_STK_T(DEFAULT_ISTKSZ)
};

STK_T *const _kernel_istk_table[TNUM_PRCID] = {
	_kernel_istack_prc1,
	_kernel_istack_prc2,
	_kernel_istack_prc3,
	_kernel_istack_prc4
};

#ifdef TOPPERS_ISTKPT
STK_T *const _kernel_istkpt_table[TNUM_PRCID] = {
	TOPPERS_ISTKPT(_kernel_istack_prc1, ROUND_STK_T(DEFAULT_ISTKSZ)),
	TOPPERS_ISTKPT(_kernel_istack_prc2, ROUND_STK_T(DEFAULT_ISTKSZ)),
	TOPPERS_ISTKPT(_kernel_istack_prc3, ROUND_STK_T(DEFAULT_ISTKSZ)),
	TOPPERS_ISTKPT(_kernel_istack_prc4, ROUND_STK_T(DEFAULT_ISTKSZ))
};
#endif /* TOPPERS_ISTKPT */

/*
 *  Stack Area for Idle
 */

static STK_T _kernel_idstack_prc1[COUNT_STK_T(DEFAULT_IDSTKSZ)] __attribute__((section(".system_stack_CLS_PRC1"),nocommon));
static STK_T _kernel_idstack_prc2[COUNT_STK_T(DEFAULT_IDSTKSZ)] __attribute__((section(".system_stack_CLS_PRC2"),nocommon));
static STK_T _kernel_idstack_prc3[COUNT_STK_T(DEFAULT_IDSTKSZ)] __attribute__((section(".system_stack_CLS_PRC3"),nocommon));
static STK_T _kernel_idstack_prc4[COUNT_STK_T(DEFAULT_IDSTKSZ)] __attribute__((section(".system_stack_CLS_PRC4"),nocommon));

#ifndef TOPPERS_ISTKPT
STK_T *const _kernel_idstk_table[TNUM_PRCID] = {
	_kernel_idstack_prc1,
	_kernel_idstack_prc2,
	_kernel_idstack_prc3,
	_kernel_idstack_prc4
};
#endif /* TOPPERS_ISTKPT */

#ifdef TOPPERS_ISTKPT
STK_T *const _kernel_idstkpt_table[TNUM_PRCID] = {
	TOPPERS_ISTKPT(_kernel_idstack_prc1, ROUND_STK_T(DEFAULT_IDSTKSZ)),
	TOPPERS_ISTKPT(_kernel_idstack_prc2, ROUND_STK_T(DEFAULT_IDSTKSZ)),
	TOPPERS_ISTKPT(_kernel_idstack_prc3, ROUND_STK_T(DEFAULT_IDSTKSZ)),
	TOPPERS_ISTKPT(_kernel_idstack_prc4, ROUND_STK_T(DEFAULT_IDSTKSZ))
};
#endif /* TOPPERS_ISTKPT */

/*
 *  Scheduling Unit Control Blocks
 */

SCHEDCB _kernel_schedcb_kernel_prc1 __attribute__((section(".kernel_data_CLS_PRC1"),nocommon));
SCHEDCB _kernel_schedcb_idle_prc1 __attribute__((section(".kernel_data_CLS_PRC1"),nocommon));
SCHEDCB _kernel_schedcb_DOM1_prc1 __attribute__((section(".kernel_data_CLS_PRC1"),nocommon));
SCHEDCB _kernel_schedcb_kernel_prc2 __attribute__((section(".kernel_data_CLS_PRC2"),nocommon));
SCHEDCB _kernel_schedcb_idle_prc2 __attribute__((section(".kernel_data_CLS_PRC2"),nocommon));
SCHEDCB _kernel_schedcb_DOM1_prc2 __attribute__((section(".kernel_data_CLS_PRC2"),nocommon));

SCHEDCB	*const _kernel_p_schedcb_idle_table[TNUM_PRCID] = {
	&_kernel_schedcb_idle_prc1,
	&_kernel_schedcb_idle_prc2,
	NULL,
	NULL
};

/*
 *  Time Event Management
 */

TMEVTN	_kernel_tmevt_heap_kernel_prc1[1 + 3] __attribute__((section(".kernel_data_CLS_PRC1"),nocommon));
TMEVTN	_kernel_tmevt_heap_idle_prc1[1 + 2] __attribute__((section(".kernel_data_CLS_PRC1"),nocommon));
TMEVTN	_kernel_tmevt_heap_DOM1_prc1[1 + 2] __attribute__((section(".kernel_data_CLS_PRC1"),nocommon));
TMEVTN	_kernel_tmevt_heap_kernel_prc2[1 + 3] __attribute__((section(".kernel_data_CLS_PRC2"),nocommon));
TMEVTN	_kernel_tmevt_heap_idle_prc2[1 + 2] __attribute__((section(".kernel_data_CLS_PRC2"),nocommon));
TMEVTN	_kernel_tmevt_heap_DOM1_prc2[1 + 2] __attribute__((section(".kernel_data_CLS_PRC2"),nocommon));
TMEVTN	_kernel_tmevt_heap_kernel_prc3[1 + 1] __attribute__((section(".kernel_data_CLS_PRC3"),nocommon));
TMEVTN	_kernel_tmevt_heap_kernel_prc4[1 + 1] __attribute__((section(".kernel_data_CLS_PRC4"),nocommon));

TMEVTN	*const _kernel_p_tmevt_heap_idle_table[TNUM_PRCID] = {
	_kernel_tmevt_heap_idle_prc1,
	_kernel_tmevt_heap_idle_prc2,
	NULL,
	NULL
};

/*
 *  Module Initialization Function
 */

void
_kernel_initialize_object(PCB *p_my_pcb)
{
	_kernel_initialize_task(p_my_pcb);
	_kernel_initialize_alarm(p_my_pcb);
	_kernel_initialize_interrupt(p_my_pcb);
	_kernel_initialize_exception(p_my_pcb);
}

/*
 *  Initialization Routine
 */

const INIRTNB _kernel_inirtnb_table_prc1[2] = {
	{ (INIRTN)(_kernel_target_hrt_initialize), (EXINF)(0) },
	{ (INIRTN)(_kernel_target_twdtimer_initialize), (EXINF)(0) }
};

const INIRTNB _kernel_inirtnb_table_prc2[2] = {
	{ (INIRTN)(_kernel_target_hrt_initialize), (EXINF)(0) },
	{ (INIRTN)(_kernel_target_twdtimer_initialize), (EXINF)(0) }
};

const INIRTNB _kernel_inirtnb_table_prc3[2] = {
	{ (INIRTN)(_kernel_target_hrt_initialize), (EXINF)(0) },
	{ (INIRTN)(_kernel_target_twdtimer_initialize), (EXINF)(0) }
};

const INIRTNB _kernel_inirtnb_table_prc4[2] = {
	{ (INIRTN)(_kernel_target_hrt_initialize), (EXINF)(0) },
	{ (INIRTN)(_kernel_target_twdtimer_initialize), (EXINF)(0) }
};

const INIRTNBB _kernel_inirtnbb_table[TNUM_PRCID + 1] = {
	{ 0, NULL },
	{ 2, _kernel_inirtnb_table_prc1 },
	{ 2, _kernel_inirtnb_table_prc2 },
	{ 2, _kernel_inirtnb_table_prc3 },
	{ 2, _kernel_inirtnb_table_prc4 }
};

/*
 *  Termination Routine
 */

const TERRTNB _kernel_terrtnb_table_prc1[2] = {
	{ (TERRTN)(_kernel_target_twdtimer_terminate), (EXINF)(0) },
	{ (TERRTN)(_kernel_target_hrt_terminate), (EXINF)(0) }
};

const TERRTNB _kernel_terrtnb_table_prc2[2] = {
	{ (TERRTN)(_kernel_target_twdtimer_terminate), (EXINF)(0) },
	{ (TERRTN)(_kernel_target_hrt_terminate), (EXINF)(0) }
};

const TERRTNB _kernel_terrtnb_table_prc3[2] = {
	{ (TERRTN)(_kernel_target_twdtimer_terminate), (EXINF)(0) },
	{ (TERRTN)(_kernel_target_hrt_terminate), (EXINF)(0) }
};

const TERRTNB _kernel_terrtnb_table_prc4[2] = {
	{ (TERRTN)(_kernel_target_twdtimer_terminate), (EXINF)(0) },
	{ (TERRTN)(_kernel_target_hrt_terminate), (EXINF)(0) }
};

const TERRTNBB _kernel_terrtnbb_table[TNUM_PRCID + 1] = {
	{ 0, NULL },
	{ 2, _kernel_terrtnb_table_prc1 },
	{ 2, _kernel_terrtnb_table_prc2 },
	{ 2, _kernel_terrtnb_table_prc3 },
	{ 2, _kernel_terrtnb_table_prc4 }
};

/*
 *  Temporal Partitioning Functions
 */

const RELTIM _kernel_system_cyctim = 10000;

const TWDINIB _kernel_twdinib_table_prc1[2] = {
	{ 4000, &(_kernel_dominib_table[INDEX_DOM(DOM1)]), (intptr_t)(0), NULL },
	{ 4000, &(_kernel_dominib_table[INDEX_DOM(DOM2)]), (intptr_t)(0), NULL }
 };

const TWDINIB _kernel_twdinib_table_prc2[2] = {
	{ 4000, &(_kernel_dominib_table[INDEX_DOM(DOM1)]), (intptr_t)(0), NULL },
	{ 4000, &(_kernel_dominib_table[INDEX_DOM(DOM2)]), (intptr_t)(0), NULL }
 };

TOPPERS_EMPTY_LABEL(const TWDINIB, _kernel_twdinib_table_prc3);

TOPPERS_EMPTY_LABEL(const TWDINIB, _kernel_twdinib_table_prc4);

const ID _kernel_tmax_somid = (TMIN_SOMID + TNUM_SOMID - 1);

const SOMINIB *const _kernel_p_inisom = &(_kernel_sominib_table[INDEX_SOM(SOM1)]);

const SOMINIB _kernel_sominib_table[TNUM_SOMID + 1] = {
	{ { &(_kernel_twdinib_table_prc1[0]), &(_kernel_twdinib_table_prc2[0]), &(_kernel_twdinib_table_prc3[0]), &(_kernel_twdinib_table_prc4[0]) },
	  &(_kernel_sominib_table[INDEX_SOM(SOM1)]) },
	{ { &(_kernel_twdinib_table_prc1[2]), &(_kernel_twdinib_table_prc2[2]), &(_kernel_twdinib_table_prc3[0]), &(_kernel_twdinib_table_prc4[0]) },
	  NULL }
};

