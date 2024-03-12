#include "target_test.h"

#define MAIN_PRIORITY	5		/* メインタスクの優先度 */

#define HIGH_PRIORITY	9		/* 並列に実行されるタスクの優先度 */
#define MID_PRIORITY	10
#define LOW_PRIORITY	11

#ifndef TASK_PORTID
#define	TASK_PORTID		1			/* 文字入力するシリアルポートID */
#endif /* TASK_PORTID */

#ifndef STACK_SIZE
#define	STACK_SIZE		4096		/* タスクのスタックサイズ */
#endif /* STACK_SIZE */

#ifndef LOOP_REF
#define LOOP_REF		ULONG_C(1000000)	/* 速度計測用のループ回数 */
#endif /* LOOP_REF */

#undef  CYC_TIME
#define CYC_TIME 10000
