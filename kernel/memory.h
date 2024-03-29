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
 *  $Id: memory.h 1060 2023-01-08 13:38:16Z ertl-hiro $
 */

/*
 *		メモリオブジェクト管理モジュール
 */

#ifndef TOPPERS_MEMORY_H
#define TOPPERS_MEMORY_H

#include "task.h"

#ifndef OMIT_STANDARD_MEMINIB

/*
 *  メモリオブジェクト初期化ブロック
 *
 *  未使用領域（TA_NOEXS）に対しては，すべてのアクセス許可パターン
 *  （acptn1，acptn2，acptn4）を0にする．また，リードオンリー（TA_RO）
 *  の領域に対しては，通常操作1（書込みアクセス）のアクセス許可パターン
 *  （acptn1）を0にする．
 */
typedef struct memory_initialization_block {
	ATR		accatr;			/* メモリオブジェクトアクセス属性 */
	ACPTN	acptn1;			/* 通常操作1のアクセス許可パターン */
	ACPTN	acptn2;			/* 通常操作2のアクセス許可パターン */
	ACPTN	acptn4;			/* 参照操作のアクセス許可パターン */
} MEMINIB;

/*
 *  メモリオブジェクト初期化ブロックのエントリ数（kernel_cfg.c）
 */
extern const uint_t		tnum_meminib;

/*
 *  メモリオブジェクトの先頭番地の領域（kernel_cfg.c）
 */
extern void *const		memtop_table[];

/*
 *  メモリオブジェクト初期化ブロックの領域（kernel_cfg.c）
 */
extern const MEMINIB	meminib_table[];

#endif /* OMIT_STANDARD_MEMINIB */

/*
 *  メモリオブジェクト管理モジュールの初期化
 */
extern void		initialize_memory(PCB *p_my_pcb);

/*
 *  メモリオブジェクト初期化ブロックの検索
 *
 *  メモリオブジェクト初期化ブロックから，addrを含むメモリオブジェクト
 *  を検索し，そのインデックスを返す．
 */
#ifndef OMIT_STANDARD_MEMINIB

extern int_t	search_meminib(const void *addr);

#endif /* OMIT_STANDARD_MEMINIB */

/*
 *  メモリ領域がメモリオブジェクトに含まれているかのチェック
 *
 *  先頭番地がbaseでサイズがsizeのメモリ領域が，先頭番地がmobaseでサイ
 *  ズがmosizeのメモリオブジェクトに含まれている場合にtrue，そうでない
 *  場合にfalseを返す．
 */
Inline bool_t
within_memobj(const void *base, size_t size, void *mobase, size_t mosize)
{
	return((size_t)(mobase) <= (size_t)(base) && size <= mosize
			&& (size_t)((char *)(base) - (char *)(mobase)) <= mosize - size);
}

/*
 *  メモリ領域がユーザスタック領域に含まれているかのチェック
 *
 *  先頭番地がbaseでサイズがsizeのメモリ領域が，p_tcbで指定されるタスク
 *  のユーザスタック領域に含まれている場合にtrue，そうでない場合に
 *  falseを返す．
 */
#ifndef USE_TSKINICTXB

Inline bool_t
within_ustack(const void *base, size_t size, TCB *p_tcb)
{
	return(within_memobj(base, size,
					p_tcb->p_tinib->ustk, p_tcb->p_tinib->ustksz));
}

#else /* USE_TSKINICTXB */

extern bool_t	within_ustack(const void *base, size_t size, TCB *p_tcb);

#endif /* USE_TSKINICTXB */

/*
 *  メモリへの書込み権のチェック
 *
 *  実行中の保護ドメイン（ユーザドメイン）が，指定したメモリ領域への書
 *  込み権を持っているかをチェックし，書込み権を持っている場合にtrue，
 *  そうでない場合にfalseを返す．実行中の保護ドメインがカーネルドメイン
 *  の時は，この関数を呼んではならない．
 */
extern bool_t	probe_mem_write(const void *base, size_t size,
										TCB *p_selftsk, ACPTN selfdom);

/*
 *  メモリからの読出し権のチェック
 *
 *  実行中の保護ドメイン（ユーザドメイン）が，指定したメモリ領域への読
 *  出し権を持っているかをチェックし，読出し権を持っている場合にtrue，
 *  そうでない場合にfalseを返す．実行中の保護ドメインがカーネルドメイン
 *  の時は，この関数を呼んではならない．
 */
extern bool_t	probe_mem_read(const void *base, size_t size,
										TCB *p_selftsk, ACPTN selfdom);

/*
 *  メモリアクセス権のチェックのためのマクロ
 */
#ifndef KERNEL_PROBE_MEM_WRITE
#define KERNEL_PROBE_MEM_WRITE(p_var, type, p_selftsk, selfdom) \
				(ALIGN_TYPE(p_var, type) && (selfdom == TACP_KERNEL \
					|| probe_mem_write((void *)(p_var), sizeof(type), \
													p_selftsk, selfdom)))
#endif /* KERNEL_PROBE_MEM_WRITE */

#ifndef KERNEL_PROBE_MEM_READ
#define KERNEL_PROBE_MEM_READ(p_var, type, p_selftsk, selfdom) \
				(ALIGN_TYPE(p_var, type) && (selfdom == TACP_KERNEL \
					|| probe_mem_read((void *)(p_var), sizeof(type), \
													p_selftsk, selfdom)))
#endif /* KERNEL_PROBE_MEM_READ */

#ifndef KERNEL_PROBE_BUF_WRITE
#define KERNEL_PROBE_BUF_WRITE(base, size, p_selftsk, selfdom) \
				(selfdom == TACP_KERNEL || probe_mem_write(base, size, \
													p_selftsk, selfdom))
#endif /* KERNEL_PROBE_BUF_WRITE */

#ifndef KERNEL_PROBE_BUF_READ
#define KERNEL_PROBE_BUF_READ(base, size, p_selftsk, selfdom) \
				(selfdom == TACP_KERNEL || probe_mem_read(base, size, \
													p_selftsk, selfdom))
#endif /* KERNEL_PROBE_BUF_READ */

/*
 *  dataセクション初期化ブロック
 */
typedef struct {
	void	*start_data;		/* dataセクションの先頭番地 */
	void	*end_data;			/* dataセクションの終了番地 */
	void	*start_idata;		/* 初期化データ領域の先頭番地 */
} DATASECINIB;

/*
 *  dataセクションの数と初期化ブロックのエリア（kernel_cfg.c）
 */
extern const uint_t	tnum_datasec;
extern const DATASECINIB	datasecinib_table[];

/*
 *  bssセクション初期化ブロック
 */
typedef struct {
	void	*start_bss;			/* bssセクションの先頭番地 */
	void	*end_bss;			/* bssセクションの終了番地 */
} BSSSECINIB;

/*
 *  bssセクションの数と初期化ブロックのエリア（kernel_cfg.c）
 */
extern const uint_t	tnum_bsssec;
extern const BSSSECINIB	bsssecinib_table[];

#endif /* TOPPERS_MEMORY_H */
