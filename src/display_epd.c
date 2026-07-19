
#include "display_epd.h"
#include "gpio.h"

#include "stm32f1xx_ll_utils.h"

#define EPD_WIDTH   800 
#define EPD_HEIGHT  480
#define EPD_ARRAY  EPD_WIDTH*EPD_HEIGHT/8  

void EPD_init(void)
{
  EPD_reset();

  EPD_WriteCMD(0x01);     //POWER SETTING
  EPD_WriteData (0x07);
  EPD_WriteData (0x07);    //VGH=20V,VGL=-20V
  EPD_WriteData (0x3f);   //VDH=15V
  EPD_WriteData (0x3f);   //VDL=-15V

  //Enhanced display drive(Add 0x06 command)
  EPD_WriteCMD(0x06);     //Booster Soft Start 
  EPD_WriteData (0x17);
  EPD_WriteData (0x17);   
  EPD_WriteData (0x28);	
  EPD_WriteData (0x17);

  EPD_WriteCMD(0x04); //POWER ON
  LL_mDelay(100);  
  // delay until EPD is not busy
  while(EPD_is_busy());        //waiting for the electronic paper IC to release the idle signal

  EPD_WriteCMD(0X00);   //PANNEL SETTING
  EPD_WriteData(0x1F);   //KW-3f   KWR-2F	BWROTP 0f	BWOTP 1f

  EPD_WriteCMD(0x61);        	//tres
  EPD_WriteData (0x03);           //source 800
  EPD_WriteData (0x20);
  EPD_WriteData (0x01);           //gate 480
  EPD_WriteData (0xE0);  

  EPD_WriteCMD(0X15);
  EPD_WriteData(0x00);

  EPD_WriteCMD(0X50);       //VCOM AND DATA INTERVAL SETTING
  EPD_WriteData(0x10);
  EPD_WriteData(0x07);

  EPD_WriteCMD(0X60);       //TCON SETTING
  EPD_WriteData(0x22);
}

void EPD_update(void)
{
  EPD_WriteCMD(0x12);
  LL_mDelay(2);
  while(EPD_is_busy());           // delay until EPD is not busy
}

void EPD_deepsleep(void)
{
    EPD_WriteCMD(0x50);   //VCOM AND DATA INTERVAL SETTING
    EPD_WriteData(0xf7);     //WBmode:VBDF 17|D7 VBDW 97 VBDB 57    WBRmode:VBDF F7 VBDW 77 VBDB 37  VBDR B7

    EPD_WriteCMD(0x02);   // power off
    while(EPD_is_busy());           // delay until EPD is not busy

    EPD_WriteCMD(0x07);   // deep sleep
    EPD_WriteData(0xa5);
}

void EPD_whitescreen_white(void)
{
  uint32_t i;
  EPD_WriteCMD(0x10);
  for(i = 0; i < EPD_ARRAY; i++){
    EPD_WriteData(0x00);
  }
  EPD_WriteCMD(0x13);
  for(i = 0; i < EPD_ARRAY; i++){
    EPD_WriteData(0x00);
  }
  EPD_update();
}
