/*
 *  TOPPERS Software
 *      Toyohashi Open Platform for Embedded Real-Time Systems
 * 
 *  Copyright (C) 2023 by Embedded and Real-Time Systems Laboratory
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
 *  $Id: test_mprot3.c 1068 2023-01-09 08:57:31Z ertl-hiro $
 */

/* 
 *		メモリ保護機能のテスト(3)
 *
 * 【テストの目的】
 *
 *  サービスコールを通じたメモリアクセスの保護機能をテストする．また，
 *  prb_memとref_memのテストも行う．
 *
 *  テストには，get_tst，ref_tsk，snd_mbf，rcv_mbfを用いる．
 *
 * 【テスト項目】
 *
 *	(A) ユーザタスクからアクセスできるメモリ領域へのアクセス
 *		(A-1) get_tstがE_OKを返す
 *		(A-2) ref_tskがE_OKを返す
 *		(A-3) snd_mbfがE_OKを返す
 *		(A-4) rcv_mbfがE_OKを返す
 *		(A-5) prb_memがE_OKを返す
 *		(A-6) ref_memがE_OKを返す
 *	(B) ユーザタスクからのアラインしていないメモリアクセス［NGKI0670］
 *		(B-1) get_tstがE_MACVを返す
 *		(B-2) ref_tskがE_MACVを返す
 *		(B-3) prb_memがE_OKを返す
 *		(B-4) ref_memがE_OKを返す
 *	(C) ユーザタスクからカーネルに登録されていないメモリ領域へのアクセ
 *		ス［NGKI0412］
 *		(C-1) get_tstがE_MACVを返す
 *		(C-2) ref_tskがE_MACVを返す
 *		(C-3) snd_mbfがE_MACVを返す
 *		(C-4) rcv_mbfがE_MACVを返す
 *		(C-5) prb_memがE_NOEXSを返す［NGKI2930］
 *		(C-6) ref_memがE_NOEXSを返す［NGKI3956］
 *	(D) ユーザタスクからメモリオブジェクトの境界を越えるメモリ領域への
 *		アクセス［NGKI0670］
 *		(D-1) ref_tskがE_MACVを返す
 *		(D-2) snd_mbfがE_MACVを返す
 *		(D-3) rcv_mbfがE_MACVを返す
 *		(D-4) prb_memがE_OBJを返す［NGKI2932］
 *	(E) ユーザタスクからメモリオブジェクト属性によって書込みが許可され
 *		ていないメモリ領域への書込みアクセス［NGKI0414］
 *		(E-1) get_tstがE_MACVを返す
 *		(E-2) ref_tskがE_MACVを返す
 *		(E-3) snd_mbfがE_OKを返す
 *		(E-4) rcv_mbfがE_MACVを返す
 *		(E-5) prb_memがE_MACVを返す［NGKI2933］
 *		(E-6) ref_memがE_OKを返す
 *	(F) ユーザタスクからアクセス許可ベクタによって書込みが許可されてい
 *		ないメモリ領域への書込みアクセス［NGKI0414］
 *		(F-1) get_tstがE_MACVを返す
 *		(F-2) ref_tskがE_MACVを返す
 *		(F-3) snd_mbfがE_OKを返す
 *		(F-4) rcv_mbfがE_MACVを返す
 *		(F-5) prb_memがE_MACVを返す［NGKI2933］
 *		(F-6) ref_memがE_OKを返す
 *
 * 【使用リソース】
 *
 *	TASK1: 中優先度タスク，メインタスク，最初から起動，カーネルドメイン
 *	TASK2: 低優先度タスク，最初から起動，ユーザドメイン
 *
 * 【テストシーケンス】
 *
 *	== TASK1（優先度：中）==
 *	1:	slp_tsk()
 *	== TASK2（優先度：低）==
 *	2:	get_tst(TSK_SELF, (void *) buf1)							... (A-1)
 *		assert(*((STAT *) buf1) == TTS_RUN)
 *		ref_tsk(TSK_SELF, (void *) buf1)							... (A-2)
 *		assert(((T_RTSK *) buf1)->tskstat == TTS_RUN)
 *		assert(((T_RTSK *) buf1)->tskpri == LOW_PRIORITY)
 *	3:	DO(memcpy(buf1, string1, 16))
 *		snd_mbf(MBF1, buf1, 16)										... (A-3)
 *		rcv_mbf(MBF1, buf1 + 8) -> 16								... (A-4)
 *		assert(memcmp(buf1 + 8, string1, 16) == 0)
 *	4:	prb_mem(buf1, 16, TSK_SELF, TPM_READ|TPM_WRITE)				... (A-5)
 *		ref_mem(buf1, &rmem)										... (A-6)
 *	5:	get_tst(TSK_SELF, (void *)(buf1 + 1)) -> E_MACV				... (B-1)
 *		ref_tsk(TSK_SELF, (void *)(buf1 + 1)) -> E_MACV				... (B-2)
 *	6:	prb_mem((void *)(buf1 + 1), 16, TSK_SELF, TPM_READ|TPM_WRITE) ... (B-3)
 *		ref_mem((void *)(buf1 + 1), &rmem)							... (B-4)
 *	7:	get_tst(TSK_SELF, (void *) buf2) -> E_MACV					... (C-1)
 *		ref_tsk(TSK_SELF, (void *) buf2) -> E_MACV					... (C-2)
 *	8:	snd_mbf(MBF1, buf2, 16) -> E_MACV							... (C-3)
 *		rcv_mbf(MBF1, buf2 + 8) -> E_MACV							... (C-4)
 *	9:	prb_mem(buf2, 16, TSK_SELF, TPM_READ|TPM_WRITE) -> E_NOEXS	... (C-5)
 *		ref_mem(buf2, &rmem) -> E_NOEXS								... (C-6)
 *	10:	ref_tsk(TSK_SELF, (void *)(p_buf3 - 8)) -> E_MACV			... (D-1)
 *	11:	snd_mbf(MBF1, p_buf3 - 8, 16) -> E_MACV						... (D-2)
 *		rcv_mbf(MBF1, p_buf3 - 8) -> E_MACV							... (D-3)
 *	12:	prb_mem((void *)(p_buf3 - 8), 16, TSK_SELF, TPM_READ|TPM_WRITE) -> E_OBJ
 *																	... (D-4)
 *	13:	get_tst(TSK_SELF, (void *) buf4) -> E_MACV					... (E-1)
 *		ref_tsk(TSK_SELF, (void *) buf4) -> E_MACV					... (E-2)
 *	14:	snd_mbf(MBF1, buf4, 16)										... (E-3)
 *		rcv_mbf(MBF1, buf4 + 8) -> E_MACV							... (E-4)
 *	15:	prb_mem(buf4, 16, TSK_SELF, TPM_READ|TPM_WRITE) -> E_MACV	... (E-5)
 *		ref_mem(buf4, &rmem)										... (E-6)
 *		assert((rmem.accatr & TA_NOREAD) == 0)
 *		assert((rmem.accatr & TA_NOWRITE) != 0)
 *	16:	get_tst(TSK_SELF, (void *) buf5) -> E_MACV					... (F-1)
 *		ref_tsk(TSK_SELF, (void *) buf5) -> E_MACV					... (F-2)
 *	17:	snd_mbf(MBF1, buf5, 16)										... (F-3)
 *		rcv_mbf(MBF1, buf5 + 8) -> E_MACV							... (F-4)
 *	18:	prb_mem(buf5, 16, TSK_SELF, TPM_READ|TPM_WRITE) -> E_MACV	... (F-5)
 *		ref_mem(buf5, &rmem)										... (F-6)
 *		assert((rmem.accatr & TA_NOREAD) == 0)
 *		assert((rmem.accatr & TA_NOWRITE) == 0)
 *	19:	END
 */

#include <kernel.h>
#include <t_syslog.h>
#include "syssvc/test_svc.h"
#include "kernel_cfg.h"
#include "test_common.h"
#include <string.h>

char buf1[256] __attribute__((aligned(8)));
char buf2[256] __attribute__((aligned(8),section(".losec_1")));
char buf3[256] __attribute__((aligned(8),section(".rwdata_DOM1_1")));
char *p_buf3 = buf3;	/* コンパイラの警告を回避するために使用 */
char buf4[256] __attribute__((aligned(8),section(".rodata_DOM1_1")));
char buf5[256] __attribute__((aligned(8),section(".rodata_DOM1_2")));
const char string1[16] = "0123456789abcdef";

/* DO NOT DELETE THIS LINE -- gentest depends on it. */

void
task1(EXINF exinf)
{
	ER_UINT	ercd;

	test_start(__FILE__);

	check_point(1);
	ercd = slp_tsk();
	check_ercd(ercd, E_OK);

	check_assert(false);
}

void
task2(EXINF exinf)
{
	ER_UINT	ercd;
	T_RMEM	rmem;

	check_point(2);
	ercd = get_tst(TSK_SELF, (void *) buf1);
	check_ercd(ercd, E_OK);

	check_assert(*((STAT *) buf1) == TTS_RUN);

	ercd = ref_tsk(TSK_SELF, (void *) buf1);
	check_ercd(ercd, E_OK);

	check_assert(((T_RTSK *) buf1)->tskstat == TTS_RUN);

	check_assert(((T_RTSK *) buf1)->tskpri == LOW_PRIORITY);

	check_point(3);
	memcpy(buf1, string1, 16);

	ercd = snd_mbf(MBF1, buf1, 16);
	check_ercd(ercd, E_OK);

	ercd = rcv_mbf(MBF1, buf1 + 8);
	check_ercd(ercd, 16);

	check_assert(memcmp(buf1 + 8, string1, 16) == 0);

	check_point(4);
	ercd = prb_mem(buf1, 16, TSK_SELF, TPM_READ|TPM_WRITE);
	check_ercd(ercd, E_OK);

	ercd = ref_mem(buf1, &rmem);
	check_ercd(ercd, E_OK);

	check_point(5);
	ercd = get_tst(TSK_SELF, (void *)(buf1 + 1));
	check_ercd(ercd, E_MACV);

	ercd = ref_tsk(TSK_SELF, (void *)(buf1 + 1));
	check_ercd(ercd, E_MACV);

	check_point(6);
	ercd = prb_mem((void *)(buf1 + 1), 16, TSK_SELF, TPM_READ|TPM_WRITE);
	check_ercd(ercd, E_OK);

	ercd = ref_mem((void *)(buf1 + 1), &rmem);
	check_ercd(ercd, E_OK);

	check_point(7);
	ercd = get_tst(TSK_SELF, (void *) buf2);
	check_ercd(ercd, E_MACV);

	ercd = ref_tsk(TSK_SELF, (void *) buf2);
	check_ercd(ercd, E_MACV);

	check_point(8);
	ercd = snd_mbf(MBF1, buf2, 16);
	check_ercd(ercd, E_MACV);

	ercd = rcv_mbf(MBF1, buf2 + 8);
	check_ercd(ercd, E_MACV);

	check_point(9);
	ercd = prb_mem(buf2, 16, TSK_SELF, TPM_READ|TPM_WRITE);
	check_ercd(ercd, E_NOEXS);

	ercd = ref_mem(buf2, &rmem);
	check_ercd(ercd, E_NOEXS);

	check_point(10);
	ercd = ref_tsk(TSK_SELF, (void *)(p_buf3 - 8));
	check_ercd(ercd, E_MACV);

	check_point(11);
	ercd = snd_mbf(MBF1, p_buf3 - 8, 16);
	check_ercd(ercd, E_MACV);

	ercd = rcv_mbf(MBF1, p_buf3 - 8);
	check_ercd(ercd, E_MACV);

	check_point(12);
	ercd = prb_mem((void *)(p_buf3 - 8), 16, TSK_SELF, TPM_READ|TPM_WRITE);
	check_ercd(ercd, E_OBJ);

	check_point(13);
	ercd = get_tst(TSK_SELF, (void *) buf4);
	check_ercd(ercd, E_MACV);

	ercd = ref_tsk(TSK_SELF, (void *) buf4);
	check_ercd(ercd, E_MACV);

	check_point(14);
	ercd = snd_mbf(MBF1, buf4, 16);
	check_ercd(ercd, E_OK);

	ercd = rcv_mbf(MBF1, buf4 + 8);
	check_ercd(ercd, E_MACV);

	check_point(15);
	ercd = prb_mem(buf4, 16, TSK_SELF, TPM_READ|TPM_WRITE);
	check_ercd(ercd, E_MACV);

	ercd = ref_mem(buf4, &rmem);
	check_ercd(ercd, E_OK);

	check_assert((rmem.accatr & TA_NOREAD) == 0);

	check_assert((rmem.accatr & TA_NOWRITE) != 0);

	check_point(16);
	ercd = get_tst(TSK_SELF, (void *) buf5);
	check_ercd(ercd, E_MACV);

	ercd = ref_tsk(TSK_SELF, (void *) buf5);
	check_ercd(ercd, E_MACV);

	check_point(17);
	ercd = snd_mbf(MBF1, buf5, 16);
	check_ercd(ercd, E_OK);

	ercd = rcv_mbf(MBF1, buf5 + 8);
	check_ercd(ercd, E_MACV);

	check_point(18);
	ercd = prb_mem(buf5, 16, TSK_SELF, TPM_READ|TPM_WRITE);
	check_ercd(ercd, E_MACV);

	ercd = ref_mem(buf5, &rmem);
	check_ercd(ercd, E_OK);

	check_assert((rmem.accatr & TA_NOREAD) == 0);

	check_assert((rmem.accatr & TA_NOWRITE) == 0);

	check_finish(19);
	check_assert(false);
}
