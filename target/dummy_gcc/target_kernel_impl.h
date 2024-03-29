/*
 *  TOPPERS/HRMP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      High Reliable Multiprocessing Profile Kernel
 * 
 *  Copyright (C) 2013-2020 by Embedded and Real-Time Systems Laboratory
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
 *  $Id: target_kernel_impl.h 839 2020-10-03 06:47:09Z ertl-hiro $
 */

/*
 *		カーネルのターゲット依存部（ダミーターゲット用）
 *
 *  カーネルのターゲット依存部のヘッダファイル．kernel_impl.hのターゲッ
 *  ト依存部の位置付けとなる．
 */

#ifndef TOPPERS_TARGET_KERNEL_IMPL_H
#define TOPPERS_TARGET_KERNEL_IMPL_H

#include <kernel.h>

/*
 *  ターゲットシステムのハードウェア資源の定義
 */
#include "dummy.h"

/*
 *  タイムマスタプロセッサ
 */
#define TOPPERS_TMASTER_PRCID	PRC1

/*
 *  エラーチェック方法の指定
 */
#define CHECK_SSTKSZ_ALIGN	4	/* システムスタックサイズのアライン単位 */
#define CHECK_USTKSZ_ALIGN	4	/* ユーザスタックサイズのアライン単位 */
#define CHECK_INTPTR_ALIGN	4	/* intptr_t型の変数のアライン単位 */
#define CHECK_INTPTR_NONNULL	/* intptr_t型の変数の非NULLチェック */
#define CHECK_FUNC_ALIGN	4	/* 関数のアライン単位 */
#define CHECK_FUNC_NONNULL		/* 関数の非NULLチェック */
#define CHECK_SSTACK_ALIGN	4	/* システムスタック領域のアライン単位 */
#define CHECK_SSTACK_NONNULL	/* システムスタック領域の非NULLチェック */
#define CHECK_USTACK_ALIGN	4	/* ユーザスタック領域のアライン単位 */
#define CHECK_USTACK_NONNULL	/* ユーザスタック領域の非NULLチェック */
#define CHECK_MPF_ALIGN		4	/* 固定長メモリプール領域のアライン単位 */
#define CHECK_MPF_NONNULL		/* 固定長メモリプール領域の非NULLチェック */
#define CHECK_MPK_ALIGN		4	/* カーネルメモリプール領域のアライン単位 */
#define CHECK_MPK_NONNULL		/* カーネルメモリプール領域の非NULLチェック */
#define CHECK_MB_ALIGN		4	/* 管理領域のアライン単位 */

/*
 *  タスクのシステムスタックのデフォルトのサイズ
 */
#define DEFAULT_SSTKSZ			2048

/*
 *  非タスクコンテキスト用スタックのデフォルトのサイズ
 */
#define DEFAULT_ISTKSZ			4096

/*
 *  デフォルトのアイドル処理用のスタック領域の定義
 */
#define DEFAULT_IDSTKSZ		0x0100U

#ifndef TOPPERS_MACRO_ONLY

/*
 *  プロセッサ管理ブロックのターゲット依存部の定義
 */
typedef struct target_processor_control_block {
	uint32_t	excpt_nest_count;	/* 例外ネストカウント */
	STK_T		*istkpt;			/* 非タスクコンテキスト用スタックの初期値 */
	STK_T		*idstkpt;			/* アイドル処理用スタックの初期値 */
	const FP	*p_inh_tbl;			/* 割込みハンドラテーブルへのポインタ */
	const FP	*p_exc_tbl;			/* CPU例外ハンドラテーブルへのポインタ */
} TPCB;

/*
 *  自プロセッサのインデックスを取り出す
 */
Inline uint_t
get_my_prcidx(void)
{
	return(0);
}

/*
 *  タスクコンテキストブロックの定義
 */
typedef struct task_context_block {
	void		*sp;			/* スタックポインタ */
	void		*pc;			/* 実行再開番地 */
} TSKCTXB;

/*
 *  コンテキストの参照
 */
Inline bool_t
sense_context(PCB *my_pcb)
{
	return(false);
}

/*
 *  CPUロック状態への遷移
 */
Inline void
lock_cpu(void)
{
}

/*
 *  CPUロック状態への移行（ディスパッチできる状態）
 */
#define lock_cpu_dsp()		lock_cpu()

/*
 *  CPUロック状態の解除
 */
Inline void
unlock_cpu(void)
{
}

/*
 *  CPUロック状態の解除（ディスパッチできる状態）
 */
#define unlock_cpu_dsp()	unlock_cpu()

/*
 *  CPUロック状態の参照
 */
Inline bool_t
sense_lock(void)
{
	return(false);
}

/*
 *  割込みを受け付けるための遅延処理
 */
Inline void
delay_for_interrupt(void)
{
}

/*
 *  割込み優先度マスクの設定
 */
Inline void
t_set_ipm(PRI intpri)
{
}

/*
 *  割込み優先度マスクの参照
 */
Inline PRI
t_get_ipm(void)
{
	return(TIPM_ENAALL);
}

/*
 *  割込み番号の範囲の判定
 */
#define VALID_INTNO(prcid, intno) \
			(((0 <= INTNO_MASK(intno))						\
					&& (INTNO_MASK(intno) <= 31)			\
					&& (INTNO_PRCID(intno) == prcid))		\
			|| ((32 <= INTNO_MASK(intno))					\
					&& (INTNO_MASK(intno) <= 63)			\
					&& (INTNO_PRCID(intno) == 0U)))

/*
 *  割込み属性の設定のチェック
 */
Inline bool_t
check_intno_cfg(INTNO intno)
{
	return(true);
}

/*
 *  割込み要求禁止フラグのセット
 */
Inline void
disable_int(INTNO intno)
{
}

/*
 *  割込み要求禁止フラグのクリア
 */
Inline void
enable_int(INTNO intno)
{
}

/*
 *  割込み要求がクリアできる状態か？
 */
Inline bool_t
check_intno_clear(INTNO intno)
{
	return(true);
}

/*
 *  割込み要求のクリア
 */
Inline void
clear_int(INTNO intno)
{
}

/*
 *  割込みが要求できる状態か？
 */
Inline bool_t
check_intno_raise(INTNO intno)
{
	return(true);
}

/*
 *  割込みの要求
 */
Inline void
raise_int(INTNO intno)
{
}

/*
 *  割込み要求のチェック
 */
Inline bool_t
probe_int(INTNO intno)
{
	return(true);
}

/*
 *  最高優先順位タスクへのディスパッチ
 */
extern void	dispatch(void);

/*
 *  非タスクコンテキストからのディスパッチ要求
 */
#define request_dispatch_retint()

/*
 *  ディスパッチャの動作開始
 */
extern void start_dispatch(void);

/*
 *  現在のコンテキストを捨ててディスパッチ
 */
extern void	exit_and_dispatch(void);

/*
 *  割込みハンドラ出入口処理
 */
extern void	int_handler_entry(void);

/*
 *  CPU例外ハンドラ出入口処理
 */
extern void	exc_handler_entry(void);

/*
 *  カーネルの終了処理の呼出し
 */
extern void call_exit_kernel(PCB *p_my_pcb) NoReturn;

/*
 *  自タスクのマイグレーション
 */
extern void dispatch_and_migrate(PCB *p_my_pcb, TCB *p_selftsk);

/*
 *  現在のコンテキストを捨てマイグレーション
 */
extern void exit_and_migrate(PCB *p_my_pcb, TCB *p_selftsk);

/*
 *  メモリバリア操作
 */
Inline void
memory_barrier(void)
{
}

/*
 *  タスクコンテキストの初期化
 */
extern void	start_r(void);

#define activate_context(p_tcb) \
	/* 指定されたタスク（p_tcb）のTCB中のスタックポインタを初期化する */
	/* start_rを，実行再開番地として自タスクのTCBに保存する */

/*
 *  割込みハンドラの設定
 *
 *  ベクトル番号inhnoの割込みハンドラの出入口処理の番地をint_entryに
 *  設定する．
 */
Inline void
define_inh(PCB *p_my_pcb, INHNO inhno, FP int_entry)
{
}

/*
 *  割込み要求ライン属性の設定
 */
Inline void
config_int(PCB *p_my_pcb, INTNO intno, ATR intatr, PRI intpri, uint_t affinity)
{
}

/*
 *  CPU例外ハンドラの設定
 *
 *  ベクトル番号excnoのCPU例外ハンドラの出入口処理の番地をexc_entryに設
 *  定する．
 */
Inline void
define_exc(EXCNO excno, FP exc_entry)
{
}

/*
 *  オーバランハンドラ停止のためのマクロ
 */
#ifdef TOPPERS_SUPPORT_OVRHDR
#define OVRTIMER_STOP()	{				\
			lock_cpu();					\
			_kernel_ovrtimer_stop();	\
			unlock_cpu();				\
		}
#else /* TOPPERS_SUPPORT_OVRHDR */
#define OVRTIMER_STOP()
#endif /* TOPPERS_SUPPORT_OVRHDR */

/*
 *  割込みハンドラの入口処理の生成マクロ
 */
#define INT_ENTRY(inhno, inthdr)	inthdr
#define INTHDR_ENTRY(inhno, inthdr, intpri)

/*
 *  CPU例外ハンドラの入口処理の生成マクロ
 */
#define EXC_ENTRY(excno, exchdr)	exchdr
#define EXCHDR_ENTRY(excno, excno_num, exchdr)

/*
 *  CPU例外の発生した時のコンテキストと割込みのマスク状態の参照
 *
 *  CPU例外の発生した時のシステム状態が，カーネル内のクリティカルセクショ
 *  ンの実行中でなく，全割込みロック状態でなく，CPUロック状態でなく，カー
 *  ネル管理外の割込みハンドラ実行中でなく，カーネル管理外のCPU例外ハン
 *  ドラ実行中でなく，タスクコンテキストであり，割込み優先度マスクが全
 *  解除である時にtrue，そうでない時にfalseを返す．
 */
Inline bool_t
exc_sense_intmask(void *p_excinf)
{
	return(true);
}

/*
 *  レッドゾーン方式を使用する
 */
#define USE_REDZONE
#define TARGET_DUMMY_STKSZ		4096

/*
 *  ターゲットシステム依存の初期化
 */
extern void	target_initialize(PCB *p_my_pcb);

/*
 *  ターゲットシステムの終了
 *
 *  システムを終了する時に使う．
 */
extern void	target_exit(void) NoReturn;

/*
 *  システム周期オーバラン例外の要求
 */
Inline void
raise_scycovr_exception(void)
{
}

/*
 *  ロックに関する定義と操作
 */

/*
 *  ロックの型
 */
typedef uint32_t	LOCK;

/*
 *  ロックの初期化
 */
Inline void
initialize_lock(LOCK *p_lock)
{
	*p_lock = 0U;
}

/*
 *  ロックの取得
 */
Inline void
acquire_lock(LOCK *p_lock)
{
	while (*p_lock) ;
	*p_lock = 1U;
}

/*
 *  ロックの取得の試行
 */
Inline bool_t
try_lock(LOCK *p_lock)
{
	if (*p_lock) {
		return(true);
	}
	*p_lock = 1U;
	return(false);
}

/*
 *  ロックの解放
 */
Inline void
release_lock(LOCK *p_lock)
{
	*p_lock = 0U;
}

/*
 *  ロック状態の参照
 */
Inline bool_t
refer_lock(LOCK *p_lock)
{
	return(*p_lock != 0U);
}

/*
 *  ジャイアントロックに関する定義と操作
 */

/*
 *  ジャイアントロック用のロック変数
 */
extern LOCK giant_lock;

/*
 *  ロックの初期化
 */
#define initialize_glock()		initialize_lock(&giant_lock)

/*
 *  ロックの取得
 */
#define acquire_glock()		acquire_lock(&giant_lock)

/*
 *  ロックの解放
 */
#define release_glock()		release_lock(&giant_lock)

/*
 *  ネイティブスピンロックに関する定義と操作
 *
 *  この時点では，SPNINIBが定義されていないため，ネイティブスピンロッ
 *  クに対する操作はマクロで実現している．
 */

/*
 *  スピンロックの初期化
 */
#define initialize_native_spn(p_spninib) \
				initialize_lock((LOCK *)((p_spninib)->lock))

/*
 *  スピンロックの取得
 */
#define lock_native_spn(p_spninib) \
				acquire_lock((LOCK *)((p_spninib)->lock))

/*
 *  スピンロックの取得の試行
 */
#define try_native_spn(p_spninib) \
				try_lock((LOCK *)((p_spninib)->lock))

/*
 *  スピンロックの返却
 */
#define unlock_native_spn(p_spninib) \
				release_lock((LOCK *)((p_spninib)->lock))

/*
 *  スピンロック状態の参照
 */
#define refer_native_spn(p_spninib) \
				refer_lock((LOCK *)((p_spninib)->lock))

/*
 *  エミュレーションされたスピンロックに関する定義
 */
#define delay_for_emulate_spn()

#endif /* TOPPERS_MACRO_ONLY */
#endif /* TOPPERS_TARGET_KERNEL_IMPL_H */
