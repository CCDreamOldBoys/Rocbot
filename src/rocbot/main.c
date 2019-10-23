/**********************************************************************************************************
                                天穹飞控 —— 致力于打造中国最好的多旋翼开源飞控
                                Github: github.com/loveuav/BlueSkyFlightControl
                                技术讨论：bbs.loveuav.com/forum-68-1.html

 * @作者：   BlueSky
 * @QQ:      352707983
 * @论坛:    爱无人机 bbs.loveuav.com
 * @Q群:     472648354
 * @编译：   Keil ARM MDK 5.25

 * 1缩进等于4空格!
 * 文件编码格式为UTF-8

 * @文件     main.c
 * @说明     程序入口文件
 * @版本  	 V1.0
 * @作者     BlueSky
 * @网站     bbs.loveuav.com
 * @日期     2018.05
**********************************************************************************************************/
#include "sys.h"
#include "delay.h"
#include "usart.h"

#define LED0 PAout(6)	// D2
#define LED1 PAout(7)	// D3


void LED_Init(void)
{    	 
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//Ê¹ÄÜGPIOAÊ±ÖÓ

  //GPIOF9,F10³õÊ¼»¯ÉèÖÃ
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;//LED0ºÍLED1¶ÔÓ¦IO¿Ú
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//ÆÕÍ¨Êä³öÄ£Ê½
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//ÍÆÍìÊä³ö
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//ÉÏÀ­
  GPIO_Init(GPIOA, &GPIO_InitStructure);//³õÊ¼»¯GPIO
	
	GPIO_SetBits(GPIOA,GPIO_Pin_6 | GPIO_Pin_7);//ÉèÖÃ¸ß£¬µÆÃð

}


/**********************************************************************************************************
*函 数 名: main
*功能说明: 系统程序入口
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
int main(void)
{

    delay_init(168);

    LED_Init();		        
	
	
	while(1)
	{
        GPIO_ResetBits(GPIOA,GPIO_Pin_6);  //LED0¶ÔÓ¦Òý½ÅGPIOA.6À­µÍ£¬ÁÁ  µÈÍ¬LED0=0;
        GPIO_ResetBits(GPIOA,GPIO_Pin_7);   //LED1¶ÔÓ¦Òý½ÅGPIOA.7À­¸ß£¬Ãð µÈÍ¬LED1=1;
        delay_ms(1500);  		   //ÑÓÊ±300ms
        GPIO_SetBits(GPIOA,GPIO_Pin_6);	   //LED0¶ÔÓ¦Òý½ÅGPIOA.6À­¸ß£¬Ãð  µÈÍ¬LED0=1;
        GPIO_SetBits(GPIOA,GPIO_Pin_7); //LED1¶ÔÓ¦Òý½ÅGPIOA.7À­µÍ£¬ÁÁ µÈÍ¬LED1=0;
        delay_ms(1500);                     //ÑÓÊ±300ms
	}
}




