/*
 *  TOPPERS/HRMP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      High Reliable Multiprocessing Profile Kernel
 * 
 *  Copyright (C) 2008-2020 by Embedded and Real-Time Systems Laboratory
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
 *  $Id: domain.h 796 2020-05-25 07:06:39Z ertl-hiro $
 */

/*
 *		保護ドメイン管理モジュール
 */

#ifndef TOPPERS_DOMAIN_H
#define TOPPERS_DOMAIN_H

#include <queue.h>
#include "time_event.h"

/*
 *  システム周期（kernel_cfg.c）
 *
 *  時間パーティショニングを使用しない場合は0にする．
 */
extern const RELTIM	system_cyctim;

/*
 *  保護ドメイン初期化ブロックの型の前方参照
 */
typedef struct domain_initialization_block DOMINIB;

/*
 *  スケジューリング単位管理ブロック
 *
 *  p_predtskは，そのスケジューリング単位の中で優先順位が最も高いタス
 *  クのTCBを指すポインタである．実行できるタスクがない場合はNULLにす
 *  る．p_schedtskと異なり，割込み優先度マスク全解除でない状態の間と
 *  ディスパッチ禁止状態の間（すなわち，dspflgがfalseである間）であっ
 *  ても更新する．
 *
 *  ready_queueは，実行できる状態のタスクを管理するためのキューである．
 *  実行状態のタスクも管理しているため，レディ（実行可能）キューという
 *  名称は正確ではないが，レディキューという名称が定着しているため，こ
 *  の名称で呼ぶことにする．レディキューは，優先度ごとのタスクキューで
 *  構成されている．タスクのTCBは，該当する優先度のキューに登録される．
 *
 *  ready_primapは，優先度ごとのタスクキューにタスクが入っているかどう
 *  かを示すビットマップであり，レディキューのサーチを効率よく行うため
 *  用意している．ビットマップを使うことで，メモリアクセスの回数を減ら
 *  すことができるが，ビット操作命令が充実していないプロセッサで，優先
 *  度の段階数が少ない場合には，ビットマップ操作のオーバーヘッドのため
 *  に，逆に効率が落ちる可能性もある．優先度が16段階であることを仮定し
 *  ているため，uint16_t型としている．
 */
typedef struct scheduing_unit_control_block {
	TCB			*p_predtsk;					/* 優先順位が最も高いタスク */
	QUEUE		ready_queue[TNUM_TPRI];		/* レディキュー */
	uint16_t	ready_primap;				/* レディキューサーチのための */
} SCHEDCB;									/*				 ビットマップ */

/*
 *  アイドルドメインに対応するスケジューリング単位管理ブロックへのポイ
 *  ンタテーブル（kernel_cfg.c）
 */
extern SCHEDCB *const	p_schedcb_idle_table[];

/*
 *  タイムウィンドウ初期化ブロック
 */
typedef struct time_window_initialization_block {
	PRCTIM			twdlen;			/* タイムウィンドウ長 */
	const DOMINIB	*p_dominib;		/* 保護ドメイン初期化ブロック */
	EXINF			exinf;			/* 通知ハンドラの拡張情報 */
	NFYHDR			nfyhdr;			/* 通知ハンドラの起動番地 */
} TWDINIB;

/*
 *  システム動作モード初期化ブロック
 */
typedef struct system_operation_mode_initialization_block {
	const TWDINIB	*p_twdinib[TNUM_PRCID];	/* タイムウィンドウ初期化ブロック */
	const SOMINIB	*p_nxtsom;				/* 次のシステム動作モード */
} SOMINIB;

/*
 *  現在のシステム動作モード
 *
 *  現在のシステム全体でのシステム動作モードの初期化ブロックを指すポイ
 *  ンタ．システム周期停止モードの場合は，NULLにする．
 */
extern const SOMINIB	*p_global_cursom;

/*
 *  次のシステム動作モード
 *
 *  現在のシステム動作モードがシステム周期停止モードの場合，この変数は
 *  無効である．
 */
extern const SOMINIB	*p_global_nxtsom;

/*
 *  システム周期切換えを実行したプロセッサ（ビットマップ）
 */
extern uint_t	scycprc_bitmap;

/*
 *  システム周期を同期させるための変数
 */
extern EVTTIM	scyc_evttim;

/*
 *  システム動作モードIDの最大値（kernel_cfg.c）
 */
extern const ID		tmax_somid;

/*
 *  初期システム動作モード（kernel_cfg.c）
 *
 *  初期システム動作モードが設定されていない時は，NULLにする．
 */
extern const SOMINIB	*const p_inisom;

/*
 *  システム動作モード初期化ブロックのエリア（kernel_cfg.c）
 */
extern const SOMINIB	sominib_table[];

/*
 *  システム動作モードIDからシステム動作モード初期化ブロックを取り出す
 *  ためのマクロ
 */
#define INDEX_SOM(somid)	((uint_t)((somid) - TMIN_SOMID))
#define get_sominib(somid)	(&(sominib_table[INDEX_SOM(somid)]))

/*
 *  システム動作モード初期化ブロックからシステム動作モードIDを取り出す
 *  ためのマクロ
 */
#define	SOMID(p_sominib)	((ID)(((p_sominib) - sominib_table) + TMIN_SOMID))

/*
 *  保護ドメイン初期化ブロック
 */
typedef struct domain_initialization_block {
	ACPTN		domptn;						/* 保護ドメインのビットパターン */
	SCHEDCB		*p_schedcb[TNUM_PRCID];		/* スケジューリング単位  */
											/* 			管理ブロック */
	TMEVTN		*p_tmevt_heap[TNUM_PRCID];	/* タイムイベントヒープ */
#ifdef USE_DOMINICTXB
	DOMINICTXB	domctxb;					/* 保護ドメイン初期化		 */
#endif /* USE_DOMINICTXB */					/*		コンテキストブロック */
	ACVCT		acvct;						/* アクセス許可ベクタ */
} DOMINIB;

/*
 *  保護ドメインIDの最大値（kernel_cfg.c）
 */
extern const ID	tmax_domid;

/*
 *  カーネルドメインの保護ドメイン初期化ブロック（kernel_cfg.c）
 */
extern const DOMINIB	dominib_kernel;

/*
 *  保護ドメイン初期化ブロックのエリア（kernel_cfg.c）
 */
extern const DOMINIB	dominib_table[];

/*
 *  ユーザドメインの数
 */
#define tnum_udom	((uint_t)(tmax_domid - TMIN_DOMID + 1))

/*
 *  保護ドメインIDから保護ドメイン初期化ブロックを取り出すためのマクロ
 */
#define INDEX_DOM(domid)	((uint_t)((domid) - TMIN_DOMID))
#define get_dominib(domid)	(&(dominib_table[INDEX_DOM(domid)]))

/*
 *  保護ドメイン管理モジュールの初期化
 */
extern void		initialize_domain(PCB *p_my_pcb);

/*
 *  タイムウィンドウタイマの動作開始
 */
extern void		twdtimer_start(PCB *p_my_pcb);

/*
 *  タイムウィンドウタイマの停止
 */
extern void		twdtimer_stop(PCB *p_my_pcb);

/*
 *  タイムウィンドウタイマの切換え
 */
extern void		twdtimer_control(PCB *p_my_pcb);

/*
 *  システム周期の実行開始
 */
extern void		scyc_start(PCB *p_my_pcb);

/*
 *  システム周期切換え処理
 */
extern void		scyc_switch(PCB *p_my_pcb);

/*
 *  タイムウィンドウの実行開始
 */
extern void		twd_start(PCB *p_my_pcb);

/*
 *  タイムウィンドウ周期切換え処理
 */
extern void		twd_switch(void);

/*
 *  タスクディスパッチ可能状態への遷移
 *
 *  割込み優先度マスクをTIPM_ENAALLに，タスクディスパッチ可能状態であ
 *  ることを示すフラグ（dspflg）をtrueにし，保留された切換え処理を実行
 *  する．また，実行すべきタスクを更新する．
 */
extern void		set_dspflg(PCB *p_my_pcb);

/*
 *  システム周期開始ハンドラ
 */
extern void		start_scyc_handler(void);

#endif /* TOPPERS_DOMAIN_H_ */
