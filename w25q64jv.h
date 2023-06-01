#ifndef __W25Q64JV_H__
#define __W25Q64JV_H__

#include <string.h>
#include <stdint.h>
#include "quadspi.h"

/* Chip Define */
#define W25Q64JV_FLASH_SIZE 			(8 * 1024 * 1024)
#define W25Q64JV_64KB_BLOCK_SIZE 		(0x10000UL)				// 64KB
#define W25Q64JV_32KB_BLOCK_SIZE		(0x8000UL)				// 32KB
#define W25Q64JV_SECTOR_SIZE 			(0x1000UL)				// 4KB
#define W25Q64JV_PAGE_SIZE 				(0x100U)				// 256Bytes
#define W25Q64JV_BLOCK1_START_ADDR		(0x10000UL)
#define W25Q64JV_BLOCK127_START_ADDR	(0x7F0000UL)

#define W25Q64JV_SFDP_REG_START_ADDR		(0x000000UL)
#define W25Q64JV_SECURITY_REG1_START_ADDR	(0x001000UL)
#define W25Q64JV_SECURITY_REG2_START_ADDR	(0x002000UL)
#define W25Q64JV_SECURITY_REG3_START_ADDR	(0x003000UL)

#define W25Q64JV_CHIP_ERASE_MAX_TIME_MS				(100000UL)
#define W25Q64JV_64KB_BLOCK_ERASE_MAX_TIME_MS		(2000UL)
#define W25Q64JV_32KB_BLOCK_ERASE_MAX_TIME_MS		(1600UL)
#define W25Q64JV_SECTOR_ERASE_MAX_TIME_MS			(400U)
#define W25Q64JV_PAGE_PROGRAM_MAX_TIME_MS			(3U)		// 小于5000ms的一律使用HAL_QSPI_TIMEOUT_DEFAULT_VALUE替换
#define W25Q64JV_WRITE_SR_MAX_TIME_MS				(15U)
#define W25Q64JV_RESET_TIME_US						(50U)

/* Return Type Define */
#define QSPI_ERROR 	(-1)
#define QSPI_OK 	(0)

/* Instruction */
#define W25Q64JV_WRITE_ENABLE 				(0x06)
#define W25Q64JV_WRITE_ENABLE_FOR_VSR		(0x50)
#define W25Q64JV_WRITE_DISABLE				(0x04)
#define W25Q64JV_READ_STATUS_REG1			(0x05)
#define W25Q64JV_READ_STATUS_REG2			(0x35)
#define W25Q64JV_READ_STATUS_REG3			(0x15)
#define W25Q64JV_WRITE_STATUS_REG1			(0x01)
#define W25Q64JV_WRITE_STATUS_REG2			(0x31)
#define W25Q64JV_WRITE_STATUS_REG3			(0x11)
#define W25Q64JV_READ_DATA					(0x03)
#define W25Q64JV_FAST_READ					(0x0B)
#define W25Q64JV_FAST_READ_DUAL_OUTPUT		(0x3B)
#define W25Q64JV_FAST_READ_QUAD_OUTPUT		(0x6B)
#define W25Q64JV_FAST_READ_DUAL_IO			(0xBB)
#define W25Q64JV_FAST_READ_QUAD_IO			(0xEB)
#define W25Q64JV_SET_BURST_WITH_WRAP		(0x77)
#define W25Q64JV_PAGE_PROGRAM				(0x02)
#define W25Q64JV_QUAD_INPUT_PAGE_PROGRAM	(0x32)
#define W25Q64JV_ERASE_SECTOR 				(0x20)
#define W25Q64JV_ERASE_32KB_BLOCK			(0x52)
#define W25Q64JV_ERASE_64KB_BLOCK			(0xD8)
#define W25Q64JV_ERASE_CHIP					(0xC7)
//#define W25Q64JV_ERASE_CHIP					(0x60)
#define W25Q64JV_ERASE_PROGRAM_SUSPEND		(0x75)
#define W25Q64JV_ERASE_PROGRAM_RESUME		(0x7A)
#define W25Q64JV_POWER_DOWN					(0xB9)
#define W25Q64JV_RELEASE_POWER_DOWN			(0xAB)
#define W25Q64JV_GET_MANUFACTURER_ID		(0x90)
#define W25Q64JV_GET_MANUFACTURER_ID_DUAL	(0x92)
#define W25Q64JV_GET_MANUFACTURER_ID_QUAD	(0x94)
#define W25Q64JV_GET_UNIQUE_ID				(0x4B)
#define W25Q64JV_GET_JEDEC_ID				(0x9F)
#define W25Q64JV_GET_SFDP_REG				(0x5A)
#define W25Q64JV_ERASE_SECURITY_REG			(0x44)
#define W25Q64JV_PROGRAM_SECURITY_REG		(0x42)
#define W25Q64JV_READ_SECURITY_REG			(0x48)
#define W25Q64JV_INDIVIDUAL_BLOCK_SECTOR_LOCK		(0x36)
#define W25Q64JV_INDIVIDUAL_BLOCK_SECTOR_UNLOCK		(0x39)
#define W25Q64JV_READ_BLOCK_SECTOR_LOCK				(0x3D)
#define W25Q64JV_GLOBAL_BLOCK_SECTOR_LOCK			(0x7E)
#define W25Q64JV_GLOBAL_BLOCK_SECTOR_UNLOCK			(0x98)
#define W25Q64JV_ENABLE_RESET						(0x66)
#define W25Q64JV_RESET_DEVICE						(0x99)

/* 
 * @ Note: 	BP0 ~ BP2, TB, SEC, WPS, CMP are used for memory protection.
 * 			if WPS == 1, we can ignore BP0 ~ BP2, TB, SEC, WPS, CMP.
 * @ Note: 	SRL and LB[3:1] cannot be changed from “1” to “0” because of the OTP protection for these bits.
 */
#define W25Q64JV_SR1_BUSY	(0x01U)		// Erase/Write In Progress, Status Only
#define W25Q64JV_SR1_WEL	(0x02U)		// Write Enable Latch, Status Only
#define W25Q64JV_SR1_BP0	(0x04U)		// Block Protect Bits, Volatile/Non-Volatile Writable
#define W25Q64JV_SR1_BP1	(0x08U)		// Block Protect Bits, Volatile/Non-Volatile Writable
#define W25Q64JV_SR1_BP2	(0x10U)		// Block Protect Bits, Volatile/Non-Volatile Writable
#define W25Q64JV_SR1_TB		(0x20U)		// Top/Bottom Block Protect, Volatile/Non-Volatile Writable
#define W25Q64JV_SR1_SEC	(0x40U)		// Sector/Block Protect Bit, Volatile/Non-Volatile Writable
#define W25Q64JV_SR1_SRP	(0x80U)		// Status Register Protect, Volatile/Non-Volatile Writable

#define W25Q64JV_SR2_SRL	(0x01U)		// Status Register Protect, Volatile/Non-Volatile Writable
#define W25Q64JV_SR2_QE		(0x02U)		// Quad Enable, Volatile/Non-Volatile Writable
#define W25Q64JV_SR2_LB1	(0x08U)		// Security Register Lock Bits, Volatile/Non-Volatile Writable
#define W25Q64JV_SR2_LB2	(0x10U)		// Security Register Lock Bits, Volatile/Non-Volatile Writable
#define W25Q64JV_SR2_LB3	(0x20U)		// Security Register Lock Bits, Volatile/Non-Volatile Writable
#define W25Q64JV_SR2_CMP	(0x40U)		// Complement Protect, Volatile/Non-Volatile Writable
#define W25Q64JV_SR2_SUS	(0x80U)		// Erase/Program Suspend Status, Status Only

#define W25Q64JV_SR3_WPS	(0x04U)		// Write Protect Selection, Volatile/Non-Volatile Writable
#define W25Q64JV_SR3_DRV0	(0x20U)		// Output Driver Strength, Volatile/Non-Volatile Writable
#define W25Q64JV_SR3_DRV1	(0x40U)		// Output Driver Strength, Volatile/Non-Volatile Writable

/* Set Burst With Wrap */
#define W25Q64JV_WRAP_NONE		(0x10U)
#define W25Q64JV_WRAP_8_BYTS	(0x00U)
#define W25Q64JV_WRAP_16_BYTS	(0x20U)
#define W25Q64JV_WRAP_32_BYTS	(0x40U)
#define W25Q64JV_WRAP_64_BYTS	(0x60U)


/**
 * @def ALIGN_UP(size, align)
 * Return the most contiguous size aligned at specified width. ALIGN_UP(13, 4)
 * would return 16.
 */
#define ALIGN_UP(size, align)           (((size) + (align) - 1) & ~((align) - 1))

/**
 * @def ALIGN_DOWN(size, align)
 * Return the down number of aligned at specified width. ALIGN_DOWN(13, 4)
 * would return 12.
 */
#define ALIGN_DOWN(size, align)      ((size) & ~((align) - 1))

int W25QXX_Auto_Polling_Busy(uint32_t timeout);
int W25QXX_Check_Busy_Sus(void);
int W25QXX_Quad_Enable(void);
int W25QXX_Write_Enable(void);
int W25QXX_Write_Enable_for_VSR(void);
int W25QXX_Write_Disable(void);
int W25QXX_Read_Status_Reg(uint8_t instruction, uint8_t *buf);
int W25QXX_Write_Status_Reg(uint8_t instruction, uint8_t *buf);
int W25QXX_Write_Volatile_Status_Reg(uint8_t instruction, uint8_t *buf);
int W25QXX_Read_Data(uint32_t addr, uint8_t *buf, uint32_t size);
int W25QXX_Fast_Read(uint32_t addr, uint8_t *buf, uint32_t size);
int W25QXX_Fast_Read_Dual_Output(uint32_t addr, uint8_t *buf, uint32_t size);
int W25QXX_Fast_Read_Quad_Output(uint32_t addr, uint8_t *buf, uint32_t size);
int W25QXX_Fast_Read_Dual_IO(uint32_t addr, uint8_t *buf, uint32_t size);
int W25QXX_Fast_Read_Quad_IO(uint32_t addr, uint8_t *buf, uint32_t size);
int W25QXX_Set_Burst_With_Wrap(uint8_t wrap_status);
int W25QXX_Page_Program(uint32_t addr, uint8_t *buf, uint16_t size);
int W25QXX_Quad_Input_Page_Program(uint32_t addr, uint8_t *buf, uint16_t size);
int W25QXX_Erase_Sector(uint32_t sector_address);
int W25QXX_Erase_32KB_Block(uint32_t block_address);
int W25QXX_Erase_64KB_Block(uint32_t block_address);
int W25QXX_Erase_Chip(void);
int W25QXX_Erase_Program_Suspend(void);
int W25QXX_Erase_Program_Resume(void);
int W25QXX_Power_Down(void);
int W25QXX_Release_Power_Down_Without_Device_ID(void);
int W25QXX_Release_Power_Down_With_Device_ID(uint8_t *buf);
int W25QXX_Read_Manufacturer_ID(uint8_t *buf);
int W25QXX_Read_Manufacturer_ID_Dual(uint8_t *buf);
int W25QXX_Read_Manufacturer_ID_Quad(uint8_t *buf);
int W25QXX_Read_Unique_ID(uint8_t *buf);
int W25QXX_Read_Jedec_ID(uint8_t *buf);
int W25QXX_Read_SFDP_Reg(uint8_t *buf, uint8_t sfdp_addr, uint16_t size);
int W25QXX_Erase_Security_Reg(uint32_t addr);
int W25QXX_Program_Security_Reg(uint32_t addr, uint8_t *buf, uint16_t size);
int W25QXX_Read_Security_Reg(uint32_t addr, uint8_t *buf, uint16_t size);
int W25QXX_Individual_Block_Sector_Lock(uint32_t addr);
int W25QXX_Individual_Block_Sector_UnLock(uint32_t addr);
int W25QXX_Read_Block_Sector_Lock(uint32_t addr, uint8_t *buf);
int W25QXX_Global_Block_Sector_Lock(void);
int W25QXX_Global_Block_Sector_Unlock(void);
int W25QXX_Reset(void);
int W25QXX_Enable_Memory_Mapped(void);
int W25QXX_Disable_Memory_Mapped(void);
int W25QXX_Write(uint32_t addr, uint8_t *buf, uint32_t size);

#endif
