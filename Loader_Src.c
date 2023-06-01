/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
// #include <string.h>

#include "main.h"
#include "quadspi.h"
#if defined(DEBUG_WITH_UART)
#include "usart.h"
#include <stdio.h>
#endif
#include "w25qxx.h"
/* Private typedef -----------------------------------------------------------*/


/* Private macro -------------------------------------------------------------*/
#define QSPI_PAGESIZE        (256)
#define SECTOR_SIZE          (4096)
#define W25Qxx_BLOCK_SIZE    (4 * 1024)

/* Private variables ---------------------------------------------------------*/
#if defined(W25QXX_WRITE_DIFF)
static uint8_t secs_diff_wr_flag = 0;
static uint8_t w25qxx_read_buf[4096];
#endif

/* Private functions ---------------------------------------------------------*/
extern void SystemClock_Config(void);
static uint32_t Checksum(uint32_t StartAddress, uint32_t Size, uint32_t InitVal);
int Init(void);
int Read(uint32_t Address, uint32_t Size, uint8_t *Buffer);

/*******************************************************************************
 Description :
 Initialization of HAL Tick, Overide default HAL_InitTick function
 Inputs :
                TickPriority 	: not used
 outputs :
                "HAL_OK" 	    : Operation succeeded
 Info :
 Note : Reference en.DM00403500.pdf STM32CubeProgrammer User Manual
********************************************************************************/
HAL_StatusTypeDef HAL_InitTick(uint32_t TickPriority)
{
    return HAL_OK;
}

/*******************************************************************************
 Description :
 Get systick, Overide default HAL_GetTick function
 Inputs :   None
 outputs :  None
 Info :
 Note : Reference en.DM00403500.pdf STM32CubeProgrammer User Manual
********************************************************************************/
uint32_t HAL_GetTick(void)
{
    static uint32_t ticks = 0U;
    uint32_t i;
    /* If Kernel is not running wait approximately 1 ms then increment
     and return auxiliary tick counter value */
    for (i = (SystemCoreClock >> 14U); i > 0U; i--)
    {
        __NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
        __NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
    }
    return ++ticks;
}

/*******************************************************************************
 Description :
 HAL Delay 1 millisecond, Overide default HAL_Delay function
 Inputs :   None
 outputs :  None
 Info :
 Note : Reference en.DM00403500.pdf STM32CubeProgrammer User Manual
********************************************************************************/
void HAL_Delay(uint32_t Delay)
{
    uint32_t tickstart = HAL_GetTick();
    uint32_t wait = Delay;
    /* Add a period to guaranty minimum wait */
    if (wait < HAL_MAX_DELAY)
    {
        wait += (uint32_t)(HAL_TICK_FREQ_DEFAULT);
    }
    while ((HAL_GetTick() - tickstart) < wait)
    {
        __NOP();
    }
}

/*******************************************************************************
 Description :
 Write data to the device
 Inputs :       Address 	: Write location
                Size 		  : Length in bytes
                buffer 		: Address where to get the data to write
 outputs :      "1" 	    : Operation succeeded
 Info :
 Note : Reference en.DM00403500.pdf STM32CubeProgrammer User Manual
        Mandatory for all types except SRAM and PSRAM
********************************************************************************/
#if defined(W25QXX_WRITE_DIFF)
int Write(uint32_t Address, uint32_t Size, uint8_t *buffer)
{
    uint32_t addr;
    uint32_t sec_pos;
    uint32_t sec_off;
    uint32_t sec_rem;
    uint32_t i;
    uint8_t *W25QXX_BUF;
    addr = Address & 0x0FFFFFFF;

    W25QXX_BUF = w25qxx_read_buf;
    sec_pos = addr / 4096;
    sec_off = addr % 4096;
    sec_rem = 4096 - sec_off;
    if (Size <= sec_rem)
    {
        sec_rem = Size;
    }
#if defined(DEBUG_WITH_UART)
    printf("Write Nocheck %d bytes to 0x%x from 0x%0x\r\n", Size, Address, (unsigned int)buffer);
#endif
    if (1 == secs_diff_wr_flag)
    {
        while (1)
        {
            Read(sec_pos * 4096 + 0x90000000, 4096, W25QXX_BUF);
#if defined(DEBUG_WITH_UART)
            printf("---Read from sector %d \r\n", sec_pos);
#endif
#if defined(DEBUG_WITH_UART)
            printf("---sec pos %d, off %d, rem %d \r\n", sec_pos, sec_off, sec_rem);
#endif
            for (i = 0; i < sec_rem; i++)
            {
                if (W25QXX_BUF[sec_off + i] != buffer[i])
                {
#if defined(DEBUG_WITH_UART)
                    printf("---Read different data from sector %d at offset %d \r\n", sec_pos, i);
#endif
                    break;
                }
            }
            if (i < sec_rem)
            {
#if defined(W25QXX_READ_BY_MEMORY_MAP)
                Init();
#endif              
                W25QXX_SectorErase(sec_pos * 4096);
#if defined(DEBUG_WITH_UART)
                printf("---Erase sector %d, address is 0x%08x\r\n", sec_pos, (sec_pos * 4096 + 0x90000000));
#endif
                for (i = 0; i < sec_rem; i++)
                {
                    W25QXX_BUF[sec_off + i] = buffer[i];
                }
                W25QXX_Write_NoCheck(W25QXX_BUF, sec_pos * 4096, 4096);
#if defined(DEBUG_WITH_UART)
                printf("---Write sector %d\r\n", sec_pos);
#endif
            }
            else
            {
#if defined(DEBUG_WITH_UART)
                printf("---Do nothing to sector %d\r\n", sec_pos);
#endif
            }
            if (Size == sec_rem)
            {
                break;
            }
            else
            {
                sec_pos++;
                sec_off = 0;

                buffer += sec_rem;
                addr += sec_rem;
                Size -= sec_rem;
                if (Size > 4096)
                {
                    sec_rem = 4096;
                }
                else
                {
                    sec_rem = Size;
                }
            }
        }
    }
    else
    {
        //do nothing
    }
    return 1;
}
#else
static void _Write(unsigned char *buf, unsigned long adr, unsigned long sz)
{
    unsigned long end_addr, current_size, current_addr;

    /* Calculation of the size between the write address and the end of the page */
    current_addr = 0;

    while (current_addr <= adr)
        current_addr += QSPI_PAGESIZE;

    current_size = current_addr - adr;

    /* Check if the size of the data is less than the remaining place in the page */
    if (current_size > sz)
        current_size = sz;

    /* Initialize the adress variables */
    current_addr = adr;
    end_addr = adr + sz;

    /* Perform the write page by page */
    do
    {
        W25QXX_PageProgram(buf, current_addr, current_size);
        /* Update the address and size variables for next page programming */
        buf += current_size;
        current_addr += current_size;
        current_size = ((current_addr + QSPI_PAGESIZE) > end_addr) ? (end_addr - current_addr) : QSPI_PAGESIZE;
    }
    while (current_addr < end_addr);

    return;
}

int Write(uint32_t Address, uint32_t Size, uint8_t *buffer)
{
    _Write(buffer, Address & 0x0fffffff, Size);
    return 1;
}
#endif

/*******************************************************************************
 Description :
 Erase a full sector in the device
 Inputs :       EraseStartAddress	: Start of sector
                EraseEndAddress   : End   of sector
 outputs :                    "1" : Operation succeeded
                              "0" : Operation failure
 Note : Reference en.DM00403500.pdf STM32CubeProgrammer User Manual
        Not Mandatory for SRAM PSRAM and NOR_FLASH
********************************************************************************/
#if defined(W25QXX_WRITE_DIFF)
int SectorErase(uint32_t EraseStartAddress, uint32_t EraseEndAddress)
{
    uint32_t BlockAddr;
#if defined(DEBUG_WITH_UART)
    printf("Erase from 0x%x to 0x%x\r\n", EraseStartAddress, EraseEndAddress);
#endif
    /*Choose sectors in CubePro, EraseEndAddress and EraseStartAddress are the same*/
    if (EraseEndAddress == EraseStartAddress)   
    {
        BlockAddr = EraseStartAddress & 0x0FFFFFFF;
        W25QXX_SectorErase(BlockAddr);
        secs_diff_wr_flag = 0;
    }
    else
    {
#if defined(DEBUG_WITH_UART)
        printf("---Erasing Sectors is not actually performed\r\n");
#endif
        secs_diff_wr_flag = 1; /* allow differential write*/
    }
    return 1;
}
#else
int SectorErase(uint32_t EraseStartAddress, uint32_t EraseEndAddress)
{
#if defined(DEBUG_WITH_UART)
    printf("Erase from 0x%x to 0x%x\r\n", EraseStartAddress, EraseEndAddress);
#endif   
    EraseStartAddress = EraseStartAddress - EraseStartAddress % SECTOR_SIZE;
    while (EraseEndAddress >= EraseStartAddress)
    {   
        if ((EraseStartAddress % (64*1024) == 0) && (EraseEndAddress - EraseStartAddress >= 64*1024))
        {
#if defined(DEBUG_WITH_UART)
            printf("---Erase Block 64K from 0x%x \r\n", EraseStartAddress);
#endif
            W25QXX_Block64K_Erase(EraseStartAddress & 0x0fffffff);
            EraseStartAddress += 64*1024;
        }
        else if ((EraseStartAddress % (32*1024) == 0) && (EraseEndAddress - EraseStartAddress >= 32*1024))
        {
#if defined(DEBUG_WITH_UART)
            printf("---Erase Block 32K from 0x%x \r\n", EraseStartAddress);
#endif          
            W25QXX_Block32K_Erase(EraseStartAddress & 0x0fffffff);
            EraseStartAddress += 32*1024;
        }
        else
        {
#if defined(DEBUG_WITH_UART)
            printf("---Erase Sector 4K from 0x%x \r\n", EraseStartAddress);
#endif            
            W25QXX_SectorErase(EraseStartAddress & 0x0fffffff);
            EraseStartAddress += SECTOR_SIZE;
        }
        HAL_Delay(1);
    }  
  return 1;
}
#endif

/*******************************************************************************
 * Description :
 * Erase a full sector in the device
 * Inputs        :   None
 *     
 * outputs   "1" : Operation succeeded
 * 			     "0" : Operation failure
 * Note: Reference en.DM00403500.pdf STM32CubeProgrammer User Manual
         Not Mandatory for SRAM PSRAM and NOR_FLASH
 ********************************************************************************/
int MassErase(void)
{
#if defined(DEBUG_WITH_UART)
    printf("Mass Chip Erase\r\n");
#endif
    W25QXX_ChipErase();
    return 1;
}

/*******************************************************************************
 Description :
 Read data from the device
 Inputs :       Address 	: Read location
                Size 		  : Length in bytes
                buffer 		: Address where to get the data to write
 outputs :       "1" 		  : Operation succeeded
                 "0" 		  : Operation failure
 Note : Reference en.DM00403500.pdf STM32CubeProgrammer User Manual
********************************************************************************/
int Read(uint32_t Address, uint32_t Size, uint8_t *Buffer)
{

#if defined(DEBUG_WITH_UART)
    printf("Read %d Size from 0x%x to 0x%x\r\n", Size, Address, (uint32_t)Buffer);
#endif

#if defined(W25QXX_READ_BY_MEMORY_MAP)
    W25Q_Memory_Mapped_Enable();
    for (volatile int i = 0; i < Size; i++)
    {
        *(uint8_t *)Buffer = *(uint8_t *)Address;
        Address++;
        Buffer++;
    }
#else
    W25QXX_Read(Buffer, Address & 0x0FFFFFFF, Size);
#endif
    return 1;
}

/*******************************************************************************
 Description :
 Verify the data
 Inputs :       MemoryAddr 	    : Write location
                RAMBufferAddr 	: RAM Address
                Size 		        : Length in bytes
 outputs :
                "1" 		        : Operation succeeded
 Note : Reference en.DM00403500.pdf STM32CubeProgrammer User Manual
        Not Mandatory
********************************************************************************/
uint64_t Verify(uint32_t MemoryAddr, uint32_t RAMBufferAddr, uint32_t Size, uint32_t missalignement)
{
    return 1;
}

/*******************************************************************************
 Description :
 System initialization
 Inputs 	:     None
 outputs 	:     "1" 		: Operation succeeded
                "0" 		: Operation failure
 Note : Reference en.DM00403500.pdf STM32CubeProgrammer User Manual
********************************************************************************/
int Init(void)
{
    /* MCU Configuration--------------------------------------------------------*/
    SystemInit();
    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    HAL_Init();
    /* STM32H7xx Enable Cache*/
    SCB_EnableICache();       /*Enable I-Cache*/
    SCB_EnableDCache();       /*Enable D-Cache*/
    SCB->CACR |= 1 << 2;      /*Enable D-Cache Write Through, which means data are simultaneously updated to cache and memory*/
    /* Configure the system clock */
    SystemClock_Config();
    /* Initialize all configured peripherals */
#if defined(DEBUG_WITH_UART)
    /* Clear the huart4 handler and Init UART4*/
    uint8_t *puart = (uint8_t *)&huart4;
    for (volatile uint8_t i = 0; i < sizeof(UART_HandleTypeDef); i++)
    {
        *puart++ = 0;
    }
    huart4.Instance = UART4;
    HAL_UART_DeInit(&huart4);
    MX_UART4_Init();
#endif
    /* Clear the hqspi handler and Init QSPI, Neccessary!!!!!*/
    uint8_t *pqspi = (uint8_t *)&hqspi;
    for (volatile uint8_t i = 0; i < sizeof(QSPI_HandleTypeDef); i++)
    {
        *pqspi++ = 0;
    }
    hqspi.Instance = QUADSPI;
    HAL_QSPI_DeInit(&hqspi);
    
#if defined(W25QXX_READ_BY_MEMORY_MAP)
    /* Force QSPI in Indirect read mode and Disable QUADSPI. 
       The following statements seems useless, and will be deleted!!!  #TODO */    
//    QUADSPI->CCR = QUADSPI->CCR & 0xf7ffffff; /* Indirect read mode */
//    QUADSPI->CR = QUADSPI->CR & 0xfffffffe;   /* Disable the QUADSPI */
#endif
    
    MX_QUADSPI_Init();

#if defined(DEBUG_WITH_UART)
    printf("Init, Exit QSPI result is %d\r\n", W25QXX_ExitQPIMode());
#else
    W25QXX_ExitQPIMode();
#endif
    W25QXX_Reset();
    W25QXX_EnterQPIMode();
#if defined(DEBUG_WITH_UART) && defined(W25QXX_WRITE_DIFF)
    printf("---Init, Sectors differential write flag is %d\r\n", secs_diff_wr_flag);
#endif
    return 1;
}

#if defined(DEBUG_WITH_UART)
#include <stdio.h>
#pragma import(__use_no_semihosting)
void _sys_exit(int x)
{
    x = x;
}

struct __FILE
{
    int handle;
};

FILE __stdout;

int fputc(int ch, FILE *f)
{
    HAL_UART_Transmit(&huart4, (uint8_t *)&ch, 1, 0xffffffff);
    return (ch);
}
#endif


uint32_t Checksum(uint32_t StartAddress, uint32_t Size, uint32_t InitVal)
{
    uint8_t missalignementAddress = StartAddress % 4;
    uint8_t missalignementSize = Size;
    int cnt;
    uint32_t Val;

    StartAddress -= StartAddress % 4;
    Size += (Size % 4 == 0) ? 0 : 4 - (Size % 4);

    for (cnt = 0; cnt < Size; cnt += 4)
    {
        Val = *(uint32_t *)StartAddress;
        if (missalignementAddress)
        {
            switch (missalignementAddress)
            {
            case 1:
                InitVal += (uint8_t)(Val >> 8 & 0xff);
                InitVal += (uint8_t)(Val >> 16 & 0xff);
                InitVal += (uint8_t)(Val >> 24 & 0xff);
                missalignementAddress -= 1;
                break;
            case 2:
                InitVal += (uint8_t)(Val >> 16 & 0xff);
                InitVal += (uint8_t)(Val >> 24 & 0xff);
                missalignementAddress -= 2;
                break;
            case 3:
                InitVal += (uint8_t)(Val >> 24 & 0xff);
                missalignementAddress -= 3;
                break;
            }
        }
        else if ((Size - missalignementSize) % 4 && (Size - cnt) <= 4)
        {
            switch (Size - missalignementSize)
            {
            case 1:
                InitVal += (uint8_t)Val;
                InitVal += (uint8_t)(Val >> 8 & 0xff);
                InitVal += (uint8_t)(Val >> 16 & 0xff);
                missalignementSize -= 1;
                break;
            case 2:
                InitVal += (uint8_t)Val;
                InitVal += (uint8_t)(Val >> 8 & 0xff);
                missalignementSize -= 2;
                break;
            case 3:
                InitVal += (uint8_t)Val;
                missalignementSize -= 3;
                break;
            }
        }
        else
        {
            InitVal += (uint8_t)Val;
            InitVal += (uint8_t)(Val >> 8 & 0xff);
            InitVal += (uint8_t)(Val >> 16 & 0xff);
            InitVal += (uint8_t)(Val >> 24 & 0xff);
        }
        StartAddress += 4;
    }
    return (InitVal);
}
