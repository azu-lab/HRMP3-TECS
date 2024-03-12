/*
 *  TOPPERS Software
 *      Toyohashi Open Platform for Embedded Real-Time Systems
 * 
 *  Copyright (C) 2000-2003 by Embedded and Real-Time Systems Laboratory
 *                              Toyohashi Univ. of Technology, JAPAN
 *  Copyright (C) 2006-2018 by Embedded and Real-Time Systems Laboratory
 *              Graduate School of Information Science, Nagoya Univ., JAPAN
 *  Copyright (C) 2019 by TECS WG, TOPPERS Project
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
 *  $Id:  $
 */

/*
 *		ARM PrimCell UART 用 簡易SIOドライバ
 */

#include <sil.h>
#include "tXUartPs_tecsgen.h"
#include "tXUartPs.h"
#include "target_syssvc.h"
// #include "xuartps.h"  xxx

/*
 *  プリミティブな送信／受信関数
 */

/*
 *  受信バッファに文字があるか？
 */
Inline bool_t
xuartps_getready(CELLCB *p_cellcb)
{
    return((sil_rew_mem(XUARTPS_SR(ATTR_baseAddress)) & XUARTPS_SR_RXEMPTY) == 0U);
    //return((sil_rew_mem(UART_FR(ATTR_baseAddress)) & UART_FR_RXFE) == 0U);  xxx
}

/*
 *  送信バッファに空きがあるか？
 */
Inline bool_t
xuartps_putready(CELLCB *p_cellcb)
{
    return((sil_rew_mem(XUARTPS_SR(ATTR_baseAddress)) & XUARTPS_SR_TXFULL) == 0U);
    //return((sil_rew_mem(UART_FR(ATTR_baseAddress)) & UART_FR_TXFF) == 0U);  xxx
}

/*
 *  受信した文字の取出し
 */
Inline char
xuartps_getchar(CELLCB *p_cellcb)
{
    return((char) sil_rew_mem(XUARTPS_FIFO(ATTR_baseAddress)));
    // return((char) sil_rew_mem(UART_DR(ATTR_baseAddress)));  xxx
}

/*
 *  送信する文字の書込み
 */
Inline void
xuartps_putchar(CELLCB *p_cellcb, char c)
{
    sil_wrw_mem(XUARTPS_FIFO(ATTR_baseAddress), (uint32_t) c);
    // sil_wrw_mem(UART_DR(ATTR_baseAddress), (uint32_t) c);  xxx
}

/*
 *  送信割込みイネーブル
 */
Inline void
xuartps_enable_send(CELLCB *p_cellcb)  // (uintptr_t base)  xxx
{
    sil_wrw_mem(XUARTPS_IER(ATTR_baseAddress), XUARTPS_IXR_TXEMPTY);
    // sil_wrw_mem(XUARTPS_IER(base), XUARTPS_IXR_TXEMPTY);  xxx
}

/*
 *  送信割込みディスエーブル
 */
Inline void
xuartps_disable_send(CELLCB *p_cellcb)  // (uintptr_t base)  xxx
{
    sil_wrw_mem(XUARTPS_IDR(ATTR_baseAddress), XUARTPS_IXR_TXEMPTY);
    // sil_wrw_mem(XUARTPS_IDR(base), XUARTPS_IXR_TXEMPTY);  xxx
}

/*
 *  受信割込みイネーブル
 */
Inline void
xuartps_enable_receive(CELLCB *p_cellcb)  // (uintptr_t base)  xxx
{
    sil_wrw_mem(XUARTPS_IER(ATTR_baseAddress), XUARTPS_IXR_RXTRIG);
    // sil_wrw_mem(XUARTPS_IER(base), XUARTPS_IXR_RXTRIG);  xxx
}

/*
 *  受信割込みディスエーブル
 */
Inline void
xuartps_disable_receive(CELLCB *p_cellcb)  // (uintptr_t base)  xxx
{
    sil_wrw_mem(XUARTPS_IDR(ATTR_baseAddress), XUARTPS_IXR_RXTRIG);
    // sil_wrw_mem(XUARTPS_IDR(base), XUARTPS_IXR_RXTRIG);  xxx
}


#if 0 // xxx  この初期化内容は、TECS で行われるので不要
/*
 *  SIOドライバの初期化
 */
void
xuartps_initialize(void)
{
	SIOPCB	*p_siopcb;
	uint_t	i;

	/*
	 *  SIOポート管理ブロックの初期化
	 */
	for (p_siopcb = siopcb_table, i = 0; i < TNUM_SIOP; p_siopcb++, i++) {
		p_siopcb->p_siopinib = &(siopinib_table[i]);
		p_siopcb->opened = false;
	}
}
#endif // 0 // xxx

/*
 *  SIOドライバの終了処理
 */
void
eTerminate_main(CELLIDX idx)
{
#if 1 /* xxx */
    CELLCB	*p_cellcb = GET_CELLCB(idx);
    if (VAR_b_opened) {
        /*
         *  送信FIFOが空になるまで待つ
         */
        while ((sil_rew_mem(XUARTPS_SR(ATTR_baseAddress))
                & XUARTPS_SR_TXEMPTY) == 0U);

        /*
         *  オープンされているSIOポートのクローズ
         */
        eSIOPort_close(idx);
    }
#else /* xxx */
    uint_t	i;
    SIOPCB	*p_siopcb;

    for (i = 0; i < TNUM_SIOP; i++) {
        p_siopcb = &(siopcb_table[i]);
        if (p_siopcb->opened) {
            /*
             *  送信FIFOが空になるまで待つ
             */
            while ((sil_rew_mem(XUARTPS_SR(p_siopcb->p_siopinib->base))
                    & XUARTPS_SR_TXEMPTY) == 0U);

            /*
             *  オープンされているSIOポートのクローズ
             */
            xuartps_cls_por(&(siopcb_table[i]));
        }
    }
#endif /* xxx */
}

/*
 *  シリアルI/Oポートのオープン
 */
void
eSIOPort_open(CELLIDX idx)
{
    CELLCB	*p_cellcb = GET_CELLCB(idx);

    if( ! VAR_b_opened ){
#if 1 /* xxx */
        uintptr_t	base = ATTR_baseAddress;

        /*
         *  全割込みをディスエーブル
         */
        sil_wrw_mem(XUARTPS_IDR(base), XUARTPS_IXR_ALL);
		
        /*
         *  ペンディングしている割込みをクリア
         */
        sil_wrw_mem(XUARTPS_ISR(base), sil_rew_mem(XUARTPS_ISR(base)));

        /*
         *  送受信のリセットとディスエーブル
         */
        sil_wrw_mem(XUARTPS_CR(base),
                    XUARTPS_CR_TXRST | XUARTPS_CR_RXRST
                    | XUARTPS_CR_TX_DIS | XUARTPS_CR_RX_DIS);

        /*
         *  ボーレートの設定
         */
        sil_wrw_mem(XUARTPS_BAUDGEN(base), ATTR_baudgen);
        sil_wrw_mem(XUARTPS_BAUDDIV(base), ATTR_bauddiv);

        /*
         *  データ長，ストップビット，パリティの設定
         */
        sil_wrw_mem(XUARTPS_MR(base), ATTR_mode);

        /*
         *  受信トリガを1バイトに設定
         */
        sil_wrw_mem(XUARTPS_RXWM(base), 1U);

        /*
         *  タイムアウトを設定
         */
        sil_wrw_mem(XUARTPS_RXTOUT(base), 10U);

        /*
         *  送受信のイネーブル
         */
        sil_wrw_mem(XUARTPS_CR(base),
                    XUARTPS_CR_TX_EN | XUARTPS_CR_RX_EN | XUARTPS_CR_STOPBRK);
#else /* xxx */
        /*
         *  UARTをディスエーブル
         */
        sil_wrw_mem(UART_CR(ATTR_baseAddress), 0U);

        /*
         *  エラーフラグをクリア
         */
        sil_wrw_mem(UART_ECR(ATTR_baseAddress), 0U);

        /*
         *  FIFOを空にする
         */
        while (xuartps_getready(p_cellcb)) {
            (void) xuartps_getchar(p_cellcb);
        }

        /*
         *  ボーレートと通信規格を設定
         */
        sil_wrw_mem(UART_IBRD(ATTR_baseAddress), ATTR_ibrd);
        sil_wrw_mem(UART_FBRD(ATTR_baseAddress), ATTR_fbrd);
        sil_wrw_mem(UART_LCR_H(ATTR_baseAddress), ATTR_lcr_h);
		
        /*
         *  UARTをイネーブル
         */
        sil_wrw_mem(UART_CR(ATTR_baseAddress),
                    UART_CR_UARTEN|UART_CR_TXE|UART_CR_RXE);
#endif /* xxx */ 
        VAR_b_opened = true;
    }
}

/*
 *  シリアルI/Oポートのクローズ
 */
void
eSIOPort_close(CELLIDX idx)
{
    CELLCB	*p_cellcb = GET_CELLCB(idx);

#if 1 /* xxx */
    if (VAR_b_opened) {
        /*
         *  送受信のディスエーブル
         */
        sil_wrw_mem(XUARTPS_CR(ATTR_baseAddress),
                    XUARTPS_CR_TX_DIS | XUARTPS_CR_RX_DIS | XUARTPS_CR_STOPBRK);

        /*
         *  全割込みをディスエーブル
         */
        sil_wrw_mem(XUARTPS_IDR(ATTR_baseAddress), XUARTPS_IXR_ALL);

        VAR_b_opened = false;
    }
#else /* xxx */
    if (p_siopcb->opened) {
        /*
         *  送受信のディスエーブル
         */
        sil_wrw_mem(XUARTPS_CR(p_siopcb->p_siopinib->base),
                    XUARTPS_CR_TX_DIS | XUARTPS_CR_RX_DIS | XUARTPS_CR_STOPBRK);

        /*
         *  全割込みをディスエーブル
         */
        sil_wrw_mem(XUARTPS_IDR(p_siopcb->p_siopinib->base), XUARTPS_IXR_ALL);

        p_siopcb->opened = false;
    }
#endif /* xxx */
}

/*
 *  シリアルI/Oポートへの文字送信
 */
bool_t
eSIOPort_putChar(CELLIDX idx, char c)
{
	CELLCB	*p_cellcb = GET_CELLCB(idx);

 	if (xuartps_putready(p_cellcb)){
		xuartps_putchar(p_cellcb, c);
		return(true);
	}
		return(false);
}

/*
 *  シリアルI/Oポートからの文字受信
 */
int_t
eSIOPort_getChar(CELLIDX idx)
{
	CELLCB	*p_cellcb = GET_CELLCB(idx);

	if (xuartps_getready(p_cellcb)) {
		return((int_t)(uint8_t) xuartps_getchar(p_cellcb));
	}
	return(-1);
}

/*
 *  シリアルI/Oポートからのコールバックの許可
 */
void
eSIOPort_enableCBR(CELLIDX idx, uint_t cbrtn)
{
    CELLCB		*p_cellcb = GET_CELLCB(idx);
#if 1 /* xxx */
    switch (cbrtn) {
    case SIO_RDY_SND:
        xuartps_enable_send(p_cellcb);
        break;
    case SIO_RDY_RCV:
        xuartps_enable_receive(p_cellcb);
        break;
    }
    
#else /* xxx */
    uint32_t	reg;

    reg = sil_rew_mem(UART_IMSC(ATTR_baseAddress));
    switch (cbrtn) {
    case SIOSendReady:
        reg |= UART_IMSC_TXIM;
        break;
    case SIOReceiveReady:
        reg |= UART_IMSC_RXIM;
        break;
    }
    sil_wrw_mem(UART_IMSC(ATTR_baseAddress), reg);
#endif /* xxx */
}

/*
 *  シリアルI/Oポートからのコールバックの禁止
 */
void
eSIOPort_disableCBR(CELLIDX idx, uint_t cbrtn)
{
    CELLCB *p_cellcb = GET_CELLCB( idx );
#if 1 /* xxx */
    switch (cbrtn) {
    case SIO_RDY_SND:
        xuartps_disable_send(p_cellcb);
        break;
    case SIO_RDY_RCV:
        xuartps_disable_receive(p_cellcb);
		break;
    }
#else /* xxx */
    CELLCB		*p_cellcb = GET_CELLCB(idx);
    uint32_t	reg;

    reg = sil_rew_mem(UART_IMSC(ATTR_baseAddress));
    switch (cbrtn) {
    case SIOSendReady:
        reg &= ~UART_IMSC_TXIM;
        break;
    case SIOReceiveReady:
        reg &= ~UART_IMSC_RXIM;
        break;
    }
    sil_wrw_mem(UART_IMSC(ATTR_baseAddress), reg);
#endif /* xxx */
}

/*
 *  シリアルI/Oポートに対する割込み処理
 */
void
eiISR_main(CELLIDX idx)
{
    CELLCB	*p_cellcb = GET_CELLCB(idx);

    if (xuartps_getready(p_cellcb)) {
        /*
         *  受信通知コールバックルーチンを呼び出す．
         */
        ciSIOCBR_readyReceive();
    }
    if (xuartps_putready(p_cellcb)) {
        /*
         *  送信可能コールバックルーチンを呼び出す．
         */
        ciSIOCBR_readySend();
    }

    /*
     *  ペンディングしている割込みをクリア
     *   xxx これってこれでよい？
     *       割込みを取りこぼすことはないのか。
     */
    sil_wrw_mem(XUARTPS_ISR(ATTR_baseAddress),
                sil_rew_mem(XUARTPS_ISR(ATTR_baseAddress)));
}
