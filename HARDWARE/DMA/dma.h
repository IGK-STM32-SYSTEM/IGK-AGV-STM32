#ifndef __DMA_H
#define	__DMA_H	   
#include "stm32f4xx.h"
#define UART_RX_LEN     250
#define UART_TX_LEN     250
#define UART6_RX_LEN    250
#define UART3_RX_LEN    250
#define UART5_RX_LEN    250
#define RX_LEN 			250
#define TX_LEN          250
#define ADC1_DR_Addr   ((uint32_t)0x4001204C)
extern u16 AD_Value[30][4];
extern u8 sendBuf[UART_RX_LEN],receBuf[UART_RX_LEN],*ps,*pr;
extern u8 rece2_buf[UART_RX_LEN],send2_buf[UART_RX_LEN],rece6_buf[UART6_RX_LEN],rece3_buf[UART3_RX_LEN],send3_buf[UART3_RX_LEN],send6_buf[UART6_RX_LEN];
extern u16 rece_index,rece2_index,rece6_index,rece3_index,rece5_index;
extern u8 SPI_RX_BUFFER[RX_LEN],SPI_TX_BUFFER[TX_LEN];
extern double After_filter[4];
extern u8 rece5_buf[UART5_RX_LEN],send5_buf[UART5_RX_LEN];
void DMA_adc1_Init(void);
void DMA_Uart4_Init(void);
void DMA_Uart2_Init(void);
void DMA_Uart6_Init(void);
void DMA_Uart3_Init(void);
void DMA_Uart5_Init(void);
void Uart4_Start_DMA_Tx(u16 size);
void Uart6_Start_DMA_Tx(u16 size);
void Uart3_Start_DMA_Tx(u16 size);
void Uart2_Start_DMA_Tx(u16 size);
void Uart5_Start_DMA_Tx(u16 size);
#endif






























