#include "shell.h"
#include "crc.h"
#include "board.h"


static const char* CRCStringTable[] = 
{
    "CRC16_IBM",
    "kCRC16_MAXIM",
    "kCRC16_USB",
    "kCRC16_MODBUS",
    "kCRC16_CCITT",
    "kCRC16_CCITTFalse",
    "kCRC16_X25",
    "kCRC16_XMODEM",
    "kCRC16_DNP",
    "kCRC32",
    "kCRC32_MPEG2",
};
static int DoCRC(int argc, char * const argv[])
{
    uint32_t result;
    uint8_t buffer[10] = {0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30};
    printf("CRC 16 test\r\n");
    for(int i=0; i < kCRC_ProtocolNameMax; i++)
    {
        CRC_QuickInit(i);
        result = CRC_Generate(buffer, 10);
        printf("crc%-*s : 0x%08X\r\n", 18, CRCStringTable[i], result);
    }
    return 0;
}

SHELL_EXPORT_CMD(DoCRC, crc , crc test)
    

