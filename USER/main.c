#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"	 
#include "24cxx.h"
#include "w25qxx.h"
#include "touch.h"
#include "hanzi.h"
#include "time.h"
 
/************************************************
 ALIENTEK精英STM32开发板实验26
 触摸屏 实验    
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com 
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/
 #define  ZERO  3000
 #define  proport  	10000 //Tclk/(psc+1)=72000000/(7199+1)
 #define  L1       ((proport/131)-1)//低调　do 的周期根据Tout= ((arr+1)*(psc+1))/Tclk推出arr值就是本句define定义的值，Tout为音调频率131Hz的倒数，Tclk=72MHz
 #define  L2       ((proport/147)-1)//低调　re 的周期
 #define  L3       ((proport/165)-1)//低调　mi 的周期
 #define  L4       ((proport/176)-1)//低调　fa 的周期
 #define  L5       ((proport/196)-1)//低调　sol的周期
 #define  L6       ((proport/220)-1)//低调　la 的周期
 #define  L7       ((proport/247)-1)//低调　si 的周期

 #define  M1       ((proport/262)-1)//中调　do 的周期
 #define  M2       ((proport/296)-1)//中调　re 的周期
 #define  M3       ((proport/330)-1)//中调　mi 的周期 
 #define  M4       ((proport/349)-1)//中调　fa 的周期
 #define  M5       ((proport/392)-1)//中调　sol的周期
 #define  M6       ((proport/440)-1)//中调　la 的周期
 #define  M7       ((proport/494)-1)//中调　si 的周期

 #define  H1       ((proport/523)-1)//高调　do 的周期
 #define  H2       ((proport/587)-1)//高调　re 的周期
 #define  H3       ((proport/659)-1)//高调　mi 的周期
 #define  H4       ((proport/699)-1)//高调　fa 的周期
 #define  H5       ((proport/784)-1)//高调　sol的周期
 #define  H6       ((proport/880)-1)//高调　la 的周期
 #define  H7       ((proport/988)-1)//高调　si 的周期
int music[]=  
{  
	M6,50,M7,50,H1,150,M7,50,H1,100,H3,100,M7,100,M7,100,M7,100,M3,50,M3,50,  

  M6,150,M5,50,M6,100,H1,100,M5,100,M5,100,M5,100,M3,100,M4,150,M3,50,M4,100,H1,100,

  M3,100,M3,100,ZERO,50,H1,50,H1,50,H1,50,M7,150,M4,50,M4,100,M7,100,M7,100,M7,100,ZERO,50,M6,50,M7,50,

  H1,150,M7,50,H1,100,H3,100,M7,100,M7,100,M7,100,M3,50,M3,50,M6,150,M5,50,M6,100,H1,100,

  M5,100,M5,100,M5,100,M2,50,M3,50,M4,100,H1,50,M7,25,M7,25,H1,25,H1,50,H2,50,H2,50,H3,50,H1,25,H1,50,H1,100,

  H1,50,M7,50,M6,50,M6,50,M7,100,M5,100,M6,100,M6,100,M6,100,H1,50,H2,50,H3,150,H2,50,H3,100,H5,100,

  H2,100,H2,100,H2,100,M5,50,M5,50,H1,150,M7,50,H1,100,H3,100,H3,100,H3,100,H3,100,H3,100,

  M6,50,M7,50,H1,100,M7,100,H2,50,H2,50,H1,150,M5,25,M5,50,M5,100,H4,100,H3,100,H2,100,H1,100,

  H3,100,H3,100,H3,100,H3,100,H6,100,H6,100,H5,100,H5,100,H3,50,H2,50,H1,100,H1,100,ZERO,50,H1,50,

  H2,100,H1,50,H2,50,H2,100,H5,100,H3,100,H3,100,H3,100,H3,100,H6,100,H6,100,H5,100,H5,100,

  H3,50,H2,50,H1,100,H1,100,ZERO,50,H1,50,H2,100,H1,50,H2,25,H2,50,M7,100,M6,100,M6,100,M6,100,M6,50,M7,50

};
int length = sizeof(music)/sizeof(music[0]);//计算数组长度
//音乐函数
void musicst(){
	int t=0;
	
	delay_init();
    while(100)
	{
		for(t=0; t<(length/2); t++)
		{  
			Timer4_Init(music[2*t]);
			delay_ms(5*music[2*t+1]);
		}
	}

}
void man(int pox,int small){
	LCD_Draw_Circle(40+pox,200,15);
	LCD_DrawLine(40+pox, 215, 40+pox,260);
	LCD_DrawLine(40+pox, 260, 25+pox,300);
	LCD_DrawLine(40+pox, 260, 55+pox,300);
	//arm1
	LCD_DrawLine(40+pox, 230, 60+pox,240);
	LCD_DrawLine(60+pox, 240, 65+pox,220);
	//arm2
	LCD_DrawLine(40+pox, 230, 30+pox,240);
	LCD_DrawLine(30+pox, 240, 25+pox,255);
}

void door(int pox){
	int count=0;
	
	
	for(count=0;count<8;count++){
		LCD_DrawLine(pox,20+count,pox+60,20+count);
	}
	for(count=0;count<8;count++){
		LCD_DrawLine(pox+count, 20, pox-5+count,60);
	}
	for(count=0;count<8;count++){
		LCD_DrawLine(pox+60+count, 20, pox+65+count,60);
	}
	//横网
	LCD_DrawLine(pox-2, 38, pox+62,38);
	LCD_DrawLine(pox-3, 48, pox+63,48);
	LCD_DrawLine(pox-4, 58, pox+64,58);
	//竖网
	LCD_DrawLine(pox+18, 20, pox+18,58);
	LCD_DrawLine(pox+28, 20, pox+28,58);
	LCD_DrawLine(pox+38, 20, pox+38,58);
	LCD_DrawLine(pox+48, 20, pox+48,58);
}
void ball(int gox,int goy){
	LCD_Draw_Circle(120,240,10);
	LCD_Draw_Circle(120,240,9);
	LCD_Draw_Circle(120,240,8);
	LCD_DrawLine(112,240,128,240);
	LCD_DrawLine(120,232,120,248);
}
//电阻触摸屏测试函数
void exercise(){
		int pox=20;
		int go=0;
		int time=100;
		int i=0;
		LCD_Clear(GREEN);
		LCD_Color_Fill(0,0,240,62,WHITE);
		
		ball(0,0);
		man(0,0);
		
		while(1){			
			int time=10;
			LCD_Color_Fill(0,0,240,62,WHITE);
			door(pox);
			if(pox>=150)go=1;
			if(pox<=20)go=0;
			if(go==0)pox+=1;
			if(go==1)pox-=1;
			if(KEY_Scan(0)){
				for(i=0;i<30;i++){
					LCD_Color_Fill(0,62,240,400,GREEN);
					ball(0,0);
					man(i,0);
					time--;
					if(time==0)break;
				}
			}
		};
}


//开始菜单展示
void start_page(){
	fWriteHz24(GB_24[0].Msk,5+5,50,2,RED);	 
	fWriteHz24(GB_24[1].Msk,50+5,50,2,RED);
	fWriteHz24(GB_24[2].Msk,95+5,50,2,RED);
	fWriteHz24(GB_24[3].Msk,140+5,50,2,RED);
	fWriteHz24(GB_24[4].Msk,185+5,50,2,RED);
	 
	fWriteHz24(GB_lianxi[0].Msk,55+5,150,1,WHITE);	 
	fWriteHz24(GB_lianxi[1].Msk,88+5,150,1,WHITE);
	fWriteHz24(GB_lianxi[2].Msk,121+5,150,1,WHITE);
	fWriteHz24(GB_lianxi[3].Msk,155+5,150,1,WHITE);
	
	fWriteHz24(GB_bisai[0].Msk,55+5,190,1,WHITE);	 
	fWriteHz24(GB_bisai[1].Msk,88+5,190,1,WHITE);
	fWriteHz24(GB_bisai[2].Msk,121+5,190,1,WHITE);
	fWriteHz24(GB_bisai[3].Msk,155+5,190,1,WHITE);
	
	fWriteHz24(GB_lianji[0].Msk,55+5,230,1,WHITE);	 
	fWriteHz24(GB_lianji[1].Msk,88+5,230,1,WHITE);
	fWriteHz24(GB_lianji[2].Msk,121+5,230,1,WHITE);
	fWriteHz24(GB_lianji[3].Msk,155+5,230,1,WHITE);
	
	fWriteHz24(GB_bangzhu[0].Msk,55+5,270,1,WHITE);	 
	fWriteHz24(GB_bangzhu[1].Msk,88+5,270,1,WHITE);
	fWriteHz24(GB_bangzhu[2].Msk,121+5,270,1,WHITE);
	fWriteHz24(GB_bangzhu[3].Msk,155+5,270,1,WHITE);
	musicst();
}

void rtp_test(void){
	u8 key;
	u8 i=0;	  
	while(1)
	{
	 	key=KEY_Scan(0);
		tp_dev.scan(0); 		
		if(tp_dev.sta&TP_PRES_DOWN)			//触摸屏被按下
		{	
		 	if(tp_dev.x[0]<lcddev.width&&tp_dev.y[0]<lcddev.height)
			{	
				if(tp_dev.x[0]>50&&tp_dev.y[0]>150&&tp_dev.x[0]<(lcddev.width-50)&&tp_dev.y[0]<190)exercise();//清除
				if(tp_dev.x[0]>50&&tp_dev.y[0]>190&&tp_dev.x[0]<(lcddev.width-50)&&tp_dev.y[0]<230)LCD_Clear(BLUE);//清除
				if(tp_dev.x[0]>50&&tp_dev.y[0]>230&&tp_dev.x[0]<(lcddev.width-50)&&tp_dev.y[0]<270)LCD_Clear(RED);//清除
				if(tp_dev.x[0]>50&&tp_dev.y[0]>270&&tp_dev.x[0]<(lcddev.width-50)&&tp_dev.y[0]<310)LCD_Clear(GREEN);//清除	
			}
		}
	}
}


int main(void)
 {	 		    
	delay_init();	    	 //延时函数初始化	  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	uart_init(115200);	 	//串口初始化为115200
	 
 	LED_Init();			     //LED端口初始化
	LCD_Init();	
	KEY_Init();	 	
 	tp_dev.init();
	 
 	POINT_COLOR=RED;//设置字体为红色 
	LCD_Clear(BLACK);
	start_page();
	rtp_test();
}


