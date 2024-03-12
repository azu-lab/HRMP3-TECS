#include <kernel.h>
#include <t_syslog.h>
#include <t_stdlib.h>
#include "syssvc/serial.h"
#include "syssvc/syslog.h"
#include "kernel_cfg.h"
#include "exc01.h"

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

void task(intptr_t exinf)
{
	volatile ulong_t	i;
	int_t		n = 0;
	int_t		tskno = (int_t) exinf;
	const char	*graph[] = { "|", "  +", "|+", "    *" };
	char		c;
	ID prcid;

	while (1) {
		get_pid(&prcid);
		syslog(LOG_NOTICE, "task%d is running on %d (%03d). this is exc01.   %s",
										tskno, prcid, ++n, graph[tskno-1]);
		for (i = 0; i < task_loop*750; i++);
		c = message[tskno-1];
		message[tskno-1] = 0;
		switch (c) {
		case 'e':
			syslog(LOG_INFO, "#%d#ext_tsk()", tskno);
			SVC_PERROR(ext_tsk());
			assert(0);
		case 's':
			syslog(LOG_INFO, "#%d#slp_tsk()", tskno);
			SVC_PERROR(slp_tsk());
			break;
		case 'S':
			syslog(LOG_INFO, "#%d#tslp_tsk(10000)", tskno);
			SVC_PERROR(tslp_tsk(10000));
			break;
		case 'd':
			syslog(LOG_INFO, "#%d#dly_tsk(10000)", tskno);
			SVC_PERROR(dly_tsk(10000));
			break;
		default:
			break;
		}
	}
}

void cyclic_handler(intptr_t exinf)
{
      	syslog(LOG_NOTICE, "cyclic_handler start!");
	SVC_PERROR(irot_rdq(HIGH_PRIORITY));
	SVC_PERROR(irot_rdq(MID_PRIORITY));
	SVC_PERROR(irot_rdq(LOW_PRIORITY));
}

void alarm_handler(intptr_t exinf)
{
	SVC_PERROR(irot_rdq(HIGH_PRIORITY));
	SVC_PERROR(irot_rdq(MID_PRIORITY));
	SVC_PERROR(irot_rdq(LOW_PRIORITY));
}

void main_task(intptr_t exinf)
{
	char	c;
	ID		tskid = TASK1;
	int_t	tskno = 1;
	ER_UINT	ercd;
	PRI		tskpri;
#ifndef TASK_LOOP
	volatile ulong_t	i;
	SYSTIM	stime1, stime2;
#endif /* TASK_LOOP */

	SVC_PERROR(syslog_msk_log(LOG_UPTO(LOG_INFO), LOG_UPTO(LOG_EMERG)));
	syslog(LOG_NOTICE, "Sample program starts (exinf = %d).", (int_t) exinf);

	ercd = serial_opn_por(TASK_PORTID);
	if (ercd < 0 && MERCD(ercd) != E_OBJ) {
		syslog(LOG_ERROR, "%s (%d) reported by `serial_opn_por'.",
									itron_strerror(ercd), SERCD(ercd));
	}
	SVC_PERROR(serial_ctl_por(TASK_PORTID,
							(IOCTL_CRLF | IOCTL_FCSND | IOCTL_FCRCV)));

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

	SVC_PERROR(act_tsk(TASK1));
	SVC_PERROR(act_tsk(TASK2));
	SVC_PERROR(act_tsk(TASK3));

	do {
		SVC_PERROR(serial_rea_dat(TASK_PORTID, &c, 1));
		switch (c) {
		case 'e':
		case 's':
		case 'S':
		case 'd':
			message[tskno-1] = c;
			break;
		case '1':
			tskno = 1;
			tskid = TASK1;
			break;
		case '2':
			tskno = 2;
			tskid = TASK2;
			break;
		case '3':
			tskno = 3;
			tskid = TASK3;
			break;
		case '4':
			tskno = 4;
			tskid = TASK4;
			break;
		case 'a':
			syslog(LOG_INFO, "#act_tsk(%d)", tskno);
			SVC_PERROR(act_tsk(tskid));
			break;
		case 'A':
			syslog(LOG_INFO, "#can_act(%d)", tskno);
			SVC_PERROR(ercd = can_act(tskid));
			if (ercd >= 0) {
				syslog(LOG_NOTICE, "can_act(%d) returns %d", tskno, ercd);
			}
			break;
		case '>':
			syslog(LOG_INFO, "#chg_pri(%d, HIGH_PRIORITY)", tskno);
			SVC_PERROR(chg_pri(tskid, HIGH_PRIORITY));
			break;
		case '=':
			syslog(LOG_INFO, "#chg_pri(%d, MID_PRIORITY)", tskno);
			SVC_PERROR(chg_pri(tskid, MID_PRIORITY));
			break;
		case '<':
			syslog(LOG_INFO, "#chg_pri(%d, LOW_PRIORITY)", tskno);
			SVC_PERROR(chg_pri(tskid, LOW_PRIORITY));
			break;
		case 'G':
			syslog(LOG_INFO, "#get_pri(%d, &tskpri)", tskno);
			SVC_PERROR(ercd = get_pri(tskid, &tskpri));
			if (ercd >= 0) {
				syslog(LOG_NOTICE, "priority of task %d is %d", tskno, tskpri);
			}
			break;
		case 'w':
			syslog(LOG_INFO, "#wup_tsk(%d)", tskno);
			SVC_PERROR(wup_tsk(tskid));
			break;
		case 'W':
			syslog(LOG_INFO, "#can_wup(%d)", tskno);
			SVC_PERROR(ercd = can_wup(tskid));
			if (ercd >= 0) {
				syslog(LOG_NOTICE, "can_wup(%d) returns %d", tskno, ercd);
			}
			break;
		case 'l':
			syslog(LOG_INFO, "#rel_wai(%d)", tskno);
			SVC_PERROR(rel_wai(tskid));
			break;
		case 'u':
			syslog(LOG_INFO, "#sus_tsk(%d)", tskno);
			SVC_PERROR(sus_tsk(tskid));
			break;
		case 'm':
			syslog(LOG_INFO, "#rsm_tsk(%d)", tskno);
			SVC_PERROR(rsm_tsk(tskid));
			break;
		case 'r':
			syslog(LOG_INFO, "#rot_rdq(three priorities)");
			SVC_PERROR(rot_rdq(HIGH_PRIORITY));
			SVC_PERROR(rot_rdq(MID_PRIORITY));
			SVC_PERROR(rot_rdq(LOW_PRIORITY));
			break;
		case 'c':
			syslog(LOG_INFO, "#sta_cyc(1)");
			SVC_PERROR(sta_cyc(CYCHDR1));
			break;
		case 'C':
			syslog(LOG_INFO, "#stp_cyc(1)");
			SVC_PERROR(stp_cyc(CYCHDR1));
			break;
		case 'b':
			syslog(LOG_INFO, "#sta_alm(1, 5000)");
			SVC_PERROR(sta_alm(ALMHDR1, 5000));
			break;
		case 'B':
			syslog(LOG_INFO, "#stp_alm(1)");
			SVC_PERROR(stp_alm(ALMHDR1));
			break;
		default:
			break;
		}
	} while (c != '\003' && c != 'Q');

	syslog(LOG_NOTICE, "Sample program ends.");
	SVC_PERROR(ext_ker());
	assert(0);
}
