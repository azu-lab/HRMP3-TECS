/*
 *  TOPPERS Software
 *      Toyohashi Open Platform for Embedded Real-Time Systems
 * 
 *  Copyright (C) 2015-2023 by Embedded and Real-Time Systems Laboratory
 *              Graduate School of Information Science, Nagoya Univ., JAPAN
 * 
 *  上記著作権者は，以下の(1)〜(4)の条件を満たす場合に限り，本ソフトウェ
 *  ア（本ソフトウェアを改変したものを含む．以下同じ）を使用・複製・改
 *  変・再配布（以下，利用と呼ぶ）することを無償で許諾する．
 *  (1) 本ソフトウェアをソースコードの形で利用する場合には，上記の著作
 *      権表示，この利用条件および下記の無保証規定が，そのままの形でソー
 *      スコード中に含まれていること．
 *  (2) 本ソフトウェアを，ライブラリ形式など，他のソフトウェア開発に使
 *      用できる形で再配布する場合には，再配布に伴うドキュメント（利用
 *      者マニュアルなど）に，上記の著作権表示，この利用条件および下記
 *      の無保証規定を掲載すること．
 *  (3) 本ソフトウェアを，機器に組み込むなど，他のソフトウェア開発に使
 *      用できない形で再配布する場合には，次のいずれかの条件を満たすこ
 *      と．
 *    (a) 再配布に伴うドキュメント（利用者マニュアルなど）に，上記の著
 *        作権表示，この利用条件および下記の無保証規定を掲載すること．
 *    (b) 再配布の形態を，別に定める方法によって，TOPPERSプロジェクトに
 *        報告すること．
 *  (4) 本ソフトウェアの利用により直接的または間接的に生じるいかなる損
 *      害からも，上記著作権者およびTOPPERSプロジェクトを免責すること．
 *      また，本ソフトウェアのユーザまたはエンドユーザからのいかなる理
 *      由に基づく請求からも，上記著作権者およびTOPPERSプロジェクトを
 *      免責すること．
 * 
 *  本ソフトウェアは，無保証で提供されているものである．上記著作権者お
 *  よびTOPPERSプロジェクトは，本ソフトウェアに関して，特定の使用目的
 *  に対する適合性も含めて，いかなる保証も行わない．また，本ソフトウェ
 *  アの利用により直接的または間接的に生じたいかなる損害に関しても，そ
 *  の責任を負わない．
 * 
 *  $Id: test_mprot1.c 1067 2023-01-09 05:31:35Z ertl-hiro $
 */

/* 
 *		メモリ保護機能のテスト(1)
 *
 * 【テストの目的】
 *
 *  データアクセスに対するメモリ保護機能を，prb_memと直接アクセスを併用
 *  してテストする．
 *
 * 【テスト項目】
 *
 *	(A) 自保護ドメインのリード／ライト領域
 *		(A-1) 直接アクセスによりリードが可能なこと
 *		(A-2) 自タスクからのprb_memがリード可能と判断すること
 *		(A-3) 他タスクからのprb_memがリード可能と判断すること
 *		(A-4) 直接アクセスによりライトが可能なこと
 *		(A-5) 自タスクからのprb_memがライト可能と判断すること
 *		(A-6) 他タスクからのprb_memがライト可能と判断すること
 *	(B) 自保護ドメインのメモリオブジェクト属性によるリードオンリー領域
 *		(B-1) 直接アクセスによりリードが可能なこと
 *		(B-2) 自タスクからのprb_memがリード可能と判断すること
 *		(B-3) 他タスクからのprb_memがリード可能と判断すること
 *		(B-4) 直接アクセスによりライトが不可能なこと
 *		(B-5) 自タスクからのprb_memがライト不可能と判断すること
 *		(B-6) 他タスクからのprb_memがライト不可能と判断すること
 *	(C) 自保護ドメインのアクセス許可ベクタによるリードオンリー領域
 *		(C-1) 直接アクセスによりリードが可能なこと
 *		(C-2) 自タスクからのprb_memがリード可能と判断すること
 *		(C-3) 他タスクからのprb_memがリード可能と判断すること
 *		(C-4) 直接アクセスによりライトが不可能なこと
 *		(C-5) 自タスクからのprb_memがライト不可能と判断すること
 *		(C-6) 他タスクからのprb_memがライト不可能と判断すること
 *	(D) カーネルドメインの共有リード／専有ライト領域
 *		(D-1) 直接アクセスによりリードが可能なこと
 *		(D-2) 自タスクからのprb_memがリード可能と判断すること
 *		(D-3) 他タスクからのprb_memがリード可能と判断すること
 *		(D-4) 直接アクセスによりライトが不可能なこと
 *		(D-5) 自タスクからのprb_memがライト不可能と判断すること
 *		(D-6) 他タスクからのprb_memがライト不可能と判断すること
 *	(E) カーネルドメインの専有リード／ライト領域
 *		(E-1) 直接アクセスによりリードが不可能なこと
 *		(E-2) 自タスクからのprb_memがリード不可能と判断すること
 *		(E-3) 他タスクからのprb_memがリード不可能と判断すること
 *		(E-4) 直接アクセスによりライトが不可能なこと
 *		(E-5) 自タスクからのprb_memがライト不可能と判断すること
 *		(E-6) 他タスクからのprb_memがライト不可能と判断すること
 *	(F) 無所属のリード／ライト領域
 *		(F-1) 直接アクセスによりリードが可能なこと
 *		(F-2) 自タスクからのprb_memがリード可能と判断すること
 *		(F-3) 他タスクからのprb_memがリード可能と判断すること
 *		(F-4) 直接アクセスによりライトが可能なこと
 *		(F-5) 自タスクからのprb_memがライト可能と判断すること
 *		(F-6) 他タスクからのprb_memがライト可能と判断すること
 *
 * 【使用リソース】
 *
 *	TASK1: 中優先度タスク，メインタスク，最初から起動，カーネルドメイン
 *	TASK2: 低優先度タスク，最初から起動，ユーザドメイン
 *	CPUEXC1: メモリ保護違反例外ハンドラ
 *
 *	var1: 自保護ドメインのリード／ライト領域においた変数
 *	var2: 自保護ドメインのメモリオブジェクト属性によるリードオンリー領
 *	      域においた変数
 *	var3: 自保護ドメインのアクセス許可ベクタによるリードオンリー領域に
 *	      おいた変数
 *	var4: カーネルドメインの共有リード／専有ライト領域においた変数
 *	var5: カーネルドメインの専有リード／ライト領域においた変数
 *
 * 【テストシーケンス】
 *
 *	== TASK1（優先度：中）==
 *	1:	DO(var1 = MAGIC1)
 *		slp_tsk()
 *	== TASK2（優先度：低）==
 *	2:	DO(var0 = var1)												... (A-1)
 *		assert(var0 == MAGIC1)
 *		DO(var0 = var2)												... (B-1)
 *		assert(var0 == MAGIC2)
 *		DO(var0 = var3)												... (C-1)
 *		assert(var0 == MAGIC3)
 *	3:	DO(var1 = MAGIC4)											... (A-4)
 *	4:	DO(var2 = MAGIC5)											... (B-4)
 *	== CPUEXC1-1 ==
 *  5:	DO(PREPARE_RETURN_MACV_DATA)
 *		RETURN
 *	== TASK2（続き）==
 *	6:	DO(var3 = MAGIC6)											... (C-4)
 *	== CPUEXC1-2 ==
 *  7:	DO(PREPARE_RETURN_MACV_DATA)
 *		RETURN
 *	== TASK2（続き）==
 *	8:	assert(var1 == MAGIC4)
 *		assert(var2 == MAGIC2)
 *		assert(var3 == MAGIC3)
 *	9:	prb_mem((void *) &var1, sizeof(uint_t), TSK_SELF, TPM_READ)	... (A-2)
 *		prb_mem((void *) &var1, sizeof(uint_t), TSK_SELF, TPM_WRITE) ... (A-5)
 *		prb_mem((void *) &var1, sizeof(uint_t), TSK_SELF, TPM_EXEC) -> E_MACV
 *		prb_mem((void *) &var2, sizeof(uint_t), TSK_SELF, TPM_READ)	... (B-2)
 *		prb_mem((void *) &var2, sizeof(uint_t), TSK_SELF, TPM_WRITE) -> E_MACV
 *																	... (B-5)
 *		prb_mem((void *) &var2, sizeof(uint_t), TSK_SELF, TPM_EXEC) -> E_MACV
 *		prb_mem((void *) &var3, sizeof(uint_t), TSK_SELF, TPM_READ)	... (C-2)
 *		prb_mem((void *) &var3, sizeof(uint_t), TSK_SELF, TPM_WRITE) -> E_MACV
 *																	... (C-5)
 *		prb_mem((void *) &var3, sizeof(uint_t), TSK_SELF, TPM_EXEC) -> E_MACV
 *	10:	wup_tsk(TASK1)
 *	== TASK1（続き）==
 *	11:	prb_mem((void *) &var1, sizeof(uint_t), TASK2, TPM_READ)	... (A-3)
 *		prb_mem((void *) &var1, sizeof(uint_t), TASK2, TPM_WRITE)	... (A-6)
 *		prb_mem((void *) &var1, sizeof(uint_t), TASK2, TPM_EXEC) -> E_MACV
 *		prb_mem((void *) &var2, sizeof(uint_t), TASK2, TPM_READ)	... (B-3)
 *		prb_mem((void *) &var2, sizeof(uint_t), TASK2, TPM_WRITE) -> E_MACV
 *																	... (B-6)
 *		prb_mem((void *) &var2, sizeof(uint_t), TASK2, TPM_EXEC) -> E_MACV
 *		prb_mem((void *) &var3, sizeof(uint_t), TASK2, TPM_READ)	... (C-3)
 *		prb_mem((void *) &var3, sizeof(uint_t), TASK2, TPM_WRITE) -> E_MACV
 *																	... (C-6)
 *		prb_mem((void *) &var3, sizeof(uint_t), TASK2, TPM_EXEC) -> E_MACV
 *	12:	DO(var4 = MAGIC1)
 *		DO(var5 = MAGIC1)
 *		slp_tsk()
 *	== TASK2（続き）==
 *	13:	DO(var0 = var4)												... (D-1)
 *		assert(var0 == MAGIC1)
 *	14:	DO(var1 = var5)												... (E-1)
 *	== CPUEXC1-3 ==
 *  15:	DO(PREPARE_RETURN_MACV_DATA)
 *		RETURN
 *	== TASK2（続き）==
 *	16:	DO(var4 = MAGIC2)											... (D-4)
 *	== CPUEXC1-4 ==
 *  17:	DO(PREPARE_RETURN_MACV_DATA)
 *		RETURN
 *	== TASK2（続き）==
 *	18:	DO(var5 = MAGIC2)											... (E-4)
 *	== CPUEXC1-5 ==
 *  19:	DO(PREPARE_RETURN_MACV_DATA)
 *		RETURN
 *	== TASK2（続き）==
 *	20:	assert(var4 == MAGIC1)
 *	21:	prb_mem((void *) &var4, sizeof(uint_t), TSK_SELF, TPM_READ)	... (D-2)
 *		prb_mem((void *) &var4, sizeof(uint_t), TSK_SELF, TPM_WRITE) -> E_MACV
 *																	... (D-5)
 *		prb_mem((void *) &var4, sizeof(uint_t), TSK_SELF, TPM_EXEC) -> E_MACV
 *		prb_mem((void *) &var5, sizeof(uint_t), TSK_SELF, TPM_READ) -> E_MACV
 *																	... (E-2)
 *		prb_mem((void *) &var5, sizeof(uint_t), TSK_SELF, TPM_WRITE) -> E_MACV
 *																	... (E-5)
 *		prb_mem((void *) &var5, sizeof(uint_t), TSK_SELF, TPM_EXEC) -> E_MACV
 *	22:	wup_tsk(TASK1)
 *	== TASK1（続き）==
 *	23:	assert(var5 == MAGIC1)
 *	24:	prb_mem((void *) &var4, sizeof(uint_t), TASK2, TPM_READ)	... (D-3)
 *		prb_mem((void *) &var4, sizeof(uint_t), TASK2, TPM_WRITE) -> E_MACV
 *																	... (D-6)
 *		prb_mem((void *) &var4, sizeof(uint_t), TASK2, TPM_EXEC) -> E_MACV
 *		prb_mem((void *) &var5, sizeof(uint_t), TASK2, TPM_READ) -> E_MACV
 *																	... (E-3)
 *		prb_mem((void *) &var5, sizeof(uint_t), TASK2, TPM_WRITE) -> E_MACV
 *																	... (E-6)
 *		prb_mem((void *) &var5, sizeof(uint_t), TASK2, TPM_EXEC) -> E_MACV
 *	25:	DO(var6 = MAGIC1)
 *		slp_tsk()
 *	== TASK2（優先度：低）==
 *	26:	DO(var0 = var6)												... (F-1)
 *		assert(var0 == MAGIC1)
 *	27:	DO(var6 = MAGIC4)											... (F-4)
 *	28:	assert(var6 == MAGIC4)
 *	29:	prb_mem((void *) &var6, sizeof(uint_t), TSK_SELF, TPM_READ)	... (F-2)
 *		prb_mem((void *) &var6, sizeof(uint_t), TSK_SELF, TPM_WRITE) ... (F-5)
 *		prb_mem((void *) &var6, sizeof(uint_t), TSK_SELF, TPM_EXEC) -> E_MACV
 *	30:	wup_tsk(TASK1)
 *	== TASK1（続き）==
 *	31:	prb_mem((void *) &var6, sizeof(uint_t), TASK2, TPM_READ)	... (F-3)
 *		prb_mem((void *) &var6, sizeof(uint_t), TASK2, TPM_WRITE)	... (F-6)
 *		prb_mem((void *) &var6, sizeof(uint_t), TASK2, TPM_EXEC) -> E_MACV
 *	32:	END
 */

#include <kernel.h>
#include <t_syslog.h>
#include "syssvc/test_svc.h"
#include "kernel_cfg.h"
#include "test_common.h"

#ifndef PREPARE_RETURN_MACV_INST
#define PREPARE_RETURN_MACV_INST
#endif /* PREPARE_RETURN_MACV_INST */

#ifndef PREPARE_RETURN_MACV_DATA
#define PREPARE_RETURN_MACV_DATA
#endif /* PREPARE_RETURN_MACV_DATA */

#define MAGIC1		0x12345678U
#define MAGIC2		0x23456789U
#define MAGIC3		0x3456789aU
#define MAGIC4		0x456789abU
#define MAGIC5		0x56789abcU
#define MAGIC6		0x6789abcdU

uint_t			var0;
volatile uint_t	var1;
volatile uint_t	var2 __attribute__((section(".rodata_DOM1_1"))) = MAGIC2;
volatile uint_t	var3 __attribute__((section(".rodata_DOM1_2"))) = MAGIC3;
volatile uint_t	var4 __attribute__((section(".srpw_kernel_1")));
volatile uint_t	var5 __attribute__((section(".data_kernel_1")));
volatile uint_t	var6 __attribute__((section(".data_shared_1")));

/* DO NOT DELETE THIS LINE -- gentest depends on it. */

static uint_t	cpuexc1_count = 0;

void
cpuexc1_handler(void *p_excinf)
{

	switch (++cpuexc1_count) {
	case 1:
		check_point(5);
		PREPARE_RETURN_MACV_DATA;

		return;

		check_assert(false);

	case 2:
		check_point(7);
		PREPARE_RETURN_MACV_DATA;

		return;

		check_assert(false);

	case 3:
		check_point(15);
		PREPARE_RETURN_MACV_DATA;

		return;

		check_assert(false);

	case 4:
		check_point(17);
		PREPARE_RETURN_MACV_DATA;

		return;

		check_assert(false);

	case 5:
		check_point(19);
		PREPARE_RETURN_MACV_DATA;

		return;

		check_assert(false);

	default:
		check_assert(false);
	}
	check_assert(false);
}

void
task1(EXINF exinf)
{
	ER_UINT	ercd;

	test_start(__FILE__);

	check_point(1);
	var1 = MAGIC1;

	ercd = slp_tsk();
	check_ercd(ercd, E_OK);

	check_point(11);
	ercd = prb_mem((void *) &var1, sizeof(uint_t), TASK2, TPM_READ);
	check_ercd(ercd, E_OK);

	ercd = prb_mem((void *) &var1, sizeof(uint_t), TASK2, TPM_WRITE);
	check_ercd(ercd, E_OK);

	ercd = prb_mem((void *) &var1, sizeof(uint_t), TASK2, TPM_EXEC);
	check_ercd(ercd, E_MACV);

	ercd = prb_mem((void *) &var2, sizeof(uint_t), TASK2, TPM_READ);
	check_ercd(ercd, E_OK);

	ercd = prb_mem((void *) &var2, sizeof(uint_t), TASK2, TPM_WRITE);
	check_ercd(ercd, E_MACV);

	ercd = prb_mem((void *) &var2, sizeof(uint_t), TASK2, TPM_EXEC);
	check_ercd(ercd, E_MACV);

	ercd = prb_mem((void *) &var3, sizeof(uint_t), TASK2, TPM_READ);
	check_ercd(ercd, E_OK);

	ercd = prb_mem((void *) &var3, sizeof(uint_t), TASK2, TPM_WRITE);
	check_ercd(ercd, E_MACV);

	ercd = prb_mem((void *) &var3, sizeof(uint_t), TASK2, TPM_EXEC);
	check_ercd(ercd, E_MACV);

	check_point(12);
	var4 = MAGIC1;

	var5 = MAGIC1;

	ercd = slp_tsk();
	check_ercd(ercd, E_OK);

	check_point(23);
	check_assert(var5 == MAGIC1);

	check_point(24);
	ercd = prb_mem((void *) &var4, sizeof(uint_t), TASK2, TPM_READ);
	check_ercd(ercd, E_OK);

	ercd = prb_mem((void *) &var4, sizeof(uint_t), TASK2, TPM_WRITE);
	check_ercd(ercd, E_MACV);

	ercd = prb_mem((void *) &var4, sizeof(uint_t), TASK2, TPM_EXEC);
	check_ercd(ercd, E_MACV);

	ercd = prb_mem((void *) &var5, sizeof(uint_t), TASK2, TPM_READ);
	check_ercd(ercd, E_MACV);

	ercd = prb_mem((void *) &var5, sizeof(uint_t), TASK2, TPM_WRITE);
	check_ercd(ercd, E_MACV);

	ercd = prb_mem((void *) &var5, sizeof(uint_t), TASK2, TPM_EXEC);
	check_ercd(ercd, E_MACV);

	check_point(25);
	var6 = MAGIC1;

	ercd = slp_tsk();
	check_ercd(ercd, E_OK);

	check_point(31);
	ercd = prb_mem((void *) &var6, sizeof(uint_t), TASK2, TPM_READ);
	check_ercd(ercd, E_OK);

	ercd = prb_mem((void *) &var6, sizeof(uint_t), TASK2, TPM_WRITE);
	check_ercd(ercd, E_OK);

	ercd = prb_mem((void *) &var6, sizeof(uint_t), TASK2, TPM_EXEC);
	check_ercd(ercd, E_MACV);

	check_finish(32);
	check_assert(false);
}

void
task2(EXINF exinf)
{
	ER_UINT	ercd;

	check_point(2);
	var0 = var1;

	check_assert(var0 == MAGIC1);

	var0 = var2;

	check_assert(var0 == MAGIC2);

	var0 = var3;

	check_assert(var0 == MAGIC3);

	check_point(3);
	var1 = MAGIC4;

	check_point(4);
	var2 = MAGIC5;

	check_point(6);
	var3 = MAGIC6;

	check_point(8);
	check_assert(var1 == MAGIC4);

	check_assert(var2 == MAGIC2);

	check_assert(var3 == MAGIC3);

	check_point(9);
	ercd = prb_mem((void *) &var1, sizeof(uint_t), TSK_SELF, TPM_READ);
	check_ercd(ercd, E_OK);

	ercd = prb_mem((void *) &var1, sizeof(uint_t), TSK_SELF, TPM_WRITE);
	check_ercd(ercd, E_OK);

	ercd = prb_mem((void *) &var1, sizeof(uint_t), TSK_SELF, TPM_EXEC);
	check_ercd(ercd, E_MACV);

	ercd = prb_mem((void *) &var2, sizeof(uint_t), TSK_SELF, TPM_READ);
	check_ercd(ercd, E_OK);

	ercd = prb_mem((void *) &var2, sizeof(uint_t), TSK_SELF, TPM_WRITE);
	check_ercd(ercd, E_MACV);

	ercd = prb_mem((void *) &var2, sizeof(uint_t), TSK_SELF, TPM_EXEC);
	check_ercd(ercd, E_MACV);

	ercd = prb_mem((void *) &var3, sizeof(uint_t), TSK_SELF, TPM_READ);
	check_ercd(ercd, E_OK);

	ercd = prb_mem((void *) &var3, sizeof(uint_t), TSK_SELF, TPM_WRITE);
	check_ercd(ercd, E_MACV);

	ercd = prb_mem((void *) &var3, sizeof(uint_t), TSK_SELF, TPM_EXEC);
	check_ercd(ercd, E_MACV);

	check_point(10);
	ercd = wup_tsk(TASK1);
	check_ercd(ercd, E_OK);

	check_point(13);
	var0 = var4;

	check_assert(var0 == MAGIC1);

	check_point(14);
	var1 = var5;

	check_point(16);
	var4 = MAGIC2;

	check_point(18);
	var5 = MAGIC2;

	check_point(20);
	check_assert(var4 == MAGIC1);

	check_point(21);
	ercd = prb_mem((void *) &var4, sizeof(uint_t), TSK_SELF, TPM_READ);
	check_ercd(ercd, E_OK);

	ercd = prb_mem((void *) &var4, sizeof(uint_t), TSK_SELF, TPM_WRITE);
	check_ercd(ercd, E_MACV);

	ercd = prb_mem((void *) &var4, sizeof(uint_t), TSK_SELF, TPM_EXEC);
	check_ercd(ercd, E_MACV);

	ercd = prb_mem((void *) &var5, sizeof(uint_t), TSK_SELF, TPM_READ);
	check_ercd(ercd, E_MACV);

	ercd = prb_mem((void *) &var5, sizeof(uint_t), TSK_SELF, TPM_WRITE);
	check_ercd(ercd, E_MACV);

	ercd = prb_mem((void *) &var5, sizeof(uint_t), TSK_SELF, TPM_EXEC);
	check_ercd(ercd, E_MACV);

	check_point(22);
	ercd = wup_tsk(TASK1);
	check_ercd(ercd, E_OK);

	check_point(26);
	var0 = var6;

	check_assert(var0 == MAGIC1);

	check_point(27);
	var6 = MAGIC4;

	check_point(28);
	check_assert(var6 == MAGIC4);

	check_point(29);
	ercd = prb_mem((void *) &var6, sizeof(uint_t), TSK_SELF, TPM_READ);
	check_ercd(ercd, E_OK);

	ercd = prb_mem((void *) &var6, sizeof(uint_t), TSK_SELF, TPM_WRITE);
	check_ercd(ercd, E_OK);

	ercd = prb_mem((void *) &var6, sizeof(uint_t), TSK_SELF, TPM_EXEC);
	check_ercd(ercd, E_MACV);

	check_point(30);
	ercd = wup_tsk(TASK1);
	check_ercd(ercd, E_OK);

	check_assert(false);
}
