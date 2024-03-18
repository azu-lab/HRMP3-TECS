#include <kernel.h>
#include <t_syslog.h>
#include <t_stdlib.h>
#include "syssvc/serial.h"
#include "syssvc/syslog.h"
#include "kernel_cfg.h"
#include "test_sample.h"

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
#define SVC_PERROR(expr)    svc_perror(__FILE__, __LINE__, #expr, (expr))

// #define count 44642000

void busy_wait_ms(int_t ms){
    volatile int_t i, j;
    
    for (i = 0; i < ms; ++i)
    {
        // for (j = 0; j < count; ++j)
        for (j = 0; j < 7442000; ++j)
        {
            /* countを1msになるように */
        }
    }
}

void motor_main(void) {
    SVC_PERROR(act_tsk(Motor));
}

void motor_task(intptr_t exinf) {
    // while (1) {
    //     busy_wait_ms(1); // 1ms待機
    //     syslog(LOG_NOTICE, "Real-time task is running on core 1");
    // }
    syslog(LOG_NOTICE, "Motor is running!");
    // busy_wait_ms(4);
    dly_tsk(4000);
    ext_tsk();
}

void light_main(void){
    SVC_PERROR(act_tsk(Light));
}

void light_task(intptr_t exinf) {
    // while (1) {
    //     busy_wait_ms(1); // 1ms待機
    //     syslog(LOG_NOTICE, "Real-time task is running on core 1");
    // }
    syslog(LOG_NOTICE, "Light is running!");
    // busy_wait_ms(4);
    dly_tsk(4000);
    ext_tsk();
}

void data1_main(void){
    // syslog(LOG_NOTICE, "Data1 is running!");
    SVC_PERROR(act_tsk(Data1));
    motor_main();
}

void data1_task(intptr_t exinf) {
    // while (1) {
    //     busy_wait_ms(5); // 4ms待機
    //     syslog(LOG_NOTICE, "Non-real-time task is running on core 2");
    // }
    // syslog(LOG_NOTICE, "DataTask is running!");
    // busy_wait_ms(4);
    dly_tsk(4000);
    ext_tsk();
}

void data2_main(void){
    // syslog(LOG_NOTICE, "Data2 is running!");
    SVC_PERROR(act_tsk(Data2));
    light_main();
}

void data2_task(intptr_t exinf) {
    // while (1) {
    //     busy_wait_ms(5); // 4ms待機
    //     syslog(LOG_NOTICE, "Non-real-time task is running on core 2");
    // }
    // syslog(LOG_NOTICE, "DataTask is running!");
    // busy_wait_ms(4);
    dly_tsk(4000);
    ext_tsk();
}

void main_task(intptr_t exinf) {

    ER_UINT ercd;
    #ifndef TASK_LOOP
        int_t N = 1000000;
        volatile ulong_t    i;
        SYSTIM  start, fin, time1, start_o, end_o, overhead;
    #endif /* TASK_LOOP */

    SVC_PERROR(syslog_msk_log(LOG_UPTO(LOG_INFO), LOG_UPTO(LOG_EMERG)));
    // syslog(LOG_NOTICE, "Sample program starts (exinf = %d).", (int_t) exinf);


    get_tim(&start_o);
    for(i=0; i<N; i++)
        get_tim(&end_o);
    overhead = (end_o - start_o)/N;

    // SYSTIM      stime1, stime2;
    // ulong_t count;

    // count = LOOP_REF;
    // SVC_PERROR(get_tim(&stime1));
    // for (i = 0; i < count; i++);
    // SVC_PERROR(get_tim(&stime2));
    // count = LOOP_REF * 100UL / (ulong_t)(stime2 - stime1)* 1000LU;
    // syslog(LOG_NOTICE, "count = %d\n", count);

    /*
     *  シリアルポートの初期化
     *
     *  システムログタスクと同じシリアルポートを使う場合など，シリアル
     *  ポートがオープン済みの場合にはここでE_OBJエラーになるが，支障は
     *  ない．
     */
    ercd = serial_opn_por(TASK_PORTID);
    if (ercd < 0 && MERCD(ercd) != E_OBJ) {
        syslog(LOG_ERROR, "%s (%d) reported by `serial_opn_por'.",
                                    itron_strerror(ercd), SERCD(ercd));
    }
    SVC_PERROR(serial_ctl_por(TASK_PORTID,
                            (IOCTL_CRLF | IOCTL_FCSND | IOCTL_FCRCV)));
    
    for(int_t i = 0; i < 10; i++){
        get_tim(&start);
        data1_main();
        // busy_wait_ms(4);
        data2_main();
        // busy_wait_ms(4);
        get_tim(&fin);
        time1 = fin - start - overhead;
        syslog(LOG_INFO,"%d",time1);
        // busy_wait_ms(15);
        dly_tsk(100000);
    }

    // SVC_PERROR(get_tim(&start));

    // SVC_PERROR(get_tim(&fin));
    // time1 = fin - start - overhead;
    // syslog(LOG_INFO,"%d",time1);

    
    // while (1) {
    //     // 何もしない
    // }



    syslog(LOG_NOTICE, "Test program ends.");
    SVC_PERROR(ext_ker());
    assert(0);
}
