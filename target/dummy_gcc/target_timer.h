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
 *  $Id: target_timer.h 932 2022-10-17 06:29:06Z ertl-hiro $
 */

/*
 *		タイマドライバ（ダミーターゲット用）
 */

#ifndef TOPPERS_TARGET_TIMER_H
#define TOPPERS_TARGET_TIMER_H

#include <sil.h>
#include "dummy.h"

/*
 *  高分解能タイマ割込みとタイムウィンドウタイマ割込みの割込み優先度
 *
 *  高分解能タイマ割込みとタイムウィンドウタイマ割込みの割込み優先度は，
 *  同じにしなければならない．
 */
#define INTPRI_TIMER		(TMAX_INTPRI - 1)

/*
 *  高分解能タイマ割込みハンドラ登録のための定数
 */
#define INHNO_HRT_PRC1		(0x10000U | TINTNO_HRT)		/* 割込みハンドラ番号 */
#define INHNO_HRT_PRC2		(0x20000U | TINTNO_HRT)		/* 割込みハンドラ番号 */
#define INHNO_HRT_PRC3		(0x30000U | TINTNO_HRT)		/* 割込みハンドラ番号 */
#define INHNO_HRT_PRC4		(0x40000U | TINTNO_HRT)		/* 割込みハンドラ番号 */
#define INTNO_HRT_PRC1		(0x10000U | TINTNO_HRT)		/* 割込み番号 */
#define INTNO_HRT_PRC2		(0x20000U | TINTNO_HRT)		/* 割込み番号 */
#define INTNO_HRT_PRC3		(0x30000U | TINTNO_HRT)		/* 割込み番号 */
#define INTNO_HRT_PRC4		(0x40000U | TINTNO_HRT)		/* 割込み番号 */
#define INTPRI_HRT			INTPRI_TIMER				/* 割込み優先度 */
#define INTATR_HRT			TA_EDGE						/* 割込み属性 */

#ifndef TOPPERS_MACRO_ONLY

/*
 *  高分解能タイマの初期化処理
 */
extern void	target_hrt_initialize(EXINF exinf);

/*
 *  高分解能タイマの終了処理
 */
extern void	target_hrt_terminate(EXINF exinf);

/*
 *  高分解能タイマの現在のカウント値の読出し
 *
 *  この関数はシステムログへのログ情報の出力時に呼び出されるため，この
 *  関数内でsyslogやassertを使ってはならない（無限の再帰呼出しが起こ
 *  る）．
 */
Inline HRTCNT
target_hrt_get_current(void)
{
	return(0U);
}

/*
 *  高分解能タイマへの割込みタイミングの設定
 */
Inline void
target_hrt_set_event(ID prcid, HRTCNT hrtcnt)
{
}

/*
 *  高分解能タイマへの割込みタイミングのクリア
 */
Inline void
target_hrt_clear_event(ID prcid)
{
}

/*
 *  高分解能タイマ割込みの要求
 */
Inline void
target_hrt_raise_event(ID prcid)
{
}

/*
 *  割込みタイミングに指定する最大値
 */
#define HRTCNT_BOUND	4000000002U

/*
 *  高分解能タイマ割込みハンドラ
 */
extern void target_hrt_handler(void);

#endif /* TOPPERS_MACRO_ONLY */

/*
 *  タイムウィンドウタイマ割込みハンドラ登録のための定数
 */
#define INHNO_TWDTIMER_PRC1	(0x10000U | TINTNO_TWDTIMER)	/* ハンドラ番号 */
#define INHNO_TWDTIMER_PRC2	(0x20000U | TINTNO_TWDTIMER)	/* ハンドラ番号 */
#define INHNO_TWDTIMER_PRC3	(0x30000U | TINTNO_TWDTIMER)	/* ハンドラ番号 */
#define INHNO_TWDTIMER_PRC4	(0x40000U | TINTNO_TWDTIMER)	/* ハンドラ番号 */
#define INTNO_TWDTIMER_PRC1	(0x10000U | TINTNO_TWDTIMER)	/* 割込み番号 */
#define INTNO_TWDTIMER_PRC2	(0x20000U | TINTNO_TWDTIMER)	/* 割込み番号 */
#define INTNO_TWDTIMER_PRC3	(0x30000U | TINTNO_TWDTIMER)	/* 割込み番号 */
#define INTNO_TWDTIMER_PRC4	(0x40000U | TINTNO_TWDTIMER)	/* 割込み番号 */
#define INTPRI_TWDTIMER		INTPRI_TIMER					/* 割込み優先度 */
#define INTATR_TWDTIMER		TA_EDGE							/* 割込み属性 */

#ifndef TOPPERS_MACRO_ONLY

/*
 *  タイムウィンドウタイマの初期化処理
 */
extern void target_twdtimer_initialize(EXINF exinf);

/*
 *  タイムウィンドウタイマの停止処理
 */
extern void target_twdtimer_terminate(EXINF exinf);

/*
 *  タイムウィンドウタイマの動作開始
 */
Inline void
target_twdtimer_start(PRCTIM twdtim)
{
}

/*
 *  タイムウィンドウタイマの停止
 */
Inline PRCTIM
target_twdtimer_stop(void)
{
	return(0U);
}

/*
 *  タイムウィンドウタイマの現在値の読出し
 */
Inline PRCTIM
target_twdtimer_get_current(void)
{
	return(0U);
}

/*
 *  タイムウィンドウタイマ割込みハンドラ
 */
extern void target_twdtimer_handler(void);

#endif /* TOPPERS_MACRO_ONLY */
#ifdef TOPPERS_SUPPORT_OVRHDR

/*
 *  オーバランタイマ割込みハンドラ登録のための定数
 */
#define INHNO_OVRTIMER_PRC1	(0x10000U | TINTNO_OVRTIMER)	/* ハンドラ番号 */
#define INHNO_OVRTIMER_PRC2	(0x20000U | TINTNO_OVRTIMER)	/* ハンドラ番号 */
#define INHNO_OVRTIMER_PRC3	(0x30000U | TINTNO_OVRTIMER)	/* ハンドラ番号 */
#define INHNO_OVRTIMER_PRC4	(0x40000U | TINTNO_OVRTIMER)	/* ハンドラ番号 */
#define INTNO_OVRTIMER_PRC1	(0x10000U | TINTNO_OVRTIMER)	/* 割込み番号 */
#define INTNO_OVRTIMER_PRC2	(0x20000U | TINTNO_OVRTIMER)	/* 割込み番号 */
#define INTNO_OVRTIMER_PRC3	(0x30000U | TINTNO_OVRTIMER)	/* 割込み番号 */
#define INTNO_OVRTIMER_PRC4	(0x40000U | TINTNO_OVRTIMER)	/* 割込み番号 */
#define INTPRI_OVRTIMER		TMAX_INTPRI						/* 割込み優先度 */
#define INTATR_OVRTIMER		TA_EDGE							/* 割込み属性 */

#ifndef TOPPERS_MACRO_ONLY

/*
 *  オーバランタイマの初期化処理
 */
extern void target_ovrtimer_initialize(EXINF exinf);

/*
 *  オーバランタイマの終了処理
 */
extern void target_ovrtimer_terminate(EXINF exinf);

/*
 *  オーバランタイマの動作開始
 */
Inline void
target_ovrtimer_start(PRCTIM ovrtim)
{
}

/*
 *  オーバランタイマの停止
 */
Inline PRCTIM
target_ovrtimer_stop(void)
{
	return(0U);
}

/*
 *  オーバランタイマの現在値の読出し
 */
Inline PRCTIM
target_ovrtimer_get_current(void)
{
	return(0U);
}

/*
 *  オーバランタイマ割込みハンドラ
 */
extern void target_ovrtimer_handler(void);

#endif /* TOPPERS_MACRO_ONLY */
#endif /* TOPPERS_SUPPORT_OVRHDR */
#endif /* TOPPERS_TARGET_TIMER_H */
