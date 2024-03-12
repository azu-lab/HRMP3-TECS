/* 
 *		警告を出す場合のテスト(1)
 * 
 *  $Id: cfg_warning1.c 992 2022-11-20 13:12:51Z ertl-hiro $
 */

#include "cfg_common.h"

/*
 *  関数の定義
 */
void	task1(EXINF exinf) { }

/*
 *  変数の定義
 */
volatile int    var1 __attribute__((section(".data_kernel_1"),nocommon)) = 1;
