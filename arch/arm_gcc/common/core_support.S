/*
 *  TOPPERS/HRMP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      High Reliable Multiprocessing Profile Kernel
 * 
 *  Copyright (C) 2000-2004 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2006-2022 by Embedded and Real-Time Systems Laboratory
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
 *  $Id: core_support.S 1038 2022-12-23 05:52:05Z ertl-hiro $
 */

/*
 *		カーネルのコア依存部のアセンブリ言語部（ARM用）
 */

#define TOPPERS_MACRO_ONLY
#define TOPPERS_ASM_MACRO
#define UINT_C(val)			(val)		/* uint_t型の定数を作るマクロ */
#define ULONG_C(val)		(val)		/* ulong_t型の定数を作るマクロ */
#define CAST(type, val)		(val)		/* 型キャストを行うマクロ */
#include "kernel_impl.h"
#include "target_ipi.h"
#include "offset.h"
#include "core_asm.inc"

/*
 *  使用する命令セットの定義
 */
#if defined(USE_ARM_FPU_ALWAYS) || defined(USE_ARM_FPU_SELECTIVE)
	.fpu	ASM_ARM_FPU_TYPE
#endif /* defined(USE_ARM_FPU_ALWAYS) || defined(USE_ARM_FPU_SELECTIVE) */

/*
 *  例外ベクタ
 */
	ASECTION(.vector)
	AALIGN(5)
	AGLOBAL(vector_table)
ALABEL(vector_table)
	ldr		pc, reset_vector		/* リセット */
	ldr		pc, undef_vector		/* 未定義命令 */
	ldr		pc, svc_vector			/* ソフトウェア割込み */
	ldr		pc, pabort_vector		/* プリフェッチアボート */
	ldr		pc, dabort_vector		/* データアボート */
	ldr		pc, reset_vector		/* 未使用 */
	ldr		pc, irq_vector			/* IRQ */
	ldr		pc, fiq_vector			/* FIQ */

/*
 *  例外ベクタの命令から参照されるジャンプ先アドレス
 */
	AGLOBAL(vector_ref_table)
ALABEL(vector_ref_table)
ALABEL(reset_vector)
	ALONG	start
ALABEL(undef_vector)
	ALONG	undef_handler
ALABEL(svc_vector)
	ALONG	svc_handler
ALABEL(pabort_vector)
	ALONG	pabort_handler
ALABEL(dabort_vector)
	ALONG	dabort_handler
ALABEL(irq_vector)
	ALONG	irq_handler
ALABEL(fiq_vector)
	ALONG	fiq_handler

/*
 *  タスクディスパッチャ
 */
	ATEXT
	AALIGN(2)
	AGLOBAL(dispatch)
ALABEL(dispatch)
	/*
	 *  このルーチンは，タスクコンテキスト・CPUロック状態・割込み優先度
	 *  マスク全解除状態・ディスパッチ許可状態で呼び出される．
	 * 
	 *  カーネルドメインのタスクが対象の場合，ユーザモードのspとlrの保
	 *  存の必要性はないが，保存しても副作用はなく，判別のオーバヘッド
	 *  を考え，無条件で保存している．
	 */
	push	{r12,lr}				/* 戻り番地（lr）を保存 */
									/* r12はスペース確保のため */
#ifdef TOPPERS_SUPPORT_OVRHDR
	bl		ovrtimer_stop
#endif /* TOPPERS_SUPPORT_OVRHDR */
	my_pcb	r1, r2
	ldr		r2, [r1,#PCB_rundom]	/* rundomをスタックに保存（r12の場所）*/
	str		r2, [sp]
	push	{r4-r11}				/* 非スクラッチレジスタの保存 */
	sub		sp, sp, #8
	stm		sp, {sp,lr}^			/* sp_usr,lr_usrを保存 */
	ldr		r0, [r1,#PCB_p_runtsk]	/* p_runtsk → r0 */
#ifdef USE_ARM_FPU_ALWAYS
	vpush	{d8-d15}				/* 非スクラッチFPUレジスタの保存 */
#endif /* USE_ARM_FPU_ALWAYS */
#ifdef USE_ARM_FPU_SELECTIVE
	vmrs	r1, fpexc
	tst		r1, #FPEXC_ENABLE
	vpushne	{d8-d15}				/* 非スクラッチFPUレジスタの保存 */
#endif /* USE_ARM_FPU_SELECTIVE */
	str		sp, [r0,#TCB_sp]		/* スタックポインタを保存 */
	adr		r1, dispatch_r
	str		r1, [r0,#TCB_pc]		/* 実行再開番地を保存 */
	b		dispatcher				/* r0にはp_runtskが格納されている */

ALABEL(dispatch_r)
	/*
	 * 【この時点のレジスタ状態】
	 *  r4：p_runtsk（タスク切換え後）
	 */
#ifdef USE_ARM_FPU_ALWAYS
	vpop	{d8-d15}				/* 非スクラッチFPUレジスタの復帰 */
#endif /* USE_ARM_FPU_ALWAYS */
#ifdef USE_ARM_FPU_SELECTIVE
	ldr		r2, [r4,#TCB_p_tinib]	/* p_runtsk->p_tinib → r2 */
	ldr		r1, [r2,#TINIB_tskatr]	/* p_runtsk->p_tinib->tskatr → r1 */
	tst		r1, #TA_FPU
	vmrs	r0, fpexc
	biceq	r0, r0, #FPEXC_ENABLE
	orrne	r0, r0, #FPEXC_ENABLE
	vmsr	fpexc, r0				/* FPEXCを設定 */
	vpopne	{d8-d15}				/* 非スクラッチFPUレジスタの復帰 */
#endif /* USE_ARM_FPU_SELECTIVE */
	ldm		sp, {sp,lr}^			/* sp_usr,lr_usrを復帰 */
	add		sp, sp, #8
	pop		{r4-r11}				/* 非スクラッチレジスタの復帰 */
	ldr		r2, [sp]				/* rundomをスタックから復帰 */
	my_pcb	r0, r1
	str		r2, [r0,#PCB_rundom]
#ifdef TOPPERS_SUPPORT_OVRHDR
	bl		ovrtimer_start
	my_pcb	r0, r1
#endif /* TOPPERS_SUPPORT_OVRHDR */
	ldr		r1, [r0,#PCB_twdtimer_enable]	/* twdtimer_enableがtrueの場合 */
	tst		r1, r1
	blne	twdtimer_control		/* タイムウィンドウタイマの切換え */
	pop		{r12,lr}				/* 戻り番地を復帰 */
	bx		lr

/*
 *  ディスパッチャの動作開始
 */
	AGLOBAL(start_dispatch)
ALABEL(start_dispatch)
	/*
	 *  このルーチンは，カーネル起動時に，非タスクコンテキストで，NMIを
	 *  除くすべての割込みを禁止した状態（全割込みロック状態と同等）で
	 *  呼び出される．
	 *
	 *  dispatcher_0へ分岐する前に，タスクコンテキスト・CPUロック状態・
	 *  割込み優先度マスク全解除状態になければならない．
	 */

	/*
	 *  各種のデバイス（特に割込みコントローラ）の設定が完了するのを待つ．
	 */
	asm_data_sync_barrier r0

	/*
	 *	タスクコンテキストに切り換える．
	 */
	my_pcb	r0, r1
	mov		r1, #0
	str		r1, [r0,#PCB_excpt_nest_count]	/* 例外ネストカウントを0に */

	/*
	 *  CPUロック状態にして，ディスパッチャ本体へ分岐する．
	 */
	msr		cpsr_c, #(CPSR_SVC_MODE AOR CPSR_CPULOCK)
	b		dispatcher_0

/*
 *  現在のコンテキストを捨ててディスパッチ
 */
	AGLOBAL(exit_and_dispatch)
ALABEL(exit_and_dispatch)
	/*
	 *  このルーチンは，タスクコンテキスト・CPUロック状態・割込み優先度
	 *  マスク全解除状態・ディスパッチ許可状態で呼び出される．
	 */
#ifdef LOG_DSP_ENTER
	my_pcb	r2, r1
	ldr		r0, [r2,#PCB_p_runtsk]	/* p_runtsk → r0 */
#endif /* LOG_DSP_ENTER */
	/* ディスパッチャ本体（dispatcher）へ */

/*
 *  ディスパッチャ本体
 */
ALABEL(dispatcher)
#ifdef LOG_DSP_ENTER
	/*
	 * 【この時点のレジスタ状態】
	 *  r0：p_runtsk（タスク切換え前）
	 */
	bl		log_dsp_enter
#endif /* LOG_DSP_ENTER */

ALABEL(dispatcher_0)
	/*
	 *  このルーチンは，タスクコンテキスト・CPUロック状態・割込み優先度
	 *  マスク全解除状態・ディスパッチ許可状態で呼び出される．実行再開番
	 *  地へもこの状態のまま分岐する．
	 */
	my_pcb	r0, r1
	ldr		r4, [r0,#PCB_p_schedtsk]	/* p_schedtsk → r4 → p_runtsk */
	str		r4, [r0,#PCB_p_runtsk]
	tst		r4, r4				/* p_runtskがNULLならdispatcher_1へ */
	beq		dispatcher_1
	ldr		sp, [r4,#TCB_sp]	/* タスクスタックを復帰 */

	/*
	 *  MMU/MPUの設定切換え
	 *
	 * 【この時点のレジスタ状態】
	 *  r0：p_my_pcb
	 *  r4：p_runtsk（タスク切換え後）
	 */
	ldr		r1, [r4,#TCB_p_dominib]		/* p_runtsk->p_dominib → r1 */
	ldr		r2, =dominib_kernel			/* カーネルドメインであれば */
	teq		r1, r2						/*			 dispatcher_3へ */
	beq		dispatcher_3
	str		r2, [r0,#PCB_p_ctxdom]		/* p_dominibとp_ctxdomが一致して */
	teq		r2, r1						/*		  いれば，dispatcher_3へ */
	beq		dispatcher_3
	str		r1, [r0,#PCB_p_ctxdom]		/* p_dominib → p_ctxdom */
	ldr		r2, [r4,#TCB_p_tinib]
	ldr		r0, [r2,#TINIB_domid]		/* ドメインID → r0 */
	mcr		p15, 0, r0, c13, c0, 1		/* r0をCONTEXTIDRに書き込む */

	/*
	 *  TTBR0の更新
	 */
	ldr		r3, [r1,#DOMINIB_p_section_table]
										/* セクションテーブルの番地 → r3 */
	orr		r3, r3, #TTBR_CONFIG
	mcr		p15, 0, r3, c2, c0, 0		/* r3をTTBR0に書き込む */

	asm_inst_sync_barrier r0			/* 命令同期バリア */

ALABEL(dispatcher_3)
#ifdef LOG_DSP_LEAVE
	mov		r0, r4				/* p_runtskをパラメータに渡す */
	bl		log_dsp_leave
#endif /* LOG_DSP_LEAVE */
	ldr		r0, [r4,#TCB_pc]	/* 実行再開番地を復帰 */
	bx		r0					/* p_runtskをr4に入れた状態で分岐する */

	/*
	 *  アイドル処理
	 *
	 *  割込みをすべて許可し，CPUロック解除状態にして割込みを待つ．
	 */
ALABEL(dispatcher_1)
	my_pcb	r0, r1					/* アイドル処理用スタックへ切り換え */
	ldr		sp, [r0,#PCB_idstkpt]
	ldr		r1, [r0,#PCB_twdtimer_enable]	/* twdtimer_enableがtrueの場合 */
	tst		r1, r1
	blne	twdtimer_start			/* タイムウィンドウタイマの動作開始 */

	/*
	 *  ターゲットによっては，省電力モード等に移行するため，標準の方法
	 *  と異なる手順が必要な場合がある．そのようなターゲットでは，ター
	 *  ゲット依存部でTOPPERS_CUSTOM_IDLEを定義し，アセンブラマクロとし
	 *  て，toppers_asm_custom_idleを用意すればよい．
	 */
ALABEL(dispatcher_2)
#ifdef TOPPERS_CUSTOM_IDLE
	toppers_asm_custom_idle
#else /* TOPPERS_CUSTOM_IDLE */
	msr		cpsr_c, #CPSR_SVC_MODE	/* 割込みを許可（スーパバイザモード）*/
#endif /* TOPPERS_CUSTOM_IDLE */
	b		dispatcher_2			/* 割込み待ち */

/*
 *  カーネルの終了処理の呼出し
 *   引数1(r0) : p_my_pcb
 *
 *  使用するスタックを非タスクコンテキスト用のスタック領域に切り替え，
 *  例外ネストカウントを1にして，exit_kernelを呼び出す．
 */
	AGLOBAL(call_exit_kernel)
ALABEL(call_exit_kernel)
	ldr		sp, [r0,#PCB_istkpt]	/* 非タスクコンテキスト用のスタックに */
	mov		r1, #1
	str		r1, [r0,#PCB_excpt_nest_count]	/* 例外ネストカウントを1に */
	b		exit_kernel

/*
 *  自タスクのマイグレート
 *   引数1(r0) : p_my_pcb
 *   引数2(r1) : p_selftsk
 */
	ATEXT
	AALIGN(2)
	AGLOBAL(dispatch_and_migrate)
ALABEL(dispatch_and_migrate)
	push	{r12,lr}				/* 戻り番地（lr）を保存 */
									/* r12はスペース確保のため */
#ifdef TOPPERS_SUPPORT_OVRHDR
	push	{r0-r1}
	bl		ovrtimer_stop
	pop		{r0-r1}
#endif /* TOPPERS_SUPPORT_OVRHDR */
	ldr		r3, [r0,#PCB_rundom]	/* rundomをスタックに保存（r12の場所）*/
	str		r3, [sp]
	push	{r4-r11}				/* 非スクラッチレジスタの保存 */
	sub		sp, sp, #8
	stm		sp, {sp,lr}^			/* sp_usr,lr_usrを保存 */
#ifdef USE_ARM_FPU_ALWAYS
	vpush	{d8-d15}				/* 非スクラッチFPUレジスタの保存 */
#endif /* USE_ARM_FPU_ALWAYS */
#ifdef USE_ARM_FPU_SELECTIVE
	vmrs	r1, fpexc
	tst		r1, #FPEXC_ENABLE
	vpushne	{d8-d15}				/* 非スクラッチFPUレジスタの保存 */
#endif /* USE_ARM_FPU_SELECTIVE */
	str		sp, [r1,#TCB_sp]		/* タスクスタックを保存 */
	adr		r3, dispatch_r
	str		r3, [r1,#TCB_pc]		/* 実行再開番地を保存 */
	ldr		sp, [r0,#PCB_istkpt]	/* 非タスクコンテキスト用のスタックに */
	b		migrate_self			/* 自タスクのマイグレート */

/*
 *  現在のコンテキストを捨ててマイグレート
 *   引数1(r0) : p_my_pcb
 *   引数2(r1) : p_selftsk
 */
	ATEXT
	AALIGN(2)
	AGLOBAL(exit_and_migrate)
ALABEL(exit_and_migrate)
	ldr		sp, [r0,#PCB_istkpt]	/* 非タスクコンテキスト用のスタックに */
	b		migrate_activate_self	/* 自タスクのマイグレートと起動 */

/*
 *  システムタスクの実行開始時処理
 */
	ATEXT
	AALIGN(2)
	AGLOBAL(start_stask_r)
	/*
	 * 【この時点のレジスタ状態】
	 *  r4：p_runtsk（タスク切換え後）
	 */
ALABEL(start_stask_r)
	my_pcb	r0, r1
	mov		r2, #0
	str		r2, [r0,#PCB_rundom]	/* TACP_KERNEL → rundom */
#ifdef TOPPERS_SUPPORT_OVRHDR
	bl		ovrtimer_start
	my_pcb	r0, r1
#endif /* TOPPERS_SUPPORT_OVRHDR */
	ldr		r1, [r0,#PCB_twdtimer_enable]	/* twdtimer_enableがtrueの場合 */
	tst		r1, r1
	blne	twdtimer_stop			/* タイムウィンドウタイマの停止 */
	msr		cpsr_c, #(CPSR_SVC_MODE AOR CPSR_UNLOCK)
									/* CPUロック解除状態に */
	ldr		lr, =ext_tsk			/* タスク本体からの戻り番地を設定 */
	ldr		r2, [r4,#TCB_p_tinib]	/* p_runtsk->p_tinib → r2 */
#ifdef USE_ARM_FPU_SELECTIVE
	ldr		r1, [r2,#TINIB_tskatr]	/* p_runtsk->p_tinib->tskatr → r1 */
	tst		r1, #TA_FPU
	vmrs	r0, fpexc
	biceq	r0, r0, #FPEXC_ENABLE
	orrne	r0, r0, #FPEXC_ENABLE
	vmsr	fpexc, r0				/* FPEXCを設定 */
#endif /* USE_ARM_FPU_SELECTIVE */
	ldr		r0, [r2,#TINIB_exinf]	/* exinfをパラメータに */
	ldr		r1, [r2,#TINIB_task]	/* タスク起動番地にジャンプ */
	bx		r1

/*
 *  ユーザタスクの実行開始時処理
 */
	ATEXT
	AALIGN(2)
	AGLOBAL(start_utask_r)
	/*
	 * 【この時点のレジスタ状態】
	 *  r4：p_runtsk（タスク切換え後）
	 */
ALABEL(start_utask_r)
	ldr		r1, [r4,#TCB_p_dominib]		/* p_runtsk->p_dominib → r1 */
	ldr		r2, [r1,#DOMINIB_domptn]	/* p_dominib->domptn → r2 */
	my_pcb	r0, r1
	str		r2, [r0,#PCB_rundom]		/* r2 → rundom */
#ifdef TOPPERS_SUPPORT_OVRHDR
	bl		ovrtimer_start
	my_pcb	r0, r1
#endif /* TOPPERS_SUPPORT_OVRHDR */
	ldr		r1, [r0,#PCB_twdtimer_enable]	/* twdtimer_enableがtrueの場合 */
	tst		r1, r1
	blne	twdtimer_start			/* タイムウィンドウタイマの動作開始 */
	ldr		r2, [r4,#TCB_p_tinib]	/* p_runtsk->p_tinib → r2 */
#ifdef USE_ARM_FPU_SELECTIVE
	ldr		r1, [r2,#TINIB_tskatr]	/* p_runtsk->p_tinib->tskatr → r1 */
	tst		r1, #TA_FPU
	vmrs	r0, fpexc
	biceq	r0, r0, #FPEXC_ENABLE
	orrne	r0, r0, #FPEXC_ENABLE
	vmsr	fpexc, r0				/* FPEXCを設定 */
#endif /* USE_ARM_FPU_SELECTIVE */
	ldr		r0, [r2, #TINIB_ustk]	/* p_tinib->ustk → r0 */
	ldr		r1, [r2, #TINIB_ustksz]	/* p_tinib->ustksz → r1 */
	add		r0, r0, r1				/* ユーザスタックの初期値 → r0 */
	ldr		r1, =call_ext_tsk		/* 戻り番地 → r1 */
	push	{r0,r1}					/* r0→sp_usr，r1→lr_usr */
	ldm		sp, {sp,lr}^
	add		sp, sp, #8
	ldr		r0, [r2,#TINIB_exinf]	/* exinfをパラメータに */
	ldr		r1, [r2, #TINIB_task]	/* タスク起動番地 → r1 */
	mov		r2, #(CPSR_USR_MODE AOR CPSR_UNLOCK)
	push	{r1,r2}					/* タスク起動番地とcpsrをスタックに積む */
	rfefd	sp!

/*
 *  割込みハンドラの出入口処理
 */
#ifndef OMIT_IRQ_HANDLER
	ATEXT
	AALIGN(2)
	AGLOBAL(irq_handler)
ALABEL(irq_handler)
	/*
	 *  ここには，IRQモードで分岐してくる．
	 */
	/*
	 *  戻り番地（lr）と戻り先のcpsr（spsr）をスーパバイザモードのスタッ
	 *  クに保存する．
	 */
	sub		lr, lr, #4				/* 戻り番地の算出 */
	srsfd	#CPSR_SVC_MODE!

	/*
	 *  スーパバイザモードに切り換え，スクラッチレジスタ＋αを保存する．
	 */
	cps		#CPSR_SVC_MODE
	push	{r0-r5,r12,lr}

	/*
	 *  スタックポインタの調整
	 */
	and		r1, sp, #4
	sub		sp, sp, r1
#ifdef USE_ARM_FPU_ALWAYS
#ifdef USE_ARM_FPU_D32
	vpush	{d16-d31}
#endif /* USE_ARM_FPU_D32 */
	vpush	{d0-d7}					/* スクラッチFPUレジスタの保存 */
	vmrs	r0, fpscr
#endif /* USE_ARM_FPU_ALWAYS */
	push	{r0,r1}					/* スタックポインタの調整値を保存 */
									/* r0の場所には，FPSCRまたはFPEXCを保存 */
	/*
	 *  例外ネストカウントをインクリメントする．割込みが非タスクコンテキ
	 *  ストで発生した場合には，irq_handler_1へ分岐する．
	 */
	my_pcb	r0, r1
	ldr		r3, [r0,#PCB_excpt_nest_count]
	add		r3, r3, #1
	str		r3, [r0,#PCB_excpt_nest_count]
	teq		r3, #1					/* 割込みが非タスクコンテキストで発生 */
	bne		irq_handler_1			/*			  ならirq_handler_1に分岐 */

#ifdef USE_ARM_FPU_SELECTIVE
	/*
	 *  FPEXCを保存し，FPUをディスエーブルする．
	 */
	vmrs	r2, fpexc
	str		r2, [sp]				/* FPEXCを保存（r0の場所）*/
	bic		r2, r2, #FPEXC_ENABLE
	vmsr	fpexc, r2				/* FPEXCを設定 */
#endif /* USE_ARM_FPU_SELECTIVE */

	/*
	 *  タイムウィンドウタイマを停止する．
	 */
	ldr		r1, [r0,#PCB_twdtimer_enable]	/* twdtimer_enableがtrueの場合 */
	tst		r1, r1
	blne	twdtimer_stop			/* タイムウィンドウタイマの停止 */

#ifdef TOPPERS_SUPPORT_OVRHDR
	/*
	 *  オーバランタイマを停止する．
	 */
	bl		ovrtimer_stop
#endif /* TOPPERS_SUPPORT_OVRHDR */

	/*
	 *  rundomをスタックに保存し，TACP_KERNELにする．
	 */
	my_pcb	r1, r0
	ldr		r2, [r1,#PCB_rundom]
	push	{r1,r2}					/* p_my_pcbとrundomの値をスタックに保存 */
	mov		r2, #0
	str		r2, [r1,#PCB_rundom]	/* TACP_KERNEL → rundom */

	/*
	 *  非タスクコンテキスト用のスタックに切り換える．
	 */
	mov		r3, sp					/* この時点のスタックポインタをr3に */
	my_pcb	r2, r1
	ldr		sp, [r2,#PCB_istkpt]	/* 非タスクコンテキスト用のスタックに */
	push	{r0,r3}					/* 切換え前のスタックポインタを保存 */
									/* r0はスペース確保のため */
ALABEL(irq_handler_1)
	/*
	 *  割込みコントローラを操作し，割込み番号を取得する．
	 *
	 *  irc_begin_intは，スタック上，またはスタックの先頭（r0の場所）に，
	 *  irc_end_intで用いる情報を保存する場合がある．
	 */
	bl		irc_begin_int
#ifdef USE_BYPASS_IPI_DISPATCH_HANDER
	cmp		r4, #IPINO_DISPATCH
	beq		irq_handler_2			/* ディスパッチ要求なら */
									/*  irq_handler_2に分岐 */
#endif /* USE_BYPASS_IPI_DISPATCH_HANDER */
#if TNUM_INHNO <= 256
	cmp		r4, #TNUM_INHNO			/* TNUM_INHNOの値によってはエラーになる */
#else /* TNUM_INHNO <= 256 */
	movw	r3, #TNUM_INHNO
	cmp		r4, r3
#endif /* TNUM_INHNO <= 256 */
	bhs		irq_handler_2			/* スプリアス割込みなら */
									/*  irq_handler_2に分岐 */
	/*
	 *  CPUロック解除状態にする．
	 */
#ifndef TOPPERS_SAFEG_SECURE
	cpsie	if
#else /* TOPPERS_SAFEG_SECURE */
	cpsie	f
#endif /* TOPPERS_SAFEG_SECURE */

#ifdef LOG_INH_ENTER
	/*
	 *  ログ出力の呼出し
	 */
	mov		r0, r4					/* 割込み番号をパラメータに渡す */
	bl		log_inh_enter
#endif /* LOG_INH_ENTER */

	/*
	 *  割込みハンドラの呼出し
	 */
	my_pcb	r1, r2
	ldr		r2,[r1,#PCB_p_inh_tbl]	/* 割込みハンドラテーブルの読込み */
	ldr		r1, [r2,r4,lsl #2]		/* 割込みハンドラの番地 → r1 */
	mov		lr, pc					/* 割込みハンドラの呼出し */
	bx		r1

#ifdef LOG_INH_LEAVE
	/*
	 *  ログ出力の呼出し
	 */
	mov		r0, r4					/* 割込み番号をパラメータに渡す */
	bl		log_inh_leave
#endif /* LOG_INH_LEAVE */

	/*
	 *  スピンロックを取得していれば解放する．
	 */
	my_pcb	r0, r1
	bl		force_unlock_spin

	/*
	 *  カーネル管理の割込みを禁止する．
	 */
#ifndef TOPPERS_SAFEG_SECURE
	cpsid	i
#else /* TOPPERS_SAFEG_SECURE */
	cpsid	if
#endif /* TOPPERS_SAFEG_SECURE */

	/*
	 *  割込みコントローラを操作する．
	 */
ALABEL(irq_handler_2)
	bl		irc_end_int

	/*
	 *  例外ネストカウントをデクリメントする．
	 */
	my_pcb	r2, r1
	ldr		r3, [r2,#PCB_excpt_nest_count]
	subs	r3, r3, #1
	str		r3, [r2,#PCB_excpt_nest_count]	/* 戻り先が非タスクコンテキスト */
	bne		irq_handler_5					/*		ならirq_handler_5に分岐 */

	/*
	 *  タスク用のスタックに戻す．
	 */
	pop		{r0,r3}
	mov   	sp, r3

	/*
	 *  p_runtskがNULLか判定する．
	 */
	ldr		r0, [r2,#PCB_p_runtsk]	/* p_runtsk → r0 */
	tst		r0, r0					/* p_runtskがNULLでなければ */
	bne		irq_handler_3			/*		irq_handler_3に分岐 */

	/*
	 *  タスクのスタックに保存したスクラッチレジスタ等を捨てる．
	 */
	add		sp, sp, #8				/* p_my_pcbとrundomの値を捨てる */
	pop		{r0,r1}					/* スタックポインタの調整を元に戻す */
	add		sp, sp, r1
#ifdef USE_ARM_FPU_ALWAYS
#ifdef USE_ARM_FPU_D32
 	add		sp, sp, #232			/* スクラッチレジスタ等を捨てる */
#else /* USE_ARM_FPU_D32 */
 	add		sp, sp, #104			/* スクラッチレジスタ等を捨てる */
#endif /* USE_ARM_FPU_D32 */
#else /* USE_ARM_FPU_ALWAYS */
	add		sp, sp, #40				/* スクラッチレジスタ等を捨てる */
#endif /* USE_ARM_FPU_ALWAYS */
	b		dispatcher_0

ALABEL(irq_handler_3)
	/*
	 * 【この時点のレジスタ状態】
	 *  r0：p_runtsk
	 *  r2：p_my_pcb
	 */
#ifdef USE_ARM_FPU_SELECTIVE
	/*
	 *  FPUを元に戻す．
	 */
	ldr		r3, [sp,#8]				/* FPEXCを復帰 */
	vmsr	fpexc, r3
#endif /* USE_ARM_FPU_SELECTIVE */

	/*
	 *  ディスパッチが必要か判定する．
	 */
	ldr		r1, [r2,#PCB_p_schedtsk]	/* p_schedtsk → r1 */
	teq		r0, r1						/* p_runtskとp_schedtskが同じなら */
	beq		irq_handler_4				/*				  irq_handler_4へ */

	/*
	 *  コンテキストを保存する．
	 *
	 * 【この時点のレジスタ状態】
	 *  r0：p_runtsk
	 *  r3：FPEXC（USE_ARM_FPU_SELECTIVEの場合）
	 */
	push	{r6-r11}				/* 残りのレジスタの保存 */
	sub		sp, sp, #8
	stm		sp, {sp,lr}^			/* sp_usr,lr_usrを保存 */
#ifdef USE_ARM_FPU_ALWAYS
	vpush	{d8-d15}				/* 非スクラッチFPUレジスタの保存 */
#endif /* USE_ARM_FPU_ALWAYS */
#ifdef USE_ARM_FPU_SELECTIVE
	tst		r3, #FPEXC_ENABLE
	beq		1f
#ifdef USE_ARM_FPU_D32
	vpush	{d16-d31}
#endif /* USE_ARM_FPU_D32 */
	vpush	{d0-d15}				/* 全FPUレジスタの保存 */
	vmrs	r2, fpscr
	push	{r1,r2}					/* FPSCRを保存 */
1:									/* r1はアラインメントのため */
#endif /* USE_ARM_FPU_SELECTIVE */
	str		sp, [r0,#TCB_sp]		/* スタックポインタを保存 */
	adr		r1, ret_int_r			/* 実行再開番地を保存 */
	str		r1, [r0,#TCB_pc]
	b		dispatcher				/* r0にはp_runtskが格納されている */

ALABEL(ret_int_r)
	/*
	 *  コンテキストを復帰する．
	 *
	 * 【この時点のレジスタ状態】
	 *  r4：p_runtsk（タスク切換え後）
	 */
#ifdef USE_ARM_FPU_ALWAYS
	vpop	{d8-d15}				/* 非スクラッチ全FPUレジスタの復帰 */
#endif /* USE_ARM_FPU_ALWAYS */
#ifdef USE_ARM_FPU_SELECTIVE
	ldr		r2, [r4,#TCB_p_tinib]	/* p_runtsk->p_tinib → r2 */
	ldr		r1, [r2,#TINIB_tskatr]	/* p_runtsk->p_tinib->tskatr → r1 */
	tst		r1, #TA_FPU
	vmrs	r0, fpexc
	biceq	r0, r0, #FPEXC_ENABLE
	orrne	r0, r0, #FPEXC_ENABLE
	vmsr	fpexc, r0				/* FPEXCを設定 */
	beq		1f						/* TA_FPU属性でない場合は分岐 */
	pop		{r1,r2}
	vmsr	fpscr, r2				/* FPSCRを復帰 */
	vpop	{d0-d15}				/* 全FPUレジスタの復帰 */
#ifdef USE_ARM_FPU_D32
	vpop	{d16-d31}
#endif /* USE_ARM_FPU_D32 */
1:
#endif /* USE_ARM_FPU_SELECTIVE */
	ldm		sp, {sp,lr}^			/* sp_usr,lr_usrを復帰 */
	add		sp, sp, #8
	pop		{r6-r11}				/* 残りのレジスタの復帰 */

ALABEL(irq_handler_4)
	/*
	 *  rundomをスタックから復帰する．
	 */
	pop		{r1,r2}					/* p_my_pcbとrundomの値をスタックから復帰 */
	str		r2, [r1,#PCB_rundom]

#ifdef TOPPERS_SUPPORT_OVRHDR
	/*
	 *  オーバランタイマを動作開始する．
	 */
	bl		ovrtimer_start
#endif /* TOPPERS_SUPPORT_OVRHDR */

	/*
	 *  タイムウィンドウタイマを動作開始する．
	 */
	my_pcb	r0, r1
	ldr		r1, [r0,#PCB_twdtimer_enable]	/* twdtimer_enableがtrueの場合 */
	tst		r1, r1
	blne	twdtimer_control		/* タイムウィンドウタイマの切換え */

	/*
	 *  割込み処理からのリターン
	 *
	 *  割込み処理からのリターンにより，CPUロック解除状態に遷移するよ
	 *  うにする必要があるが，ARMはCPSRのビットによってCPUロック状態を
	 *  表しているため，CPSRを元に戻してリターンすればよい．
	 */
ALABEL(irq_handler_5)
	pop		{r0,r1}
#ifdef USE_ARM_FPU_ALWAYS
	vmsr	fpscr, r0				/* FPSCRを復帰 */
	vpop	{d0-d7}					/* スクラッチFPUレジスタの復帰 */
#ifdef USE_ARM_FPU_D32
	vpop	{d16-d31}
#endif /* USE_ARM_FPU_D32 */
#endif /* USE_ARM_FPU_ALWAYS */
	add		sp, sp, r1				/* スタックポインタの調整を元に戻す */
	pop		{r0-r5,r12,lr}			/* スクラッチレジスタ＋αの復帰 */
	rfefd	sp!
#endif /* OMIT_IRQ_HANDLER */

/*
 *  CPU例外ハンドラ出入口処理
 */
ALABEL(start_exc_entry)

/*
 *  未定義命令
 */
#ifndef OMIT_UNDEF_HANDLER
	ATEXT
	AALIGN(2)
	AGLOBAL(undef_handler)
ALABEL(undef_handler)
	/*
	 *  ここには，未定義モードで分岐してくる．
	 */
	/*
	 *  戻り番地（lr）と戻り先のcpsr（spsr）をスーパバイザモードのスタッ
	 *  クに保存する．
	 */
	srsfd	#CPSR_SVC_MODE!

	/*
	 *  スーパバイザモードに切り換え，スクラッチレジスタ＋αを保存する．
	 */
	cps		#CPSR_SVC_MODE
	push	{r0-r5,r12,lr}
	mov		r4, #EXCNO_UNDEF
	b		exc_handler_1
#endif /* OMIT_UNDEF_HANDLER */

/*
 *  スーパバイザコール
 */
#ifndef OMIT_SVC_HANDLER
	ATEXT
	AALIGN(2)
	AGLOBAL(svc_handler)
ALABEL(svc_handler)
	/*
	 *  ここには，スーパバイザモードで分岐してくる．
	 */

#ifdef USE_ARM_SVCNO
	/*
	 *  svc命令のオペランドが0でなければsvc_handler_1に分岐する．
	 */
	push	{r0}
	mrs		r0, spsr				/* 呼出し元のThumbモードビットをチェック */
	tst		r0, #CPSR_THUMB_BIT
	ldrneh	r0, [lr,#-2]			/* Thumbモードから呼ばれた場合 */
	bicne	r0, r0, #0xff00
	ldreq	r0, [lr,#-4]			/* ARMモードから呼ばれた場合 */
	biceq	r0, r0, #0xff000000
	teq		r0, #0					/* サービスコール呼出しか？ */
	pop		{r0}
	bne		svc_handler_1			/* サービスコール呼出しでない場合は分岐 */
#endif /* USE_ARM_SVCNO */

	/*
	 *  サービスコール呼出しの場合には，svc_entryに分岐する．
	 */
	cmp		r5, #TFN_MAX_RESERVE	/* 機能番号をチェック */
	bgt		svc_entry

	/*
	 *  システム周期オーバラン例外の場合には，scycovr_handlerに分岐する．
	 *  そうでない場合は，svc_entryに分岐する（E_RSFNエラーになる）．
	 */
	push	{r0}
	mrs		r0, spsr				/* ユーザモードからの呼び出しか？ */
	and		r0, r0, #CPSR_MODE_MASK
	teq		r0, #CPSR_USR_MODE
	pop		{r0}
	beq		svc_entry
	teq		r5, #TFN_SCYCOVR		/* システム周期オーバラン例外か？ */
	beq		scycovr_handler
	b		svc_entry

#ifdef USE_ARM_SVCNO
ALABEL(svc_handler_1)
	/*
	 *  戻り番地（lr）と戻り先のcpsr（spsr）をスーパバイザモードのスタッ
	 *  クに保存する．
	 */
	srsfd	#CPSR_SVC_MODE!

	/*
	 *  スーパバイザモードで，スクラッチレジスタ＋αを保存する．
	 */
	push	{r0-r5,r12,lr}
	mov		r4, #EXCNO_SVC
	b		exc_handler_1
#endif /* USE_ARM_SVCNO */
#endif /* OMIT_SVC_HANDLER */

/*
 *  プリフェッチアボート
 */
#ifndef OMIT_PABORT_HANDLER
	ATEXT
	AALIGN(2)
	AGLOBAL(pabort_handler)
ALABEL(pabort_handler)
	/*
	 *  ここには，アボートモードで分岐してくる．
	 */
	/*
	 *  戻り番地（lr）と戻り先のcpsr（spsr）をスーパバイザモードのスタッ
	 *  クに保存する．
	 */
	srsfd	#CPSR_SVC_MODE!

	/*
	 *  スーパバイザモードに切り換え，スクラッチレジスタ＋αを保存する．
	 */
	cps		#CPSR_SVC_MODE
	push	{r0-r5,r12,lr}
	mov		r4, #EXCNO_PABORT
	b		exc_handler_1
#endif /* OMIT_PABORT_HANDLER */

/*
 *  データアボート
 */
#ifndef OMIT_DABORT_HANDLER
	ATEXT
	AALIGN(2)
	AGLOBAL(dabort_handler)
ALABEL(dabort_handler)
	/*
	 *  ここには，アボートモードで分岐してくる．
	 *
	 *  データアボートが，CPU例外の入口（start_exc_entryとend_exc_entry
	 *  の間）で発生した場合には，fatal_dabort_handlerに分岐する．アボー
	 *  トモードのspを汎用レジスタの代わりに使用する（r13と記述している）．
	 */
	adr		r13, start_exc_entry+8
	cmp		lr, r13
	bcc		dabort_handler_1
	adr		r13, end_exc_entry+8
	cmp		lr, r13
	bcc		fatal_dabort_handler

ALABEL(dabort_handler_1)
	/*
	 *  戻り番地（lr）と戻り先のcpsr（spsr）をスーパバイザモードのスタッ
	 *  クに保存する．
	 */
	srsfd	#CPSR_SVC_MODE!

	/*
	 *  スーパバイザモードに切り換え，スクラッチレジスタ＋αを保存する．
	 */
	cps		#CPSR_SVC_MODE
	push	{r0-r5,r12,lr}
	mov		r4, #EXCNO_DABORT
	b		exc_handler_1

/*
 *  CPU例外の入口で発生したデータアボート
 */
ALABEL(fatal_dabort_handler)
	/*
	 *  IビットとFビットをセットし，スーパバイザモードに切り換え，スタッ
	 *  クポインタを初期化する．
	 */
	cpsid	if, #CPSR_SVC_MODE
	my_pcb	r0, r1
	ldr		sp, [r0,#PCB_istkpt]	/* 非タスクコンテキスト用のスタックに */

	/*
	 *  アボートモードに戻して，戻り番地（lr）と戻り先のcpsr（spsr）を
	 *  スーパバイザモードのスタックに保存する．
	 */
	cps		#CPSR_ABT_MODE /* アボートモードに戻す */
	srsfd	#CPSR_SVC_MODE! /* レジスタをスーパバイザモードスタックに保存 */
	/* srsfd sp!, #CPSR_SVC_MODE */

	/*
	 *  スーパバイザモードに切り換え，スクラッチレジスタ＋αを保存する．
	 */
	cps		#CPSR_SVC_MODE
	push	{r0-r5,r12,lr}

	/*
	 *  例外ネストカウントをインクリメントする．
	 */
	my_pcb	r2, r1
	ldr		r3, [r2,#PCB_excpt_nest_count]
	add		r3, r3, #1
	str		r3, [r2,#PCB_excpt_nest_count]

	mov		r4, #EXCNO_FATAL
	b		exc_handler_1
#endif /* OMIT_DABORT_HANDLER */

/*
 *  FIQ
 */
#ifndef OMIT_FIQ_HANDLER
	ATEXT
	AALIGN(2)
	AGLOBAL(fiq_handler)
ALABEL(fiq_handler)
	/*
	 *  ここには，FIQモードで分岐してくる．
	 */
	/*
	 *  戻り番地（lr）と戻り先のcpsr（spsr）をスーパバイザモードのスタッ
	 *  クに保存する．
	 */
	srsfd	#CPSR_SVC_MODE!

	/*
	 *  スーパバイザモードに切り換え，スクラッチレジスタ＋αを保存する．
	 */
	cps		#CPSR_SVC_MODE
	push	{r0-r5,r12,lr}
	mov		r4, #EXCNO_FIQ
	b		exc_handler_1
#endif /* OMIT_FIQ_HANDLER */

/*
 *  システム周期オーバラン例外
 */
#ifndef OMIT_SCYCOVR_HANDLER
	ATEXT
	AALIGN(2)
	AGLOBAL(scycovr_handler)
ALABEL(scycovr_handler)
	/*
	 *  戻り先（lr）とspsr（cpsr_svc）をスーパバイザモードのスタックに
	 *  保存する．
	 */
	srsfd	#CPSR_SVC_MODE!

	/*
	 *  スーパバイザモードで，スクラッチレジスタ＋αを保存する．
	 */
	cps		#CPSR_SVC_MODE			/* 不要と思われる */
	push	{r0-r5,r12,lr}
	mov		r4, #EXCNO_SCYCOVR
	b		exc_handler_1
#endif /* OMIT_SCYCOVR_HANDLER */

ALABEL(end_exc_entry)

/*
 *  CPU例外ハンドラ出入口処理の共通部分
 */
ALABEL(exc_handler_1)
	/*
	 * 【この時点のレジスタ状態】
	 *  r4：CPU例外ハンドラ番号
	 *
	 *  CPU例外が発生した状況の判断に用いるために，CPU例外発生前に実行
	 *  中の保護ドメイン，割込み優先度マスク，例外ネストカウントをスタッ
	 *  に保存する．
	 */
	my_pcb	r1, r2
	ldr		r2, [r1,#PCB_rundom]
	push	{r2}					/* rundomをスタックに保存 */
	bl		irc_get_intpri
	push	{r0}					/* 割込み優先度マスクを保存 */
	my_pcb	r0, r1
	ldr		r3, [r0,#PCB_excpt_nest_count]
	push	{r3}					/* 例外ネストカウントを保存 */
	mov		r5, sp					/* CPU例外の情報を記憶している領域の */
									/*				  先頭番地をr5に保存 */
	/*
	 *  スタックポインタの調整
	 */
	and		r1, sp, #4
	sub		sp, sp, r1
#ifdef USE_ARM_FPU_ALWAYS
#ifdef USE_ARM_FPU_D32
	vpush	{d16-d31}
#endif /* USE_ARM_FPU_D32 */
	vpush	{d0-d7}					/* スクラッチFPUレジスタの保存 */
	vmrs	r0, fpscr
#endif /* USE_ARM_FPU_ALWAYS */
	push	{r0,r1}					/* スタックポインタの調整値とFPSCRを保存 */

	/*
	 *  カーネル管理外のCPU例外か判定する
	 *
	 *  カーネル管理外のCPU例外は，カーネル実行中，全割込みロック状態，
	 *  CPUロック状態，カーネル管理外の割込みハンドラ実行中に発生した
	 *  CPU例外である．ARMコアの場合は，戻り先のCPSRのIビットかFビット
	 *  のいずれかがセットされているなら，これに該当する．
	 */
	ldr		r1, [r5,#T_EXCINF_cpsr]		/* 例外フレームからcpsrを取得 */
	ands	r1, r1, #CPSR_FIQ_IRQ_BIT
	bne		nk_exc_handler_1		/* カーネル管理外のCPU例外の処理へ */

	/*
	 * 【この時点のレジスタ状態】
	 *  r0：p_my_pcb
	 *  r3：excpt_nest_countの値
	 *  r4：CPU例外ハンドラ番号
	 *  r5：CPU例外の情報を記憶している領域の先頭番地
	 */

	/*
	 *  例外ネストカウントをインクリメントする．
	 */
	add		r3, r3, #1
	str		r3, [r0,#PCB_excpt_nest_count]
	teq		r3, #1					/* CPU例外発生前が非タスクコンテキスト */
	bne		exc_handler_2			/*			   ならexc_handler_2に分岐 */

#ifdef USE_ARM_FPU_SELECTIVE
	/*
	 *  FPEXCを保存し，FPUをディスエーブルする．
	 */
	vmrs	r2, fpexc
	str		r2, [sp]				/* FPEXCを保存（r0の場所）*/
	bic		r2, r2, #FPEXC_ENABLE
	vmsr	fpexc, r2				/* FPEXCを設定 */
#endif /* USE_ARM_FPU_SELECTIVE */

	/*
	 *  タイムウィンドウタイマを停止する．
	 */
	ldr		r1, [r0,#PCB_twdtimer_enable]	/* twdtimer_enableがtrueの場合 */
	tst		r1, r1
	blne 	twdtimer_stop			/* タイムウィンドウタイマの停止 */

#ifdef TOPPERS_SUPPORT_OVRHDR
	/*
	 *  オーバランタイマを停止する．
	 */
	bl		ovrtimer_stop
#endif /* TOPPERS_SUPPORT_OVRHDR */

	/*
	 *  rundomをTACP_KERNELにする．
	 */
	my_pcb	r1, r0
	mov		r2, #0
	str		r2, [r1,#PCB_rundom]	/* TACP_KERNEL → rundom */

	/*
	 *  非タスクコンテキスト用のスタックに切り換える．
	 */
	mov		r3, sp					/* この時点のスタックポインタをr3に */
	my_pcb	r2, r1
	ldr		sp, [r2,#PCB_istkpt]	/* 非タスクコンテキスト用のスタックに */
	push	{r0,r3}					/* 切換え前のスタックポインタを保存 */
									/* r0はスペース確保のため */
ALABEL(exc_handler_2)
	/*
	 * 【この時点のレジスタ状態】
	 *  r4：CPU例外ハンドラ番号
	 *  r5：CPU例外の情報を記憶している領域の先頭番地
	 */

	/*
	 *  （必要なら）割込みコントローラを操作する．
	 *
	 *  irc_begin_excは，スタック上，またはスタックの先頭（r0の場所）に，
	 *  irc_end_excで用いる情報を保存する場合がある．
	 */
	bl		irc_begin_exc

	/*
	 *  CPUロック解除状態にする．
	 *
	 *  カーネル管理外のCPU例外ハンドラは別ルーチンで呼び出すため，単純
	 *  に割込みを許可するだけでよい．
	 */
#ifndef TOPPERS_SAFEG_SECURE
	cpsie	if
#else /* TOPPERS_SAFEG_SECURE */
	cpsie	f
#endif /* TOPPERS_SAFEG_SECURE */

	/*
	 *  ログ出力の呼出し
	 */
#ifdef LOG_EXC_ENTER
	mov		r0, r4					/* CPU例外番号をパラメータに渡す */
	bl		log_exc_enter
#endif /* LOG_EXC_ENTER */

	/*
	 *  CPU例外ハンドラの呼出し
	 */
	my_pcb	r1, r2
	ldr		r2, [r1,#PCB_p_exc_tbl]	/* CPU例外ハンドラテーブルの読込み */
	ldr		r3, [r2,r4,lsl #2]		/* CPU例外ハンドラの番地 → r3 */
	mov		r0, r5					/* CPU例外の情報を記憶している領域の */
									/*		先頭番地を第1パラメータに渡す */
	mov		r1, r4					/* CPU例外番号を第2パラメータに渡す */
	mov		lr, pc					/* CPU例外ハンドラの呼出し */
	bx		r3

	/*
	 *  ログ出力の呼出し
	 */
#ifdef LOG_EXC_LEAVE
	mov		r0, r4					/* CPU例外番号をパラメータに渡す */
	bl		log_exc_leave
#endif /* LOG_EXC_LEAVE */

	/*
	 *  スピンロックを取得していれば解放する．
	 */
	my_pcb	r0, r1
	bl		force_unlock_spin

	/*
	 *  カーネル管理の割込みを禁止する．
	 */
#ifndef TOPPERS_SAFEG_SECURE
	cpsid	i
#else /* TOPPERS_SAFEG_SECURE */
	cpsid	if
#endif /* TOPPERS_SAFEG_SECURE */

	/*
	 *  割込みコントローラを操作して，割込み優先度マスクを，CPU例外発
	 *  生時の値に設定する．
	 */
	bl		irc_end_exc

	/*
	 *  例外ネストカウントをデクリメントする．
	 */
	my_pcb	r2, r1
	ldr		r3, [r2,#PCB_excpt_nest_count]
	subs	r3, r3, #1
	str		r3, [r2,#PCB_excpt_nest_count]	/* 戻り先が非タスクコンテキスト */
	bne		exc_handler_5					/*		ならexc_handler_5に分岐 */

	/*
	 *  タスク用のスタックに戻す．
	 */
	pop		{r0,r3}
	mov		sp, r3

	/*
	 *  p_runtskがNULLか判定する．
	 */
	ldr		r0, [r2,#PCB_p_runtsk]	/* p_runtsk → r0 */
	tst		r0, r0					/* p_runtskがNULLでなければ */
	bne		exc_handler_3			/*		exc_handler_3に分岐 */

	/*
	 *  タスクのスタックに保存したスクラッチレジスタ等を捨てる．
	 */
	pop		{r0,r1}					/* スタックポインタの調整を元に戻す */
	add		sp, sp, r1
#ifdef USE_ARM_FPU_ALWAYS
#ifdef USE_ARM_FPU_D32
	add		sp, sp, #244			/* スクラッチレジスタとCPU例外が発生した */
#else /* USE_ARM_FPU_D32 */			/*	状況を判断するための追加情報を捨てる */
 	add		sp, sp, #116			/* スクラッチレジスタとCPU例外が発生した */
#endif /* USE_ARM_FPU_D32 */		/*	状況を判断するための追加情報を捨てる */
#else /* USE_ARM_FPU_ALWAYS */
	add		sp, sp, #52				/* スクラッチレジスタとCPU例外が発生した */
#endif /* USE_ARM_FPU_ALWAYS */		/*	状況を判断するための追加情報を捨てる */
	b		dispatcher_0

ALABEL(exc_handler_3)
	/*
	 * 【この時点のレジスタ状態】
	 *  r0：p_runtsk
	 *  r2：p_my_pcb
	 */
#ifdef USE_ARM_FPU_SELECTIVE
	/*
	 *  FPUを元に戻す．
	 */
	ldr		r3, [sp]				/* FPEXCを復帰 */
	vmsr	fpexc, r3
#endif /* USE_ARM_FPU_SELECTIVE */

	/*
	 *  ディスパッチが必要か判定する．
	 */
	ldr		r1, [r2,#PCB_p_schedtsk]	/* p_schedtsk → r1 */
	teq		r0, r1						/* p_runtskとp_schedtskが同じなら */
	beq		exc_handler_4				/*				  exc_handler_4へ */

	/*
	 *  コンテキストを保存する．
	 *
	 * 【この時点のレジスタ状態】
	 *  r3：FPEXC（USE_ARM_FPU_SELECTIVEの場合）
	 */
	push	{r6-r11}				/* 残りのレジスタの保存 */
	sub		sp, sp, #8
	stm		sp, {sp,lr}^			/* sp_usr,lr_usrを保存 */
#ifdef USE_ARM_FPU_ALWAYS
	vpush	{d8-d15}				/* 非スクラッチFPUレジスタの保存 */
#endif /* USE_ARM_FPU_ALWAYS */
#ifdef USE_ARM_FPU_SELECTIVE
	tst		r3, #FPEXC_ENABLE
	beq		1f
#ifdef USE_ARM_FPU_D32
	vpush	{d16-d31}
#endif /* USE_ARM_FPU_D32 */
	vpush	{d0-d15}				/* 全FPUレジスタの保存 */
	vmrs	r2, fpscr
	push	{r1,r2}					/* FPSCRを保存 */
1:									/* r1はアラインメントのため */
#endif /* USE_ARM_FPU_SELECTIVE */
	str		sp, [r0,#TCB_sp]		/* スタックポインタを保存 */
	adr		r1, ret_exc_r			/* 実行再開番地を保存 */
	str		r1, [r0,#TCB_pc]
	b		dispatcher				/* r0にはp_runtskが格納されている */

ALABEL(ret_exc_r)
	/*
	 *  コンテキストを復帰する．
	 *
	 * 【この時点のレジスタ状態】
	 *  r4：p_runtsk（タスク切換え後）
	 */
#ifdef USE_ARM_FPU_ALWAYS
	vpop	{d8-d15}				/* 非スクラッチ全FPUレジスタの復帰 */
#endif /* USE_ARM_FPU_ALWAYS */
#ifdef USE_ARM_FPU_SELECTIVE
	ldr		r2, [r4,#TCB_p_tinib]	/* p_runtsk->p_tinib → r2 */
	ldr		r1, [r2,#TINIB_tskatr]	/* p_runtsk->p_tinib->tskatr → r1 */
	tst		r1, #TA_FPU
	vmrs	r0, fpexc
	biceq	r0, r0, #FPEXC_ENABLE
	orrne	r0, r0, #FPEXC_ENABLE
	vmsr	fpexc, r0				/* FPEXCを設定 */
	beq		1f						/* TA_FPU属性でない場合は分岐 */
	pop		{r1,r2}
	vmsr	fpscr, r2				/* FPSCRを復帰 */
	vpop	{d0-d15}				/* 全FPUレジスタの復帰 */
#ifdef USE_ARM_FPU_D32
	vpop	{d16-d31}
#endif /* USE_ARM_FPU_D32 */
1:
#endif /* USE_ARM_FPU_SELECTIVE */
	ldm		sp, {sp,lr}^			/* sp_usr,lr_usrを復帰 */
	add		sp, sp, #8
	pop		{r6-r11}				/* 残りのレジスタの復帰 */

ALABEL(exc_handler_4)
	/*
	 *  rundomを復帰する．
	 */
	ldr		r2, [r5,#T_EXCINF_rundom]	/* rundomを例外フレームから復帰 */
	my_pcb	r1, r0
	str		r2, [r1,#PCB_rundom]

#ifdef TOPPERS_SUPPORT_OVRHDR
	/*
	 *  オーバランタイマを動作開始する．
	 */
	bl		ovrtimer_start
#endif /* TOPPERS_SUPPORT_OVRHDR */

	/*
	 *  タイムウィンドウタイマを動作開始する．
	 */
	my_pcb	r0, r1
	ldr		r1, [r0,#PCB_twdtimer_enable]	/* twdtimer_enableがtrueの場合 */
	tst		r1, r1
	blne	twdtimer_control		/* タイムウィンドウタイマの切換え */

	/*
	 *  CPU例外処理からのリターン
	 *
	 *  CPU例外処理からのリターンにより，CPUロック解除状態に遷移するよ
	 *  うにする必要があるが，ARMはCPSRのビットによってCPUロック状態を
	 *  表しているため，CPSRを元に戻してリターンすればよい．
	 */
ALABEL(exc_handler_5)
	pop		{r0,r1}
#ifdef USE_ARM_FPU_ALWAYS
	vmsr	fpscr, r0				/* FPSCRを復帰 */
	vpop	{d0-d7}					/* スクラッチFPUレジスタの復帰 */
#ifdef USE_ARM_FPU_D32
	vpop	{d16-d31}
#endif /* USE_ARM_FPU_D32 */
#endif /* USE_ARM_FPU_ALWAYS */
	add		sp, sp, r1				/* スタックポインタの調整を元に戻す */
	add		sp, sp, #12				/* スタック上の情報を捨てる */
	pop		{r0-r5,r12,lr}			/* スクラッチレジスタ＋αの復帰 */
	rfefd	sp!

/*
 *  カーネル管理外のCPU例外の出入口処理
 */
ALABEL(nk_exc_handler_1)
	/*
	 * 【この時点のレジスタ状態】
	 *  r0：p_my_pcb
	 *  r1：CPU例外発生前のCPSRのFビットとIビットの値
	 *  r3：excpt_nest_countの値
	 *  r4：CPU例外ハンドラ番号
	 *  r5：CPU例外の情報を記憶している領域の先頭番地
	 */

	/*
	 *  例外ネストカウントをインクリメントする．
	 */
	add		r3, r3, #1
	str		r3, [r0,#PCB_excpt_nest_count]
	teq		r3, #1					/* CPU例外発生前が非タスクコンテキスト */
	bne		nk_exc_handler_2		/*		 	ならnk_exc_handler_2に分岐 */

#ifdef USE_ARM_FPU_SELECTIVE
	/*
	 *  FPEXCを保存し，FPUをディスエーブルする．
	 */
	vmrs	r2, fpexc
	str		r2, [sp]				/* FPEXCを保存（r0の場所）*/
	bic		r2, r2, #FPEXC_ENABLE
	vmsr	fpexc, r2				/* FPEXCを設定 */
#endif /* USE_ARM_FPU_SELECTIVE */

	/*
	 *  rundomをTACP_KERNELにする．
	 */
	my_pcb	r3, r0
	mov		r2, #0
	str		r2, [r3,#PCB_rundom]	/* TACP_KERNEL → rundom */

	/*
	 *  非タスクコンテキスト用のスタックに切り換える．
	 */
	mov		r3, sp					/* この時点のスタックポインタをr3に */
	my_pcb	r2, r1
	ldr		sp, [r2,#PCB_istkpt]	/* 非タスクコンテキスト用のスタックに */
	push	{r0,r3}					/* 切換え前のスタックポインタを保存 */
									/* r0はアラインメントのため */
ALABEL(nk_exc_handler_2)
	/*
	 *  システム状態（コンテキストは除く）を，CPU例外発生時の状態へ
	 */
	orr		r1, r1, #CPSR_SVC_MODE
	msr		cpsr_c, r1

	/*
	 *  CPU例外ハンドラの呼出し
	 */
	my_pcb	r1, r2
	ldr		r2, [r1,#PCB_p_exc_tbl]	/* CPU例外ハンドラテーブルの読込み */
	ldr		r3, [r2,r4,lsl #2]		/* CPU例外ハンドラの番地 → r3 */
	mov		r0, r5					/* CPU例外の情報を記憶している領域の */
									/*		先頭番地を第1パラメータに渡す */
	mov		r1, r4					/* CPU例外番号を第2パラメータに渡す */
	mov		lr, pc					/* CPU例外ハンドラの呼出し */
	bx		r3

	/*
	 *  例外ネストカウントをデクリメントする．
	 */
	my_pcb	r2, r1
	ldr		r3, [r2,#PCB_excpt_nest_count]
	subs	r3, r3, #1
	str		r3, [r2,#PCB_excpt_nest_count]	/* 戻り先が非タスクコンテキスト */
	bne		exc_handler_5					/*		ならexc_handler_5に分岐 */

	/*
	 *  タスク用のスタックに戻す．
	 */
	pop		{r0,r3}
	mov		sp, r3

#ifdef USE_ARM_FPU_SELECTIVE
	/*
	 *  FPUを元に戻す．
	 */
	ldr		r0, [sp]				/* FPEXCを復帰 */
	vmsr	fpexc, r0
#endif /* USE_ARM_FPU_SELECTIVE */
	b		exc_handler_5

/*
 *  サービスコールの出入口処理
 *
 * 【この時点のレジスタ状態】
 *  r0〜r4：サービスコールのパラメータ（最大5つ）
 *  r5：機能コード
 *  lr：戻り番地
 *  r12は壊して良い．r4〜r11は保存しなければならない．
 */
ALABEL(svc_entry)
	/*
	 *  戻り先（lr）とspsr（cpsr_svc）をスーパバイザモードのスタックに
	 *  保存する．
	 */
	srsfd	#CPSR_SVC_MODE!
#define STACK_SVC_cpsr		4		/* スタック上でのcpsrの保存場所 */

	/* 
	 *  機能コードが0より大きければ，拡張サービスコールの出入口処理へ
	 */
	cmp		r5, #0
	bgt		extsvc_entry

	/* 
	 *  機能コードの下限値チェック
	 */
	ldr		r12, =TMIN_FNCD
	cmp		r5, r12
	blt		ret_svc_rsfn

	/*
	 *  ソフトウェア割込み前のシステム状態に戻す
	 */
	ldr		r12, [sp,#STACK_SVC_cpsr]
	and		r12, r12, #CPSR_INT_MASK
	orr		r12, r12, #CPSR_SVC_MODE
	msr		cpsr_c, r12

	/*
	 *  サービスコールを呼び出す
	 */
	push	{r4,lr}					/* 第5パラメータをスタックに積む */
									/* lrはアラインメントのため */
	ldr		r12, =svc_table
	ldr		r12, [r12,-r5,lsl #2]	/* サービスコールの番地を取り出す */
	blx		r12						/* サービスコールルーチンへ分岐 */

	/*
	 *  サービスコールからのリターン処理
	 */
ALABEL(ret_svc)
	add		sp, sp, #8				/* 第5パラメータを捨てる */
									/* r4は壊されていないので，復帰不要 */
	/*
	 *  ソフトウェア割込みからのリターン後のシステム状態が，この時点での
	 *  システム状態になるように準備する
	 */
	mrs		r12, cpsr				/* cpsr → r12 */
	and		r12, r12, #CPSR_INT_MASK
	ldr		lr, [sp,#STACK_SVC_cpsr]
	bic		lr, lr, #CPSR_INT_MASK
	orr		lr, r12, lr
	str		lr, [sp,#STACK_SVC_cpsr]

	/*
	 *  サービスコールからのリターン
	 */
ALABEL(ret_svc_1)
	rfefd	sp!

	/*
	 *  サービスコールからのリターン（予約機能コードエラー）
	 */
ALABEL(ret_svc_rsfn)
	mov		r0, #E_RSFN
	b		ret_svc_1

	/*
	 *  拡張サービスコールからのリターン（予約機能コードエラー）
	 */
ALABEL(ret_extsvc_rsfn)
	pop		{r4,lr}					/* r4を復帰．第6パラメータを捨てる */
	mov		r0, #E_RSFN
	b		ret_svc_1

	/*
	 *  拡張サービスコールからのリターン（メモリ不足エラー）
	 */
ALABEL(ret_extsvc_nomem)
	pop		{r4,lr}					/* r4を復帰．第6パラメータを捨てる */
	mov		r0, #E_NOMEM
	b		ret_svc_1

	/*
	 *  拡張サービスコールからのリターン（システムエラーエラー）
	 */
ALABEL(ret_extsvc_sys)
	pop		{r4,lr}					/* r4を復帰．第6パラメータを捨てる */
	mov		r0, #E_SYS
	b		ret_svc_1

	/*
	 *  拡張サービスコールの出入口処理
	 */
ALABEL(extsvc_entry)
	/*
	 *  第5パラメータをスタックに積む
	 *
	 *  第5パラメータを早めに積んで，使用できるレジスタを確保する．lr
	 *  をスタックに積むのは，第6パラメータ（cdmid）を置く領域を確保す
	 *  るためであり，積んでいる値には意味がない．
	 */
	push	{r4,lr}					/* lrはスペース確保のため */

	/* 
	 *  機能コードの上限値チェック
	 */
	ldr		r12, =tmax_fncd
	ldr		r12, [r12]
	cmp		r5, r12
	bgt		ret_extsvc_rsfn

	/* 
	 *  拡張サービスコールルーチンの先頭番地を取得．NULLでないかチェック
	 */
	ldr		lr, =svcinib_table-8
	add		lr, lr, r5, lsl #3			/* p_svcinib → lr */
	ldr		r4, [lr, #SVCINIB_extsvc]	/* p_svcinib->extsvc → r4 */
	tst		r4, r4						/* p_svcinib->extsvcがNULLか？ */
	beq		ret_extsvc_rsfn

	/* 
	 *  非タスクコンテキストからの呼出しなら，extsvc_entry_1へ分岐
	 */
	push	{r4}
	my_pcb	r12, r4
	pop		{r4}
	ldr		r12, [r12,#PCB_excpt_nest_count]
	tst		r12, r12
	movne	lr, #TDOM_KERNEL
	bne		extsvc_entry_1

    /*
     *  システムスタックの残量チェック［NGKI3197］
     */
	push	{r4}
	my_pcb	r12, r4
	pop		{r4}
	ldr		r12, [r12, #PCB_p_runtsk]
	ldr		r12, [r12, #TCB_p_tinib]
	ldr		r12, [r12, #TINIB_sstk]		/* スタックの先頭番地 → r12 */
	subs	r12, sp, r12				/* スタック残量 → r12 */
	blo		ret_extsvc_nomem			/* スタック残量 < 0 ならエラー */
	ldr		lr, [lr, #SVCINIB_stksz]	/* p_svcinib->stksz → lr */
	cmp		r12, lr						/* スタック残量 < lr ならエラー */
	blo		ret_extsvc_nomem

    /*
     *  svclevelのチェックとインクリメント［NGKI3198］
     */
	my_pcb	r12, lr
	ldr		r12, [r12, #PCB_p_runtsk]
	ldrb	lr, [r12,#TCB_svclevel]
	cmp		lr, #255					/* svclevel == UINT8_MAX ならエラー */
	beq		ret_extsvc_sys
	add		lr, lr, #1					/* svclevelをインクリメント */
	strb	lr, [r12,#TCB_svclevel]

	/*
	 *  cdmidを求め，rundomを更新する
	 */
	my_pcb	r12, lr
	ldr		lr, [r12,#PCB_rundom]		/* rundom → lr */
	tst		lr, lr
	moveq	lr, #TDOM_KERNEL			/* rundomがTACP_KERNELであれば */
	beq		extsvc_entry_1				/*			  extsvc_entry_1へ */
	mov		lr, #0
	str		lr, [r12,#PCB_rundom]		/* rundomをTACP_KERNELに設定 */
	ldr		r12, [r12,#PCB_p_runtsk]	/* ドメインID → lr */
	ldr		r12, [r12,#TCB_p_tinib]
	ldr		lr,  [r12,#TINIB_domid]

ALABEL(extsvc_entry_1)
	/*
	 *  ソフトウェア割込み前のシステム状態に戻す（自プロセッサのPCBに
	 *  アクセスする時には，割込みを禁止しておく必要がある）
	 *
	 * 【この時点のレジスタ状態】
	 *  r4：拡張サービスコールルーチンの先頭番地
	 *  lr：cdmid（呼出し元の保護ドメインID）
	 */
	str		lr, [sp,#4]				/* cdmidをスタック上に置く（lrの場所）*/
	ldr		r12, [sp,#STACK_SVC_cpsr+8]
	and		r12, r12, #CPSR_INT_MASK
	orr		r12, r12, #CPSR_SVC_MODE
	msr		cpsr_c, r12

#ifdef LOG_EXTSVC_ENTER
	push	{r0-r3,r4}				/* r4はアラインメントのため */
	push	{lr}					/* cdmidをスタックに積む */
	ldr		lr, [sp,#24]			/* 第5パラメータをスタックに積む */
	push	{lr}
	push	{r3}					/* 第4パラメータをスタックに積む */
	mov		r3, r2					/* 第3パラメータ → r3 */
	mov		r2, r1					/* 第2パラメータ → r2 */
	mov		r1, r0					/* 第1パラメータ → r1 */
	mov		r0, r5					/* 機能コード → r0 */
	bl		log_extsvc_enter
	add		sp, sp, #12				/* 3つのパラメータを捨てる */
	pop		{r0-r3,r4}
#endif /* LOG_EXTSVC_ENTER */

	/*
	 *  拡張サービスコールを呼び出す
	 */
	blx		r4						/* 拡張サービスコールルーチンへ分岐 */

ALABEL(ret_extsvc)
#ifdef LOG_EXTSVC_LEAVE
	push	{r0,r1}					/* r1はアラインメントのため */
	mov		r1, r0					/* 拡張サービスコールの返り値 → r1 */
	mov		r0, r5					/* 機能コード → r0 */
	bl		log_extsvc_leave
	pop		{r0,r1}
#endif /* LOG_EXTSVC_LEAVE */

	/*
	 *  r4を復帰し，第6パラメータを捨てる
	 *
	 *  拡張サービスコールは5つのパラメータを持つため，r4は壊されるもの
	 *  とする手も考えられるが，クラッチレジスタ以外は保存されるとした方
	 *  が良いと考え，r4を元の値（第5パラメータ）に戻す．第6パラメータは
	 *  捨てる（lrはダミー）．
	 */
	pop		{r4,lr}					/* r4を復帰．第6パラメータを捨てる */

	/*
	 *  ソフトウェア割込みからのリターン後のシステム状態が，この時点での
	 *  システム状態になるように準備する
	 */
	mrs		r12, cpsr				/* cpsr → r12 */
	and		r12, r12, #CPSR_INT_MASK
	ldr		lr, [sp,#STACK_SVC_cpsr]
	bic		lr, lr, #CPSR_INT_MASK
	orr		lr, r12, lr
	str		lr, [sp,#STACK_SVC_cpsr]

	/*
	 *  カーネル管理の割込みを禁止する（自プロセッサのPCBにアクセス
	 *  する時には，割込みを禁止しておく必要がある）
	 */
#ifndef TOPPERS_SAFEG_SECURE
	cpsid	i
#else /* TOPPERS_SAFEG_SECURE */
	cpsid	if
#endif /* TOPPERS_SAFEG_SECURE */

	/* 
	 *  非タスクコンテキストからの呼出しなら，リターン
	 */
	my_pcb	r12, lr
	ldr		r12, [r12,#PCB_excpt_nest_count]
	tst		r12, r12
	bne		ret_svc_1

	/*
	 *  svclevelをデクリメントし，0にならなければリターン
	 */
	my_pcb	r12, lr
	ldr		r12, [r12, #PCB_p_runtsk]
	ldrb	lr, [r12,#TCB_svclevel]
	subs	lr, lr, #1					/* svclevelをデクリメント */
	strb	lr, [r12,#TCB_svclevel]
	bne		ret_svc_1

	/*
	 *  rundomの設定
	 */
	ldr		lr, [r12,#TCB_p_dominib]
	ldr		lr, [lr,#DOMINIB_domptn]
	push	{r0}
	my_pcb	r12, r0
	pop		{r0}
	str		lr, [r12,#PCB_rundom]
	b		ret_svc_1

/*
 *  ステータスレジスタの操作関数
 *
 *  Thumbモードではmrs/msr命令が使用できないため，関数として実現して，
 *  ARMモードに移行して実行する．
 */
#ifdef __thumb__

	ATEXT
	AALIGN(2)
	AWEAK(_kernel_current_cpsr)
ALABEL(_kernel_current_cpsr)
	mrs		r0, cpsr_cxsf
	bx		lr

	AALIGN(2)
	AWEAK(_kernel_set_cpsr)
ALABEL(_kernel_set_cpsr)
	msr		cpsr_cxsf, r0
	bx		lr

#endif /* __thumb__ */

/*
 *  微少時間待ち
 *
 *  ダブルワード（64ビット）のどちらに命令があるかのよって実行時間が変
 *  わる可能性があると考え，64ビット境界にアラインしている．
 */
	ATEXT
	AALIGN(3)
	AGLOBAL(sil_dly_nse)
ALABEL(sil_dly_nse)
	mov		r1, #0
	mcr		p15, 0, r1, c7, c5, 6		/* 分岐予測全体の無効化 */
	asm_inst_sync_barrier r3
	subs	r0, r0, #SIL_DLY_TIM1
	bxls	lr
ALABEL(sil_dly_nse1)
	mcr		p15, 0, r1, c7, c5, 6		/* 分岐予測全体の無効化 */
	asm_inst_sync_barrier r3
	subs	r0, r0, #SIL_DLY_TIM2
	bhi		sil_dly_nse1
	bx		lr
