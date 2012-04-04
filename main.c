#include <stdio.h>
#include "serial.h"
#include "lcdlib.h"
#include "s3c24xx.h"

int main(void)
{
    char c;
    GPBCON=0x00000400;
    GPBDAT=0x00000000;

    uart0_init();   // ������115200��8N1(8������λ����У��λ��1��ֹͣλ)
    
    while (1)
    {
        printf("\r\n##### Test TFT LCD #####\r\n");
        printf("[1] TFT240320 8Bit\n\r");
        printf("[2] TFT240320 16Bit\n\r");
        printf("[3] TFT640480 8Bit\n\r");
        printf("[4] TFT640480 16Bit\n\r");
        printf("Enter your selection: ");

        c = getc();
        printf("%c\n\r", c);
        switch (c)
        {
            case '1':
            {
                Test_Lcd_Tft_8Bit_240320();
                break;
            }
            
            case '2':
            {
                Test_Lcd_Tft_16Bit_240320();
                break;
            }

            case '3':
            {
                Test_Lcd_Tft_8Bit_640480();
                break;
            }
            
            case '4':
            {
                Test_Lcd_Tft_16Bit_640480();
                break;
            }
            
            default: 
                break;
        }
    }
    
    return 0;
}
