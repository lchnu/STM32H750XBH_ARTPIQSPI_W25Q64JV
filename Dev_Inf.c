#include "Dev_Inf.h"

/* This structure containes information used by ST-LINK Utility to program and erase the device */
#if defined(__ICCARM__)
__root struct StorageInfo const StorageInfo = {
#else
struct StorageInfo const StorageInfo = {
#endif
    "STM32H750XBH_ARTPI_W25Q64JV",    // Device Name + EVAL Borad name
    SPI_FLASH,                 // Device Type
    0x90000000,                // Device Start Address
    0x00800000,                // Device Size in Bytes (8MBytes)
    0x1000,                    // Programming Page Size 256Bytes
    0xFF,                      // Initial Content of Erased Memory
                               // Specify Size and Address of Sectors (view example below)
    {{0x00000800, 0x00001000,},     // Sector Num : 2048 ,Sector Size: 4KBytes
    {0x00000000, 0x00000000,}}
};

