/* 
 *		全静的APIのテスト(1)
 * 
 *  $Id: cfg_all1.c 796 2020-05-25 07:06:39Z ertl-hiro $
 */

#include "cfg_common.h"

/*
 *  スタック領域，カーネルの管理領域の定義
 */
STK_T	stack_TASK2[COUNT_STK_T(STACK_SIZE)]
				 __attribute__((section(".noinit_kernel"),nocommon));
MPF_T	mpf_MPF2[20 * COUNT_MPF_T(200)]
				 __attribute__((section(".noinit_kernel"),nocommon));
STK_T	istack[COUNT_STK_T(10 * STACK_SIZE)]
				 __attribute__((section(".noinit_kernel"),nocommon));

/*
 *  通知用の変数の定義
 */
volatile intptr_t	nvar1;
volatile intptr_t	nvar2 __attribute__((section(".bss_kernel_add2"),nocommon));

/*
 *  関数の定義
 */
void	task1(EXINF exinf) { }
void	task2(EXINF exinf) { }
void	cyclic1_handler(EXINF exinf) { }
void	alarm1_handler(EXINF exinf) { }
void	isr1(EXINF exinf) { }
void	isr2(EXINF exinf) { }
void	isr3(EXINF exinf) { }
void	int1_handler(void) { }
void	cpuexc1_handler(void *p_excinf) { }
void	cpuexc2_handler(void *p_excinf) { }
ER_UINT	extended_svc1(intptr_t par1, intptr_t par2, intptr_t par3,
				intptr_t par4, intptr_t par5, ID cdmid) { return(E_OK); }
ER_UINT	extended_svc3(intptr_t par1, intptr_t par2, intptr_t par3,
				intptr_t par4, intptr_t par5, ID cdmid) { return(E_OK); }
void	initialize1_routine(EXINF exinf) { }
void	initialize2_routine(EXINF exinf) { }
void	terminate1_routine(EXINF exinf) { }
void	terminate2_routine(EXINF exinf) { }
