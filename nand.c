#include "s3c24xx.h"
#define NAND_SECTOR_SIZE_LP    2048
#define NAND_BLOCK_MASK_LP     (NAND_SECTOR_SIZE_LP - 1)
#define TACLS   1
#define TWRPH0  5
#define TWRPH1  0

#define readp1          	0x00 
#define readp2          	0x30
#define readid         	0x90
#define readstatus      0x70
#define reset          	0xff //��λ

#define send_cmd(cmd)	{NFCMD  = (cmd);  }
#define send_addr(addr)	{NFADDR = (addr); }
#define send_data(date) {NFDATA = (date); }
#define read_byte()	(NFDATA)
#define enable()	{NFCONT &= ~(1<<1);}           //nand flash������ʹ��
#define disable()	{NFCONT |= (1<<1); }
#define enable_rb()	{NFSTAT |= (1<<2); }         //����rnb����ģʽ��
#define check_busy()	{while(!(NFSTAT&(1<<2)));}  //�൱�ڵȴ�rnb��1----��˵��nand flash��æ��
#define waitrb()	{while(!(NFSTAT&(1<<0)));} 

static void nand_reset(void);
/* s3c2440��nand flash�������� */
static void nand_reset(void)
{
	enable();
	enable_rb();
	send_cmd(reset);  // ��λ����
	check_busy();
	disable();
}

void nand_init(void)
{
	NFCONF = (TACLS<<12)|(TWRPH0<<8)|(TWRPH1<<4)|(3<<2)|(1<<1);  /*��Ҫ����|(3<<2)|(1<<1) */
	/* ��ʼ��ECC, ��ֹƬѡ */
        NFCONT = (1<<4)|(1<<1)|(1<<0);
	NFSTAT =0x0;
	/* ��λNAND Flash */
	nand_reset();
}

void nf_readpage(unsigned int block,unsigned int page,unsigned char *dstaddr)
{
	unsigned int i;
	unsigned int blockpage=(block<<6)+page;	
	/*����page���Ե�ַ*/
	nand_reset();
	enable();
	enable_rb();
	
	send_cmd(readp1);	

	send_addr(0x00);
	send_addr(0x00);
	send_addr((blockpage)&0xff);
	send_addr((blockpage>>8)&0xff);
	send_addr((blockpage>>16)&0x01);

	send_cmd(readp2);

	check_busy();

	for(i=0;i<2048;i++)
	{
	dstaddr[i]=read_byte();
	}

	disable();
}
void RdNF2SDRAM(void)
{
	unsigned int i;
	unsigned int start_addr =4096;
	unsigned char * dstaddr = (unsigned char *)0x30000000; 
	unsigned int size = 0x100000;
	for(i = (start_addr >> 11); size > 0;i ++ )
	{
		nf_readpage(i/64,i%64, dstaddr);
		size -= 2048;
		dstaddr += 2048;

	}
}		

