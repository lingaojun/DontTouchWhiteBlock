#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "lcd.h"
#include "rng.h"
#include "includes.h"
#include "os_app_hooks.h"
#include "touch.h"
 //程序先从触摸任务开始，然后是开始任务，然后是随机数任务，然后是123列
 //定义任务优先级
 #define START_TASK_PRIO 3
 //任务堆栈大小
 #define START_STK_SIZE 128 //真正任务字节数 128*4
 //任务控制块
 OS_TCB StartTaskTCB;
 //任务堆栈
 CPU_STK START_TASK_STK[START_STK_SIZE];
//声明任务函数
void start_task(void *p_arg);

//任务优先级
#define TASK1_TASK_PRIO		4
//任务堆栈大小	
#define TASK1_STK_SIZE 		128
//任务控制块
OS_TCB Task1_TaskTCB;
//任务堆栈	
CPU_STK TASK1_TASK_STK[TASK1_STK_SIZE];
void task1_task(void *p_arg);

//任务优先级 
#define TASK2_TASK_PRIO		7
//任务堆栈大小	
#define TASK2_STK_SIZE 		128
//任务控制块
OS_TCB Task2_TaskTCB;
//任务堆栈	
CPU_STK TASK2_TASK_STK[TASK2_STK_SIZE];
//任务函数
void task2_task(void *p_arg);

//任务优先级 
#define TASK3_TASK_PRIO		6
//任务堆栈大小	
#define TASK3_STK_SIZE 		128
//任务控制块
OS_TCB Task3_TaskTCB;
//任务堆栈	
CPU_STK TASK3_TASK_STK[TASK3_STK_SIZE];
//任务函数
void task3_task(void *p_arg);

//任务优先级 
#define TASK4_TASK_PRIO		8
//任务堆栈大小	
#define TASK4_STK_SIZE 		128
//任务控制块
OS_TCB Task4_TaskTCB;
//任务堆栈	
CPU_STK TASK4_TASK_STK[TASK4_STK_SIZE];
//任务函数
void task4_task(void *p_arg);

//任务优先级 
#define TASK5_TASK_PRIO		9
//任务堆栈大小	
#define TASK5_STK_SIZE 		128
//任务控制块
OS_TCB Task5_TaskTCB;
//任务堆栈	
CPU_STK TASK5_TASK_STK[TASK5_STK_SIZE];
//任务函数
void task5_task(void *p_arg);

//任务优先级 
#define TASK6_TASK_PRIO		10
//任务堆栈大小	
#define TASK6_STK_SIZE 		128
//任务控制块
OS_TCB Task6_TaskTCB;
//任务堆栈	
CPU_STK TASK6_TASK_STK[TASK6_STK_SIZE];
//任务函数
void task6_task(void *p_arg);

//任务优先级 
#define TASK7_TASK_PRIO		5
//任务堆栈大小	
#define TASK7_STK_SIZE 		128
//任务控制块
OS_TCB Task7_TaskTCB;
//任务堆栈	
CPU_STK TASK7_TASK_STK[TASK7_STK_SIZE];
//任务函数
void task7_task(void *p_arg);

//任务优先级 
#define TASK8_TASK_PRIO		11
//任务堆栈大小	
#define TASK8_STK_SIZE 		128
//任务控制块
OS_TCB Task8_TaskTCB;
//任务堆栈	
CPU_STK TASK8_TASK_STK[TASK8_STK_SIZE];
//任务函数
void task8_task(void *p_arg);


int lcd_discolor[14]={	
						WHITE, BLACK, BLUE,  BRED, 
						GRED,  GBLUE, RED,   MAGENTA,       	 
						GREEN, CYAN,  YELLOW,BROWN, 			
						BRRED, GRAY };

u16 color11,color12,color21,color22,color31,color32;
u8 touch1,touch2,touch3;					
u8 startflag = 0;
u8 againstartflag = 0;
u8 overflag = 0;
						
u8 renew = 0;
						
u8 back = 0;
						
u8 goalscore1 = 0;
u8 goalscore2 = 0;
u8 goalscore3 = 0;
u8 score1 = 0;
u8 score2 = 0;
u8 score3 = 0;	


u8 overmenu = 0;
u8 beginmenu = 0;
struct block;
typedef struct block *Block;
typedef Block Position;
struct block
{
	u8 start;
	u16 length;
	u8 out;
	u8 touch;
	u8 arrdown;
	Position next;
};
Position iblock;
struct block block1[6] = {
													{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0},
													{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0}	
												};		
struct block block2[6] = {
													{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0},
													{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0}	
												};	
struct block block3[6] = {
													{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0},
													{0,0,0,0,0},{0,0,0,0,0},{0,0,0,0,0}	
												};	

void rtp_test(void)
{  
		u8 i = 0;
		tp_dev.scan(0);
		if((touch1||touch2||touch3) && tp_dev.x[0]==65535 && startflag) //离开触摸屏检查
		{
			LED1 = ~LED1;
			touch1 = 0;
			touch2 = 0;
			touch3 = 0;
		}
		if(overflag && tp_dev.x[0]>40 &&tp_dev.x[0]<100 &&tp_dev.y[0]>200 &&tp_dev.y[0]<250) //重开 触摸检测 RENEW按键按下
		{
			LCD_Clear(WHITE);
			overflag = 0;
			overmenu = 0;
			score1 = score2 = score3 = 0;
			goalscore1 = goalscore2 = goalscore3 = 0;
			renew = 1;						
			LCD_DrawLine(0,250,240,250);		//画线
			LCD_DrawLine(60,0,60,360);		//画线
			LCD_DrawLine(120,0,120,360);		//画线
			LCD_DrawLine(180,0,180,360);		//画线
		}
		if(overflag && tp_dev.x[0]>140 &&tp_dev.x[0]<200 &&tp_dev.y[0]>200 &&tp_dev.y[0]<250)//返回主页 BACK按键按下
		{
			LCD_Clear(WHITE);		
			beginmenu = 0;
			againstartflag = 0;
			overmenu = 0;	
			overflag = 0;
			score1 = score2 = score3 = 0;
			goalscore1 = goalscore2 = goalscore3 = 0;
			back = 1;
			LCD_DrawLine(60,60,180,60);		//画线	------
			LCD_DrawLine(60,132,180,132);		//画线------
			LCD_DrawLine(60,60,60,132);		//画线|
			LCD_DrawLine(180,60,180,132 );		//画线|	
			LCD_ShowString(80,86,200,200,16,"Start Game");
			LCD_ShowString(80,250,200,200,16,"All MADE BY  ");
			LCD_ShowString(85,280,200,200,16,"LinGaoJun");
		}
		if(tp_dev.sta&TP_PRES_DOWN )			//触摸屏被按下
		{	
			if(tp_dev.x[0]>60&&tp_dev.x[0]<180&&tp_dev.y[0]>60&&tp_dev.y[0]<132 && !startflag) //第一次开始游戏 按下
			{
				LED0  = ~LED0;
				startflag = 1;
			}
			if(tp_dev.x[0]>60&&tp_dev.x[0]<180&&tp_dev.y[0]>60&&tp_dev.y[0]<132 && back) //第一次开始游戏 按下
			{
				LED0 = ~LED0;
				againstartflag = 1;
			}
		 	if(tp_dev.x[0]>0&&tp_dev.x[0]<60&&tp_dev.y[0]>250) //第一列下面方块按下
			{	
				for(i=0;i<6;i++)
				{
					if(block1[i].arrdown && !block1[i].out && !block1[i].touch && !touch1) //表示方块到了底部 且未退出
					{			
						block1[i].arrdown = 0; //单执行标志位清楚 方块到底部次数为单次
						block1[i].touch =1;
						score1++;
						touch1 = 1;
					}else if(color11==65535&&color12==65535 && !touch1) { overflag = 1; renew = 0; againstartflag = 0;}					
				}
			}
		 	if(tp_dev.x[0]>60&&tp_dev.x[0]<120&&tp_dev.y[0]>250 )//触屏触发
			{				
				for(i=0;i<6;i++)
				{
					if(block2[i].arrdown && !block2[i].out && !block2[i].touch && !touch2) //表示方块到了底部 且未退出
					{			
						block2[i].arrdown = 0; //单执行标志位清楚 方块到底部次数为单次
						block2[i].touch =1;
						score2++;
						touch2 = 1;
					}else if(color21==65535&&color22==65535 && !touch2) { overflag = 1; renew = 0; againstartflag = 0;}					
				}
			}
		 	if(tp_dev.x[0]>120&&tp_dev.x[0]<180&&tp_dev.y[0]>250)
			{						
				for(i=0;i<6;i++)
				{
					if(block3[i].arrdown && !block3[i].out && !block3[i].touch && !touch3) //表示方块到了底部 且未退出
					{			
						block3[i].arrdown = 0; //单执行标志位清楚 方块到底部次数为单次
						block3[i].touch =1;
						score3++;
						touch3 = 1;
					}else if(color31==65535&&color32==65535 && !touch3) { overflag = 1; renew = 0; againstartflag = 0;}					
				}
			}

		}    
}
int main(void)
{
	OS_ERR err;
	CPU_SR_ALLOC();

	delay_init(168);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	uart_init(115200);
	LED_Init();
	LCD_Init();
	RNG_Init();
	tp_dev.init();				//触摸屏初始化
	POINT_COLOR = BLACK;
		
	
	OSInit(&err); //UCOSIII 初始化
	
	OS_CRITICAL_ENTER();	//进入临界区	
	OSTaskCreate (		(OS_TCB*)				&StartTaskTCB,     //任务创建
                    (CPU_CHAR*)			"start_task",
                    (OS_TASK_PTR)		 start_task,
                    (void*)        		0,
                    (OS_PRIO)        START_TASK_PRIO,
                    (CPU_STK*)       &START_TASK_STK[0],
                    (CPU_STK_SIZE)   TASK1_STK_SIZE/10,
                    (CPU_STK_SIZE)   TASK1_STK_SIZE,
                    (OS_MSG_QTY)     0,
                    (OS_TICK)        0,
                    (void*)          0,
                    (OS_OPT)         OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                    (OS_ERR*)        &err
								);
	OS_CRITICAL_EXIT(); //退出临界区
	OSStart(&err);
	
	
}
//任务函数 
void start_task(void *p_arg)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	p_arg = p_arg;

	CPU_Init();
#if OS_CFG_STAT_TASK_EN > 0u
   OSStatTaskCPUUsageInit(&err);  	//统计任务                
#endif
	
#ifdef CPU_CFG_INT_DIS_MEAS_EN		//如果使能了测量中断关闭时间
    CPU_IntDisMeasMaxCurReset();	
#endif
	
#if	OS_CFG_SCHED_ROUND_ROBIN_EN  //当使用时间片轮转的时候
	 //使能时间片轮转调度功能,时间片长度为1个系统时钟节拍，既1*5=5ms
	OSSchedRoundRobinCfg(DEF_ENABLED,1,&err);  
#endif	
	
	OS_CRITICAL_ENTER();	//进入临界区
	//创建TASK1任务
	OSTaskCreate((OS_TCB 	* )&Task1_TaskTCB,		
				 (CPU_CHAR	* )"Task1 task", 		
                 (OS_TASK_PTR )task1_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )TASK1_TASK_PRIO,     
                 (CPU_STK   * )&TASK1_TASK_STK[0],	
                 (CPU_STK_SIZE)TASK1_STK_SIZE/10,	
                 (CPU_STK_SIZE)TASK1_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,					
                 (void   	* )0,					
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                 (OS_ERR 	* )&err);				
				 
	//创建TASK2任务
	OSTaskCreate((OS_TCB 	* )&Task2_TaskTCB,		
				 (CPU_CHAR	* )"task2 task", 		
                 (OS_TASK_PTR )task2_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )TASK2_TASK_PRIO,     	
                 (CPU_STK   * )&TASK2_TASK_STK[0],	
                 (CPU_STK_SIZE)TASK2_STK_SIZE/10,	
                 (CPU_STK_SIZE)TASK2_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,					
                 (void   	* )0,				
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, 
                 (OS_ERR 	* )&err);	
								 

	OSTaskCreate((OS_TCB 	* )&Task3_TaskTCB,		
				 (CPU_CHAR	* )"task3 task", 		
                 (OS_TASK_PTR )task3_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )TASK3_TASK_PRIO,     	
                 (CPU_STK   * )&TASK3_TASK_STK[0],	
                 (CPU_STK_SIZE)TASK3_STK_SIZE/10,	
                 (CPU_STK_SIZE)TASK3_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,					
                 (void   	* )0,				
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, 
                 (OS_ERR 	* )&err);	
								 

	OSTaskCreate((OS_TCB 	* )&Task4_TaskTCB,		
				 (CPU_CHAR	* )"task4 task", 		
                 (OS_TASK_PTR )task4_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )TASK4_TASK_PRIO,     	
                 (CPU_STK   * )&TASK4_TASK_STK[0],	
                 (CPU_STK_SIZE)TASK4_STK_SIZE/10,	
                 (CPU_STK_SIZE)TASK4_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,					
                 (void   	* )0,				
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, 
                 (OS_ERR 	* )&err);	
								 

	OSTaskCreate((OS_TCB 	* )&Task5_TaskTCB,		
				 (CPU_CHAR	* )"task5 task", 		
                 (OS_TASK_PTR )task5_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )TASK5_TASK_PRIO,     	
                 (CPU_STK   * )&TASK5_TASK_STK[0],	
                 (CPU_STK_SIZE)TASK5_STK_SIZE/10,	
                 (CPU_STK_SIZE)TASK5_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,					
                 (void   	* )0,				
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, 
                 (OS_ERR 	* )&err);	
								 

	OSTaskCreate((OS_TCB 	* )&Task6_TaskTCB,		
				 (CPU_CHAR	* )"task6 task", 		
                 (OS_TASK_PTR )task6_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )TASK6_TASK_PRIO,     	
                 (CPU_STK   * )&TASK6_TASK_STK[0],	
                 (CPU_STK_SIZE)TASK6_STK_SIZE/10,	
                 (CPU_STK_SIZE)TASK6_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,					
                 (void   	* )0,				
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, 
                 (OS_ERR 	* )&err);	
								 
	OSTaskCreate((OS_TCB 	* )&Task7_TaskTCB,		
				 (CPU_CHAR	* )"task7 task", 		
                 (OS_TASK_PTR )task7_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )TASK7_TASK_PRIO,     	
                 (CPU_STK   * )&TASK7_TASK_STK[0],	
                 (CPU_STK_SIZE)TASK7_STK_SIZE/10,	
                 (CPU_STK_SIZE)TASK7_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,					
                 (void   	* )0,				
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, 
                 (OS_ERR 	* )&err);	

	OSTaskCreate((OS_TCB 	* )&Task8_TaskTCB,		
				 (CPU_CHAR	* )"task8 task", 		
                 (OS_TASK_PTR )task8_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )TASK8_TASK_PRIO,     	
                 (CPU_STK   * )&TASK8_TASK_STK[0],	
                 (CPU_STK_SIZE)TASK8_STK_SIZE/10,	
                 (CPU_STK_SIZE)TASK8_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,					
                 (void   	* )0,				
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, 
                 (OS_ERR 	* )&err);	
								 
	OS_CRITICAL_EXIT();	//退出临界区
	OSTaskDel((OS_TCB*)0,&err);	//删除start_task任务自身	
}

u16 flag1 = 0;
u16 flag2 = 0;
u16 flag3 = 0;
u16 timeflag1 = 0;
u16 timeflag2 = 0;
u16 timeflag3 = 0;
//task1任务函数
void task1_task(void *p_arg) //触屏任务
{
	OS_ERR err;
	CPU_SR_ALLOC();
	p_arg = p_arg;	
	POINT_COLOR = BLACK;
	OS_CRITICAL_ENTER();
	OS_CRITICAL_EXIT();
	while(1)
	{		
		color11 = LCD_ReadPoint(30,251); //采色
		color12 = LCD_ReadPoint(30,319);
		color21 = LCD_ReadPoint(90,251);
		color22 = LCD_ReadPoint(90,319);
		color31 = LCD_ReadPoint(150,251);
		color32 = LCD_ReadPoint(150,319);
		if(overflag) //失败标志位
		{
			OSTaskResume((OS_TCB*)&Task8_TaskTCB,&err);//解挂失败界面
			OSTaskSuspend((OS_TCB*)&Task2_TaskTCB,&err);//挂起第一列
			OSTaskSuspend((OS_TCB*)&Task4_TaskTCB,&err);//挂起第二列
			OSTaskSuspend((OS_TCB*)&Task5_TaskTCB,&err);//挂起第三列	
			OSTaskSuspend((OS_TCB*)&Task3_TaskTCB,&err);//挂起随机数
		}
		rtp_test();

		OSTimeDlyHMSM(0,0,0,50,OS_OPT_TIME_HMSM_STRICT,&err); //延时1s
	}
}
u8 random=0;
void task2_task(void *p_arg) //第一列方块下落任务
{
	u8 i = 0;
	u8 scoreflag[6] = {0};
	OS_ERR err;
	CPU_SR_ALLOC();
	p_arg = p_arg;
	POINT_COLOR = BLACK;
	OS_CRITICAL_ENTER();
	OS_CRITICAL_EXIT();
	while(1)
	{ 
		for(i = 0;i <= 5;i++)
		{
			if(flag1==i+1)
			{
				block1[i].start= 1;
				if(i<=3)
				{
					block1[i+1].out= 0;
					block1[i+2].out= 0;	
				}
				else if(i==4)
				{
					block1[i+1].out= 0;
					block1[0].out= 0;						
				}
				else if(i==5)
				{
					block1[0].out= 0;
					block1[1].out= 0;						
				}				
			}
			if(block1[i].start && block1[i].length<=390)
			{
				block1[i].length++;	
				LCD_DrawRectangle(0,block1[i].length,60,block1[i].length);
				if(block1[i].length>70)
				{
					LCD_Fill(0,block1[i].length-71,59,block1[i].length-71,WHITE); //一条线 
				}
				if(block1[i].length>=250)
				{
					LCD_DrawLine(0,250,240,250);		//画线
					block1[i].arrdown = 1;
					if(!scoreflag[i])
					{
						goalscore1++;
						scoreflag[i] = 1;
					}
				} 
				if(block1[i].length>=390) 
				{
					block1[i].out= 1;
					block1[i].arrdown = 0;
					block1[i].touch = 0;
					block1[i].start= 0;
					block1[i].length = 0;
					flag1 = 0;
					scoreflag[i] = 0;
					if(goalscore1 != score1)
					{
						overflag = 1; renew = 0;againstartflag = 0;
					}
				}
				if(block1[i].touch)
				{
					block1[i].length=390;
					LCD_Fill(0,180,59,390,WHITE); //一条线 
					LCD_DrawLine(0,250,240,250);		//画线
				}
			}
		}		
		
		OSTimeDlyHMSM(0,0,0,5,OS_OPT_TIME_HMSM_STRICT,&err); //延时1s	
	}
}

void task3_task(void *p_arg) //随机数发生器任务
{
	OS_ERR err;
	CPU_SR_ALLOC();
	p_arg = p_arg;
	POINT_COLOR = BLACK;
	OS_CRITICAL_ENTER();
	OS_CRITICAL_EXIT();
	
	LCD_DrawLine(0,250,240,250);		//画线
	LCD_DrawLine(60,0,60,360);		//画线
	LCD_DrawLine(120,0,120,360);		//画线
	LCD_DrawLine(180,0,180,360);		//画线	
	OSTaskSuspend((OS_TCB*)&Task2_TaskTCB,&err);//挂起第一列
	OSTaskSuspend((OS_TCB*)&Task4_TaskTCB,&err);//挂起第二列
	OSTaskSuspend((OS_TCB*)&Task5_TaskTCB,&err);//挂起第三列	
	OSTaskSuspend((OS_TCB*)&Task8_TaskTCB,&err);//挂起失败界面	
	while(1)
	{	
		random=RNG_Get_RandomRange(1,3);//获取[1,3]区间的随机数
		if(random==1 &&!overflag)
		{
			timeflag1++;
			OSTaskResume((OS_TCB*)&Task2_TaskTCB,&err);//任务1执行10次解挂任务2	
			if(timeflag1>6)
			{
				timeflag1 = 1;
			}
			flag1 = timeflag1;
		}
		if(random==2 &&!overflag)
		{
			timeflag2++;
			OSTaskResume((OS_TCB*)&Task4_TaskTCB,&err);//任务1执行10次解挂任务2
			if(timeflag2>6)
			{
				timeflag2 = 1;
			}
			flag2=timeflag2;		
		}
		if(random==3 &&!overflag)
		{
			timeflag3++;
			OSTaskResume((OS_TCB*)&Task5_TaskTCB,&err);//任务1执行10次解挂任务2
			if(timeflag3>6)
			{
				timeflag3 = 1;			
			}	
			flag3=timeflag3;		
		}
		OSTimeDlyHMSM(0,0,1,0,OS_OPT_TIME_HMSM_STRICT,&err); //延时1s
	}
}
void task4_task(void *p_arg) //第二列方块下落任务
{
	u8 i =0;
	u8 scoreflag[6] = {0};
	OS_ERR err;
	CPU_SR_ALLOC();
	p_arg = p_arg;
	POINT_COLOR = BLACK;
	OS_CRITICAL_ENTER();
	OS_CRITICAL_EXIT();
	while(1)
	{ 
		for(i = 0 ;i <= 5;i++)
		{
			if(flag2==i+1)
			{
				block2[i].start= 1;
				if(i<=4)
				{
					block2[i+1].out= 0;
					block2[i+2].out= 0;	
				}
				else if(i==4)
				{
					block2[i+1].out= 0;
					block2[0].out= 0;						
				}
				else if(i==5)
				{
					block2[0].out= 0;
					block2[1].out= 0;						
				}				
			}
			if(block2[i].start && block2[i].length<=390)
			{
				block2[i].length++;	
				LCD_DrawRectangle(61,block2[i].length,120,block2[i].length);
				if(block2[i].length>70)
				{
					LCD_Fill(61,block2[i].length-71,119,block2[i].length-71,WHITE); //一条线 
				}
				if(block2[i].length>=250)
				{
					LCD_DrawLine(0,250,240,250);		//画线
					block2[i].arrdown = 1;
					if(!scoreflag[i])
					{
						goalscore2++;
						scoreflag[i] = 1;
					}
				} 
				if(block2[i].length>=390) 
				{
					block2[i].out= 1;
					block2[i].arrdown = 0;
					block2[i].touch = 0;
					block2[i].start= 0;
					block2[i].length = 0;
					flag2 = 0;
					scoreflag[i] = 0;
					if(goalscore2 != score2)
					{
						overflag = 1; renew = 0;againstartflag = 0;
					}
				}
				if(block2[i].touch)
				{
					block2[i].length=390;
					LCD_Fill(61,180,119,390,WHITE); //一条线 
					LCD_DrawLine(0,250,240,250);		//画线
				}
			}
		}	
		
		OSTimeDlyHMSM(0,0,0,5,OS_OPT_TIME_HMSM_STRICT,&err); //延时1s	
	}
}

void task5_task(void *p_arg) //第三列方块下落
{
	u8 i = 0;
	u8 scoreflag[6] = {0};
	OS_ERR err;
	CPU_SR_ALLOC();
	p_arg = p_arg;
	POINT_COLOR = BLACK;
	OS_CRITICAL_ENTER();
	OS_CRITICAL_EXIT();
	while(1)
	{
		for(i = 0;i <= 5;i++)
		{
			if(flag3==i+1)
			{
				block3[i].start= 1;
				if(i<=3)
				{
					block3[i+1].out= 0;
					block3[i+2].out= 0;	
				}
				else if(i==4)
				{
					block3[i+1].out= 0;
					block3[0].out= 0;						
				}
				else if(i==5)
				{
					block3[0].out= 0;
					block3[1].out= 0;						
				}				
			}
			if(block3[i].start && block3[i].length<=390)
			{
				block3[i].length++;	
				LCD_DrawRectangle(121,block3[i].length,180,block3[i].length);
				if(block3[i].length>70)
				{
					LCD_Fill(121,block3[i].length-71,179,block3[i].length-71,WHITE); //一条线 
				}
				if(block3[i].length>=250)
				{
					LCD_DrawLine(0,250,240,250);		//画线
					block3[i].arrdown = 1;
					if(!scoreflag[i])
					{
						goalscore3++;
						scoreflag[i] = 1;
					}
				} 
				if(block3[i].length>=390) 
				{
					block3[i].out= 1;
					block3[i].arrdown = 0;
					block3[i].touch = 0;
					block3[i].start= 0;
					block3[i].length = 0;
					flag3 = 0;
					scoreflag[i] = 0;
					if(goalscore3 != score3)
					{
						overflag = 1; renew = 0;againstartflag = 0;
					}
				}
				if(block3[i].touch)
				{
					block3[i].length=390;
					LCD_Fill(121,180,179,390,WHITE); //一条线 
					LCD_DrawLine(0,250,240,250);		//画线
				}
			}
		}
		OSTimeDlyHMSM(0,0,0,5,OS_OPT_TIME_HMSM_STRICT,&err); //延时1s			
	}
	
}

void task6_task(void *p_arg) //加分任务
 {
	OS_ERR err;
	CPU_SR_ALLOC();
	p_arg = p_arg;
	POINT_COLOR = BLACK;
	OS_CRITICAL_ENTER();
	OS_CRITICAL_EXIT();
	while(1)
	{
//		LCD_ShowxNum(206,181,overmenu,3,16,0x80);  //显示任务执行次数
//				LCD_ShowxNum(206,201,length1[0],3,16,0x80);  //显示任务执行次数
	//			LCD_ShowxNum(206,181,flag1,3,16,0x80);  //显示任务执行次数
		LCD_ShowxNum(206,101,random,3,16,0x80);  //显示任务执行次�
//		LCD_ShowxNum(206,121,startflag,3,16,0x80);  //显示任务执行次�
//		LCD_ShowxNum(206,141,overflag,3,16,0x80);  //显示任务执行次�		
//		LCD_ShowxNum(106,141,color22,5,16,0x80);  //显示任务执行次�	
//		printf("block_out2[0]:%d \n",block_out2[0]);
		OSTimeDlyHMSM(0,0,0,5,OS_OPT_TIME_HMSM_STRICT,&err); //延时1s	
	}
}
void task7_task(void *p_arg) //开始界面
{

	OS_ERR err;
	CPU_SR_ALLOC();
	p_arg = p_arg;
	POINT_COLOR = BLACK;
	OS_CRITICAL_ENTER();
	OS_CRITICAL_EXIT();
	OSTaskSuspend((OS_TCB*)&Task3_TaskTCB,&err);//挂起随机数
	LCD_DrawLine(60,60,180,60);		//画线	------
	LCD_DrawLine(60,132,180,132);		//画线------
	LCD_DrawLine(60,60,60,132);		//画线|
	LCD_DrawLine(180,60,180,132 );		//画线|	
	LCD_ShowString(80,86,200,200,16,"Start Game");
	LCD_ShowString(80,250,200,200,16,"All MADE BY  ");
	LCD_ShowString(85,280,200,200,16,"LinGaoJun");
//	LCD_ShowString(30,70,200200,,16,"TFTLCD TEST");
	while(1)
	{
			if(startflag ) //
			{
			LCD_Clear(WHITE);
			OSTaskResume((OS_TCB*)&Task3_TaskTCB,&err);//解挂随机数
			OSTaskSuspend((OS_TCB*)&Task7_TaskTCB,&err);//挂起开始界面		
			}
	}
}
void task8_task(void *p_arg)//失败界面
{
	OS_ERR err;
	CPU_SR_ALLOC();
	p_arg = p_arg;
	POINT_COLOR = BLACK;
	OS_CRITICAL_ENTER();
	OS_CRITICAL_EXIT();
	while(1)
 	{
		if(!overmenu && overflag) //仅执行一次
		{
			overmenu  =1;
			LCD_Fill(30,40,210,290,WHITE);
			LCD_DrawLine(30,40,210,40);		//画线	------
			LCD_DrawLine(30,290,210,290);		//画线------
			LCD_DrawLine(30,40,30,290);		//画线|
			LCD_DrawLine(210,40,210,290 );		//画线|				
			LCD_ShowString(80,60,200,200,16,"Your Scores");	
			LCD_ShowxNum(100,120,score2+score1+score3,2,24,0x80);  //显示任务执行次数
			LCD_DrawRectangle(40,200,100,250);
			LCD_ShowString(50,220,200,200,16,"Renew");	
			LCD_DrawRectangle(140,200,200,250);
			LCD_ShowString(150,220,200,200,16,"Back");
//			timeflag1 = timeflag2 = timeflag3 = 0;
			block1[0].start = block1[1].start = block1[2].start = block1[3].start = block1[4].start = block1[5].start= 0; 
			block2[0].start = block2[1].start = block2[2].start = block2[3].start = block2[4].start = block2[5].start= 0; 
			block3[0].start = block3[1].start = block3[2].start = block3[3].start = block3[4].start = block3[5].start= 0; 
			flag1 =flag2 = flag3 = 0;
			block1[0].length = block1[1].length = block1[2].length = block1[3].length = block1[4].length = block1[5].length= 0; 
			block2[0].length = block2[1].length = block2[2].length = block2[3].length = block2[4].length = block2[5].length= 0; 
			block3[0].length = block3[1].length = block3[2].length = block3[3].length = block3[4].length = block3[5].length= 0; 
//			LCD_ShowxNum(206,181,score2+score1+score3,3,16,0x80);  //显示任务执行次数			
		}
		if(renew) //选择重开游戏
		{
			overflag = 0;
			OSTaskResume((OS_TCB*)&Task3_TaskTCB,&err);//任务1执行10次解挂任务2		
			OSTaskResume((OS_TCB*)&Task2_TaskTCB,&err);//任务1执行10次解挂任务2		
			OSTaskResume((OS_TCB*)&Task4_TaskTCB,&err);//任务1执行10次解挂任务2		
			OSTaskResume((OS_TCB*)&Task5_TaskTCB,&err);//任务1执行10次解挂任务2		
		}
		if(!beginmenu && back && againstartflag)//仅执行一次
 		{
			beginmenu = 1;
			LCD_Clear(WHITE);
			LCD_DrawLine(0,250,240,250);		//画线
			LCD_DrawLine(60,0,60,360);		//画线
			LCD_DrawLine(120,0,120,360);		//画线
			LCD_DrawLine(180,0,180,360);		//画线	
			block1[0].start = block1[1].start = block1[2].start = block1[3].start = block1[4].start = block1[5].start= 0; 			
			block2[0].start = block2[1].start = block2[2].start = block2[3].start = block2[4].start = block2[5].start= 0; 			
			block3[0].start = block3[1].start = block3[2].start = block3[3].start = block3[4].start = block3[5].start= 0; 				
			flag1 =flag2 = flag3 = 0;
			block1[0].length = block1[1].length = block1[2].length = block1[3].length = block1[4].length = block1[5].length= 0; 
			block2[0].length = block2[1].length = block2[2].length = block2[3].length = block2[4].length = block2[5].length= 0; 
			block3[0].length = block3[1].length = block3[2].length = block3[3].length = block3[4].length = block3[5].length= 0; 			
		}
		if(againstartflag)
		{	
			overflag = 0;
			OSTaskResume((OS_TCB*)&Task3_TaskTCB,&err);//任务1执行10次解挂任务2		
			OSTaskResume((OS_TCB*)&Task2_TaskTCB,&err);//任务1执行10次解挂任务2		
			OSTaskResume((OS_TCB*)&Task4_TaskTCB,&err);//任务1执行10次解挂任务2		
			OSTaskResume((OS_TCB*)&Task5_TaskTCB,&err);//任务1执行10次解挂任务2					
		}
		OSTimeDlyHMSM(0,0,0,50,OS_OPT_TIME_HMSM_STRICT,&err); //延时1s
		
	}
}

