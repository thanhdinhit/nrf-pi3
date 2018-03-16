#include <main.h>
#include "nrf24l01.h"
//********************************************************************************
unsigned char TxBuf[32]=
{
   0x01,0x02,0x03,0x4,0x05,0x06,0x07,0x08,
   0x09,0x10,0x11,0x12,0x13,0x14,0x15,0x16,
   0x17,0x18,0x19,0x20,0x21,0x22,0x23,0x24,
   0x25,0x26,0x27,0x28,0x29,0x30,0x31,0x32,
};

unsigned char  TX_ADDRESS[TX_ADR_WIDTH]= {0x34,0x43,0x10,0x10,0x01};   //
unsigned char  RX_ADDRESS[RX_ADR_WIDTH]= {0x34,0x43,0x10,0x10,0x01};   //

//**********************************************************************************
/*
void Delay(unsigned int s)
{
   unsigned int i;
   for(i=0; i<s; i++);
   for(i=0; i<s; i++);
}
*/

//******************************************************************************************

//uint    bdata sta;
//sbit   RX_DR   =sta^6;
//sbit   TX_DS   =sta^5;
//sbit   MAX_RT   =sta^4;

/******************************************************************************************/

/******************************************************************************************/
/*
void inerDelay_us(unsigned char n)
{
   for(;n>0;n--)
      delay_us(1);
}
*/
//****************************************************************************************
//*NRF24L01
//***************************************************************************************/
void init_NRF24L01(void)
{
    //inerDelay_us(100);
     delay_us(100);
    digitalWrite(CE,LOW);    // chip enable
    digitalWrite(CSN,HIGH);   // Spi disable
    digitalWrite(SCK,LOW);   // Spi clock line init high
   SPI_Write_Buf(WRITE_REG + TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH);    //
   SPI_Write_Buf(WRITE_REG + RX_ADDR_P0, RX_ADDRESS, RX_ADR_WIDTH); //
   SPI_RW_Reg(WRITE_REG + EN_AA, 0x01);      // EN P0, 2-->P1
   SPI_RW_Reg(WRITE_REG + EN_RXADDR, 0x01);  //Enable data P0
   SPI_RW_Reg(WRITE_REG + RF_CH, 0);        // Chanel 0 RF = 2400 + RF_CH* (1or 2 M)
   SPI_RW_Reg(WRITE_REG + RX_PW_P0, RX_PLOAD_WIDTH); // Do rong data truyen 32 byte
   SPI_RW_Reg(WRITE_REG + RF_SETUP, 0x07);         // 1M, 0dbm
   SPI_RW_Reg(WRITE_REG + CONFIG, 0x0e);          // Enable CRC, 2 byte CRC, Send

}
/****************************************************************************************************/
//unsigned char SPI_RW(unsigned char Buff)
//NRF24L01
/****************************************************************************************************/
unsigned char SPI_RW(unsigned char Buff)
{
      unsigned char bit_ctr;
      for(bit_ctr=0;bit_ctr<8;bit_ctr++) // output 8-bit
      {
         if((Buff & 0x80)==0x80) digitalWrite(MOSI,HIGH) ;   // output 'uchar', MSB to MOSI    
         else digitalWrite(MOSI,LOW);
         delay_us(5);
         Buff = (Buff << 1);           // shift next bit into MSB..
         digitalWrite(SCK,HIGH);                      // Set SCK high..          
         delay_us(5);
         Buff |= MISO;               // capture current MISO bit
         digitalWrite(SCK,LOW);                    // ..then set SCK low again
      }
      return(Buff);                   // return read uchar
}
/****************************************************************************************************/
//unsigned char SPI_Read(unsigned char reg)
//NRF24L01
/****************************************************************************************************/
unsigned char SPI_Read(unsigned char reg)
{
   unsigned char reg_val;

   digitalWrite(CSN,LOW);                // CSN low, initialize SPI communication...
   SPI_RW(reg);            // Select register to read from..
   reg_val = SPI_RW(0);    // ..then read registervalue
   digitalWrite(CSN,HIGH);                // CSN high, terminate SPI communication

   return(reg_val);        // return register value
}
/****************************************************************************************************/
//unsigned char SPI_RW_Reg(unsigned char reg, unsigned char value)
/****************************************************************************************************/
unsigned char SPI_RW_Reg(unsigned char reg,unsigned char value)
{
   unsigned char status1;

   digitalWrite(CSN,LOW);                   // CSN low, init SPI transaction
   status1 = SPI_RW(reg);      // select register
   SPI_RW(value);             // ..and write value to it..
   digitalWrite(CSN,HIGH);                   // CSN high again

   return(status1);            // return nRF24L01 status uchar
}
/****************************************************************************************************/
//unsigned char SPI_Read_Buf(unsigned char reg, unsigned char *pBuf, unsigned char uchars)
//
/****************************************************************************************************/
unsigned char SPI_Read_Buf(unsigned char reg,unsigned char *pBuf,unsigned char uchars)
{
   unsigned char status1,uchar_ctr;

   digitalWrite(CSN,LOW);                          // Set CSN low, init SPI tranaction
   status1 = SPI_RW(reg);             // Select register to write to and read status uchar

   for(uchar_ctr=0;uchar_ctr<uchars;uchar_ctr++)
      pBuf[uchar_ctr] = SPI_RW(0);    //

   digitalWrite(CSN,HIGH);

   return(status1);                    // return nRF24L01 status uchar
}
/*********************************************************************************************************/
//uint SPI_Write_Buf(uchar reg, uchar *pBuf, uchar uchars)
/*****************************************************************************************************/
unsigned char SPI_Write_Buf(unsigned char reg,unsigned char *pBuf,unsigned char uchars)
{
   unsigned char status1,uchar_ctr;
   digitalWrite(CSN,LOW);            //SPI
   status1 = SPI_RW(reg);
   for(uchar_ctr=0; uchar_ctr<uchars; uchar_ctr++) //
      SPI_RW(*pBuf++);
   digitalWrite(CSN,HIGH);           //SPI
   return(status1);    //
}
/****************************************************************************************************/
//void SetRX_Mode(void)
//
/****************************************************************************************************/
void SetRX_Mode(void)
{
   digitalWrite(CE,LOW);
   SPI_Write_Buf(RD_RX_PLOAD,NOP, TX_PLOAD_WIDTH);
   SPI_RW_Reg(WRITE_REG + CONFIG, 0x0f);         // Enable CRC, 2 byte CRC, Recive
   digitalWrite(CE,HIGH);
   delay_us(130);    //
}

/****************************************************************************************************/
//void SetTX_Mode(void)
//
/****************************************************************************************************/
void SetTX_Mode(void)
{
   digitalWrite(CE,LOW);
   SPI_RW_Reg(WRITE_REG + CONFIG, 0x0e);         // Enable CRC, 2 byte CRC, Send
   digitalWrite(CE,HIGH);
   delay_us(130);    //
}

/******************************************************************************************************/
//unsigned char nRF24L01_RxPacket(unsigned char* rx_buf)
/******************************************************************************************************/
unsigned char nRF24L01_RxPacket(unsigned char* rx_buf)
{
   unsigned char revale=0;
   unsigned char sta;
  
   //if(RX_DR)            // Data in RX FIFO
   sta=SPI_Read(STATUS);   // Read Status  
   if((sta&0x40)!=0)      // Data in RX FIFO
   {
      digitalWrite(CE,LOW);          //SPI
      SPI_Read_Buf(RD_RX_PLOAD,rx_buf,TX_PLOAD_WIDTH);// read receive payload from RX_FIFO buffer
      revale =1;
   }
   SPI_RW_Reg(WRITE_REG+STATUS,sta);   
   return revale;
}
/***********************************************************************************************************/
//void nRF24L01_TxPacket(unsigned char * tx_buf)
//
/**********************************************************************************************************/
void nRF24L01_TxPacket(unsigned char * tx_buf)
{
   digitalWrite(CE,LOW);  
   SPI_Write_Buf(WRITE_REG + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH); // Send Address
   SPI_Write_Buf(WR_TX_PLOAD, tx_buf, TX_PLOAD_WIDTH);           //send data
   SPI_RW_Reg(WRITE_REG + CONFIG, 0x0e);          // Send Out
   digitalWrite(CE,HIGH);       
}

// --------------------END OF FILE------------------------
// -------------------------------------------------------
