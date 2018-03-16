//#include <intrins.h>

#ifndef _NRF24L01_H
#define _NRF24L01_H

extern unsigned char TxBuf[32];
//*********************************************NRF24L01*************************************
#define TX_ADR_WIDTH    5      // 5 uints TX address width
#define RX_ADR_WIDTH    5      // 5 uints RX address width
#define TX_PLOAD_WIDTH  32     // 32 uints TX payload
#define RX_PLOAD_WIDTH  32     // 32 uints TX payload

//***************************************NRF24L01*******************************************************
#define READ_REG        0x00     //
#define WRITE_REG       0x20    //
#define RD_RX_PLOAD     0x61     //
#define WR_TX_PLOAD     0xA0     //
#define FLUSH_TX        0xE1    //
#define FLUSH_RX        0xE2     //
#define REUSE_TX_PL     0xE3     //
#define NOP             0xFF     //
//*************************************SPI(nRF24L01)�****************************************************
#define CONFIG          0x00  //
#define EN_AA           0x01  //
#define EN_RXADDR       0x02  //
#define SETUP_AW        0x03  //
#define SETUP_RETR      0x04  //
#define RF_CH           0x05  //
#define RF_SETUP        0x06  //
#define STATUS          0x07  //
#define OBSERVE_TX      0x08  // �
#define CD              0x09  //
#define RX_ADDR_P0      0x0A  //
#define RX_ADDR_P1      0x0B  //
#define RX_ADDR_P2      0x0C  //
#define RX_ADDR_P3      0x0D  //
#define RX_ADDR_P4      0x0E  //
#define RX_ADDR_P5      0x0F  //
#define TX_ADDR         0x10  //
#define RX_PW_P0        0x11  //
#define RX_PW_P1        0x12  //
#define RX_PW_P2        0x13  //
#define RX_PW_P3        0x14  //
#define RX_PW_P4        0x15  //
#define RX_PW_P5        0x16  //
#define FIFO_STATUS     0x17  //
//**************************************************************************************

//void Delay(unsigned int s);

//void inerDelay_us(unsigned char n);

void init_NRF24L01(void);

unsigned char SPI_RW(unsigned char Buff);

unsigned char SPI_Read(unsigned char reg);

unsigned char SPI_RW_Reg(unsigned char reg, unsigned char value);

unsigned char SPI_Read_Buf(unsigned char reg, unsigned char *pBuf, unsigned char uchars);

unsigned char SPI_Write_Buf(unsigned char reg, unsigned char *pBuf, unsigned uchars);

void SetRX_Mode(void);

void SetTX_Mode(void);

unsigned char nRF24L01_RxPacket(unsigned char* rx_buf);

void nRF24L01_TxPacket(unsigned char * tx_buf);

#endif
// --------------------END OF FILE------------------------
// -------------------------------------------------------
