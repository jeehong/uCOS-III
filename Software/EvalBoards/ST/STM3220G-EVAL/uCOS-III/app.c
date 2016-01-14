/*
*********************************************************************************************************
*                                              EXAMPLE CODE
*
*                          (c) Copyright 2003-2012; Micrium, Inc.; Weston, FL
*
*               All rights reserved.  Protected by international copyright laws.
*               Knowledge of the source code may NOT be used to develop a similar product.
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                            EXAMPLE CODE
*
*                                     ST Microelectronics STM32
*                                              on the
*
*                                           STM3220G-EVAL
*                                         Evaluation Board
*
* Filename      : app.c
* Version       : V1.00
* Programmer(s) : EHS
*                 DC
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#include  <includes.h>

/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                       LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/

/* ----------------- APPLICATION GLOBALS ------------------ */
static  OS_TCB   AppTaskStartTCB;
static  CPU_STK  AppTaskStartStk[APP_TASK_START_STK_SIZE];

static  OS_TCB   AppTaskLed0TCB;
static  CPU_STK  AppTaskLed0Stk[APP_TASK_LED0_STK_SIZE];

static  OS_TCB   AppTaskLed1TCB;
static  CPU_STK  AppTaskLed1Stk[APP_TASK_LED1_STK_SIZE];

static  OS_TCB   AppTaskLed2TCB;
static  CPU_STK  AppTaskLed2Stk[APP_TASK_LED2_STK_SIZE];

static  OS_TCB   AppTaskLed3TCB;
static  CPU_STK  AppTaskLed3Stk[APP_TASK_LED3_STK_SIZE];

static  OS_TCB   AppTaskLogTCB;
static  CPU_STK  AppTaskLogStk[APP_TASK_LOG_STK_SIZE];

/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/

static  void  AppTaskStart          (void             *p_arg);

static  void  AppTaskCreate         (void);
static  void  AppObjCreate          (void);

static 	void 	AppTaskLed0						(void);
static 	void 	AppTaskLed1						(void);
static 	void 	AppTaskLed2						(void);
static 	void 	AppTaskLed3						(void);
static 	void 	AppTaskLog						(void);

/*
*********************************************************************************************************
*                                                main()
*
* Description : This is the standard entry point for C code.  It is assumed that your code will call
*               main() once you have performed all necessary initialization.
*
* Arguments   : none
*
* Returns     : none
*********************************************************************************************************
*/

int main(void)
{
    OS_ERR  err;

    BSP_IntDisAll();                                            /* Disable all interrupts.                                  */

    CPU_Init();                                                 /* Initialize uC/CPU services.                              */

    OSInit(&err);                                               /* Init OS.                                                 */

    OSTaskCreate((OS_TCB     *)&AppTaskStartTCB,                /* Create the start task                                    */
                 (CPU_CHAR   *)"App Task Start",
                 (OS_TASK_PTR ) AppTaskStart,
                 (void       *) 0,
                 (OS_PRIO     ) APP_TASK_START_PRIO,
                 (CPU_STK    *)&AppTaskStartStk[0],
                 (CPU_STK     )(APP_TASK_START_STK_SIZE / 10u),
                 (CPU_STK_SIZE) APP_TASK_START_STK_SIZE,
                 (OS_MSG_QTY  ) 0,
                 (OS_TICK     ) 0,
                 (void       *) 0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR     *)&err);

    OSStart(&err);

    while (DEF_ON) {                                            /* Should Never Get Here                                    */
    };
}


/*
*********************************************************************************************************
*                                          STARTUP TASK
*
* Description : This is an example of a startup task.  As mentioned in the book's text, you MUST
*               initialize the ticker only once multitasking has started.
*
* Arguments   : p_arg   is the argument passed to 'AppTaskStart()' by 'OSTaskCreate()'.
*
* Returns     : none
*
* Notes       : 1) The first line of code is used to prevent a compiler warning because 'p_arg' is not
*                  used.  The compiler should not generate any code for this statement.
*********************************************************************************************************
*/

static  void  AppTaskStart (void *p_arg)
{
    OS_ERR      os_err;
		

   (void)p_arg;

    BSP_Init();                                                 /* Init BSP fncts.                                          */

    CPU_Init();                                                 /* Init CPU name & int. dis. time measuring fncts.          */

    Mem_Init();                                                 /* Initialize Memory managment                              */

    BSP_CPU_TickInit();                                         /* Start Tick Initialization                                */

#if (OS_TASK_STAT_EN > 0)
    OSStatInit();                                               /* Determine CPU capacity                                   */
#endif

#if (APP_CFG_SERIAL_EN == DEF_ENABLED)
    BSP_Ser_Init(115200);                                       /* Initialize Serial Interface                              */
#endif

		APP_TRACE_INFO(("********* \n\r"));
    APP_TRACE_INFO(("Creating Application Objects... \n\r"));
		//AppObjCreate();                                             /* Create Application Kernel Objects                        */

    APP_TRACE_INFO(("Creating Application Tasks... \n\r"));
    AppTaskCreate();                                            /* Create Application Tasks                                 */

    while (DEF_TRUE) 
		{                                          /* Task body, always written as an infinite loop.           */
        OSTimeDlyHMSM(0, 1, 0, 0,
                      OS_OPT_TIME_HMSM_STRICT, &os_err);
		}
}


/*
*********************************************************************************************************
*                                          AppTaskCreate()
*
* Description : Create application tasks.
*
* Argument(s) : none
*
* Return(s)   : none
*
* Caller(s)   : AppTaskStart()
*
* Note(s)     : none.
*********************************************************************************************************
*/
static  void  AppTaskCreate (void)
{
		OS_ERR  err;
	
    OSTaskCreate((OS_TCB     *)&AppTaskLed0TCB,                
                 (CPU_CHAR   *)"App Task Led 0",
                 (OS_TASK_PTR ) AppTaskLed0,
                 (void       *) 0,
                 (OS_PRIO     ) APP_TASK_LED0_PRIO,
                 (CPU_STK    *)&AppTaskLed0Stk[0],
                 (CPU_STK     )(APP_TASK_LED0_STK_SIZE / 10u),
                 (CPU_STK_SIZE) APP_TASK_LED0_STK_SIZE,
                 (OS_MSG_QTY  ) 0,
                 (OS_TICK     ) 0,
                 (void       *) 0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR     *)&err);
								 
    OSTaskCreate((OS_TCB     *)&AppTaskLed1TCB,                
                 (CPU_CHAR   *)"App Task Led 1",
                 (OS_TASK_PTR ) AppTaskLed1,
                 (void       *) 0,
                 (OS_PRIO     ) APP_TASK_LED1_PRIO,
                 (CPU_STK    *)&AppTaskLed1Stk[0],
                 (CPU_STK     )(APP_TASK_LED1_STK_SIZE / 10u),
                 (CPU_STK_SIZE) APP_TASK_LED1_STK_SIZE,
                 (OS_MSG_QTY  ) 0,
                 (OS_TICK     ) 0,
                 (void       *) 0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR     *)&err);

    OSTaskCreate((OS_TCB     *)&AppTaskLed2TCB,                
                 (CPU_CHAR   *)"App Task Led 2",
                 (OS_TASK_PTR ) AppTaskLed2,
                 (void       *) 0,
                 (OS_PRIO     ) APP_TASK_LED2_PRIO,
                 (CPU_STK    *)&AppTaskLed2Stk[0],
                 (CPU_STK     )(APP_TASK_LED2_STK_SIZE / 10u),
                 (CPU_STK_SIZE) APP_TASK_LED2_STK_SIZE,
                 (OS_MSG_QTY  ) 0,
                 (OS_TICK     ) 0,
                 (void       *) 0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR     *)&err);
	
    OSTaskCreate((OS_TCB     *)&AppTaskLed3TCB,                
                 (CPU_CHAR   *)"App Task Led 3",
                 (OS_TASK_PTR ) AppTaskLed3,
                 (void       *) 0,
                 (OS_PRIO     ) APP_TASK_LED3_PRIO,
                 (CPU_STK    *)&AppTaskLed3Stk[0],
                 (CPU_STK     )(APP_TASK_LED3_STK_SIZE / 10u),
                 (CPU_STK_SIZE) APP_TASK_LED3_STK_SIZE,
                 (OS_MSG_QTY  ) 0,
                 (OS_TICK     ) 0,
                 (void       *) 0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR     *)&err);								 
		OSTaskCreate((OS_TCB     *)&AppTaskLogTCB,                
                 (CPU_CHAR   *)"App Task Log Print",
                 (OS_TASK_PTR ) AppTaskLog,
                 (void       *) 0,
                 (OS_PRIO     ) APP_TASK_LOG_PRIO,
                 (CPU_STK    *)&AppTaskLogStk[0],
                 (CPU_STK     )(APP_TASK_LOG_STK_SIZE / 10u),
                 (CPU_STK_SIZE) APP_TASK_LOG_STK_SIZE,
                 (OS_MSG_QTY  ) 0,
                 (OS_TICK     ) 0,
                 (void       *) 0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR     *)&err);
}


/*
*********************************************************************************************************
*                                          AppObjCreate()
*
* Description : Create application kernel objects tasks.
*
* Argument(s) : none
*
* Return(s)   : none
*
* Caller(s)   : AppTaskStart()
*
* Note(s)     : none.
*********************************************************************************************************
*/
static  void  AppObjCreate (void)
{
	OS_ERR      os_err;
	u32 count = 1;
	
	while (DEF_TRUE) 
	{ 					
		APP_TRACE_INFO(("%3d ", count++));
		if((count - 1) % 10)
		{}
		else
			APP_TRACE_INFO(("\r\n "));
		OSTimeDlyHMSM(0, 1, 0, 0,
                OS_OPT_TIME_HMSM_STRICT, &os_err);
	}
}

static void AppTaskLed0(void)
{
		OS_ERR      os_err;
	
    while (DEF_TRUE) 
		{                                          /* Task body, always written as an infinite loop.           */
        BSP_LED_Off (0);
        OSTimeDlyHMSM(0, 0, 2, 0,
                      OS_OPT_TIME_HMSM_STRICT, &os_err);
        BSP_LED_On (0);
        OSTimeDlyHMSM(0, 0, 2, 0,
                      OS_OPT_TIME_HMSM_STRICT, &os_err);
		}
}

static void AppTaskLed1(void)
{
		OS_ERR      os_err;
	
    while (DEF_TRUE) 
		{                                          /* Task body, always written as an infinite loop.           */
        BSP_LED_Off (1);
        OSTimeDlyHMSM(0, 0, 4, 0,
                      OS_OPT_TIME_HMSM_STRICT, &os_err);
        BSP_LED_On (1);
        OSTimeDlyHMSM(0, 0, 4, 0,
                      OS_OPT_TIME_HMSM_STRICT, &os_err);
		}
}

static void AppTaskLed2(void)
{
		OS_ERR      os_err;
	
    while (DEF_TRUE) 
		{                                          /* Task body, always written as an infinite loop.           */
        BSP_LED_Off (2);
        OSTimeDlyHMSM(0, 0, 8, 0,
                      OS_OPT_TIME_HMSM_STRICT, &os_err);
        BSP_LED_On (2);
        OSTimeDlyHMSM(0, 0, 8, 0,
                      OS_OPT_TIME_HMSM_STRICT, &os_err);
		}
}

static void AppTaskLed3(void)
{
		OS_ERR      os_err;
	
    while (DEF_TRUE) 
		{                                          /* Task body, always written as an infinite loop.           */
        BSP_LED_Off (3);
        OSTimeDlyHMSM(0, 0, 16, 0,
                      OS_OPT_TIME_HMSM_STRICT, &os_err);
        BSP_LED_On (3);
        OSTimeDlyHMSM(0, 0, 16, 0,
                      OS_OPT_TIME_HMSM_STRICT, &os_err);
		}
}

static void AppTaskLog(void)
{
		OS_ERR      os_err;
		u32 count = 0;
	
    while (DEF_TRUE) 
		{                                          /* Task body, always written as an infinite loop.           */
				APP_TRACE_INFO(("%30d\n", count++));
//				if(!((count - 1) % 10))
//				{
//					APP_TRACE_INFO(("\n"));
//					if(!((count - 1) % 100))
//						APP_TRACE_INFO(("\n"));
//				}

				OSTimeDlyHMSM(0, 0, 1, 0,
										OS_OPT_TIME_HMSM_STRICT, &os_err);
		}
}
