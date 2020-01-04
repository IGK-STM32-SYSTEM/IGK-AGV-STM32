#ifndef __SPI_H
#define __SPI_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//SPI驱动 代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/9
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////

 
 				  	    													  
void SPI2_Init(void);			 //初始化SPI口
void SPI2_SetSpeed(u8 SpeedSet); //设置SPI速度   
u8 SPI2_ReadWriteByte(u8 TxData);//SPI总线读写一个字节
void spi_trans(u8 *rx_buf,u8 *tx_buf,u16 length);

void spi_trans_read(u8 *rx_buf,u8 *tx_data,u16 length);
	
void spi_trans_write(u8 *rx_data,u8 *tx_buffer,u16 length);	

void SPI1_Init(void);			 //初始化SPI口
void SPI1_SetSpeed(u8 SpeedSet); //设置SPI速度 
void SPI1_WriteByte(uint8_t TxData);
uint8_t SPI1_ReadByte(void);
void SPI1_CS_Select(void);
void SPI1_CS_Deselect(void);
void SPI_CrisEnter(void);
	void SPI_CrisExit(void);
#endif

