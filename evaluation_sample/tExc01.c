#include "tExc01_tecsgen.h"
#include <kernel.h>
#include <t_syslog.h>
#include <t_stdlib.h>
#include "kernel_cfg.h"
#include "tExc01.h"

Inline void
svc_perror(const char *file, int_t line, const char *expr, ER ercd)
{
	if (ercd < 0) {
		t_perror(LOG_ERROR, file, line, expr, ercd);
	}
}

#define	SVC_PERROR(expr)	svc_perror(__FILE__, __LINE__, #expr, (expr))

char	message[3];

ulong_t	task_loop;		/* タスク内でのループ回数 */

void eSampleTask_main(intptr_t exinf)
{
	volatile ulong_t	i;
	int_t		n = 0;
	int_t		tskno = (int_t) exinf +1;
	const char	*graph[] = { "|", "  +", "    *" };
	char		c;
	ID prcid;

	while (1) {
		getProcessorID(&prcid);
		syslog(LOG_NOTICE, "task%d is running on %d (%03d).   %s",
										tskno, prcid, ++n, graph[tskno-1]);
		for (i = 0; i < task_loop*750; i++);
		c = message[tskno-1];
		message[tskno-1] = 0;
		switch (c) {
		case 'e':
			syslog(LOG_INFO, "#%d#ext_tsk()", tskno);
			SVC_PERROR(exit());
			assert(0);
		case 's':
			syslog(LOG_INFO, "#%d#slp_tsk()", tskno);
			SVC_PERROR(sleep());
			break;
		case 'S':
			syslog(LOG_INFO, "#%d#tslp_tsk(10000)", tskno);
			SVC_PERROR(sleepTimeout(10000));
			break;
		case 'd':
			syslog(LOG_INFO, "#%d#dly_tsk(10000)", tskno);
			SVC_PERROR(delay(10000));
			break;
		default:
			break;
		}
	}
}

void eiCyclicHandler_main()
{
    syslog(LOG_NOTICE, "cyclic_handler start!");
	SVC_PERROR(ciKernel_rotateReadyQueue(HIGH_PRIORITY));
	SVC_PERROR(ciKernel_rotateReadyQueue(MID_PRIORITY));
	SVC_PERROR(ciKernel_rotateReadyQueue(LOW_PRIORITY));
}

void eiAlarmHandler_main()
{
	SVC_PERROR(ciKernel_rotateReadyQueue(HIGH_PRIORITY));
	SVC_PERROR(ciKernel_rotateReadyQueue(MID_PRIORITY));
	SVC_PERROR(ciKernel_rotateReadyQueue(LOW_PRIORITY));
}

void eMainTask_main(intptr_t exinf)
{
	char	c;
	int_t	tskno = 1;
	ER_UINT	ercd;
	PRI		tskpri;
#ifndef TASK_LOOP
	volatile ulong_t	i;
	SYSTIM	stime1, stime2;
#endif /* TASK_LOOP */

	SVC_PERROR(cSysLog_mask(LOG_UPTO(LOG_INFO), LOG_UPTO(LOG_EMERG)));
	syslog(LOG_NOTICE, "Sample program starts (exinf = %d). this is tExc01.", (int_t) exinf);

	ercd = cSerialPort_open();
	if (ercd < 0 && MERCD(ercd) != E_OBJ) {
		syslog(LOG_ERROR, "%s (%d) reported by `serial_opn_por'.",
									itron_strerror(ercd), SERCD(ercd));
	}
	SVC_PERROR(cSerialPort_control(IOCTL_CRLF | IOCTL_FCSND | IOCTL_FCRCV));

#ifdef TASK_LOOP
	task_loop = TASK_LOOP;
#else /* TASK_LOOP */

#ifdef MEASURE_TWICE
	task_loop = LOOP_REF;
	SVC_PERROR(get_tim(&stime1));
	for (i = 0; i < task_loop; i++);
	SVC_PERROR(get_tim(&stime2));
#endif /* MEASURE_TWICE */

	task_loop = LOOP_REF;
	SVC_PERROR(get_tim(&stime1));
	for (i = 0; i < task_loop; i++);
	SVC_PERROR(get_tim(&stime2));
	task_loop = LOOP_REF * 400UL / (stime2 - stime1);

#endif /* TASK_LOOP */

	SVC_PERROR(cTask_activate( 1 ));
	SVC_PERROR(cTask_activate( 2 ));
	SVC_PERROR(cTask_activate( 3 ));
	SVC_PERROR(cTask_activate( 4 ));

	do {
		SVC_PERROR(cSerialPort_read(&c, 1));
		switch (c) {
		case 'e':
		case 's':
		case 'S':
		case 'd':
			message[tskno-1] = c;
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
		case '4':
			tskno = 4;
			break;
		case 'a':
			syslog(LOG_INFO, "#act_tsk(%d)", tskno);
			SVC_PERROR(cTask_activate(tskno));
			break;
		case 'A':
			syslog(LOG_INFO, "#can_act(%d)", tskno);
			SVC_PERROR(cTask_cancelActivate(tskno));			
			if (ercd >= 0) {
				syslog(LOG_NOTICE, "can_act(%d) returns %d", tskno, ercd);
			}
			break;
		case '>':
			syslog(LOG_INFO, "#chg_pri(%d, HIGH_PRIORITY)", tskno);
			SVC_PERROR(cTask_changePriority(tskno, HIGH_PRIORITY));
			break;
		case '=':
			syslog(LOG_INFO, "#chg_pri(%d, MID_PRIORITY)", tskno);
			SVC_PERROR(cTask_changePriority(tskno, MID_PRIORITY));
			break;
		case '<':
			syslog(LOG_INFO, "#chg_pri(%d, LOW_PRIORITY)", tskno);
			SVC_PERROR(cTask_changePriority(tskno, LOW_PRIORITY));
			break;
		case 'G':
			syslog(LOG_INFO, "#get_pri(%d, &tskpri)", tskno);
			SVC_PERROR(ercd = cTask_getPriority(tskno, &tskpri));
			if (ercd >= 0) {
				syslog(LOG_NOTICE, "priority of task %d is %d", tskno, tskpri);
			}
			break;
		case 'w':
			syslog(LOG_INFO, "#wup_tsk(%d)", tskno);
			SVC_PERROR(cTask_wakeup(tskno));
			break;
		case 'W':
			syslog(LOG_INFO, "#can_wup(%d)", tskno);
			SVC_PERROR(ercd = cTask_cancelWakeup(tskno));
			if (ercd >= 0) {
				syslog(LOG_NOTICE, "can_wup(%d) returns %d", tskno, ercd);
			}
			break;
		case 'l':
			syslog(LOG_INFO, "#rel_wai(%d)", tskno);
			SVC_PERROR(cTask_releaseWait(tskno));
			break;
		case 'u':
			syslog(LOG_INFO, "#sus_tsk(%d)", tskno);
			SVC_PERROR(cTask_suspend(tskno));
			break;
		case 'm':
			syslog(LOG_INFO, "#rsm_tsk(%d)", tskno);
			SVC_PERROR(cTask_resume(tskno));
			break;
		case 'r':
			syslog(LOG_INFO, "#rot_rdq(three priorities)");
			SVC_PERROR(rotateReadyQueue(HIGH_PRIORITY));
			SVC_PERROR(rotateReadyQueue(MID_PRIORITY));
			SVC_PERROR(rotateReadyQueue(LOW_PRIORITY));
			break;
		case 'c':
			syslog(LOG_INFO, "#sta_cyc(1)");
			SVC_PERROR(cCyclic_start());
			break;
		case 'C':
			syslog(LOG_INFO, "#stp_cyc(1)");
			SVC_PERROR(cCyclic_stop());
			break;
		case 'b':
			syslog(LOG_INFO, "#sta_alm(1, 5000)");
			SVC_PERROR(cAlarm_start(5000));
			break;
		case 'B':
			syslog(LOG_INFO, "#stp_alm(1)");
			SVC_PERROR(cAlarm_stop());
			break;
		default:
			break;
		}
	} while (c != '\003' && c != 'Q');

	syslog(LOG_NOTICE, "Sample program ends.");
	SVC_PERROR(ciKernel_exitKernel());
	assert(0);
}
