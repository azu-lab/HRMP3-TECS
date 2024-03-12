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

#ifndef TOPPERS_MACRO_ONLY

extern void	task(intptr_t exinf);
extern void	main_task(intptr_t exinf);
extern void	cyclic_handler(intptr_t exinf);
extern void	alarm_handler(intptr_t exinf);

#endif /* TOPPERS_MACRO_ONLY */
