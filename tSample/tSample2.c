/*
 *  TOPPERS/ASP Kernel
 *      Toyohashi Open Platform for Embedded Real-Time Systems/
 *      Advanced Standard Profile Kernel
 *
 *  Copyright (C) 2015-2016 by Ushio Laboratory
 *              Graduate School of Engineering Science, Osaka Univ., JAPAN
 *  Copyright (C) 2015-2018 by Embedded and Real-Time Systems Laboratory
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
 *  $Id: tSample2.c 945 2018-04-18 00:40:22Z ertl-hiro $
 */

/* 
 *  サンプルプログラム(2)の本体
 *
 *  ASPカーネルの基本的な動作を確認するためのサンプルプログラム（TECS版）．
 *
 *  プログラムの概要:
 *
 *  ユーザインタフェースを受け持つメインタスク（優先度: MAIN_PRIORITY）
 *  と，3つの並行実行されるタスク（初期優先度: MID_PRIORITY）で構成さ
 *  れる．また，起動周期が2秒の周期ハンドラを用いる．
 *
 *  並行実行されるタスクは，task_loop回空ループを実行する度に，タスクが
 *  実行中であることをあらわすメッセージを表示する．空ループを実行する
 *  のは，空ループなしでメッセージを出力すると，多量のメッセージが出力
 *  され，プログラムの動作が確認しずらくなるためである．また，低速なシ
 *  リアルポートを用いてメッセージを出力する場合に，すべてのメッセージ
 *  が出力できるように，メッセージの量を制限するという理由もある．
 *
 *  周期ハンドラは，三つの優先度（HIGH_PRIORITY，MID_PRIORITY，
 *  LOW_PRIORITY）のレディキューを回転させる．プログラムの起動直後は，
 *  周期ハンドラは停止状態になっている．
 *
 *  メインタスクは，シリアルI/Oポートからの文字入力を行い（文字入力を
 *  待っている間は，並列実行されるタスクが実行されている），入力された
 *  文字に対応した処理を実行する．入力された文字と処理の関係は次の通り．
 *  Control-Cまたは'Q'が入力されると，プログラムを終了する．
 *
 *  '1' : 対象タスクをTASK1に切り換える（初期設定）．
 *  '2' : 対象タスクをTASK2に切り換える．
 *  '3' : 対象タスクをTASK3に切り換える．
 *  '8' : 対象サンプルをSample2_2(PRC2) => Sample2(PRC1)に切り換える．
 *  '9' : 対象サンプルをSample2(PRC1) => Sample2_2(PRC2)に切り換える．
 *  *  'a' : 対象タスクをcTask_activateにより起動する．
 *  'A' : 対象タスクに対する起動要求をcTask_cancelActivateによりキャンセルする．
 *  'e' : 対象タスクにexitTaskを呼び出させ，終了させる．
 *  't' : 対象タスクをcTask_terminateにより強制終了する．
 *  '>' : 対象タスクの優先度をHIGH_PRIORITYにする．
 *  '=' : 対象タスクの優先度をMID_PRIORITYにする．
 *  '<' : 対象タスクの優先度をLOW_PRIORITYにする．
 *  'G' : 対象タスクの優先度をcTask_getPriorityで読み出す．
 *  's' : 対象タスクにsleepを呼び出させ，起床待ちにさせる．
 *  'S' : 対象タスクにsleepTimeout10秒)を呼び出させ，起床待ちにさせる．
 *  'w' : 対象タスクをcTask_wakeupにより起床する．
 *  'W' : 対象タスクに対する起床要求をcTask_cancelWakeupによりキャンセルする．
 *  'l' : 対象タスクをcTask_releaseWaitにより強制的に待ち解除にする．
 *  'u' : 対象タスクをcTask_suspendにより強制待ち状態にする．
 *  'm' : 対象タスクの強制待ち状態をcTask_resumeにより解除する．
 *  'd' : 対象タスクにdelay(10秒)を呼び出させ，時間経過待ちにさせる．
 *  'x' : 対象タスクにraiseTerminateにより終了要求する．
 *  'y' : 対象タスクにdisableTerminateを呼び出させ，タスク終了を禁止する．
 *  'Y' : 対象タスクにenableTerminateを呼び出させ，タスク終了を許可する．
 *  'r' : 3つの優先度（HIGH_PRIORITY，MID_PRIORITY，LOW_PRIORITY）のレ
 *        ディキューを回転させる．
 *  'c' : 周期ハンドラを動作開始させる．
 *  'C' : 周期ハンドラを動作停止させる．
 *  'b' : アラームハンドラを5秒後に起動するよう動作開始させる．
 *  'B' : アラームハンドラを動作停止させる．
 *  'z' : 対象タスクにCPU例外を発生させる（タスクを終了させる）．
 *  'Z' : 対象タスクにCPUロック状態でCPU例外を発生させる（プログラムを
 *        終了する）．
 *  'V' : fetchHighResolutionTimerで高分解能タイマを2回読む．
 *  'v' : 発行したシステムコールを表示する（デフォルト）．
 *  'q' : 発行したシステムコールを表示しない．
 */

#include "tSample2_tecsgen.h"
#include <kernel.h>
#include <t_syslog.h>
#include <t_stdlib.h>
#include "kernel_cfg.h"
#include "tSample2.h"

/*
 *  サービスコールのエラーのログ出力
 */
Inline void
svc_perror(const char *file, int_t line, const char *expr, ER ercd)
{
	if (ercd < 0) {
		t_perror(LOG_ERROR, file, line, expr, ercd);
	}
}

#define	SVC_PERROR(expr)	svc_perror(__FILE__, __LINE__, #expr, (expr))

/*
 *  並行実行されるタスクへのメッセージ領域
 */
char	message[N_SUB_TASK];

/*
 *  ループ回数
 */
ulong_t	task_loop;		/* タスク内でのループ回数 */

/*
 *  並行実行されるタスク
 */
void
eSampleTask_main(CELLIDX idx, int_t subscript)
{
	CELLCB		*p_cellcb = GET_CELLCB(idx);
	volatile ulong_t	i;
	int_t		n = 0;
	int_t		tskno = subscript +1;
	char		c;

	while (true) {
		syslog(LOG_NOTICE, "%s is running (%03d).   %s",
							ATTR_taskName[tskno-1], ++n, ATTR_taskGraph[tskno-1]);
		for (i = 0; i < task_loop; i++);
		c = message[tskno-1];
		message[tskno-1] = 0;
		switch (c) {
		case 'e':
			syslog(LOG_INFO, "#%d#exit()", tskno);
			SVC_PERROR(exit());
			assert(0);
		case 's':
			syslog(LOG_INFO, "#%d#sleep()", tskno);
			SVC_PERROR(sleep());
			break;
		case 'S':
			syslog(LOG_INFO, "#%d#sleepTimeout(10000)", tskno);
			SVC_PERROR(sleepTimeout(10000));
			break;
		case 'd':
			syslog(LOG_INFO, "#%d#delay(10000)", tskno);
			SVC_PERROR(delay(10000));
			break;
		case 'y':
			syslog(LOG_INFO, "#%d#disableTerminate()", tskno);
			SVC_PERROR(disableTerminate());
			break;
		case 'Y':
			syslog(LOG_INFO, "#%d#enableTerminate()", tskno);
			SVC_PERROR(enableTerminate());
			break;
#ifdef CPUEXC1
		case 'z':
			syslog(LOG_NOTICE, "#%d#raise CPU exception", tskno);
			RAISE_CPU_EXCEPTION;
			break;
		case 'Z':
			SVC_PERROR(lockCpu());
			syslog(LOG_NOTICE, "#%d#raise CPU exception", tskno);
			RAISE_CPU_EXCEPTION;
			SVC_PERROR(unlockCpu());
			break;
#endif /* CPUEXC1 */
		default:
			break;
		}
	}
}

/*
 *  割込みサービスルーチン
 */
#ifdef INTNO1

void
eiISR_main(CELLIDX idx)
{
	intno1_clear();
	CELLCB		*p_cellcb = GET_CELLCB(idx);
	SVC_PERROR(ciKernel_rotateReadyQueue(HIGH_PRIORITY));
	SVC_PERROR(ciKernel_rotateReadyQueue(MID_PRIORITY));
	SVC_PERROR(ciKernel_rotateReadyQueue(LOW_PRIORITY));
}

#endif /* INTNO1 */

/*
 *  CPU例外ハンドラ
 */

ID	cpuexc_tskid;		/* CPU例外を起こしたタスクのID */

#ifdef CPUEXC1

void
eiCpuExceptionHandler_main(CELLIDX idx, const void *p_excinf)
{
	CELLCB		*p_cellcb = GET_CELLCB(idx);
	
	syslog(LOG_NOTICE, "CPU exception handler (p_excinf = %08p).", p_excinf);
	if (ciKernel_senseContext() != true) {
		syslog(LOG_WARNING,
					"ciKernel_senseContext() is not true in CPU exception handler.");
	}
	if (ciKernel_senseDispatchPendingState() != true) {
		syslog(LOG_WARNING,
					"ciKernel_senseDispatchPendingState() is not true in CPU exception handler.");
	}
	syslog(LOG_INFO, "ciKernel_senseLock() = %d ciKernel_senseDispatch() = %d ciKernel_exceptionSenseDispatchPendingState() = %d",
		   ciKernel_senseLock(), ciKernel_senseDispatch(), ciKernel_exceptionSenseDispatchPendingState(p_excinf));

	if (ciKernel_exceptionSenseDispatchPendingState(p_excinf)) {
		syslog(LOG_NOTICE, "Sample program ends with exception.");
		SVC_PERROR(ciKernel_exitKernel());
		assert(0);
	}

#ifdef PREPARE_RETURN_CPUEXC
	PREPARE_RETURN_CPUEXC;
	SVC_PERROR(ciKernel_getTaskId(&cpuexc_tskid));
	cExceptionTask_activate();
#else /* PREPARE_RETURN_CPUEXC */
	syslog(LOG_NOTICE, "Sample program ends with exception.");
	SVC_PERROR(ciKernel_exitKernel());
	assert(0);
#endif /* PREPARE_RETURN_CPUEXC */
}

#endif /* CPUEXC1 */

/*
 *  周期ハンドラ
 *
 *  HIGH_PRIORITY，MID_PRIORITY，LOW_PRIORITY の各優先度のレディキュー
 *  を回転させる．
 */
void
eiCyclicHandler_main(CELLIDX idx)
{
	CELLCB		*p_cellcb = GET_CELLCB(idx);
	SVC_PERROR(ciKernel_rotateReadyQueue(HIGH_PRIORITY));
	SVC_PERROR(ciKernel_rotateReadyQueue(MID_PRIORITY));
	SVC_PERROR(ciKernel_rotateReadyQueue(LOW_PRIORITY));
}

/*
 *  アラームハンドラ
 *
 *  HIGH_PRIORITY，MID_PRIORITY，LOW_PRIORITY の各優先度のレディキュー
 *  を回転させる．
 */
void
eiAlarmHandler_main(CELLIDX idx)
{
	CELLCB		*p_cellcb = GET_CELLCB(idx);
	SVC_PERROR(ciKernel_rotateReadyQueue(HIGH_PRIORITY));
	SVC_PERROR(ciKernel_rotateReadyQueue(MID_PRIORITY));
	SVC_PERROR(ciKernel_rotateReadyQueue(LOW_PRIORITY));
}

/*
 *  例外処理タスク
 */
void
eExceptionTask_main(CELLIDX idx)
{
	CELLCB		*p_cellcb = GET_CELLCB(idx);
	(void)p_cellcb;
	SVC_PERROR(ras_ter(cpuexc_tskid));
}

/*
 *  プロセッサ時間の消費
 *
 *  ループによりプロセッサ時間を消費する．最適化ができないように，ルー
 *  プ内でvolatile変数を読み込む．
 */
static volatile long_t	volatile_var;

static void
consume_time(ulong_t ctime)
{
	ulong_t		i;

	for (i = 0; i < ctime; i++) {
		(void) volatile_var;
	}
}

/*
 *  メインタスク
 */
void
eMainTask_main(CELLIDX idx)
{
	
	char	c;
	int_t	tskno = 1;
	ER_UINT	ercd;
	PRI		tskpri;
	CELLCB  *p_cellcb = GET_CELLCB(idx);
#ifndef TASK_LOOP
	SYSTIM	stime1, stime2;
#endif /* TASK_LOOP */
	HRTCNT	hrtcnt1, hrtcnt2;
	ID      prcid;
	int_t   mainno = ATTR_sampleNo;

	SVC_PERROR(cSysLog_mask(LOG_UPTO(LOG_INFO), LOG_UPTO(LOG_EMERG)));
	SVC_PERROR(getProcessorID(&prcid));
	syslog(LOG_NOTICE, "Sample program (No.%d) starts on processorID=%d", mainno, prcid);

	/*
	 *  シリアルポートの初期化
	 *
	 *  システムログタスクと同じシリアルポートを使う場合など，シリアル
	 *  ポートがオープン済みの場合にはここでE_OBJエラーになるが，支障は
	 *  ない．
	 */
	ercd = cSerialPort_open();
	if (ercd < 0 && MERCD(ercd) != E_OBJ) {
		syslog(LOG_ERROR, "%s (%d) reported by `cSerialPort_open'.",
									itron_strerror(ercd), SERCD(ercd));
	}
	SVC_PERROR(cSerialPort_control(IOCTL_CRLF | IOCTL_FCSND | IOCTL_FCRCV));

	/*
 	 *  ループ回数の設定
	 *
	 *  並行実行されるタスク内でのループの回数（task_loop）は，ループ
	 *  の実行時間が約0.2秒になるように設定する．この設定のために，
	 *  LOOP_REF回のループの実行時間を，その前後でget_timを呼ぶことで
	 *  測定し，その測定結果から空ループの実行時間が0.2秒になるループ
	 *  回数を求め，task_loopに設定する．
	 *
	 *  LOOP_REFは，デフォルトでは1,000,000に設定しているが，想定した
	 *  より遅いプロセッサでは，サンプルプログラムの実行開始に時間がか
	 *  かりすぎるという問題を生じる．逆に想定したより速いプロセッサで
	 *  は，LOOP_REF回のループの実行時間が短くなり，task_loopに設定す
	 *  る値の誤差が大きくなるという問題がある．そこで，そのようなター
	 *  ゲットでは，target_test.hで，LOOP_REFを適切な値に定義すること
	 *  とする．
	 *
	 *  また，task_loopの値を固定したい場合には，その値をTASK_LOOPにマ
	 *  クロ定義する．TASK_LOOPがマクロ定義されている場合，上記の測定
	 *  を行わずに，TASK_LOOPに定義された値をループの回数とする．
	 *
	 *  ターゲットによっては，ループの実行時間の1回目の測定で，本来より
	 *  も長めになるものがある．このようなターゲットでは，MEASURE_TWICE
	 *  をマクロ定義することで，1回目の測定結果を捨てて，2回目の測定結
	 *  果を使う．
	 */
#ifdef TASK_LOOP
	task_loop = TASK_LOOP;
#else /* TASK_LOOP */

#ifdef MEASURE_TWICE
	SVC_PERROR(get_tim(&stime1));
	consume_time(LOOP_REF);
	SVC_PERROR(get_tim(&stime2));
#endif /* MEASURE_TWICE */

	SVC_PERROR(get_tim(&stime1));
	consume_time(LOOP_REF);
	SVC_PERROR(get_tim(&stime2));
	task_loop = LOOP_REF * 200LU / (ulong_t)(stime2 - stime1) * 1000LU;

#endif /* TASK_LOOP */

	/*
 	 *  タスクの起動
	 */
	SVC_PERROR(cTask_activate(1));
	SVC_PERROR(cTask_activate(2));
	SVC_PERROR(cTask_activate(3));
    //SVC_PERROR(changeSystemOperationMode(SOM1));

	/*
	 * sample No.1 は停止する (一方のみがシリアルから読み込むようにする)
	 */
	if( mainno == 2 ){
		SVC_PERROR(sleep());
		syslog(LOG_INFO, "MainTask %d get up on processorID %d", mainno, prcid);
	}

	/*
 	 *  メインループ
	 */
	do {
		SVC_PERROR(cSerialPort_read(&c, 1));
		switch (c) {
		case 'e':
		case 's':
		case 'S':
		case 'd':
		case 'y':
		case 'Y':
		case 'z':
		case 'Z':
			message[tskno - 1] = c;
			break;
		case '1':
			tskno = 1;
			break;
		case '2':
			tskno = 2;
			break;
		case '3':
			tskno = 3;
			break;
		case '8':	// 同様の処理を2回書くのを避けるため、break しないで続ける
		case '9':
			if( (c == '8' && mainno == 2) || (c == '9' && mainno == 1) ){
				syslog(LOG_INFO, "MainTask %d sleep and wakeup the other one", mainno);
				SVC_PERROR(cTask_wakeup(0));
				SVC_PERROR(sleep());
				syslog(LOG_INFO, "MainTask %d get up on processorID %d", mainno, prcid);
			} else {
				syslog(LOG_INFO, "MainTask %d is running. typing '%c' is ignored.", mainno, c);
			}
			break;
		case 'a':
			syslog(LOG_INFO, "MainTask %d #cTask_activate(%d)", mainno, tskno);
			SVC_PERROR(cTask_activate(tskno));
			break;
		case 'A':
			syslog(LOG_INFO, "MainTask %d #cTask_cancelActivate(%d)", mainno, tskno);
			SVC_PERROR(ercd = cTask_cancelActivate(tskno));
			if (ercd >= 0) {
				syslog(LOG_NOTICE, "cTask_cancelActivate(%d) returns %d",
															tskno, ercd);
			}
			break;
		case 't':
			syslog(LOG_INFO, "MainTask %d #cTask_terminate(%d)", mainno, tskno);
			SVC_PERROR(cTask_terminate(tskno));
			break;
		case '>':
			syslog(LOG_INFO, "MainTask %d #cTask_changePriority(%d, HIGH_PRIORITY)", mainno, tskno);
			SVC_PERROR(cTask_changePriority(tskno, HIGH_PRIORITY));
			break;
		case '=':
			syslog(LOG_INFO, "MainTask %d #cTask_changePriority(%d, MID_PRIORITY)", mainno, tskno);
			SVC_PERROR(cTask_changePriority(tskno, MID_PRIORITY));
			break;
		case '<':
			syslog(LOG_INFO, "MainTask %d #(cTask_changePriority(%d, LOW_PRIORITY)", mainno, tskno);
			SVC_PERROR(cTask_changePriority(tskno, LOW_PRIORITY));
			break;
		case 'G':
			syslog(LOG_INFO, "MainTask %d #cTask_getPriority(%d, &tskpri)", mainno, tskno);
			SVC_PERROR(ercd = cTask_getPriority(tskno, &tskpri));
			if (ercd >= 0) {
				syslog(LOG_NOTICE, "priority of task %d is %d", tskno, tskpri);
			}
			break;
		case 'w':
			syslog(LOG_INFO, "MainTask %d #cTask_wakeup(%d)", mainno, tskno);
			SVC_PERROR(cTask_wakeup(tskno));
			break;
		case 'W':
			syslog(LOG_INFO, "MainTask %d #cTask_cancelWakeup(%d)", mainno, tskno);
			SVC_PERROR(ercd = cTask_cancelWakeup(tskno));
			if (ercd >= 0) {
				syslog(LOG_NOTICE, "cTask_cancelWakeup(%d) returns %d",
															tskno, ercd);
			}
			break;
		case 'l':
			syslog(LOG_INFO, "MainTask %d #cTask_releaseWait(%d)", mainno, tskno);
			SVC_PERROR(cTask_releaseWait(tskno));
			break;
		case 'u':
			syslog(LOG_INFO, "MainTask %d #cTask_suspend(%d)", mainno, tskno);
			SVC_PERROR(cTask_suspend(tskno));
			break;
		case 'm':
			syslog(LOG_INFO, "MainTask %d #cTask_resume(%d)", mainno, tskno);
			SVC_PERROR(cTask_resume(tskno));
			break;
		case 'x':
			syslog(LOG_INFO, "MainTask %d #cTask_raiseTerminate(%d)", mainno, tskno);
			SVC_PERROR(cTask_raiseTerminate(tskno));
			break;
		case 'X':
			syslog(LOG_INFO, "MainTask %d #cTask_raiseTerminate(%d)", mainno, tskno);
			SVC_PERROR(cTask_raiseTerminate(tskno));
			break;
		case 'r':
			syslog(LOG_INFO, "MainTask %d #rotateReadyQueue(three priorities)", mainno);
			SVC_PERROR(multiRotateReadyQueue(SCHEDNO(prcid,rUserDomain1), HIGH_PRIORITY));
			SVC_PERROR(multiRotateReadyQueue(SCHEDNO(prcid,rUserDomain1), MID_PRIORITY));
			SVC_PERROR(multiRotateReadyQueue(SCHEDNO(prcid,rUserDomain1), LOW_PRIORITY));
			break;
		case 'c':
			syslog(LOG_INFO, "MainTask %d #cCyclic_start()", mainno);
			SVC_PERROR(cCyclic_start());
			break;
		case 'C':
			syslog(LOG_INFO, "MainTask %d #cCyclic_stop()", mainno);
			SVC_PERROR(cCyclic_stop());
			break;
		case 'b':
			syslog(LOG_INFO, "MainTask %d #cAlarm_start(5000000)", mainno);
			SVC_PERROR(cAlarm_start(5000000));
			break;
		case 'B':
			syslog(LOG_INFO, "MainTask %d #cAlarm_stop()", mainno);
			SVC_PERROR(cAlarm_stop());
			break;

		case 'V':
			hrtcnt1 = fetchHighResolutionTimer();
			consume_time(1000LU);
			hrtcnt2 = fetchHighResolutionTimer();
			syslog(LOG_NOTICE, "hrtcnt1 = %tu, hrtcnt2 = %tu",
											hrtcnt1, hrtcnt2);
			break;

		case 'v':
			SVC_PERROR(cSysLog_mask(LOG_UPTO(LOG_INFO),
										LOG_UPTO(LOG_EMERG)));
			break;
		case 'q':
			SVC_PERROR(cSysLog_mask(LOG_UPTO(LOG_NOTICE),
										LOG_UPTO(LOG_EMERG)));
			break;

		default:
			break;
		}
	} while (c != '\003' && c != 'Q');

	syslog(LOG_NOTICE, "Sample program ends.");
	SVC_PERROR(ciKernel_exitKernel());
	assert(0);
}
