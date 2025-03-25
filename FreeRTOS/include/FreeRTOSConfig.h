#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

extern unsigned int system_core_clock;

#define configUSE_PREEMPTION                1//抢占式调度器开关
#define configUSE_IDLE_HOOK                 0//空闲任务钩子函数
#define configUSE_TICK_HOOK                 0//时间片钩子函数
#define configCPU_CLOCK_HZ                  ( system_core_clock )//CPU主频
#define configTICK_RATE_HZ                  ( ( TickType_t ) 1000 )//RTOS节拍频率 即1秒的中断次数
#define configMAX_PRIORITIES                ( 10 )//可使用的最大优先级
#define configMINIMAL_STACK_SIZE            ( ( unsigned short ) 64 )//定义空闲任务使用的堆栈大小
#define configTOTAL_HEAP_SIZE               ( ( size_t ) ( 10*1024 ) )//RTOS内核总计可用的有效的RAM大小
#define configMAX_TASK_NAME_LEN             ( 12 )//任务名最大长度
#define configUSE_TRACE_FACILITY            0//是否启动可视化跟踪调试
#define configUSE_16_BIT_TICKS              0//系统节拍计数器的变量类型  0为32位 1为16为
#define configIDLE_SHOULD_YIELD             1//空闲任务放弃CPU使用权给其他同优先级的用户任务
#define configUSE_MUTEXES                   1//是否使用互斥信号量
#define configQUEUE_REGISTRY_SIZE           8//设置可以注册的信号量和消息队列个数
#define configCHECK_FOR_STACK_OVERFLOW      0//堆栈溢出检查 大于0有效
#define configUSE_RECURSIVE_MUTEXES         1//是否使用递归互斥信号量
#define configUSE_MALLOC_FAILED_HOOK        0//内存申请失败钩子函数
#define configUSE_APPLICATION_TASK_TAG      0//为任务分配标签
#define configUSE_COUNTING_SEMAPHORES       1//是否使用计数信号量
#define configGENERATE_RUN_TIME_STATS       0//是否启用运行时间统计功能
 
/* Co-routine definitions. */
#define configUSE_CO_ROUTINES               0//启用协程，启用协程以后必须添加文件croutine.c
#define configMAX_CO_ROUTINE_PRIORITIES     ( 2 )//协程的有效优先级数目
 
/* Software timer definitions. */
#define configUSE_TIMERS                    0//是否启用软件定时器
#define configTIMER_TASK_PRIORITY           ( 2 )//软件定时器优先级
#define configTIMER_QUEUE_LENGTH            ( 5 )//软件定时器队列长度
#define configTIMER_TASK_STACK_DEPTH        ( 80 )//软件定时器任务堆栈大小
 
/* Set the following definitions to 1 to include the API function, or zero
to exclude the API function. *///是否启用RTOS API函数
#define INCLUDE_vTaskPrioritySet            1
#define INCLUDE_uxTaskPriorityGet           1
#define INCLUDE_vTaskDelete                 1
#define INCLUDE_vTaskCleanUpResources       1
#define INCLUDE_vTaskSuspend                1
#define INCLUDE_vTaskDelayUntil             1
#define INCLUDE_vTaskDelay                  1
#define INCLUDE_xTaskGetCurrentTaskHandle   1
#define INCLUDE_uxTaskGetStackHighWaterMark 1
 
/* Normal assert() semantics without relying on the provision of an assert.h
header file. */
#define configASSERT( x ) if( ( x ) == 0 )  { taskDISABLE_INTERRUPTS(); for( ;; ); }
 
/* Definitions that map the FreeRTOS port interrupt handlers to their CMSIS
standard names - or at least those used in the unmodified vector table. */
#define vPortSVCHandler SVC_Handler
#define xPortPendSVHandler PendSV_Handler
#define xPortSysTickHandler SysTick_Handler

/* This is the raw value as per the Cortex-M3 NVIC.  Values can be 255
(lowest) to 0 (1?) (highest). */
#define configKERNEL_INTERRUPT_PRIORITY 		255
/* !!!! configMAX_SYSCALL_INTERRUPT_PRIORITY must not be set to zero !!!!
See http://www.FreeRTOS.org/RTOS-Cortex-M3-M4.html. */
#define configMAX_SYSCALL_INTERRUPT_PRIORITY 	191 /* equivalent to 0xb0, or priority 11. */

/* This is the value being used as per the ST library which permits 16
priority values, 0 to 15.  This must correspond to the
configKERNEL_INTERRUPT_PRIORITY setting.  Here 15 corresponds to the lowest
NVIC value of 255. */
#define configLIBRARY_KERNEL_INTERRUPT_PRIORITY	15

#endif /* FREERTOS_CONFIG_H */

