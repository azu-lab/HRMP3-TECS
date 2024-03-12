/*
 *		全静的APIのパス4でのエラー検出のテスト(2)
 * 
 *  $Id: pass4_all2.c 1005 2022-11-24 06:18:14Z ertl-hiro $
 */

#include "cfg_common.h"

/*
 *  スタック領域，カーネルの管理領域の定義
 */
STK_T	stack_TASK1[COUNT_STK_T(STACK_SIZE)]
				__attribute__((section(".system_stack_CLS_PRC1"),nocommon));
STK_T	stack_TASK3[COUNT_STK_T(STACK_SIZE)];
STK_T	stack_TASK4[COUNT_STK_T(STACK_SIZE)];
STK_T	stack_TASK5[COUNT_STK_T(STACK_SIZE)];
STK_T	istack[COUNT_STK_T(10 * STACK_SIZE)];

/*
 *  通知用の変数の定義
 */
volatile intptr_t	nvar1;
volatile intptr_t	nvar2;
const intptr_t		nvar3 = 0;
volatile intptr_t	nvar4 __attribute__((section(".bss_shared_add1")));
volatile intptr_t	nvar5 __attribute__((section(".bss_shared_add2")));
volatile intptr_t	nvar6 __attribute__((section(".bss_shared_add3")));

/*
 *  関数の定義
 */
void	task1(EXINF exinf) { }
void	task2(EXINF exinf) { }
void	task3(EXINF exinf) { }
void	task4(EXINF exinf) { }
void	task5(EXINF exinf) { }
void	task6(EXINF exinf) { }
ER_UINT	extended_svc1(intptr_t par1, intptr_t par2, intptr_t par3,
				intptr_t par4, intptr_t par5, ID cdmid) { return(E_OK); }
