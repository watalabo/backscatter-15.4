#include "mbed.h"
#include    "stm32f4xx.h"

volatile int    SysState = 0;
char            ustr[256], ubuf[256];

#define     MyPAN   0xBF01

//#define     MyMAC   0xBB01
//#define     MyMAC   0xBB02
//#define     MyMAC   0xBB03
#define     MyMAC   0xBB04
//#define     MyMAC   0xBB05
//#define     MyMAC   0xBB06
//#define     MyMAC   0xBB07
//#define     MyMAC   0xBB08
//#define     MyMAC   0xBB09

//---------------------------------------------------------------------------
// CRC Calc
//---------------------------------------------------------------------------
static unsigned short CRC16Table[ 256 ] = {
    0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50A5, 0x60C6, 0x70E7,
    0x8108, 0x9129, 0xA14A, 0xB16B, 0xC18C, 0xD1AD, 0xE1CE, 0xF1EF,
    0x1231, 0x0210, 0x3273, 0x2252, 0x52B5, 0x4294, 0x72F7, 0x62D6,
    0x9339, 0x8318, 0xB37B, 0xA35A, 0xD3BD, 0xC39C, 0xF3FF, 0xE3DE,
    0x2462, 0x3443, 0x0420, 0x1401, 0x64E6, 0x74C7, 0x44A4, 0x5485,
    0xA56A, 0xB54B, 0x8528, 0x9509, 0xE5EE, 0xF5CF, 0xC5AC, 0xD58D,
    0x3653, 0x2672, 0x1611, 0x0630, 0x76D7, 0x66F6, 0x5695, 0x46B4,
    0xB75B, 0xA77A, 0x9719, 0x8738, 0xF7DF, 0xE7FE, 0xD79D, 0xC7BC,

    0x48C4, 0x58E5, 0x6886, 0x78A7, 0x0840, 0x1861, 0x2802, 0x3823,
    0xC9CC, 0xD9ED, 0xE98E, 0xF9AF, 0x8948, 0x9969, 0xA90A, 0xB92B,
    0x5AF5, 0x4AD4, 0x7AB7, 0x6A96, 0x1A71, 0x0A50, 0x3A33, 0x2A12,
    0xDBFD, 0xCBDC, 0xFBBF, 0xEB9E, 0x9B79, 0x8B58, 0xBB3B, 0xAB1A,
    0x6CA6, 0x7C87, 0x4CE4, 0x5CC5, 0x2C22, 0x3C03, 0x0C60, 0x1C41,
    0xEDAE, 0xFD8F, 0xCDEC, 0xDDCD, 0xAD2A, 0xBD0B, 0x8D68, 0x9D49,
    0x7E97, 0x6EB6, 0x5ED5, 0x4EF4, 0x3E13, 0x2E32, 0x1E51, 0x0E70,
    0xFF9F, 0xEFBE, 0xDFDD, 0xCFFC, 0xBF1B, 0xAF3A, 0x9F59, 0x8F78,

    0x9188, 0x81A9, 0xB1CA, 0xA1EB, 0xD10C, 0xC12D, 0xF14E, 0xE16F,
    0x1080, 0x00A1, 0x30C2, 0x20E3, 0x5004, 0x4025, 0x7046, 0x6067,
    0x83B9, 0x9398, 0xA3FB, 0xB3DA, 0xC33D, 0xD31C, 0xE37F, 0xF35E,
    0x02B1, 0x1290, 0x22F3, 0x32D2, 0x4235, 0x5214, 0x6277, 0x7256,
    0xB5EA, 0xA5CB, 0x95A8, 0x8589, 0xF56E, 0xE54F, 0xD52C, 0xC50D,
    0x34E2, 0x24C3, 0x14A0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
    0xA7DB, 0xB7FA, 0x8799, 0x97B8, 0xE75F, 0xF77E, 0xC71D, 0xD73C,
    0x26D3, 0x36F2, 0x0691, 0x16B0, 0x6657, 0x7676, 0x4615, 0x5634,

    0xD94C, 0xC96D, 0xF90E, 0xE92F, 0x99C8, 0x89E9, 0xB98A, 0xA9AB,
    0x5844, 0x4865, 0x7806, 0x6827, 0x18C0, 0x08E1, 0x3882, 0x28A3,
    0xCB7D, 0xDB5C, 0xEB3F, 0xFB1E, 0x8BF9, 0x9BD8, 0xABBB, 0xBB9A,
    0x4A75, 0x5A54, 0x6A37, 0x7A16, 0x0AF1, 0x1AD0, 0x2AB3, 0x3A92,
    0xFD2E, 0xED0F, 0xDD6C, 0xCD4D, 0xBDAA, 0xAD8B, 0x9DE8, 0x8DC9,
    0x7C26, 0x6C07, 0x5C64, 0x4C45, 0x3CA2, 0x2C83, 0x1CE0, 0x0CC1,
    0xEF1F, 0xFF3E, 0xCF5D, 0xDF7C, 0xAF9B, 0xBFBA, 0x8FD9, 0x9FF8,
    0x6E17, 0x7E36, 0x4E55, 0x5E74, 0x2E93, 0x3EB2, 0x0ED1, 0x1EF0,
};


extern unsigned short getMemCRC16(unsigned short crc16, unsigned const char buff[], int size )
{
    while(size != 0){
        crc16 = CRC16Table[(crc16 >> 8) ^ *buff] ^ (crc16 << 8);        // Left Shift
        buff++;
        size--;
    }
    return crc16;
}

//===========================================================================
// ZigBee Mod
//---------------------------------------------------------------------------
unsigned int Z_Chip[] = {
    0x744AC39B, 0x44AC39B7, 0x4AC39B74, 0xAC39B744,
    0xC39B744A, 0x39B744AC, 0x9B744AC3, 0xB744AC39,
    0xDEE06931, 0xEE06931D, 0xE06931DE, 0x06931DEE,
    0x6931DEE0, 0x931DEE06, 0x31DEE069, 0x1DEE0693
};

unsigned int    TxBuf[4096], TxBuf_B[4096];
unsigned char   TxPkt[1024];
volatile int    TxBuf_Dp = 0, TxBuf_B_Dp = 0;
volatile int    TxBuf_Op = 0;

//---------------------------------------------------------------------------
// Make ZigBee Mod  (Carrier=8, Mod=XX)
//---------------------------------------------------------------------------
int ModLen = 12;

void MakeZigBeeMod(unsigned char *buf, int len)
{
    int     n, Obuf;
    int     HalfByteP, Symbol, ChipCnt;
    int     Ibit, Qbit;
    int     msft, osft;
    unsigned int    MODg;
    
    for(n = 0; n < 4096; n++) TxBuf_B[n] = 0;
    TxBuf_B[0] = 0x0F0F0F0F;
    TxBuf_B[1] = 0x0F0F0F0F;
    TxBuf_B[2] = 0x0F0F0F0F;
    TxBuf_B[3] = 0x0F0F0F0F;
    TxBuf_B_Dp = 4;
    Qbit = 0;
    msft = 0;
    osft = 31;
    Obuf = 0;
    for(HalfByteP = 0; HalfByteP < len * 2; HalfByteP++) {
        // Bit to Symbol
        if((HalfByteP & 0x01) == 0) {
            Symbol = buf[HalfByteP / 2] & 0x0F;
        } else {
            Symbol = buf[HalfByteP / 2] >> 4;
        }
        for(ChipCnt = 0; ChipCnt < 32; ChipCnt++) {
            // Symbol to Chip
            if((ChipCnt & 0x01) == 0) {
                Ibit = (Z_Chip[Symbol] >> ChipCnt) & 0x01;
                if(Ibit == 0) Ibit = -1;
                Qbit = 0;
            } else {
                Qbit = (Z_Chip[Symbol] >> ChipCnt) & 0x01;
                if(Qbit == 0) Qbit = -1;
                Ibit = 0;
            }
            // O_QPSK modulation
            if(Ibit ==  1 && Qbit ==  0) {
                //MODg = 0x0F0F0F0F;
                MODg = 0xC3C3C3C3;
            } else if(Ibit ==  0 && Qbit ==  1) {
                //MODg = 0x1E1E1E1E;
                MODg = 0x0F0F0F0F;
            } else if(Ibit == -1 && Qbit ==  0) {
                //MODg = 0x3C3C3C3C;
                MODg = 0x3C3C3C3C;
            } else {
                //MODg = 0x78787878;
                MODg = 0xF0F0F0F0;
            }
            for(n = 0; n < ModLen; n++) {
                Obuf |= ((MODg >> msft) & 0x01) << osft;
                msft++;
                msft &= 0x1F;
                osft--;
                if(osft < 0) {
                    osft = 31;
                    TxBuf_B[TxBuf_B_Dp] = Obuf;
                    Obuf = 0;
                    TxBuf_B_Dp++;
                }
            }
        }
    }
    TxBuf_B[TxBuf_B_Dp] = Obuf;
}

//---------------------------------------------------------------------------
// Make ZigbeeTx Packet
//---------------------------------------------------------------------------
unsigned char BitSwap8[] = {
    0x00, 0x80, 0x40, 0xC0, 0x20, 0xA0, 0x60, 0xE0, 0x10, 0x90, 0x50, 0xD0, 0x30, 0xB0, 0x70, 0xF0,
    0x08, 0x88, 0x48, 0xC8, 0x28, 0xA8, 0x68, 0xE8, 0x18, 0x98, 0x58, 0xD8, 0x38, 0xB8, 0x78, 0xF8,
    0x04, 0x84, 0x44, 0xC4, 0x24, 0xA4, 0x64, 0xE4, 0x14, 0x94, 0x54, 0xD4, 0x34, 0xB4, 0x74, 0xF4,
    0x0C, 0x8C, 0x4C, 0xCC, 0x2C, 0xAC, 0x6C, 0xEC, 0x1C, 0x9C, 0x5C, 0xDC, 0x3C, 0xBC, 0x7C, 0xFC,
    0x02, 0x82, 0x42, 0xC2, 0x22, 0xA2, 0x62, 0xE2, 0x12, 0x92, 0x52, 0xD2, 0x32, 0xB2, 0x72, 0xF2,
    0x0A, 0x8A, 0x4A, 0xCA, 0x2A, 0xAA, 0x6A, 0xEA, 0x1A, 0x9A, 0x5A, 0xDA, 0x3A, 0xBA, 0x7A, 0xFA,
    0x06, 0x86, 0x46, 0xC6, 0x26, 0xA6, 0x66, 0xE6, 0x16, 0x96, 0x56, 0xD6, 0x36, 0xB6, 0x76, 0xF6,
    0x0E, 0x8E, 0x4E, 0xCE, 0x2E, 0xAE, 0x6E, 0xEE, 0x1E, 0x9E, 0x5E, 0xDE, 0x3E, 0xBE, 0x7E, 0xFE,
    0x01, 0x81, 0x41, 0xC1, 0x21, 0xA1, 0x61, 0xE1, 0x11, 0x91, 0x51, 0xD1, 0x31, 0xB1, 0x71, 0xF1,
    0x09, 0x89, 0x49, 0xC9, 0x29, 0xA9, 0x69, 0xE9, 0x19, 0x99, 0x59, 0xD9, 0x39, 0xB9, 0x79, 0xF9,
    0x05, 0x85, 0x45, 0xC5, 0x25, 0xA5, 0x65, 0xE5, 0x15, 0x95, 0x55, 0xD5, 0x35, 0xB5, 0x75, 0xF5,
    0x0D, 0x8D, 0x4D, 0xCD, 0x2D, 0xAD, 0x6D, 0xED, 0x1D, 0x9D, 0x5D, 0xDD, 0x3D, 0xBD, 0x7D, 0xFD,
    0x03, 0x83, 0x43, 0xC3, 0x23, 0xA3, 0x63, 0xE3, 0x13, 0x93, 0x53, 0xD3, 0x33, 0xB3, 0x73, 0xF3,
    0x0B, 0x8B, 0x4B, 0xCB, 0x2B, 0xAB, 0x6B, 0xEB, 0x1B, 0x9B, 0x5B, 0xDB, 0x3B, 0xBB, 0x7B, 0xFB,
    0x07, 0x87, 0x47, 0xC7, 0x27, 0xA7, 0x67, 0xE7, 0x17, 0x97, 0x57, 0xD7, 0x37, 0xB7, 0x77, 0xF7,
    0x0F, 0x8F, 0x4F, 0xCF, 0x2F, 0xAF, 0x6F, 0xEF, 0x1F, 0x9F, 0x5F, 0xDF, 0x3F, 0xBF, 0x7F, 0xFF
};

int   TxSEQ = 0;

int MakeZigbeeTxPacket(unsigned char *buf, int len, unsigned short MyPan, unsigned short MyMac)
{
    int     n, m;
unsigned char   CRCdat[128];
unsigned short  crc;

    TxPkt[ 0] = 0;                      // Preamble0
    TxPkt[ 1] = 0;                      // Preamble1
    TxPkt[ 2] = 0;                      // Preamble2
    TxPkt[ 3] = 0;                      // Preamble3
    TxPkt[ 4] = 0xA7;                   // SFD
    TxPkt[ 5] = len + 13;               // FrameLength   
    TxPkt[ 6] = 0x41;                   // FCF_L
    TxPkt[ 7] = 0x88;                   // FCF_H
    TxPkt[ 8] = TxSEQ & 0xFF;           // Sequence Number
    TxPkt[ 9] = 0xFF;                   // DstPAN_L
    TxPkt[10] = 0xFF;                   // DstPAN_H
    TxPkt[11] = 0xFF;                   // DstMAC_L
    TxPkt[12] = 0xFF;                   // DstMAC_H
    TxPkt[13] =  MyPan & 0x00FF;        // SrcPAN_L
    TxPkt[14] = (MyPan & 0xFF00) >> 8;  // SrcPAN_H
    TxPkt[15] =  MyMac & 0x00FF;        // SrcMAC_L
    TxPkt[16] = (MyMac & 0xFF00) >> 8;  // SrcMAC_H
    // Payload
    for(n = 0; n < len; n++) {
        TxPkt[n + 17] = buf[n];
    }
    // CRC
    for(m = 0; m < len + 11; m++) {
        CRCdat[m] = BitSwap8[TxPkt[m + 6] & 0xFF];
    }
    crc = 0;
    crc = getMemCRC16(crc, CRCdat, len + 11);
    //sprintf(ustr, "CRC=%04X\r\n", crc); USB_Tx(ustr);
    TxPkt[n + 17] = BitSwap8[(crc >> 8) & 0xFF];
    TxPkt[n + 18] = BitSwap8[crc & 0xFF];
    TxSEQ++;
    return(n + 19);
}

//---------------------------------------------------------------------------
// I2S3 Tx End ISR
//---------------------------------------------------------------------------
volatile int TxStart = 0;

static void I2S3_IRQ_Handler(void)
{
    __disable_irq();
    if(TxStart == 0) {
        SPI3->DR = 0x0000;
        __enable_irq();
        return;
    } 
    
    //GPIOB->ODR |= 0x1000;  
    //GPIOB->ODR &= 0xEFFF;
    
    //==========================================
    // It will not work properly without this
    GPIOB->ODR |= 0x0004;       // PB_2 -> H
    GPIOB->ODR &= 0xFFFB;       // PB_2 -> L
    //------------------------------------------
      
    if((TxBuf_Op / 2) < TxBuf_Dp) {
        //printf("O=%d D=%d %04X\r\n", TxBuf_Op, TxBuf_Dp, SPI1->SR);
        if((TxBuf_Op & 0x01) == 0) {
            SPI3->DR = TxBuf[TxBuf_Op / 2] >> 16;
        } else {
            SPI3->DR = TxBuf[TxBuf_Op / 2] & 0x0000FFFF;
        }
        TxBuf_Op++;
    } else {
        SPI3->DR = 0x0000;
        TxStart = 0;
    }
    __enable_irq();
}

//---------------------------------------------------------------------------
// Backscatter Tx
//---------------------------------------------------------------------------
int  TxCnt = 0;

void zTx()
{
    for(TxBuf_Dp = 0; TxBuf_Dp < TxBuf_B_Dp; TxBuf_Dp++) {
        TxBuf[TxBuf_Dp] = TxBuf_B[TxBuf_Dp];
    }
    if(TxBuf_Dp == 0) return;
    TxBuf_Op = 0;
    TxStart  = 1;
    while(TxStart != 0);
    //printf("%5d Send Modulation Code[%d x 32bit][%d]\r\n", TxCnt, TxBuf_Dp, TxBuf_Op);
    TxCnt++;
}

//---------------------------------------------------------------------------
// CC2520_SPI_Cnt
//---------------------------------------------------------------------------
char * CC2520_SPI(char *TxD, int Len)
{
    int     n;
static char str[128];
    
    GPIOA->ODR &= 0xFFEF;                   // PA_4:CSn  H -> L
    
    SPI1->DR = TxD[0];
    for(n = 1; n < Len; n++) {
        while((SPI1->SR & 0x02) == 0);      // Wait TXE  Flug = 1
        SPI1->DR = TxD[n];
        while((SPI1->SR & 0x01) == 0);      // Wait RXNE Flug = 1
        str[n - 1] = SPI1->DR;
    }
    while((SPI1->SR & 0x01) == 0);          // Wait RXNE Flug = 1
    str[n - 1] = SPI1->DR;
    while((SPI1->SR & 0x02) == 0);          // Wait TXE  Flug = 1
    while((SPI1->SR & 0x80) != 0);          // Wait BSY = 0
    GPIOA->ODR |= 0x0010;                   // PA_4:CSn  L -> H
    return(str);
}

//---------------------------------------------------------------------------
// SetI2sClock(Mod)
//---------------------------------------------------------------------------
int SetI2sClock(int Mod)
{
    switch(Mod) {
        case    9:
                // PLLI2Sclock = 144MHz   { (XTAL=8MHz * (PLLI2SN=288 / PLLM=8)) / PLLI2SR=2 }
                RCC->PLLI2SCFGR  = 2 << 28;         // PLLI2SR = 2
                RCC->PLLI2SCFGR |= 2 << 24;         // PLLI2SQ = 2
                RCC->PLLI2SCFGR |= 288 << 6;        // PLLI2SN = 288   Max:9bit
                // (144Mhz / I2SDIV=2) / 4
                SPI3->I2SPR      = 0x0000;          // MCKOE=0, I2SODD=0
                SPI3->I2SPR     |= 2;               // I2SDIV  = 2
                return(18000);
        case    10:
                // PLLI2Sclock = 160MHz   { (XTAL=8MHz * (PLLI2SN=320 / PLLM=8)) / PLLI2SR=2 }
                RCC->PLLI2SCFGR  = 2 << 28;         // PLLI2SR = 2
                RCC->PLLI2SCFGR |= 2 << 24;         // PLLI2SQ = 2
                RCC->PLLI2SCFGR |= 320 << 6;        // PLLI2SN
                // (160Mhz / I2SDIV=2) / 4
                SPI3->I2SPR      = 0x0000;          // MCKOE=0, I2SODD=0
                SPI3->I2SPR     |= 2;               // I2SDIV  = 2
                return(20000);
        case    11:
                // PLLI2Sclock = 176MHz   { (XTAL=8MHz * (PLLI2SN=352 / PLLM=8)) / PLLI2SR=2 }
                RCC->PLLI2SCFGR  = 2 << 28;         // PLLI2SR = 2
                RCC->PLLI2SCFGR |= 2 << 24;         // PLLI2SQ = 2
                RCC->PLLI2SCFGR |= 352 << 6;        // PLLI2SN
                // (176Mhz / I2SDIV=2) / 4
                SPI3->I2SPR      = 0x0000;          // MCKOE=0, I2SODD=0
                SPI3->I2SPR     |= 2;               // I2SDIV  = 2
                return(22000);
        case    12:
                // PLLI2Sclock = 192MHz   { (XTAL=8MHz * (PLLI2SN=384 / PLLM=8)) / PLLI2SR=2 }
                RCC->PLLI2SCFGR  = 2 << 28;         // PLLI2SR = 2
                RCC->PLLI2SCFGR |= 2 << 24;         // PLLI2SQ = 2
                RCC->PLLI2SCFGR |= 384 << 6;        // PLLI2SN
                // (192Mhz / I2SDIV=2) / 4
                SPI3->I2SPR      = 0x0000;          // MCKOE=0, I2SODD=0
                SPI3->I2SPR     |= 2;               // I2SDIV  = 2
                return(24000);
        case    13:
                // PLLI2Sclock = 208MHz   { (XTAL=8MHz * (PLLI2SN=416 / PLLM=8)) / PLLI2SR=2 }
                RCC->PLLI2SCFGR  = 2 << 28;         // PLLI2SR = 2
                RCC->PLLI2SCFGR |= 2 << 24;         // PLLI2SQ = 2
                RCC->PLLI2SCFGR |= 416 << 6;        // PLLI2SN
                // (192Mhz / I2SDIV=2) / 4
                SPI3->I2SPR      = 0x0000;          // MCKOE=0, I2SODD=0
                SPI3->I2SPR     |= 2;               // I2SDIV  = 2
                return(26000);
        default:
                return(0);
    }
}

//---------------------------------------------------------------------------
// MPU Init
//---------------------------------------------------------------------------
void InitMPU()
{
    int     k;
    
    //------------------------------------------
    // Port_A SetUp
    //------------------------------------------
    RCC->AHB1ENR  |= 0x00000001;        // GPIOA Clock Enable
    GPIOA->MODER = 0x4000A9A0;
    // 1098 7654 3210 9876 FEDC BA98 7654 3210
    // 0100 0000 0000 0000 1010 1001 1010 0000
    // GPIOA->MODER   = 0;
    // GPIOA->MODER  |= 1 << 30;           // MODER15=Do PA_15=GP5(NotUse)
    // GPIOA->MODER  |= 0 << 24;           // MODER12=Di PA_12=GP4(SFD)
    // GPIOA->MODER  |= 0 << 22;           // MODER11=Di PA_11=GP3(CCA)
    // GPIOA->MODER  |= 0 << 20;           // MODER10=Di PA_10=GP2(FIFOP)
    // GPIOA->MODER  |= 0 << 18;           // MODER9 =Di PA_9 =GP1(FIFO)
    // GPIOA->MODER  |= 0 << 16;           // MODER8 =Di PA_8 =GP0(1MHzClock)
    // GPIOA->MODER  |= 2 << 14;           // MODER7 =AF PA_7 =SPI1_MOSI
    // GPIOA->MODER  |= 2 << 12;           // MODER6 =AF PA_6 =SPI1_MISO
    // GPIOA->MODER  |= 2 << 10;           // MODER5 =AF PA_5 =SPI1_SCK
    // GPIOA->MODER  |= 1 <<  8;           // MODER4 =Do PA_4 =CSn(CC2520)
    // GPIOA->MODER  |= 2 <<  6;           // MODER3 =AF PA_3 =USART_Rx
    // GPIOA->MODER  |= 2 <<  4;           // MODER2 =AF PA_2 =USART_Tx
    GPIOA->AFR[0] = 0x55557700;
    // 1098 7654 3210 9876 FEDC BA98 7654 3210
    // 0101 0101 0101 0101 0111 0111 0000 0000
    // GPIOA->AFR[0]  = 0;                 // PA_7_0
    // GPIOA->AFR[0] |= 5 << 28;           // AFRL7=AF5
    // GPIOA->AFR[0] |= 5 << 24;           // AFRL6=AF5
    // GPIOA->AFR[0] |= 5 << 20;           // AFRL5=AF5
    // GPIOA->AFR[0] |= 5 << 16;           // AFRL4=AF5
    // GPIOA->AFR[0] |= 7 << 12;           // AFRL3=AF7
    // GPIOA->AFR[0] |= 7 <<  8;           // AFRL2=AF7
    GPIOA->ODR     = 0x0010;
    //------------------------------------------
    // Port_B SetUp
    //------------------------------------------
    RCC->AHB1ENR  |= 0x00000002;        // GPIOB Clock Enable 
    GPIOB->MODER = 0x41000915;
    // 1098 7654 3210 9876 FEDC BA98 7654 3210
    // 0100 0001 0000 0000 0000 1001 0001 0101
    // GPIOB->MODER   = 0;
    // GPIOB->MODER  |= 1 << 30;           // MODER15=Do PB_15=RESETn(CC2520)
    // GPIOB->MODER  |= 1 << 24;           // MODER12=Do PB_12=LED
    // GPIOB->MODER  |= 2 << 10;           // MODER5 =AF PB_5 =I2S3_SD
    // GPIOB->MODER  |= 1 <<  8;           // MODER4 =Do PB_4 =15.4Rx
    // GPIOB->MODER  |= 1 <<  4;           // MODER2 =Do PB_2 =NC[I2S3_IRQ_Handler()]
    // GPIOB->MODER  |= 1 <<  2;           // MODER1 =Do PB_1 =CSB(BME260)
    // GPIOB->MODER  |= 1;                 // MODER0 =Do PB_0 =VREG_EN(CC2520)
    GPIOB->AFR[0] = 0x00600000;
    // 1098 7654 3210 9876 FEDC BA98 7654 3210
    // 0000 0000 0110 0000 0000 0000 0000 0000
    // GPIOB->AFR[0]  = 0;                 // PA_7_0
    // GPIOB->AFR[0] |= 6 << 20;           // AFRL5=AF6
    GPIOB->OSPEEDR = 0x55555555;
    GPIOB->ODR     = 0x0012;
    //------------------------------------------
    // RCC SetUp
    //------------------------------------------
    RCC->CR       |= 1 << 16;                   // HSE ON  
    while((RCC->CR & 0x00020000) == 0);         // Wait HSE oscillator ready
    k = RCC->CFGR;
    RCC->CFGR      = (k & 0xFFFFFFFC) | 0x01;   // System clock = HSE
    while((RCC->CFGR & 0x0C) != 0x04);
    RCC->CR       &= 0xEAFFFFFE;                // PLLSAI & PLLI2S & HSI PLL -> OFF
    while((RCC->CR & 0x3F000000) != 0x00000000);
    RCC->PLLCFGR   = 0x27405A08;                // PLL Set To HSE 180MHz
    RCC->CR       |= 0x05000000;                // PLL  & PLLI2S -> ON
    while((RCC->CR & 0x0A000000) != 0x0A000000);
    RCC->CFGR      = (k & 0xFFFFFFFC) | 0x02;   // System clock = PLL_P
    while((RCC->CFGR & 0x0C) != 0x08);
    //------------------------------------------
    // SPI1 SetUp
    //------------------------------------------
    RCC->APB2ENR  |= 0x00001000;        // SPI1 Clock Enable
    SPI1->CR1 = 0x435C;
    // FEDC BA98 7654 3210
    // 0100 0011 0101 1100
    // SPI1->CR1      = 0;
    // SPI1->CR1     |= 1 << 14;           // BIDIOE:   Output Enable
    // SPI1->CR1     |= 1 <<  9;           // SSM:      Software slave management enabled
    // SPI1->CR1     |= 1 <<  8;           // SSI:      NSS pin and the IO value of the NSS pin is ignored
    // SPI1->CR1     |= 1 <<  6;           // SPE:      SPI1 Enable
    // SPI1->CR1     |= 3 <<  3;           // BR[2:0]:  Baud rate control Fpclk(96MHz) / 0=2_7=256 / 3=6MHz
    // SPI1->CR1     |= 1 <<  2;           // MSTR:     Mster Mode = 1
    //------------------------------------------
    // I2S3 SetUp
    //------------------------------------------
    RCC->APB1ENR    |= 0x00008000;      // SPI3 Clock Enable
    SPI3->I2SCFGR = 0x0E10;
    // FEDC BA98 7654 3210
    // 0000 1110 0001 0000
    // SPI3->I2SCFGR  = 0x0800;            // I2SMOD =I2S Mode
    // SPI3->I2SCFGR |= 0x0400;            // I2SE   =I2S Enable
    // SPI3->I2SCFGR |= 2 << 8;            // I2SCFG 2=MasterTx 3=MasterRx
    // SPI3->I2SCFGR |= 0 << 7;            // PCMSYNC=ShortFrameSync
    // SPI3->I2SCFGR |= 1 << 4;            // I2SSTD =MSB(LeftStart)
    // SPI3->I2SCFGR |= 0 << 3;            // CKPOL  =Low to High
    k = SetI2sClock(12);
    //------------------------------------------
    // ADC1 SetUp
    //------------------------------------------
    RCC->APB2ENR  |= 0x00000100;        // ADC1EN:   ADC1 clock enable
    ADC1->SQR3     = 17;                // SEQ1[4:0]=ADC_IN17:VREFINT
    // ADC1->CR1      = 0;                 // Default 12bit
    ADC1->CR2      = 0x00000001;        // ADON=1:
    ADC->CCR       = 0x00800000;        // Temp ON
}

//---------------------------------------------------------------------------
// Init CC2520
//---------------------------------------------------------------------------
void InitCC2420()
{
    char    Sin[128];

    //------------------------------------------
    // RESETn PowerON Sequence
    //------------------------------------------
    GPIOB->ODR |= 0x0001;               // PB_0 :VREG_EN L -> H
    wait_us(100);
    GPIOB->ODR |= 0x8000;               // PB_15:RESETn  L -> H
    wait_us(10);
    GPIOA->ODR &= 0xFFEF;               // PA_4:CSn  H -> L
    wait_us(200);
    GPIOA->ODR |= 0x0010;               // PA_4:CSn  L -> H
    //------------------------------------------
    // REG Set
    //------------------------------------------
    // MEMWR(CCACTRL0)
    ustr[0] = 0x20; ustr[1] = 0x36; ustr[2] = 0xF8;
    strcpy(Sin, CC2520_SPI(ustr, 3));
    // MEMWR(MDMCTRL0)
    ustr[0] = 0x20; ustr[1] = 0x46; ustr[2] = 0x85;
    strcpy(Sin, CC2520_SPI(ustr, 3));
    // MEMWR(MDMCTRL1)
    ustr[0] = 0x20; ustr[1] = 0x47; ustr[2] = 0x14;
    strcpy(Sin, CC2520_SPI(ustr, 3));
    // MEMWR(RXCTRL)
    ustr[0] = 0x20; ustr[1] = 0x4A; ustr[2] = 0x3F;
    strcpy(Sin, CC2520_SPI(ustr, 3));
    // MEMWR(FSCTRL)
    ustr[0] = 0x20; ustr[1] = 0x4C; ustr[2] = 0x5A;
    strcpy(Sin, CC2520_SPI(ustr, 3));
    // MEMWR(FREQCTRL)
    ustr[0] = 0x20; ustr[1] = 0x2E; ustr[2] = 2480 - 2394;
    strcpy(Sin, CC2520_SPI(ustr, 3));
    // MEMWR(TXPOWER)
    ustr[0] = 0x20; ustr[1] = 0x30; ustr[2] = 0x32;
    strcpy(Sin, CC2520_SPI(ustr, 3));
    // MEMWR(FRMFILT0)
    ustr[0] = 0x20; ustr[1] = 0x00; ustr[2] = 0x0C;     // Frame Filter OFF
    strcpy(Sin, CC2520_SPI(ustr, 3));
    //------------------------------------------
    // SRXON
    //------------------------------------------
    ustr[0] = 0x42;
    strcpy(Sin, CC2520_SPI(ustr, 1));
}

//---------------------------------------------------------------------------
// RTC_Backup Register Write
//---------------------------------------------------------------------------
void BckRegWr(int Num, int Dat)
{
    PWR->CR |= PWR_CR_DBP;                  //Enable power domain
    switch(Num) {
        case    0:      RTC->BKP0R = Dat;       break;
        case    1:      RTC->BKP1R = Dat;       break;
        case    2:      RTC->BKP2R = Dat;       break;
        case    3:      RTC->BKP3R = Dat;       break;
        case    4:      RTC->BKP4R = Dat;       break;
        case    5:      RTC->BKP5R = Dat;       break;
        case    6:      RTC->BKP6R = Dat;       break;
        case    7:      RTC->BKP7R = Dat;       break;
        case    8:      RTC->BKP8R = Dat;       break;
        case    9:      RTC->BKP9R = Dat;       break;
        case   10:      RTC->BKP10R = Dat;      break;
        case   11:      RTC->BKP11R = Dat;      break;
        case   12:      RTC->BKP12R = Dat;      break;
        case   13:      RTC->BKP13R = Dat;      break;
        case   14:      RTC->BKP14R = Dat;      break;
        case   15:      RTC->BKP15R = Dat;      break;
        case   16:      RTC->BKP16R = Dat;      break;
        case   17:      RTC->BKP17R = Dat;      break;
        case   18:      RTC->BKP18R = Dat;      break;
        case   19:      RTC->BKP19R = Dat;      break;
    }
    PWR->CR &= ~PWR_CR_DBP;                 //Disable power domain
}

//---------------------------------------------------------------------------
// Get Time
//---------------------------------------------------------------------------
struct uTime {
    int     YY;
    int     MM;
    int     DD;
    int     hh;
    int     mm;
    float   ss;
} uTime;

void GetTime()
{
    uTime.YY  = ((RTC->DR & 0x00F00000)  >> 20) * 10 + ((RTC->DR & 0x000F0000) >> 16) - 32;
    uTime.MM  = ((RTC->DR & 0x00001000)  >> 16) * 10 + ((RTC->DR & 0x00000F00) >> 8);
    uTime.DD  = ((RTC->DR & 0x00000030)  >>  4) * 10 +  (RTC->DR & 0x0000000F);
    uTime.hh  = ((RTC->TR & 0x00300000)  >> 20) * 10 + ((RTC->TR & 0x000F0000) >> 16);
    uTime.mm  = ((RTC->TR & 0x00007000)  >> 12) * 10 + ((RTC->TR & 0x00000F00) >> 8);
    uTime.ss  = ((RTC->TR & 0x00000070)  >>  4) * 10 +  (RTC->TR & 0x0000000F);
    uTime.ss += (float)(16383 - RTC->SSR) / 16384;
} 

//---------------------------------------------------------------------------
// Set Time
//---------------------------------------------------------------------------
void SetTime(int YY, int MM, int DD, int hh, int mm, int ss)
{
    __disable_irq();                //At this point we don't want IRQs anymore
    PWR->CR |= PWR_CR_DBP;          //Enable power domain
    RTC->WPR = 0xCA;
    RTC->WPR = 0x53;
    RTC->ISR   |= 0x00000080;       // INIT -> 1
    while((RTC->ISR & 0x40) == 0);  // Wait INITF -> 1
    RTC->DR     = ((YY + 2000 - 1968) / 10) << 20;
    RTC->DR    |= ((YY + 2000 - 1968) % 10) << 16;
    RTC->DR    |= (MM / 10) << 12;
    RTC->DR    |= (MM % 10) <<  8;
    RTC->DR    |= (DD / 10) <<  4;
    RTC->DR    |=  DD % 10;
    RTC->TR     = (hh / 10) << 20;
    RTC->TR    |= (hh % 10) << 16;
    RTC->TR    |= (mm / 10) << 12;
    RTC->TR    |= (mm % 10) <<  8;
    RTC->TR    |= (ss / 10) <<  4;
    RTC->TR    |=  ss % 10;
    RTC->PRER   = 16383;            // Sync PreScaler
    RTC->PRER  |= 1 << 16;          // Async PreScaler
    RTC->ISR   &= 0xFFFFFF7F;       // INIT -> 0
    while((RTC->ISR & 0x40) != 0);  // Wait INITF -> 0
    RTC->WPR = 0xFF;                //Enable RTC write protection
    PWR->CR &= ~PWR_CR_DBP;         //Disable power domain
    __enable_irq();                 //Alarm is set, so irqs can be enabled again
}

//---------------------------------------------------------------------------
// Init RTC
//---------------------------------------------------------------------------
void InitRTC()
{
    if((RCC->BDCR & 0x02) != 0) {
        SysState = RTC->BKP0R;
        return;
    }
    __disable_irq();                //At this point we don't want IRQs anymore
    set_time(0);                    // 1970/01/01:00:00:00 SubSec=0.5
    //------------------------------------------
    // LSE Start
    //------------------------------------------
    PWR->CR |= PWR_CR_DBP;          //Enable power domain
    RCC->BDCR   = 0x00000001;       // LSE ON 
    while((RCC->BDCR & 0x02) == 0); // Wait LSE Redy
    RCC->BDCR  |= 1 << 15;          // RTCEN:RTC Clock Enable
    RCC->BDCR  |= 1 <<  8;          // RTCSEL:Clock Source LSE
    //------------------------------------------
    // RTC Set
    //------------------------------------------
    RTC->WPR = 0xCA;
    RTC->WPR = 0x53;
    
    RTC->CR &= 0xFFFFFEFF;          // ALARE Clear
    while((RTC->ISR & 0x01) == 0);  // Wait ALRAWF -> 1
    
    RTC->ISR   |= 0x00000080;       // INIT -> 1
    while((RTC->ISR & 0x40) == 0);  // Wait INITF -> 1
    RTC->PRER   = 16383;            // Sync PreScaler
    RTC->PRER  |= 1 << 16;          // Async PreScaler
    RTC->ISR   &= 0xFFFFFF7F;       // INIT -> 0
    while((RTC->ISR & 0x40) != 0);  // Wait INITF -> 0
    
    RTC->WPR = 0xFF;                //Enable RTC write protection
    PWR->CR &= ~PWR_CR_DBP;         //Disable power domain
    //------------------------------------------
    // RTC_Backup Register Write SysStale
    BckRegWr(0, 0);
    //------------------------------------------
    __enable_irq();                 //Alarm is set, so irqs can be enabled again
}

//---------------------------------------------------------------------------
// RTC ISR
//---------------------------------------------------------------------------
void   RTC_ISR()
{
    system_reset();
}

//---------------------------------------------------------------------------
// Set ALARM
//---------------------------------------------------------------------------
void SetALARM_A(int aHH, int aMM, float aSS, int bHH, int bMM, int bSS)
{
    float   fk;
    
    __disable_irq();
    PWR->CR |= PWR_CR_DBP;                  //Enable power domain
    RTC->WPR = 0xCA;                        //Disable RTC write protection
    RTC->WPR = 0x53;
    //------------------------------------------
    // Alarm A
    //------------------------------------------
    //Alarm must be disabled to change anything
    RTC->CR &= ~RTC_CR_ALRAE;
    while(!(RTC->ISR & RTC_ISR_ALRAWF));
    RTC->ALRMAR  = 0x80000000;              // MSK4:Day & Weekday
    if(aHH >= 0) {
        RTC->ALRMAR &= 0xFF00FFFF;          // Hour Setup Clear
        RTC->ALRMAR |= (aHH / 10) << 20;
        RTC->ALRMAR |= (aHH % 10) << 16;
    } else {
        RTC->ALRMAR |= 0x00800000;          // MSK3:Hour ON
    }
    if(aMM >= 0) {
        RTC->ALRMAR &= 0xFFFF00FF;          // Minute Setup Clear
        RTC->ALRMAR |= (aMM / 10) << 12;
        RTC->ALRMAR |= (aMM % 10) <<  8;
    } else {
        RTC->ALRMAR |= 0x00008000;          // MSK2:Minute
    }
    if(aSS >= 0) {
        RTC->ALRMAR &= 0xFFFFFF00;          // Secound Setup Clear
        RTC->ALRMAR |= ((int)aSS / 10) << 4;
        RTC->ALRMAR |=  (int)aSS % 10;
        fk = aSS - (int)aSS;
        if(fk > 0) {
            RTC->ALRMASSR  = 0;
            RTC->ALRMASSR |= 0x0F000000;    // SS(14:0) Compare
            RTC->ALRMASSR |= (16384 - (int)(fk / (1.0f / 16384)));
        } else {
            RTC->ALRMASSR  = 16383;
        }
    } else {
        RTC->ALRMAR &= 0x00000080;          // MSK1:Secound
    }
    RTC->CR |= RTC_CR_ALRAE | RTC_CR_ALRAIE;    //Enable Alarm
    //------------------------------------------
    // Alarm B
    //------------------------------------------
    //Alarm must be disabled to change anything
    RTC->CR &= ~RTC_CR_ALRBE;
    while(!(RTC->ISR & RTC_ISR_ALRBWF));
    RTC->ALRMBR  = 0x80000000;              // MSK4:Day & Weekday
    if(bHH >= 0) {
        RTC->ALRMBR &= 0xFF00FFFF;          // Hour Setup Clear
        RTC->ALRMBR |= (bHH / 10) << 20;
        RTC->ALRMBR |= (bHH % 10) << 16;
    } else {
        RTC->ALRMBR |= 0x00800000;          // MSK3:Hour ON
    }
    if(bMM >= 0) {
        RTC->ALRMBR &= 0xFFFF00FF;          // Minute Setup Clear
        RTC->ALRMBR |= (bMM / 10) << 12;
        RTC->ALRMBR |= (bMM % 10) <<  8;
    } else {
        RTC->ALRMBR |= 0x00008000;          // MSK2:Minute
    }
    if(bSS >= 0) {
        RTC->ALRMBR &= 0xFFFFFF00;          // Secound Setup Clear
        RTC->ALRMBR |= (bSS / 10) << 4;
        RTC->ALRMBR |=  bSS % 10;
        RTC->ALRMBSSR  = 16383;
    } else {
        RTC->ALRMBR &= 0x00000080;          // MSK1:Secound
    }
    RTC->CR |= RTC_CR_ALRBE | RTC_CR_ALRBIE;    //Enable Alarm
    
    
    
    RTC->WPR = 0xFF;                        //Enable RTC write protection
    PWR->CR &= ~PWR_CR_DBP;                 //Disable power domain
    
    NVIC_SetVector(RTC_Alarm_IRQn, (uint32_t)RTC_ISR);
    NVIC_EnableIRQ(RTC_Alarm_IRQn);  
    
    EXTI->IMR  |= RTC_EXTI_LINE_ALARM_EVENT;
    EXTI->EMR  &= ~RTC_EXTI_LINE_ALARM_EVENT;
    EXTI->RTSR |= RTC_EXTI_LINE_ALARM_EVENT;
    EXTI->FTSR &= ~RTC_EXTI_LINE_ALARM_EVENT;

    __enable_irq();
}

//---------------------------------------------------------------------------
// CC2520 Interupt(PA_10=GP2(FIFOP))
//---------------------------------------------------------------------------
InterruptIn FIFOP(PA_10);

unsigned short int DstPAN, DstMAC, SrcPAN, SrcMAC;
        int RSSI;

void ZRX_ISR()
{
    int     n, m, Len;
    int     YY, MM, DD, hh, mm, ss;
    int     Alm_Interval;
    float   Alm_Set;
    int     Init_Alm_hh, Init_Alm_mm, Init_Alm_ss;
    char    RxBuf[128];
    
    __disable_irq();
    while((GPIOA->IDR & 0x1000) != 0);      //PA_12=GP4(SFD)
    
    // REGRD(RXFIFOCNT)
    ustr[0] = 0x80 | 0x3E;
    strcpy(ubuf, CC2520_SPI(ustr, 2));
    Len = ubuf[1] & 0x7F;
    for(n = 0; n < Len; n++) {
        // REGRD(RXBUF)
        ustr[0] = 0x30;
        strcpy(ubuf, CC2520_SPI(ustr, 2));
        RxBuf[n] = ubuf[1];
        //printf("%02X ", ubuf[1]);
    }
    //printf("\r\n");
    
    DstPAN  = RxBuf[ 4];
    DstPAN |= RxBuf[ 5] << 8;
    DstMAC  = RxBuf[ 6];
    DstMAC |= RxBuf[ 7] << 8;
    SrcPAN  = RxBuf[ 8];
    SrcPAN |= RxBuf[ 9] << 8;
    SrcMAC  = RxBuf[10];
    SrcMAC |= RxBuf[11] << 8;
    RSSI    = (signed char)RxBuf[Len - 2] - 76;
    //------------------------------------------
    // FCS(CRC) Error
    //------------------------------------------
    if((RxBuf[Len - 1] & 0x80) == 0) {
        // SFLUSHRX
        ustr[0] = 0x47;
        strcpy(ubuf, CC2520_SPI(ustr, 1));
        EXTI->PR  |= 0x00000400;    // Interupt Request Clear
        __enable_irq();
    }
    //printf("RSSI=%d DstPAN=%04X DstMAC=%04X SrcPAN=%04X SrcMAC=%04X\r\n", RSSI, DstPAN, DstMAC, SrcPAN,SrcMAC);
    //------------------------------------------
    // Command Exec
    //------------------------------------------
    if((DstPAN == MyPAN || DstPAN == 0xFFFF) && (DstMAC == MyMAC || DstMAC == 0xFFFF)) {
        switch(RxBuf[12]) {
            //------------------------------------------
            // Time Sync
            //------------------------------------------
            case    'T':
                YY  = (RxBuf[13] & 0x0F) * 10;
                YY += (RxBuf[14] & 0x0F); 
                MM  = (RxBuf[15] & 0x0F) * 10;
                MM += (RxBuf[16] & 0x0F);
                DD  = (RxBuf[17] & 0x0F) * 10;
                DD += (RxBuf[18] & 0x0F);
                hh  = (RxBuf[19] & 0x0F) * 10;
                hh += (RxBuf[20] & 0x0F);
                mm  = (RxBuf[21] & 0x0F) * 10;
                mm += (RxBuf[22] & 0x0F);
                ss  = (RxBuf[23] & 0x0F) * 10;
                ss += (RxBuf[24] & 0x0F);
                SetTime(YY, MM, DD, hh, mm, ss);
                SysState = 1;
                BckRegWr(0, SysState);
                break;
            //------------------------------------------
            // Alarm Set
            //------------------------------------------
            case    'A':
                // Sens Alarmm Interval
                strcpy(ustr, &RxBuf[13]);
                for(n = 0; ustr[n] != ' ' && ustr[n] != '\t'; n++);
                ustr[n] = 0;
                sscanf(ustr, "%d", &Alm_Interval);
                // Sens Alm
                strcpy(ustr, &RxBuf[13 + n + 1]);
                for(m = 0; ustr[m] != ' ' && ustr[m] != '\t'; m++);
                ustr[m] = 0;
                sscanf(ustr, "%f", &Alm_Set);
                // Init Alarm
                strcpy(ustr, &RxBuf[13 + n + m + 2]);
                Init_Alm_hh = (ustr[0] & 0x0F) * 10 + (ustr[1] & 0x0f);
                Init_Alm_mm = (ustr[2] & 0x0F) * 10 + (ustr[3] & 0x0F);
                Init_Alm_ss = (ustr[4] & 0x0F) * 10 + (ustr[5] & 0x0F);
                
                //printf("Alarm %d %5.4f %02d:%02d:%02d\r\n",
                //        Alm_Interval, Alm_Set, Init_Alm_hh, Init_Alm_mm, Init_Alm_ss);
                
                BckRegWr(3, Alm_Interval);
                n = (int)Alm_Set << 16 | (int)((Alm_Set - (int)Alm_Set) * 0xFFFF);
                //printf("NNNNNNNNN=%08X\r\n", n);
                BckRegWr(4, n);
                BckRegWr(5, Init_Alm_hh);
                BckRegWr(6, Init_Alm_mm);
                BckRegWr(7, Init_Alm_ss);
                SysState = 2;
                BckRegWr(0, SysState);
                break;
        }
    }
    // SFLUSHRX
    ustr[0] = 0x47;
    strcpy(ubuf, CC2520_SPI(ustr, 1));
    
    EXTI->PR  |= 0x00000400;    // Interupt Request Clear
    __enable_irq();
}

//---------------------------------------------------------------------------
// BM280_SPI_Cnt
//---------------------------------------------------------------------------
char BM280_SPI(char Addr, char iDat)
{
static char oDat;
    
    GPIOB->ODR &= 0xFFFD;               // PB_1 =CSB(BME260) -> L
    
    SPI1->DR = Addr;
    while((SPI1->SR & 0x02) == 0);      // Wait TXE  Flug = 1
    SPI1->DR = iDat;
    while((SPI1->SR & 0x01) == 0);      // Wait RXNE Flug = 1
    oDat = SPI1->DR;
    while((SPI1->SR & 0x01) == 0);      // Wait RXNE Flug = 1
    oDat = SPI1->DR;
    while((SPI1->SR & 0x02) == 0);          // Wait TXE  Flug = 1
    while((SPI1->SR & 0x80) != 0);          // Wait BSY = 0
    GPIOB->ODR |= 0x0002;                   // PB_1 =CSB(BME260) -> H
    return(oDat);
}


//---------------------------------------------------------------------------
// V_Bat ADC & BM280
//---------------------------------------------------------------------------
volatile float    V_bat, fTemp, fPres, fHum;
int             temp_raw;
unsigned int    hum_raw, pres_raw;
unsigned short  dig_T1, dig_P1;
short int       dig_T2, dig_T3;
short int       dig_P2, dig_P3, dig_P4, dig_P5, dig_P6, dig_P7, dig_P8, dig_P9;
short int       dig_H2, dig_H4, dig_H5;
char            dig_H1, dig_H3, dig_H6; 
int             t_fine;

int calibration_T(signed int adc_T)
{
    int     var1, var2, T;
    
    var1 = ((((adc_T >> 3) - ((int)dig_T1 << 1))) * ((int)dig_T2)) >> 11;
    var2 = (((((adc_T >> 4) - ((int)dig_T1)) * ((adc_T >> 4) - ((int)dig_T1))) >> 12) * ((int)dig_T3)) >> 14;
    t_fine = var1 + var2;
    T = (t_fine * 5 + 128) >> 8;
    return(T); 
}

unsigned int calibration_P(int adc_P)
{
    int     var1, var2;
unsigned int P;

    var1 = (((int)t_fine)>>1) - (int)64000;
    var2 = (((var1 >> 2) * (var1 >> 2)) >> 11) * ((int)dig_P6);
    var2 = var2 + ((var1 * ((int)dig_P5)) << 1);
    var2 = (var2 >> 2)+(((int)dig_P4) << 16);
    var1 = (((dig_P3 * (((var1 >> 2)*(var1 >> 2)) >> 13)) >> 3) + ((((int)dig_P2) * var1) >> 1)) >> 18;
    var1 = ((((32768 + var1))*((int)dig_P1)) >> 15);
    if (var1 == 0) {
        return 0;
    }    
    P = (((unsigned int)(((int)1048576) - adc_P) - (var2 >> 12))) * 3125;
    if(P<0x80000000) {
       P = (P << 1) / ((unsigned int) var1);   
    } else {
        P = (P / (unsigned int)var1) * 2;    
    }
    var1 = (((int)dig_P9) * ((int)(((P >> 3) * (P >> 3)) >> 13))) >> 12;
    var2 = (((int)(P >> 2)) * ((int)dig_P8)) >> 13;
    P = (unsigned int)((signed int)P + ((var1 + var2 + dig_P7) >> 4));
    return P;
}

unsigned int calibration_H(int adc_H)
{
    int     v_x1;
    
    v_x1 = (t_fine - ((int)76800));
    v_x1 = (((((adc_H << 14) -(((int)dig_H4) << 20) - (((int)dig_H5) * v_x1)) + 
              ((int)16384)) >> 15) * (((((((v_x1 * ((int)dig_H6)) >> 10) * 
              (((v_x1 * ((int)dig_H3)) >> 11) + ((int) 32768))) >> 10) + ((int)2097152)) * 
              ((int) dig_H2) + 8192) >> 14));
   v_x1 = (v_x1 - (((((v_x1 >> 15) * (v_x1 >> 15)) >> 7) * ((int)dig_H1)) >> 4));
   v_x1 = (v_x1 < 0 ? 0 : v_x1);
   v_x1 = (v_x1 > 419430400 ? 419430400 : v_x1);
   return (unsigned int)(v_x1 >> 12);   
}

void GetSens()
{
    int     n, m;
    int     temp_cal;
unsigned int press_cal, hum_cal;
unsigned char Data[32];
    
    //------------------------------------------
    // Vbat
    //------------------------------------------
    while((ADC1->SR & 0x02) == 0);
    V_bat = (float)4096 / ADC1->DR * 1.2f;
    ADC1->CR2   = 0x40000001;       // SWSTART:
    //------------------------------------------
    // BM280_Init
    //------------------------------------------
    BM280_SPI(0x72, 0x01);          // osrs_h = 1
    //BM280_SPI(0x74, 0x27);          // osrs_t = 1 | osrs_p = 1 | mode = 3(Normal)
    BM280_SPI(0x74, 0x25);          // osrs_t = 1 | osrs_p = 1 | mode = 1(Fource)
    BM280_SPI(0x75, 0xA0);          // t_sb = 5 | fllter = 0 | spi3w_en = 0
    //------------------------------------------
    // BM280_Trimming parameter readout 
    //------------------------------------------
    for(n = 0, m = 0x88; m <= 0x9F; n++, m++) Data[n] = BM280_SPI(m, 0);
    Data[n] = BM280_SPI(0xA1, 0);
    for(n += 1, m = 0xE1; m <= 0xE7; n++, m++) Data[n] = BM280_SPI(m, 0);
    
    //for(n = 0; n < 32; n++) printf("%02X ", Data[n]);
    //printf("\r\n");
    
    dig_T1 = (Data[1] << 8) | Data[0];
    dig_T2 = (Data[3] << 8) | Data[2];
    dig_T3 = (Data[5] << 8) | Data[4];
    dig_P1 = (Data[7] << 8) | Data[6];
    dig_P2 = (Data[9] << 8) | Data[8];
    dig_P3 = (Data[11]<< 8) | Data[10];
    dig_P4 = (Data[13]<< 8) | Data[12];
    dig_P5 = (Data[15]<< 8) | Data[14];
    dig_P6 = (Data[17]<< 8) | Data[16];
    dig_P7 = (Data[19]<< 8) | Data[18];
    dig_P8 = (Data[21]<< 8) | Data[20];
    dig_P9 = (Data[23]<< 8) | Data[22];
    dig_H1 = Data[24];
    dig_H2 = (Data[26]<< 8) | Data[25];
    dig_H3 = Data[27];
    dig_H4 = (Data[28]<< 4) | (0x0F & Data[29]);
    dig_H5 = (Data[30] << 4) | ((Data[29] >> 4) & 0x0F);
    dig_H6 = Data[31];
    //------------------------------------------
    // BM280_Read
    //------------------------------------------
    Data[0] = BM280_SPI(0xF7, 0x00);
    Data[1] = BM280_SPI(0xF8, 0x00);
    Data[2] = BM280_SPI(0xF9, 0x00);
    Data[3] = BM280_SPI(0xFA, 0x00);
    Data[4] = BM280_SPI(0xFB, 0x00);
    Data[5] = BM280_SPI(0xFC, 0x00);
    Data[6] = BM280_SPI(0xFD, 0x00);
    Data[7] = BM280_SPI(0xFE, 0x00);
    pres_raw = (Data[0] << 12) | (Data[1] << 4) | (Data[2] >> 4);
    temp_raw = (Data[3] << 12) | (Data[4] << 4) | (Data[5] >> 4);
    hum_raw  = (Data[6] <<  8) |  Data[7];
    
    temp_cal  = calibration_T(temp_raw);
    press_cal = calibration_P(pres_raw);
    hum_cal   = calibration_H(hum_raw);
    
    fTemp = (float)temp_cal / 100;
    fPres = (float)press_cal / 100;
    fHum  = (float)hum_cal / 1024;
}

//---------------------------------------------------------------------------
// Main
//---------------------------------------------------------------------------
int main()
{
    int     k, n;
    int     Alm_Interval;
    float   Alm_Set;
    int     InitH, InitM, InitS;

    InitMPU();
    //printf("MPU_Start\r\n");
    GPIOB->ODR |= 0x1000;           // LED ON
    //InitCC2420();
    //printf("CC2420_Start\r\n");
    InitRTC();
    //printf("RTC_Start\r\n");
    //GPIOB->ODR &= 0xEFFF;
    //------------------------------------------
    // CC2520: PA_10=GP2(FIFOP) Interupt
    //------------------------------------------
    NVIC_SetVector(EXTI15_10_IRQn, (uint32_t)ZRX_ISR);
    NVIC_EnableIRQ(EXTI15_10_IRQn);
    EXTI->IMR  |= 0x00000400;
    EXTI->EMR  &= 0xFFFFFBFF;
    EXTI->RTSR |= 0x00000400;
    EXTI->FTSR &= 0xFFFFFBFF;
    EXTI->PR   |= 0x00000400;
    //------------------------------------------
    // I2S3 Interupt Enable
    //------------------------------------------
    NVIC_SetVector(SPI3_IRQn, (uint32_t)I2S3_IRQ_Handler);
    NVIC_EnableIRQ(SPI3_IRQn);
    SPI3->CR2 |= 0x0080;                // TXEIE Interupt Enable
    //------------------------------------------
    // RAND Init
    //------------------------------------------
    srand(MyMAC);
    //------------------------------------------
    // InitAlarm ?
    //------------------------------------------
    SysState = RTC->BKP0R;
    if((RTC->ISR & 0x00000200) != 0) {
        SysState = 0;
        BckRegWr(0, SysState);
    }
    //printf("\r\nSysState=%d  RTC_ISR=%08X\r\n", SysState, RTC->ISR);
    //------------------------------------------
    // Init CC2420
    //------------------------------------------
    if(SysState == 0 || SysState == 1) InitCC2420();
    //------------------------------------------
    // ADC1 Start
    //------------------------------------------
    ADC1->CR2   = 0x40000001;        // ADC1_SWSTART:
    //------------------------------------------
    // Wait TimeSync SysState 0 -> 1
    //------------------------------------------
    while(SysState == 0) {
        wait(((float)rand() * 0.3f) / 0x7FFFFFFF);
        GetTime();
        GetSens();
        sprintf(ustr, "%d00%02d%02d%02d%02d%07.4f %3.2f %04.1f %04.1f %06.1f", SysState,
                uTime.MM, uTime.DD, uTime.hh, uTime.mm, uTime.ss, V_bat, fTemp, fHum, fPres);
        n = MakeZigbeeTxPacket((unsigned char *)ustr, strlen(ustr), MyPAN, MyMAC);
        //printf("[1][%d]=[%s]\r\n", n, ustr);
        MakeZigBeeMod(TxPkt, n);
        GPIOB->ODR &= 0xFFEF;       // PB_4 =15.4Rx -> L
        zTx();
        GPIOB->ODR |= 0x0010;       // PB_4 =15.4Rx -> H
        //printf("wait=%3.5f\r\n", (float)rand() / 0x7FFFFFFF);
        //wait((float)rand() / 0x7FFFFFFF + 0.1f);
        wait(1);
        if(SysState != 0) break;
        
        GetTime();
        //printf("S%d:%02d%02d%02d%02d%07.4f\r\n",SysState, uTime.MM, uTime.DD, uTime.hh, uTime.mm, uTime.ss);
        if(uTime.ss < 10) {
            SetALARM_A( -1, -1, 25, -1, -1, -1);
        } else if(uTime.ss < 20) {
            SetALARM_A( -1, -1, 35, -1, -1, -1);
        } else if(uTime.ss < 30) {
            SetALARM_A( -1, -1, 45, -1, -1, -1);
        } else if(uTime.ss < 40) {
            SetALARM_A( -1, -1, 55, -1, -1, -1);
        } else if(uTime.ss < 50) {
            SetALARM_A( -1, -1,  5, -1, -1, -1);
        } else {
            SetALARM_A( -1, -1, 15, -1, -1, -1);
        }
        //------------------------------------------
        // Sleep
        //------------------------------------------
        GPIOB->ODR &= 0xFFFE;               // PB_0 :VREG_EN H -> L
        GPIOB->ODR &= 0x7FFF;               // PB_15:RESETn  H -> L
        sleep_manager_lock_deep_sleep();
        sleep_manager_unlock_deep_sleep_internal();   
        //SPI1->CR1     = 0;
        SPI3->I2SCFGR = 0;
        ADC->CCR      = 0;
        ADC1->CR2     = 0;
        GPIOB->ODR &= 0xEFFF;               // LED OFF
        //------------------------------------------
        // System clock = HSE
        //------------------------------------------
        k = RCC->CFGR;
        RCC->CFGR      = (k & 0xFFFFFFFC) | 0x01;
        while((RCC->CFGR & 0x0C) != 0x04);
        //------------------------------------------
        // PLLSAI & PLLI2S & PLL -> OFF
        //------------------------------------------
        RCC->CR       &= 0xEAFFFFFF;
        while((RCC->CR & 0x0F000000) != 0);
        //------------------------------------------
        // Peripheral circuit
        //------------------------------------------
        RCC->APB1ENR  &= 0xFFFDFFFF;        // USART2 Clock Disable
        //------------------------------------------
        // Sleep
        //------------------------------------------
        sleep();
    }
    //------------------------------------------
    // Wait Alarm Set SysState 1 -> 2
    //------------------------------------------
    while(SysState == 1) {
        GetTime();
        GetSens();
        sprintf(ustr, "%d%02d%02d%02d%02d%02d%07.4f %3.2f %04.1f %04.1f %06.1f", SysState,
                uTime.YY, uTime.MM, uTime.DD, uTime.hh, uTime.mm, uTime.ss, V_bat, fTemp, fHum, fPres);
        n = MakeZigbeeTxPacket((unsigned char *)ustr, strlen(ustr), MyPAN, MyMAC);
        //printf("[1][%d]=[%s]\r\n", n, ustr);
        MakeZigBeeMod(TxPkt, n);
        GPIOB->ODR &= 0xFFEF;       // PB_4 =15.4Rx -> L
        zTx();
        GPIOB->ODR |= 0x0010;       // PB_4 =15.4Rx -> H
        //printf("wait=%3.5f\r\n", (float)rand() / 0x7FFFFFFF);
        wait((float)rand() / 0x7FFFFFFF + 0.1f);
    }
    //------------------------------------------
    // Alarm Set (SysState = 2)
    //------------------------------------------
    Alm_Interval = RTC->BKP3R;
    Alm_Set      = (RTC->BKP4R >> 16) + ((float)(RTC->BKP4R & 0xFFFF) / 0xFFFF);
    //printf("Alarm %d %5.4f\r\n", Alm_Interval, Alm_Set);
    InitH = RTC->BKP5R;
    InitM = RTC->BKP6R;
    InitS = RTC->BKP7R;
    GetTime();
    //------------------------------------------
    // Init Alarm Check -> SysState 0
    //------------------------------------------
    k = InitH * 3600 + InitM * 60 + InitS;
    n = uTime.hh * 3600 + uTime.mm * 60 + uTime.ss;
    if(k > n) {
        if((k - n) > (3600 * 12)) BckRegWr(0, 0);                   // SysState = 0:
    } else {
        if(((k + 3600 * 24) - n) >  (3600 * 12)) BckRegWr(0, 0);
    }
            
    if(Alm_Interval <= 10) {
        if(uTime.ss < 10) {
            SetALARM_A( -1, -1, Alm_Set + 10, InitH, InitM, InitS);
        } else if(uTime.ss < 20) {
            SetALARM_A( -1, -1, Alm_Set + 20, InitH, InitM, InitS);
        } else if(uTime.ss < 30) {
            SetALARM_A( -1, -1, Alm_Set + 30, InitH, InitM, InitS);
        } else if(uTime.ss < 40) {
            SetALARM_A( -1, -1, Alm_Set + 40, InitH, InitM, InitS);
        } else if(uTime.ss < 50) {
            SetALARM_A( -1, -1, Alm_Set + 50, InitH, InitM, InitS);
        } else {
            SetALARM_A( -1, -1, Alm_Set, InitH, InitM, InitS);
        }
    } else if(Alm_Interval <= 30) {
        if(uTime.ss < 30) {
            SetALARM_A( -1, -1, Alm_Set + 30, InitH, InitM, InitS);
        } else {
            SetALARM_A( -1, -1, Alm_Set, InitH, InitM, InitS);
        }
    } else if(Alm_Interval <= 60) {
        SetALARM_A( -1, -1, Alm_Set, InitH, InitM, InitS);
    } else {
        k = (int)Alm_Set / 60 + Alm_Interval / 60 + uTime.mm;
        SetALARM_A((k / 60 + uTime.hh) % 24, k % 60, Alm_Set - (int)Alm_Set / 60 * 60,
                    InitH, InitM, InitS);
        //printf("%02d%02d%02d  %02d %02d %07.4f\r\n",
                //uTime.YY, uTime.MM, uTime.DD, uTime.hh, uTime.mm, uTime.ss);
        //printf("%d       %d %d %5.4f\r\n", n, k / 60 + uTime.hh % 24, k % 60, Alm_Set - (int)Alm_Set / 60 * 60);
    }
    //------------------------------------------
    // Sens Packet Tx
    //------------------------------------------
    GetTime();
    GetSens();
    sprintf(ustr, "%d%02d%02d%02d%02d%02d%07.4f %3.2f %04.1f %04.1f %06.1f", SysState,
            uTime.YY, uTime.MM, uTime.DD, uTime.hh, uTime.mm, uTime.ss, V_bat, fTemp, fHum, fPres);
    n = MakeZigbeeTxPacket((unsigned char *)ustr, strlen(ustr), MyPAN, MyMAC);
    //printf("[2][%d]=[%s]\r\n", n, ustr);
    MakeZigBeeMod(TxPkt, n);
    GPIOB->ODR &= 0xFFEF;       // PB_4 =15.4Rx -> L
    zTx();
    GPIOB->ODR |= 0x0010;       // PB_4 =15.4Rx -> H
    //------------------------------------------
    // Sleep
    //------------------------------------------
    GPIOB->ODR &= 0xFFFE;               // PB_0 :VREG_EN H -> L
    GPIOB->ODR &= 0x7FFF;               // PB_15:RESETn  H -> L
    sleep_manager_lock_deep_sleep();
    sleep_manager_unlock_deep_sleep_internal();   
    //SPI1->CR1     = 0;
    SPI3->I2SCFGR = 0;
    ADC->CCR      = 0;
    ADC1->CR2     = 0;
    GPIOB->ODR &= 0xEFFF;               // LED OFF
    //------------------------------------------
    // System clock = HSE
    //------------------------------------------
    k = RCC->CFGR;
    RCC->CFGR      = (k & 0xFFFFFFFC) | 0x01;
    while((RCC->CFGR & 0x0C) != 0x04);
    //------------------------------------------
    // PLLSAI & PLLI2S & PLL -> OFF
    //------------------------------------------
    RCC->CR       &= 0xEAFFFFFF;
    while((RCC->CR & 0x0F000000) != 0);
    //------------------------------------------
    // Peripheral circuit
    //------------------------------------------
    RCC->APB1ENR  &= 0xFFFDFFFF;        // USART2 Clock Disable
    //------------------------------------------
    // Sleep
    //------------------------------------------
    sleep();
}