#include "tIPISend_tecsgen.h"

/*
 *  ディスパッチ要求プロセッサ間割込みの発行
 */
void
eDispatch_dispatchPrc(CELLIDX idx){
	CELLCB	*p_cellcb = GET_CELLCB(idx);
	// return(request_dispatch_prc(prcid));
	request_dispatch_prc(ATTR_prcid);
}

/*
 *  カーネル終了要求プロセッサ間割込みの発行
 */
void
eExitKernel_extKernel(CELLIDX idx){
	CELLCB	*p_cellcb = GET_CELLCB(idx);
	// return(request_ext_ker(prcid));
	request_ext_ker(ATTR_prcid);
}

/*
 *  高分解能タイマ設定要求プロセッサ間割込みの発行
 */
void
eHrtEvent_setHrtEvent(CELLIDX idx){
	CELLCB	*p_cellcb = GET_CELLCB(idx);
	// return(request_set_hrt_event(prcid));
	request_set_hrt_event(ATTR_prcid);
}	

/*
 *  システム周期開始要求プロセッサ間割込みの発行
 */
void
eScyc_startScycEvent(CELLIDX idx);{
	CELLCB	*p_cellcb = GET_CELLCB(idx);
	// return(request_start_scyc_event(*p_my_pcb));
	request_start_scyc_event(ATTR_p_my_pcb);
}
