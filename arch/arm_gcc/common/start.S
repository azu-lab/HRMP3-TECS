/*
 *  TOPPERS/HRMP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      High Reliable Multiprocessing Profile Kernel
 * 
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2005-2022 by Embedded and Real-Time Systems Laboratory
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
 *  $Id: start.S 1038 2022-12-23 05:52:05Z ertl-hiro $
 */

/*
 *		カーネル用のスタートアップモジュール（ARM用）
 */
	
#define TOPPERS_MACRO_ONLY
#define TOPPERS_ASM_MACRO
#define UINT_C(val)			(val)		/* uint_t型の定数を作るマクロ */
#define ULONG_C(val)		(val)		/* ulong_t型の定数を作るマクロ */
#define CAST(type, val)		(val)		/* 型キャストを行うマクロ */
#include "kernel_impl.h"
#include "core_asm.inc"

/*
 *  使用する命令セットの定義
 */
#if defined(USE_ARM_FPU_ALWAYS) || defined(USE_ARM_FPU_SELECTIVE)
	.fpu	ASM_ARM_FPU_TYPE
#endif /* defined(USE_ARM_FPU_ALWAYS) || defined(USE_ARM_FPU_SELECTIVE) */

/*
 *  同期用のマジックナンバー
 */
#define MAGIC_START 0x87654321

	ATEXT
	AALIGN(2)
	AGLOBAL(start)
ALABEL(start)
	/*
	 *  プロセッサモードの初期化
	 *
	 *  スーパバイザモード，全割込みロック状態に初期化する．
	 */
	msr		cpsr_cxsf, #(CPSR_SVC_MODE AOR CPSR_INTLOCK)

	/*
	 *  スタックポインタとフレームポインタの初期化
	 */
	my_istkpt sp, r0			/* スタックポインタ（sp）*/
	mov		fp, #0				/* ARMモード用フレームポインタ（r11）*/
	mov		r7, #0				/* Thumbモード用フレームポインタ（r7）*/

#if defined(USE_ARM_FPU_ALWAYS) || defined(USE_ARM_FPU_SELECTIVE)
	/*
	 *  FPU（CP10とCP11）をアクセス可能に設定する．
	 *
	 *	CP15_READ_CPACR(reg);
	 *	reg |= (CP15_CPACR_CP10_FULLACCESS | CP15_CPACR_CP11_FULLACCESS);
	 *	CP15_WRITE_CPACR(reg);
	 */
	mrc		p15, 0, r0, c1, c0, 2
	orr		r0, r0, #(CP15_CPACR_CP10_FULLACCESS | CP15_CPACR_CP11_FULLACCESS)
	mcr		p15, 0, r0, c1, c0, 2
	asm_inst_sync_barrier r0
#endif /* defined(USE_ARM_FPU_ALWAYS) || defined(USE_ARM_FPU_SELECTIVE) */

#ifdef USE_ARM_FPU_ALWAYS
	/*
	 *  FPUをイネーブルする．
	 *
	 *	set_fpexc(current_fpexc() | FPEXC_ENABLE);
	 */
	vmrs	r0, fpexc
	orr		r0, r0, #FPEXC_ENABLE
	vmsr	fpexc, r0
#endif /* USE_ARM_FPU_ALWAYS */
#ifdef USE_ARM_FPU_SELECTIVE
	/*
	 *  FPUをディスエーブルする．
	 *
	 *	set_fpexc(current_fpexc() & ~FPEXC_ENABLE);
	 */
	vmrs	r0, fpexc
	bic		r0, r0, #FPEXC_ENABLE
	vmsr	fpexc, r0
#endif /* USE_ARM_FPU_SELECTIVE */

	/*
	 *  hardware_init_hookの呼出し
	 *
	 *  ターゲットハードウェアに依存して必要な初期化処理がある場合は，
	 *  hardware_init_hookという関数を用意する．
	 */
	bl		hardware_init_hook

	/*
	 *  マスタプロセッサ以外は初期化待ち
	 */
	my_core_index	r0
	cmp				r0, #(TOPPERS_MASTER_PRCID - 1)
	bne				slave_wait

	/*
	 *  dataセクションとbssセクションの初期化
	 */
	bl   _kernel_initialize_sections

	/*
	 *  software_init_hookの呼出し
	 *
	 *  ソフトウェア環境（特にライブラリ）に依存して必要な初期化処理が
	 *  ある場合は，software_init_hookという関数を用意する．
	 */
	bl		software_init_hook

	/*
	 *  target_mprc_initialize の呼び出し
	 *
	 *  sta_ker 以前にマスタプロセッサのみで行う必要のある初期化を実行
	 */
ALABEL(start_6)
	bl		target_mprc_initialize

	/*
	 *  カーネルを起動する
	 */
ALABEL(start_7)
	ldr		r0, =start_sync
	ldr		r1, =MAGIC_START
	mov		r2, #0
ALABEL(start_8)
	str		r1, [r0, r2]
	add		r2, r2, #4
	cmp		r2, #(TNUM_PRCID * 4)
	bne		start_8

	/* マスタプロセッサ用のstart_syncを初期化 */
	my_core_index	r1
	str		r2, [r0, r1, lsl #2]
	b		sta_ker

	/*
	 *  マスタプロセッサ以外の初期化待ちルーチン
	 */
ALABEL(slave_wait)
	my_core_index	r0
	ldr		r1, =start_sync
ALABEL(slave_wait_1)
	ldr		r2, =MAGIC_START
	ldr		r3, [r1, r0, lsl #2]
	cmp		r3, r2
	bne		slave_wait_1
	mov		r4, #0
	str		r4, [r1, r0, lsl #2]
	b		sta_ker

/*
 *  デフォルトのhardware_init_hook（weak定義）
 */
	ATEXT
	AALIGN(2)
	AWEAK(hardware_init_hook)
ALABEL(hardware_init_hook)
	bx		lr

/*
 *  デフォルトのsoftware_init_hook（weak定義）
 */
	ATEXT
	AALIGN(2)
	AWEAK(software_init_hook)
ALABEL(software_init_hook)
	bx		lr
