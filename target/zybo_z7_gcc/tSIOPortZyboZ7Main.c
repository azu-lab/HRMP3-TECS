#include "tSIOPortZyboZ7Main_inline.h"

#if 0
#include "tXUartPs_rProcessor1Migratable_tecsgen.c"
/*
*　ポートオープン
*/
void eSIOPort_open(CELLIDX idx)
{
	CELLCB	*p_cellcb = GET_CELLCB(idx);

	cSIOPort_open();
}


/*
*　ポートクローズ
*/
void eSIOPort_close(CELLIDX idx)
{
	CELLCB	*p_cellcb = GET_CELLCB(idx);

	cSIOPort_close();
}

/*
*　1文字出力
*/
bool_t eSIOPort_putchar(CELLIDX idx, char c)
{
	CELLCB	*p_cellcb = GET_CELLCB(idx);

	return(cSIOPort_putChar(c));
}

/*
*	1文字読み取り
*/
int_t eSIOPort_getchar(CELLIDX idx)
{
	CELLCB	*p_cellcb = GET_CELLCB(idx);

	return(cSIOPort_getChar());
}

/*
*　コールバック有効
*/
void sSioPort_enableCBR(CELLIDX idx, uint_t cbrtn)
{
	CELLCB	*p_cellcb = GET_CELLCB(idx);

	cSIOPort_enableCBR(cbrtn);
}

/*
*コールバック無効
*/
void eSIOPort_disableCBR(CELLIDX idx, uint_t cbrtn)
{
	CELLCB	*p_cellcb = GET_CELLCB(idx);

	cSIOPort_disableCBR(cbrtn);
}

/*
*受信可能コールバック
*/
void ciSIOCBR_redayReceive(CELLIDX idx)
{
	CELLCB	*p_cellcb = GET_CELLCB(idx);

	if (is_ciSIOCBR_joined()) {
		ciSIOCBR_readyReceive();
	}
}

/*
*送信可能コールバック
*/
void ciSIOCBR_redaySend(CELLIDX idx)
{
	CELLCB	*p_cellcb = GET_CELLCB(idx);

	if (is_ciSIOCBR_joined()) {
		ciSIOCBR_readySend();
	}
}

/*
*割込みハンドラ
*/
void eiISR_main(CELLIDX idx)
{
	CELLCB	*p_cellcb = GET_CELLCB(idx);
}

/*
*初期化ルーチン
*/
void eInitialize_main(CELLIDX idx)
{
	CELLCB	*p_cellcb = GET_CELLCB(idx);
}
#endif