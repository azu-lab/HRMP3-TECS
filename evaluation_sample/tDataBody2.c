/*
 * This file was automatically generated by tecsgen.
 * Move and rename like below before editing,
 *   gen/tDataBody2_templ.c => src/tDataBody2.c
 * to avoid to be overwritten by tecsgen.
 */
/* #[<PREAMBLE>]#
 * Don't edit the comments between #[<...>]# and #[</...>]#
 * These comment are used by tecsmerege when merging.
 *
 * call port function #_TCPF_#
 * call port: cLightBody signature: sLight context:task
 *   void           cLightBody_main( );
 * call port: cTask signature: sTask context:task
 *   ER             cTask_activate( );
 *   ER_UINT        cTask_cancelActivate( );
 *   ER             cTask_getTaskState( STAT* p_tskstat );
 *   ER             cTask_changePriority( PRI priority );
 *   ER             cTask_getPriority( PRI* p_priority );
 *   ER             cTask_refer( T_RTSK* pk_taskStatus );
 *   ER             cTask_wakeup( );
 *   ER_UINT        cTask_cancelWakeup( );
 *   ER             cTask_releaseWait( );
 *   ER             cTask_suspend( );
 *   ER             cTask_resume( );
 *   ER             cTask_raiseTerminate( );
 *   ER             cTask_terminate( );
 *   ER             cTask_migrateAndActivate( ID prcid );
 *   ER             cTask_migrate( ID prcid );
 *
 * #[</PREAMBLE>]# */

/* Put prototype declaration and/or variale definition here #_PAC_# */
#include "tDataBody2_tecsgen.h"

#ifndef E_OK
#define	E_OK	0		/* success */
#define	E_ID	(-18)	/* illegal ID */
#endif

/* entry port function #_TEPF_# */
/* #[<ENTRY_PORT>]# eDataBody
 * entry port: eDataBody
 * signature:  sData
 * context:    task
 * #[</ENTRY_PORT>]# */

/* #[<ENTRY_FUNC>]# eDataBody_main
 * name:         eDataBody_main
 * global_name:  tDataBody2_eDataBody_main
 * oneway:       false
 * #[</ENTRY_FUNC>]# */
void
eDataBody_main(CELLIDX idx)
{
	CELLCB	*p_cellcb;
	if (VALID_IDX(idx)) {
		p_cellcb = GET_CELLCB(idx);
	}
	else {
		/* Write error processing code here */
	} /* end if VALID_IDX(idx) */

	/* Put statements here #_TEFB_# */
	syslog(LOG_NOTICE, "Data2 is running!");
	cTask_activate();
	// cTask_terminate();
	cLightBody_main();
}

/* #[<ENTRY_PORT>]# eDataTask
 * entry port: eDataTask
 * signature:  sTaskBody
 * context:    task
 * #[</ENTRY_PORT>]# */

/* #[<ENTRY_FUNC>]# eDataTask_main
 * name:         eDataTask_main
 * global_name:  tDataBody2_eDataTask_main
 * oneway:       false
 * #[</ENTRY_FUNC>]# */
void
eDataTask_main(CELLIDX idx)
{
	CELLCB	*p_cellcb;
	if (VALID_IDX(idx)) {
		p_cellcb = GET_CELLCB(idx);
	}
	else {
		/* Write error processing code here */
	} /* end if VALID_IDX(idx) */

	/* Put statements here #_TEFB_# */
	syslog(LOG_NOTICE, "DataTask2 is running!");
	dly_tsk(4000);

}

/* #[<POSTAMBLE>]#
 *   Put non-entry functions below.
 * #[</POSTAMBLE>]#*/
