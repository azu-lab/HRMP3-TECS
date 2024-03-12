/* 
 *		手動メモリ配置の場合の静的APIのテスト(1)
 * 
 *  $Id: cfg_mml1.c 890 2022-05-30 09:18:51Z ertl-hiro $
 */

#include "cfg_common.h"

/*
 *  カーネルの管理領域の定義
 */
MPF_T	mpf_MPF3[10 * COUNT_MPF_T(100)]
				 __attribute__((section(".noinit_kernel"),nocommon));
MPF_T	mpf_MPF6[20 * COUNT_MPF_T(200)]
				 __attribute__((section(".noinit_kernel"),nocommon));
/*
 *  関数の定義
 */
void	task1(EXINF exinf) { }
void	task2(EXINF exinf) { }
void	task3(EXINF exinf) { }

/*
 *
 */
#include "kernel/kernel_int.h"
const uint_t _kernel_tnum_datasec = 0;
TOPPERS_EMPTY_LABEL(const DATASECINIB, _kernel_datasecinib_table);
const uint_t _kernel_tnum_bsssec = 0;
const BSSSECINIB _kernel_bsssecinib_table[0];
