#include "w25q64jv.h"

int W25QXX_Auto_Polling_Busy(uint32_t timeout)
{
	QSPI_CommandTypeDef s_command_cfg = {0};
	QSPI_AutoPollingTypeDef s_auto_polling_cfg = {0};

	/* 1.指令阶段 */
	s_command_cfg.Instruction 		= W25Q64JV_READ_STATUS_REG1;
	s_command_cfg.InstructionMode 	= QSPI_INSTRUCTION_1_LINE;
	
	/* 2.地址阶段 */
	s_command_cfg.Address 			= 0x00;
	s_command_cfg.AddressSize		= QSPI_ADDRESS_8_BITS;
	s_command_cfg.AddressMode		= QSPI_ADDRESS_NONE;

	/* 3.交替字节阶段 */
	s_command_cfg.AlternateBytes	= 0x00;
	s_command_cfg.AlternateBytesSize= QSPI_ALTERNATE_BYTES_8_BITS;
	s_command_cfg.AlternateByteMode	= QSPI_ALTERNATE_BYTES_NONE;
	
	/* 4.空周期阶段 */
	s_command_cfg.DummyCycles		= 0x00;
	
	/* 5.数据阶段 */
	s_command_cfg.DataMode			= QSPI_DATA_1_LINE;
	s_command_cfg.NbData 			= 0x01;  
	
	/* 6.其他设置 */
	s_command_cfg.DdrMode			= QSPI_DDR_MODE_DISABLE;
	s_command_cfg.DdrHoldHalfCycle 	= QSPI_DDR_HHC_ANALOG_DELAY;
	s_command_cfg.SIOOMode			= QSPI_SIOO_INST_EVERY_CMD;
	
	/* 屏蔽位设置的bit0，匹配位等待bit0为0，即不断查询状态寄存器bit0，等待其为0 */
    s_auto_polling_cfg.Match 		= 0x00;
    s_auto_polling_cfg.Mask 		= 0x01;
    s_auto_polling_cfg.MatchMode 	= QSPI_MATCH_MODE_AND;
    s_auto_polling_cfg.StatusBytesSize 	= 0x01;
    s_auto_polling_cfg.Interval 	= 0x10;
    s_auto_polling_cfg.AutomaticStop	= QSPI_AUTOMATIC_STOP_ENABLE;

    if (HAL_QSPI_AutoPolling(&hqspi, &s_command_cfg, &s_auto_polling_cfg, timeout) != HAL_OK)
    {
        return QSPI_ERROR;
    }

    return QSPI_OK;
}

int W25QXX_Check_Busy_Sus(void)
{
	uint8_t reg1_buf = 0x00;
	uint8_t reg2_buf = 0x00;

	if(W25QXX_Read_Status_Reg(W25Q64JV_READ_STATUS_REG1, &reg1_buf) != QSPI_OK)
	{
		return QSPI_ERROR;
	}

	if(W25QXX_Read_Status_Reg(W25Q64JV_READ_STATUS_REG2, &reg2_buf) != QSPI_OK)
	{
		return QSPI_ERROR;
	}

	if((reg1_buf & W25Q64JV_SR1_BUSY) || (reg2_buf & W25Q64JV_SR2_SUS))
	{
		return QSPI_ERROR;
	}

    return QSPI_OK;
}


int W25QXX_Quad_Enable(void)
{
	uint8_t reg_data = 0x00;

	if(W25QXX_Read_Status_Reg(W25Q64JV_READ_STATUS_REG2, (uint8_t *)&reg_data) != QSPI_OK)
	{
		return QSPI_ERROR;
	}

	if(reg_data & W25Q64JV_SR2_QE)	return QSPI_OK;

	reg_data |= W25Q64JV_SR2_QE;
	if(W25QXX_Write_Volatile_Status_Reg(W25Q64JV_WRITE_STATUS_REG2, (uint8_t *)&reg_data) != QSPI_OK)
	{
		return QSPI_ERROR;
	}

	if(W25QXX_Auto_Polling_Busy(HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		return QSPI_ERROR;
	}

	return QSPI_OK;
}

int W25QXX_Write_Enable(void)
{
	QSPI_CommandTypeDef s_command_cfg = {0};

	/* 1.指令阶段 */
	s_command_cfg.Instruction 		= W25Q64JV_WRITE_ENABLE;
	s_command_cfg.InstructionMode 	= QSPI_INSTRUCTION_1_LINE;
	
	/* 2.地址阶段 */
	s_command_cfg.Address 			= 0x00;
	s_command_cfg.AddressSize		= QSPI_ADDRESS_8_BITS;
	s_command_cfg.AddressMode		= QSPI_ADDRESS_NONE;

	/* 3.交替字节阶段 */
	s_command_cfg.AlternateBytes	= 0x00;
	s_command_cfg.AlternateBytesSize= QSPI_ALTERNATE_BYTES_8_BITS;
	s_command_cfg.AlternateByteMode	= QSPI_ALTERNATE_BYTES_NONE;
	
	/* 4.空周期阶段 */
	s_command_cfg.DummyCycles		= 0x00;
	
	/* 5.数据阶段 */
	s_command_cfg.DataMode			= QSPI_DATA_NONE;
	s_command_cfg.NbData 			= 0x00;
	
	/* 6.其他设置 */
	s_command_cfg.DdrMode			= QSPI_DDR_MODE_DISABLE;
	s_command_cfg.DdrHoldHalfCycle 	= QSPI_DDR_HHC_ANALOG_DELAY;
	s_command_cfg.SIOOMode			= QSPI_SIOO_INST_EVERY_CMD;

    if (HAL_QSPI_Command(&hqspi, &s_command_cfg, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        return QSPI_ERROR;
    }
	
    return QSPI_OK;
}

int W25QXX_Write_Enable_for_VSR(void)
{
	QSPI_CommandTypeDef s_command_cfg = {0};
	
	/* 1.指令阶段 */
	s_command_cfg.Instruction 		= W25Q64JV_WRITE_ENABLE_FOR_VSR;
	s_command_cfg.InstructionMode 	= QSPI_INSTRUCTION_1_LINE;
	
	/* 2.地址阶段 */
	s_command_cfg.Address 			= 0x00;
	s_command_cfg.AddressSize		= QSPI_ADDRESS_8_BITS;
	s_command_cfg.AddressMode		= QSPI_ADDRESS_NONE;

	/* 3.交替字节阶段 */
	s_command_cfg.AlternateBytes	= 0x00;
	s_command_cfg.AlternateBytesSize= QSPI_ALTERNATE_BYTES_8_BITS;
	s_command_cfg.AlternateByteMode	= QSPI_ALTERNATE_BYTES_NONE;
	
	/* 4.空周期阶段 */
	s_command_cfg.DummyCycles		= 0x00;
	
	/* 5.数据阶段 */
	s_command_cfg.DataMode			= QSPI_DATA_NONE;
	s_command_cfg.NbData 			= 0x00;
	
	/* 6.其他设置 */
	s_command_cfg.DdrMode			= QSPI_DDR_MODE_DISABLE;
	s_command_cfg.DdrHoldHalfCycle 	= QSPI_DDR_HHC_ANALOG_DELAY;
	s_command_cfg.SIOOMode			= QSPI_SIOO_INST_EVERY_CMD;

    if (HAL_QSPI_Command(&hqspi, &s_command_cfg, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        return QSPI_ERROR;
    }

	return QSPI_OK;
}

int W25QXX_Write_Disable(void)
{
	QSPI_CommandTypeDef s_command_cfg = {0};
	
	/* 1.指令阶段 */
	s_command_cfg.Instruction 		= W25Q64JV_WRITE_DISABLE;
	s_command_cfg.InstructionMode 	= QSPI_INSTRUCTION_1_LINE;
	
	/* 2.地址阶段 */
	s_command_cfg.Address 			= 0x00;
	s_command_cfg.AddressSize		= QSPI_ADDRESS_8_BITS;
	s_command_cfg.AddressMode		= QSPI_ADDRESS_NONE;

	/* 3.交替字节阶段 */
	s_command_cfg.AlternateBytes	= 0x00;
	s_command_cfg.AlternateBytesSize= QSPI_ALTERNATE_BYTES_8_BITS;
	s_command_cfg.AlternateByteMode	= QSPI_ALTERNATE_BYTES_NONE;
	
	/* 4.空周期阶段 */
	s_command_cfg.DummyCycles		= 0x00;
	
	/* 5.数据阶段 */
	s_command_cfg.DataMode			= QSPI_DATA_NONE;
	s_command_cfg.NbData 			= 0x00;
	
	/* 6.其他设置 */
	s_command_cfg.DdrMode			= QSPI_DDR_MODE_DISABLE;
	s_command_cfg.DdrHoldHalfCycle 	= QSPI_DDR_HHC_ANALOG_DELAY;
	s_command_cfg.SIOOMode			= QSPI_SIOO_INST_EVERY_CMD;

    if (HAL_QSPI_Command(&hqspi, &s_command_cfg, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        return QSPI_ERROR;
    }

	return QSPI_OK;
}

int W25QXX_Read_Status_Reg(uint8_t instruction, uint8_t *buf)
{
	QSPI_CommandTypeDef s_command_cfg = {0};
	
	/* 1.指令阶段 */
	s_command_cfg.Instruction 		= instruction;
	s_command_cfg.InstructionMode 	= QSPI_INSTRUCTION_1_LINE;
	
	/* 2.地址阶段 */
	s_command_cfg.Address 			= 0x00;
	s_command_cfg.AddressSize		= QSPI_ADDRESS_8_BITS;
	s_command_cfg.AddressMode		= QSPI_ADDRESS_NONE;

	/* 3.交替字节阶段 */
	s_command_cfg.AlternateBytes	= 0x00;
	s_command_cfg.AlternateBytesSize= QSPI_ALTERNATE_BYTES_8_BITS;
	s_command_cfg.AlternateByteMode	= QSPI_ALTERNATE_BYTES_NONE;
	
	/* 4.空周期阶段 */
	s_command_cfg.DummyCycles		= 0x00;
	
	/* 5.数据阶段 */
	s_command_cfg.DataMode			= QSPI_DATA_1_LINE;
	s_command_cfg.NbData 			= 0x01;
	
	/* 6.其他设置 */
	s_command_cfg.DdrMode			= QSPI_DDR_MODE_DISABLE;
	s_command_cfg.DdrHoldHalfCycle 	= QSPI_DDR_HHC_ANALOG_DELAY;
	s_command_cfg.SIOOMode			= QSPI_SIOO_INST_EVERY_CMD;

    if (HAL_QSPI_Command(&hqspi, &s_command_cfg, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        return QSPI_ERROR;
    }
	
	if (HAL_QSPI_Receive(&hqspi, (uint8_t *)buf, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        return QSPI_ERROR;
    }

    return QSPI_OK;
}

int W25QXX_Write_Status_Reg(uint8_t instruction, uint8_t *buf)
{
	QSPI_CommandTypeDef s_command_cfg = {0};
	
	if(W25QXX_Write_Enable() != QSPI_OK)		return QSPI_ERROR;
	
	/* 1.指令阶段 */
	s_command_cfg.Instruction 		= instruction;
	s_command_cfg.InstructionMode 	= QSPI_INSTRUCTION_1_LINE;
	
	/* 2.地址阶段 */
	s_command_cfg.Address 			= 0x00;
	s_command_cfg.AddressSize		= QSPI_ADDRESS_8_BITS;
	s_command_cfg.AddressMode		= QSPI_ADDRESS_NONE;

	/* 3.交替字节阶段 */
	s_command_cfg.AlternateBytes	= 0x00;
	s_command_cfg.AlternateBytesSize= QSPI_ALTERNATE_BYTES_8_BITS;
	s_command_cfg.AlternateByteMode	= QSPI_ALTERNATE_BYTES_NONE;
	
	/* 4.空周期阶段 */
	s_command_cfg.DummyCycles		= 0x00;
	
	/* 5.数据阶段 */
	s_command_cfg.DataMode			= QSPI_DATA_1_LINE;
	s_command_cfg.NbData 			= 1;  
	
	/* 6.其他设置 */
	s_command_cfg.DdrMode			= QSPI_DDR_MODE_DISABLE;
	s_command_cfg.DdrHoldHalfCycle 	= QSPI_DDR_HHC_ANALOG_DELAY;
	s_command_cfg.SIOOMode			= QSPI_SIOO_INST_EVERY_CMD;

    if (HAL_QSPI_Command(&hqspi, &s_command_cfg, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        return QSPI_ERROR;
    }
	
	if(HAL_QSPI_Transmit(&hqspi, (uint8_t *)buf, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		return QSPI_ERROR;
	}

	if(W25QXX_Auto_Polling_Busy(HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != QSPI_OK)
    {
        return QSPI_ERROR;
    }

	return QSPI_OK;
}

/*
 * Note: SRL and LB[3:1] cannot be changed from “1” to “0” 
 *		 because of the OTP protection for these bits
 */
int W25QXX_Write_Volatile_Status_Reg(uint8_t instruction, uint8_t *buf)
{
	QSPI_CommandTypeDef s_command_cfg = {0};
	
	if(W25QXX_Write_Enable_for_VSR() != QSPI_OK)		return QSPI_ERROR;
	if(W25QXX_Write_Enable() != QSPI_OK)		return QSPI_ERROR;
	
	/* 1.指令阶段 */
	s_command_cfg.Instruction 		= instruction;
	s_command_cfg.InstructionMode 	= QSPI_INSTRUCTION_1_LINE;
	
	/* 2.地址阶段 */
	s_command_cfg.Address 			= 0x00;
	s_command_cfg.AddressSize		= QSPI_ADDRESS_8_BITS;
	s_command_cfg.AddressMode		= QSPI_ADDRESS_NONE;

	/* 3.交替字节阶段 */
	s_command_cfg.AlternateBytes	= 0x00;
	s_command_cfg.AlternateBytesSize= QSPI_ALTERNATE_BYTES_8_BITS;
	s_command_cfg.AlternateByteMode	= QSPI_ALTERNATE_BYTES_NONE;
	
	/* 4.空周期阶段 */
	s_command_cfg.DummyCycles		= 0x00;
	
	/* 5.数据阶段 */
	s_command_cfg.DataMode			= QSPI_DATA_1_LINE;
	s_command_cfg.NbData 			= 1;  
	
	/* 6.其他设置 */
	s_command_cfg.DdrMode			= QSPI_DDR_MODE_DISABLE;
	s_command_cfg.DdrHoldHalfCycle 	= QSPI_DDR_HHC_ANALOG_DELAY;
	s_command_cfg.SIOOMode			= QSPI_SIOO_INST_EVERY_CMD;

    if (HAL_QSPI_Command(&hqspi, &s_command_cfg, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        return QSPI_ERROR;
    }
	
	if(HAL_QSPI_Transmit(&hqspi, (uint8_t *)buf, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		return QSPI_ERROR;
	}

	if(W25QXX_Auto_Polling_Busy(HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != QSPI_OK)
    {
        return QSPI_ERROR;
    }

	return QSPI_OK;
}

int W25QXX_Read_Data(uint32_t addr, uint8_t *buf, uint32_t size)
{
	QSPI_CommandTypeDef s_command_cfg = {0};

	/* 1.指令阶段 */
	s_command_cfg.Instruction 		= W25Q64JV_READ_DATA;
	s_command_cfg.InstructionMode 	= QSPI_INSTRUCTION_1_LINE;
	
	/* 2.地址阶段 */
	s_command_cfg.Address 			= addr;
	s_command_cfg.AddressSize		= QSPI_ADDRESS_24_BITS;
	s_command_cfg.AddressMode		= QSPI_ADDRESS_1_LINE;

	/* 3.交替字节阶段 */
	s_command_cfg.AlternateBytes	= 0x00;
	s_command_cfg.AlternateBytesSize= QSPI_ALTERNATE_BYTES_8_BITS;
	s_command_cfg.AlternateByteMode	= QSPI_ALTERNATE_BYTES_NONE;
	
	/* 4.空周期阶段 */
	s_command_cfg.DummyCycles		= 0x00;
	
	/* 5.数据阶段 */
	s_command_cfg.DataMode			= QSPI_DATA_1_LINE;
	s_command_cfg.NbData 			= size;  
	
	/* 6.其他设置 */
	s_command_cfg.DdrMode			= QSPI_DDR_MODE_DISABLE;
	s_command_cfg.DdrHoldHalfCycle 	= QSPI_DDR_HHC_ANALOG_DELAY;
	s_command_cfg.SIOOMode			= QSPI_SIOO_INST_EVERY_CMD;

    if (HAL_QSPI_Command(&hqspi, &s_command_cfg, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        return QSPI_ERROR;
    }
	
	if(HAL_QSPI_Receive(&hqspi, (uint8_t *)buf, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		return QSPI_ERROR;
	}

	return QSPI_OK;
}

int W25QXX_Fast_Read(uint32_t addr, uint8_t *buf, uint32_t size)
{
	QSPI_CommandTypeDef s_command_cfg = {0};
	
	/* 1.指令阶段 */
	s_command_cfg.Instruction 		= W25Q64JV_FAST_READ;
	s_command_cfg.InstructionMode 	= QSPI_INSTRUCTION_1_LINE;
	
	/* 2.地址阶段 */
	s_command_cfg.Address 			= addr;
	s_command_cfg.AddressSize		= QSPI_ADDRESS_24_BITS;
	s_command_cfg.AddressMode		= QSPI_ADDRESS_1_LINE;

	/* 3.交替字节阶段 */
	s_command_cfg.AlternateBytes	= 0x00;
	s_command_cfg.AlternateBytesSize= QSPI_ALTERNATE_BYTES_8_BITS;
	s_command_cfg.AlternateByteMode	= QSPI_ALTERNATE_BYTES_NONE;
	
	/* 4.空周期阶段 */
	s_command_cfg.DummyCycles		= 0x08;
	
	/* 5.数据阶段 */
	s_command_cfg.DataMode			= QSPI_DATA_1_LINE;
	s_command_cfg.NbData 			= size;  
	
	/* 6.其他设置 */
	s_command_cfg.DdrMode			= QSPI_DDR_MODE_DISABLE;
	s_command_cfg.DdrHoldHalfCycle 	= QSPI_DDR_HHC_ANALOG_DELAY;
	s_command_cfg.SIOOMode			= QSPI_SIOO_INST_EVERY_CMD;

    if (HAL_QSPI_Command(&hqspi, &s_command_cfg, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        return QSPI_ERROR;
    }
	
	if(HAL_QSPI_Receive(&hqspi, (uint8_t *)buf, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		return QSPI_ERROR;
	}

	return QSPI_OK;
}

int W25QXX_Fast_Read_Dual_Output(uint32_t addr, uint8_t *buf, uint32_t size)
{
	QSPI_CommandTypeDef s_command_cfg = {0};
	
	/* 1.指令阶段 */
	s_command_cfg.Instruction 		= W25Q64JV_FAST_READ_DUAL_OUTPUT;
	s_command_cfg.InstructionMode 	= QSPI_INSTRUCTION_1_LINE;
	
	/* 2.地址阶段 */
	s_command_cfg.Address 			= addr;
	s_command_cfg.AddressSize		= QSPI_ADDRESS_24_BITS;
	s_command_cfg.AddressMode		= QSPI_ADDRESS_1_LINE;

	/* 3.交替字节阶段 */
	s_command_cfg.AlternateBytes	= 0x00;
	s_command_cfg.AlternateBytesSize= QSPI_ALTERNATE_BYTES_8_BITS;
	s_command_cfg.AlternateByteMode	= QSPI_ALTERNATE_BYTES_NONE;
	
	/* 4.空周期阶段 */
	s_command_cfg.DummyCycles		= 0x08;
	
	/* 5.数据阶段 */
	s_command_cfg.DataMode			= QSPI_DATA_2_LINES;
	s_command_cfg.NbData 			= size;  
	
	/* 6.其他设置 */
	s_command_cfg.DdrMode			= QSPI_DDR_MODE_DISABLE;
	s_command_cfg.DdrHoldHalfCycle 	= QSPI_DDR_HHC_ANALOG_DELAY;
	s_command_cfg.SIOOMode			= QSPI_SIOO_INST_EVERY_CMD;

    if (HAL_QSPI_Command(&hqspi, &s_command_cfg, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        return QSPI_ERROR;
    }
	
	if(HAL_QSPI_Receive(&hqspi, (uint8_t *)buf, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		return QSPI_ERROR;
	}

	return QSPI_OK;
}

int W25QXX_Fast_Read_Quad_Output(uint32_t addr, uint8_t *buf, uint32_t size)
{
	QSPI_CommandTypeDef s_command_cfg = {0};
	
	/* 1.指令阶段 */
	s_command_cfg.Instruction 		= W25Q64JV_FAST_READ_QUAD_OUTPUT;
	s_command_cfg.InstructionMode 	= QSPI_INSTRUCTION_1_LINE;
	
	/* 2.地址阶段 */
	s_command_cfg.Address 			= addr;
	s_command_cfg.AddressSize		= QSPI_ADDRESS_24_BITS;
	s_command_cfg.AddressMode		= QSPI_ADDRESS_1_LINE;

	/* 3.交替字节阶段 */
	s_command_cfg.AlternateBytes	= 0x00;
	s_command_cfg.AlternateBytesSize= QSPI_ALTERNATE_BYTES_8_BITS;
	s_command_cfg.AlternateByteMode	= QSPI_ALTERNATE_BYTES_NONE;
	
	/* 4.空周期阶段 */
	s_command_cfg.DummyCycles		= 0x08;
	
	/* 5.数据阶段 */
	s_command_cfg.DataMode			= QSPI_DATA_4_LINES;
	s_command_cfg.NbData 			= size;  
	
	/* 6.其他设置 */
	s_command_cfg.DdrMode			= QSPI_DDR_MODE_DISABLE;
	s_command_cfg.DdrHoldHalfCycle 	= QSPI_DDR_HHC_ANALOG_DELAY;
	s_command_cfg.SIOOMode			= QSPI_SIOO_INST_EVERY_CMD;

    if (HAL_QSPI_Command(&hqspi, &s_command_cfg, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        return QSPI_ERROR;
    }
	
	if(HAL_QSPI_Receive(&hqspi, (uint8_t *)buf, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		return QSPI_ERROR;
	}

	return QSPI_OK;
}

int W25QXX_Fast_Read_Dual_IO(uint32_t addr, uint8_t *buf, uint32_t size)
{
	QSPI_CommandTypeDef s_command_cfg = {0};
	
	/* 1.指令阶段 */
	s_command_cfg.Instruction 		= W25Q64JV_FAST_READ_DUAL_IO;
	s_command_cfg.InstructionMode 	= QSPI_INSTRUCTION_1_LINE;
	
	/* 2.地址阶段 */
	s_command_cfg.Address 			= addr;
	s_command_cfg.AddressSize		= QSPI_ADDRESS_24_BITS;
	s_command_cfg.AddressMode		= QSPI_ADDRESS_2_LINES;

	/* 3.交替字节阶段 */
	s_command_cfg.AlternateBytes	= 0xFF;
	s_command_cfg.AlternateBytesSize= QSPI_ALTERNATE_BYTES_8_BITS;
	s_command_cfg.AlternateByteMode	= QSPI_ALTERNATE_BYTES_2_LINES;
	
	/* 4.空周期阶段 */
	s_command_cfg.DummyCycles		= 0x00;
	
	/* 5.数据阶段 */
	s_command_cfg.DataMode			= QSPI_DATA_2_LINES;
	s_command_cfg.NbData 			= size;  
	
	/* 6.其他设置 */
	s_command_cfg.DdrMode			= QSPI_DDR_MODE_DISABLE;
	s_command_cfg.DdrHoldHalfCycle 	= QSPI_DDR_HHC_ANALOG_DELAY;
	s_command_cfg.SIOOMode			= QSPI_SIOO_INST_EVERY_CMD;

    if (HAL_QSPI_Command(&hqspi, &s_command_cfg, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        return QSPI_ERROR;
    }
	
	if(HAL_QSPI_Receive(&hqspi, (uint8_t *)buf, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		return QSPI_ERROR;
	}
	
	return QSPI_OK;
}

int W25QXX_Fast_Read_Quad_IO(uint32_t addr, uint8_t *buf, uint32_t size)
{
	QSPI_CommandTypeDef s_command_cfg = {0};

	if(W25QXX_Quad_Enable() != QSPI_OK)		return QSPI_ERROR;
	
	/* 1.指令阶段 */
	s_command_cfg.Instruction 		= W25Q64JV_FAST_READ_QUAD_IO;
	s_command_cfg.InstructionMode 	= QSPI_INSTRUCTION_1_LINE;
	
	/* 2.地址阶段 */
	s_command_cfg.Address 			= addr;
	s_command_cfg.AddressSize		= QSPI_ADDRESS_24_BITS;
	s_command_cfg.AddressMode		= QSPI_ADDRESS_4_LINES;

	/* 3.交替字节阶段 */
	s_command_cfg.AlternateBytes	= 0xFF;
	s_command_cfg.AlternateBytesSize= QSPI_ALTERNATE_BYTES_8_BITS;
	s_command_cfg.AlternateByteMode	= QSPI_ALTERNATE_BYTES_4_LINES;
	
	/* 4.空周期阶段 */
	s_command_cfg.DummyCycles		= 0x04;
	
	/* 5.数据阶段 */
	s_command_cfg.DataMode			= QSPI_DATA_4_LINES;
	s_command_cfg.NbData 			= size;
	
	/* 6.其他设置 */
	s_command_cfg.DdrMode			= QSPI_DDR_MODE_DISABLE;
	s_command_cfg.DdrHoldHalfCycle 	= QSPI_DDR_HHC_ANALOG_DELAY;
	s_command_cfg.SIOOMode			= QSPI_SIOO_INST_EVERY_CMD;

    if (HAL_QSPI_Command(&hqspi, &s_command_cfg, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        return QSPI_ERROR;
    }
	
	if(HAL_QSPI_Receive(&hqspi, (uint8_t *)buf, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		return QSPI_ERROR;
	}

	return QSPI_OK;
}

int W25QXX_Set_Burst_With_Wrap(uint8_t wrap_status)
{
	QSPI_CommandTypeDef s_command_cfg = {0};
	uint8_t status = wrap_status;
	
	/* 1.指令阶段 */
	s_command_cfg.Instruction 		= W25Q64JV_SET_BURST_WITH_WRAP;
	s_command_cfg.InstructionMode 	= QSPI_INSTRUCTION_1_LINE;

	/* 2.地址阶段 */
	s_command_cfg.Address 			= 0x00;
	s_command_cfg.AddressSize		= QSPI_ADDRESS_8_BITS;
	s_command_cfg.AddressMode		= QSPI_ADDRESS_NONE;

	/* 3.交替字节阶段 */
	s_command_cfg.AlternateBytes	= 0x00;
	s_command_cfg.AlternateBytesSize= QSPI_ALTERNATE_BYTES_8_BITS;
	s_command_cfg.AlternateByteMode	= QSPI_ALTERNATE_BYTES_NONE;
	
	/* 4.空周期阶段 */
	s_command_cfg.DummyCycles		= 0x06;
	
	/* 5.数据阶段 */
	s_command_cfg.DataMode			= QSPI_DATA_4_LINES;
	s_command_cfg.NbData 			= 0x01;
	
	/* 6.其他设置 */
	s_command_cfg.DdrMode			= QSPI_DDR_MODE_DISABLE;
	s_command_cfg.DdrHoldHalfCycle 	= QSPI_DDR_HHC_ANALOG_DELAY;
	s_command_cfg.SIOOMode			= QSPI_SIOO_INST_EVERY_CMD;

    if (HAL_QSPI_Command(&hqspi, &s_command_cfg, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        return QSPI_ERROR;
    }

	if(HAL_QSPI_Transmit(&hqspi, (uint8_t *)&status, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		return QSPI_ERROR;
	}

	return QSPI_OK;
}

int W25QXX_Page_Program(uint32_t addr, uint8_t *buf, uint16_t size)
{
	QSPI_CommandTypeDef s_command_cfg = {0};

	if(size == 0)	return QSPI_OK;
	if(W25QXX_Write_Enable() != QSPI_OK)		return QSPI_ERROR;
	
	/* 1.指令阶段 */
	s_command_cfg.Instruction 		= W25Q64JV_PAGE_PROGRAM;
	s_command_cfg.InstructionMode 	= QSPI_INSTRUCTION_1_LINE;

	/* 2.地址阶段 */
	s_command_cfg.Address 			= addr;
	s_command_cfg.AddressSize		= QSPI_ADDRESS_24_BITS;
	s_command_cfg.AddressMode		= QSPI_ADDRESS_1_LINE;

	/* 3.交替字节阶段 */
	s_command_cfg.AlternateBytes	= 0x00;
	s_command_cfg.AlternateBytesSize= QSPI_ALTERNATE_BYTES_8_BITS;
	s_command_cfg.AlternateByteMode	= QSPI_ALTERNATE_BYTES_NONE;
	
	/* 4.空周期阶段 */
	s_command_cfg.DummyCycles		= 0x00;
	
	/* 5.数据阶段 */
	s_command_cfg.DataMode			= QSPI_DATA_1_LINE;
	s_command_cfg.NbData 			= size;
	
	/* 6.其他设置 */
	s_command_cfg.DdrMode			= QSPI_DDR_MODE_DISABLE;
	s_command_cfg.DdrHoldHalfCycle 	= QSPI_DDR_HHC_ANALOG_DELAY;
	s_command_cfg.SIOOMode			= QSPI_SIOO_INST_EVERY_CMD;

    if (HAL_QSPI_Command(&hqspi, &s_command_cfg, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        return QSPI_ERROR;
    }

	if(HAL_QSPI_Transmit(&hqspi, (uint8_t *)buf, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		return QSPI_ERROR;
	}

	if(W25QXX_Auto_Polling_Busy(HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		return QSPI_ERROR;
	}

	return QSPI_OK;
}

int W25QXX_Quad_Input_Page_Program(uint32_t addr, uint8_t *buf, uint16_t size)
{
	QSPI_CommandTypeDef s_command_cfg = {0};

	if(size == 0)	return QSPI_OK;
	if(W25QXX_Quad_Enable() != QSPI_OK)		return QSPI_ERROR;
	if(W25QXX_Write_Enable() != QSPI_OK)		return QSPI_ERROR;

	/* 1.指令阶段 */
	s_command_cfg.Instruction 		= W25Q64JV_QUAD_INPUT_PAGE_PROGRAM;
	s_command_cfg.InstructionMode 	= QSPI_INSTRUCTION_1_LINE;

	/* 2.地址阶段 */
	s_command_cfg.Address 			= addr;
	s_command_cfg.AddressSize		= QSPI_ADDRESS_24_BITS;
	s_command_cfg.AddressMode		= QSPI_ADDRESS_1_LINE;

	/* 3.交替字节阶段 */
	s_command_cfg.AlternateBytes	= 0x00;
	s_command_cfg.AlternateBytesSize= QSPI_ALTERNATE_BYTES_8_BITS;
	s_command_cfg.AlternateByteMode	= QSPI_ALTERNATE_BYTES_NONE;
	
	/* 4.空周期阶段 */
	s_command_cfg.DummyCycles		= 0x00;
	
	/* 5.数据阶段 */
	s_command_cfg.DataMode			= QSPI_DATA_4_LINES;
	s_command_cfg.NbData 			= size;
	
	/* 6.其他设置 */
	s_command_cfg.DdrMode			= QSPI_DDR_MODE_DISABLE;
	s_command_cfg.DdrHoldHalfCycle 	= QSPI_DDR_HHC_ANALOG_DELAY;
	s_command_cfg.SIOOMode			= QSPI_SIOO_INST_EVERY_CMD;

    if (HAL_QSPI_Command(&hqspi, &s_command_cfg, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        return QSPI_ERROR;
    }

	if(HAL_QSPI_Transmit(&hqspi, (uint8_t *)buf, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		return QSPI_ERROR;
	}

	if(W25QXX_Auto_Polling_Busy(HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		return QSPI_ERROR;
	}

	return QSPI_OK;
}

int W25QXX_Erase_Sector(uint32_t sector_address)
{
	QSPI_CommandTypeDef s_command_cfg = {0};
	uint32_t address = ALIGN_DOWN(sector_address, W25Q64JV_SECTOR_SIZE);

	if(W25QXX_Write_Enable() != QSPI_OK)		return QSPI_ERROR;

	/* 1.指令阶段 */
	s_command_cfg.Instruction 		= W25Q64JV_ERASE_SECTOR;
	s_command_cfg.InstructionMode 	= QSPI_INSTRUCTION_1_LINE;

	/* 2.地址阶段 */
	s_command_cfg.Address 			= address;
	s_command_cfg.AddressSize		= QSPI_ADDRESS_24_BITS;
	s_command_cfg.AddressMode		= QSPI_ADDRESS_1_LINE;

	/* 3.交替字节阶段 */
	s_command_cfg.AlternateBytes	= 0x00;
	s_command_cfg.AlternateBytesSize= QSPI_ALTERNATE_BYTES_8_BITS;
	s_command_cfg.AlternateByteMode	= QSPI_ALTERNATE_BYTES_NONE;
	
	/* 4.空周期阶段 */
	s_command_cfg.DummyCycles		= 0x00;
	
	/* 5.数据阶段 */
	s_command_cfg.DataMode			= QSPI_DATA_NONE;
	s_command_cfg.NbData 			= 0x00;
	
	/* 6.其他设置 */
	s_command_cfg.DdrMode			= QSPI_DDR_MODE_DISABLE;
	s_command_cfg.DdrHoldHalfCycle 	= QSPI_DDR_HHC_ANALOG_DELAY;
	s_command_cfg.SIOOMode			= QSPI_SIOO_INST_EVERY_CMD;

    if (HAL_QSPI_Command(&hqspi, &s_command_cfg, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        return QSPI_ERROR;
    }

    if (W25QXX_Auto_Polling_Busy(HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != QSPI_OK)
    {
        return QSPI_ERROR;
    }

    return QSPI_OK;
}

int W25QXX_Erase_32KB_Block(uint32_t block_address)
{
	QSPI_CommandTypeDef s_command_cfg = {0};
	uint32_t address = ALIGN_DOWN(block_address, W25Q64JV_SECTOR_SIZE);
	
	if(W25QXX_Write_Enable() != QSPI_OK)		return QSPI_ERROR;

	/* 1.指令阶段 */
	s_command_cfg.Instruction 		= W25Q64JV_ERASE_32KB_BLOCK;
	s_command_cfg.InstructionMode 	= QSPI_INSTRUCTION_1_LINE;

	/* 2.地址阶段 */
	s_command_cfg.Address 			= address;
	s_command_cfg.AddressSize		= QSPI_ADDRESS_24_BITS;
	s_command_cfg.AddressMode		= QSPI_ADDRESS_1_LINE;

	/* 3.交替字节阶段 */
	s_command_cfg.AlternateBytes	= 0x00;
	s_command_cfg.AlternateBytesSize= QSPI_ALTERNATE_BYTES_8_BITS;
	s_command_cfg.AlternateByteMode	= QSPI_ALTERNATE_BYTES_NONE;
	
	/* 4.空周期阶段 */
	s_command_cfg.DummyCycles		= 0x00;
	
	/* 5.数据阶段 */
	s_command_cfg.DataMode			= QSPI_DATA_NONE;
	s_command_cfg.NbData 			= 0x00;
	
	/* 6.其他设置 */
	s_command_cfg.DdrMode			= QSPI_DDR_MODE_DISABLE;
	s_command_cfg.DdrHoldHalfCycle 	= QSPI_DDR_HHC_ANALOG_DELAY;
	s_command_cfg.SIOOMode			= QSPI_SIOO_INST_EVERY_CMD;

    if (HAL_QSPI_Command(&hqspi, &s_command_cfg, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        return QSPI_ERROR;
    }

    if (W25QXX_Auto_Polling_Busy(HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != QSPI_OK)
    {
        return QSPI_ERROR;
    }

    return QSPI_OK;
}

int W25QXX_Erase_64KB_Block(uint32_t block_address)
{
	QSPI_CommandTypeDef s_command_cfg = {0};
	uint32_t address = ALIGN_DOWN(block_address, W25Q64JV_SECTOR_SIZE);
	
	if(W25QXX_Write_Enable() != QSPI_OK)		return QSPI_ERROR;

	/* 1.指令阶段 */
	s_command_cfg.Instruction 		= W25Q64JV_ERASE_64KB_BLOCK;
	s_command_cfg.InstructionMode 	= QSPI_INSTRUCTION_1_LINE;

	/* 2.地址阶段 */
	s_command_cfg.Address 			= address;
	s_command_cfg.AddressSize		= QSPI_ADDRESS_24_BITS;
	s_command_cfg.AddressMode		= QSPI_ADDRESS_1_LINE;

	/* 3.交替字节阶段 */
	s_command_cfg.AlternateBytes	= 0x00;
	s_command_cfg.AlternateBytesSize= QSPI_ALTERNATE_BYTES_8_BITS;
	s_command_cfg.AlternateByteMode	= QSPI_ALTERNATE_BYTES_NONE;
	
	/* 4.空周期阶段 */
	s_command_cfg.DummyCycles		= 0x00;
	
	/* 5.数据阶段 */
	s_command_cfg.DataMode			= QSPI_DATA_NONE;
	s_command_cfg.NbData 			= 0x00;
	
	/* 6.其他设置 */
	s_command_cfg.DdrMode			= QSPI_DDR_MODE_DISABLE;
	s_command_cfg.DdrHoldHalfCycle 	= QSPI_DDR_HHC_ANALOG_DELAY;
	s_command_cfg.SIOOMode			= QSPI_SIOO_INST_EVERY_CMD;

    if (HAL_QSPI_Command(&hqspi, &s_command_cfg, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        return QSPI_ERROR;
    }

    if (W25QXX_Auto_Polling_Busy(HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != QSPI_OK)
    {
        return QSPI_ERROR;
    }

    return QSPI_OK;
}

int W25QXX_Erase_Chip(void)
{
	QSPI_CommandTypeDef s_command_cfg = {0};
	
	if(W25QXX_Write_Enable() != QSPI_OK)		return QSPI_ERROR;

	/* 1.指令阶段 */
	s_command_cfg.Instruction 		= W25Q64JV_ERASE_CHIP;
	s_command_cfg.InstructionMode 	= QSPI_INSTRUCTION_1_LINE;

	/* 2.地址阶段 */
	s_command_cfg.Address 			= 0x00;
	s_command_cfg.AddressSize		= QSPI_ADDRESS_8_BITS;
	s_command_cfg.AddressMode		= QSPI_ADDRESS_NONE;

	/* 3.交替字节阶段 */
	s_command_cfg.AlternateBytes	= 0x00;
	s_command_cfg.AlternateBytesSize= QSPI_ALTERNATE_BYTES_8_BITS;
	s_command_cfg.AlternateByteMode	= QSPI_ALTERNATE_BYTES_NONE;
	
	/* 4.空周期阶段 */
	s_command_cfg.DummyCycles		= 0x00;
	
	/* 5.数据阶段 */
	s_command_cfg.DataMode			= QSPI_DATA_NONE;
	s_command_cfg.NbData 			= 0x00;
	
	/* 6.其他设置 */
	s_command_cfg.DdrMode			= QSPI_DDR_MODE_DISABLE;
	s_command_cfg.DdrHoldHalfCycle 	= QSPI_DDR_HHC_ANALOG_DELAY;
	s_command_cfg.SIOOMode			= QSPI_SIOO_INST_EVERY_CMD;

    if (HAL_QSPI_Command(&hqspi, &s_command_cfg, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        return QSPI_ERROR;
    }

    if (W25QXX_Auto_Polling_Busy(W25Q64JV_CHIP_ERASE_MAX_TIME_MS) != QSPI_OK)
    {
        return QSPI_ERROR;
    }

    return QSPI_OK;
}

int W25QXX_Erase_Program_Suspend(void)
{
	QSPI_CommandTypeDef s_command_cfg = {0};

	/* 1.指令阶段 */
	s_command_cfg.Instruction 		= W25Q64JV_ERASE_PROGRAM_SUSPEND;
	s_command_cfg.InstructionMode 	= QSPI_INSTRUCTION_1_LINE;

	/* 2.地址阶段 */
	s_command_cfg.Address 			= 0x00;
	s_command_cfg.AddressSize		= QSPI_ADDRESS_8_BITS;
	s_command_cfg.AddressMode		= QSPI_ADDRESS_NONE;

	/* 3.交替字节阶段 */
	s_command_cfg.AlternateBytes	= 0x00;
	s_command_cfg.AlternateBytesSize= QSPI_ALTERNATE_BYTES_8_BITS;
	s_command_cfg.AlternateByteMode	= QSPI_ALTERNATE_BYTES_NONE;
	
	/* 4.空周期阶段 */
	s_command_cfg.DummyCycles		= 0x00;
	
	/* 5.数据阶段 */
	s_command_cfg.DataMode			= QSPI_DATA_NONE;
	s_command_cfg.NbData 			= 0x00;
	
	/* 6.其他设置 */
	s_command_cfg.DdrMode			= QSPI_DDR_MODE_DISABLE;
	s_command_cfg.DdrHoldHalfCycle 	= QSPI_DDR_HHC_ANALOG_DELAY;
	s_command_cfg.SIOOMode			= QSPI_SIOO_INST_EVERY_CMD;

    if(HAL_QSPI_Command(&hqspi, &s_command_cfg, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        return QSPI_ERROR;
    }

    return QSPI_OK;
}

int W25QXX_Erase_Program_Resume(void)
{
	QSPI_CommandTypeDef s_command_cfg = {0};
	
	/* 1.指令阶段 */
	s_command_cfg.Instruction 		= W25Q64JV_ERASE_PROGRAM_RESUME;
	s_command_cfg.InstructionMode 	= QSPI_INSTRUCTION_1_LINE;

	/* 2.地址阶段 */
	s_command_cfg.Address 			= 0x00;
	s_command_cfg.AddressSize		= QSPI_ADDRESS_8_BITS;
	s_command_cfg.AddressMode		= QSPI_ADDRESS_NONE;

	/* 3.交替字节阶段 */
	s_command_cfg.AlternateBytes	= 0x00;
	s_command_cfg.AlternateBytesSize= QSPI_ALTERNATE_BYTES_8_BITS;
	s_command_cfg.AlternateByteMode	= QSPI_ALTERNATE_BYTES_NONE;
	
	/* 4.空周期阶段 */
	s_command_cfg.DummyCycles		= 0x00;
	
	/* 5.数据阶段 */
	s_command_cfg.DataMode			= QSPI_DATA_NONE;
	s_command_cfg.NbData 			= 0x00;
	
	/* 6.其他设置 */
	s_command_cfg.DdrMode			= QSPI_DDR_MODE_DISABLE;
	s_command_cfg.DdrHoldHalfCycle 	= QSPI_DDR_HHC_ANALOG_DELAY;
	s_command_cfg.SIOOMode			= QSPI_SIOO_INST_EVERY_CMD;

    if(HAL_QSPI_Command(&hqspi, &s_command_cfg, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        return QSPI_ERROR;
    }

    return QSPI_OK;
}

int W25QXX_Power_Down(void)
{
	QSPI_CommandTypeDef s_command_cfg = {0};
	
	/* 1.指令阶段 */
	s_command_cfg.Instruction 		= W25Q64JV_POWER_DOWN;
	s_command_cfg.InstructionMode 	= QSPI_INSTRUCTION_1_LINE;

	/* 2.地址阶段 */
	s_command_cfg.Address 			= 0x00;
	s_command_cfg.AddressSize		= QSPI_ADDRESS_8_BITS;
	s_command_cfg.AddressMode		= QSPI_ADDRESS_NONE;

	/* 3.交替字节阶段 */
	s_command_cfg.AlternateBytes	= 0x00;
	s_command_cfg.AlternateBytesSize= QSPI_ALTERNATE_BYTES_8_BITS;
	s_command_cfg.AlternateByteMode	= QSPI_ALTERNATE_BYTES_NONE;
	
	/* 4.空周期阶段 */
	s_command_cfg.DummyCycles		= 0x00;
	
	/* 5.数据阶段 */
	s_command_cfg.DataMode			= QSPI_DATA_NONE;
	s_command_cfg.NbData 			= 0x00;
	
	/* 6.其他设置 */
	s_command_cfg.DdrMode			= QSPI_DDR_MODE_DISABLE;
	s_command_cfg.DdrHoldHalfCycle 	= QSPI_DDR_HHC_ANALOG_DELAY;
	s_command_cfg.SIOOMode			= QSPI_SIOO_INST_EVERY_CMD;

    if(HAL_QSPI_Command(&hqspi, &s_command_cfg, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        return QSPI_ERROR;
    }

    return QSPI_OK;
}

int W25QXX_Release_Power_Down_Without_Device_ID(void)
{
	QSPI_CommandTypeDef s_command_cfg = {0};
	
	/* 1.指令阶段 */
	s_command_cfg.Instruction 		= W25Q64JV_RELEASE_POWER_DOWN;
	s_command_cfg.InstructionMode 	= QSPI_INSTRUCTION_1_LINE;

	/* 2.地址阶段 */
	s_command_cfg.Address 			= 0x00;
	s_command_cfg.AddressSize		= QSPI_ADDRESS_8_BITS;
	s_command_cfg.AddressMode		= QSPI_ADDRESS_NONE;

	/* 3.交替字节阶段 */
	s_command_cfg.AlternateBytes	= 0x00;
	s_command_cfg.AlternateBytesSize= QSPI_ALTERNATE_BYTES_8_BITS;
	s_command_cfg.AlternateByteMode	= QSPI_ALTERNATE_BYTES_NONE;
	
	/* 4.空周期阶段 */
	s_command_cfg.DummyCycles		= 0x00;
	
	/* 5.数据阶段 */
	s_command_cfg.DataMode			= QSPI_DATA_NONE;
	s_command_cfg.NbData 			= 0x00;
	
	/* 6.其他设置 */
	s_command_cfg.DdrMode			= QSPI_DDR_MODE_DISABLE;
	s_command_cfg.DdrHoldHalfCycle 	= QSPI_DDR_HHC_ANALOG_DELAY;
	s_command_cfg.SIOOMode			= QSPI_SIOO_INST_EVERY_CMD;

    if(HAL_QSPI_Command(&hqspi, &s_command_cfg, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        return QSPI_ERROR;
    }

	HAL_Delay(1);

    return QSPI_OK;
}

int W25QXX_Release_Power_Down_With_Device_ID(uint8_t *buf)
{
	QSPI_CommandTypeDef s_command_cfg = {0};
	
	/* 1.指令阶段 */
	s_command_cfg.Instruction 		= W25Q64JV_RELEASE_POWER_DOWN;
	s_command_cfg.InstructionMode 	= QSPI_INSTRUCTION_1_LINE;

	/* 2.地址阶段 */
	s_command_cfg.Address 			= 0x00;
	s_command_cfg.AddressSize		= QSPI_ADDRESS_8_BITS;
	s_command_cfg.AddressMode		= QSPI_ADDRESS_NONE;

	/* 3.交替字节阶段 */
	s_command_cfg.AlternateBytes	= 0x00;
	s_command_cfg.AlternateBytesSize= QSPI_ALTERNATE_BYTES_8_BITS;
	s_command_cfg.AlternateByteMode	= QSPI_ALTERNATE_BYTES_NONE;
	
	/* 4.空周期阶段 */
	s_command_cfg.DummyCycles		= 0x18;
	
	/* 5.数据阶段 */
	s_command_cfg.DataMode			= QSPI_DATA_1_LINE;
	s_command_cfg.NbData 			= 0x01;
	
	/* 6.其他设置 */
	s_command_cfg.DdrMode			= QSPI_DDR_MODE_DISABLE;
	s_command_cfg.DdrHoldHalfCycle 	= QSPI_DDR_HHC_ANALOG_DELAY;
	s_command_cfg.SIOOMode			= QSPI_SIOO_INST_EVERY_CMD;

    if(HAL_QSPI_Command(&hqspi, &s_command_cfg, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        return QSPI_ERROR;
    }

	if(HAL_QSPI_Receive(&hqspi, (uint8_t *)buf, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		return QSPI_ERROR;
	}

	HAL_Delay(1);

    return QSPI_OK;
}

int W25QXX_Read_Manufacturer_ID(uint8_t *buf)
{
	QSPI_CommandTypeDef s_command_cfg = {0};
	
	/* 1.指令阶段 */
	s_command_cfg.Instruction 		= W25Q64JV_GET_MANUFACTURER_ID;
	s_command_cfg.InstructionMode 	= QSPI_INSTRUCTION_1_LINE;
	
	/* 2.地址阶段 */
	s_command_cfg.Address 			= 0x00;
	s_command_cfg.AddressSize		= QSPI_ADDRESS_24_BITS;
	s_command_cfg.AddressMode		= QSPI_ADDRESS_1_LINE;

	/* 3.交替字节阶段 */
	s_command_cfg.AlternateBytes	= 0x00;
	s_command_cfg.AlternateBytesSize= QSPI_ALTERNATE_BYTES_8_BITS;
	s_command_cfg.AlternateByteMode	= QSPI_ALTERNATE_BYTES_NONE;
	
	/* 4.空周期阶段 */
	s_command_cfg.DummyCycles		= 0x00;
	
	/* 5.数据阶段 */
	s_command_cfg.DataMode			= QSPI_DATA_1_LINE;
	s_command_cfg.NbData 			= 0x02;
	
	/* 6.其他设置 */
	s_command_cfg.DdrMode			= QSPI_DDR_MODE_DISABLE;
	s_command_cfg.DdrHoldHalfCycle 	= QSPI_DDR_HHC_ANALOG_DELAY;
	s_command_cfg.SIOOMode			= QSPI_SIOO_INST_EVERY_CMD;

    if (HAL_QSPI_Command(&hqspi, &s_command_cfg, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        return QSPI_ERROR;
    }

    if (HAL_QSPI_Receive(&hqspi, (uint8_t *)buf, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        return QSPI_ERROR;
    }

    return QSPI_OK;
}

int W25QXX_Read_Manufacturer_ID_Dual(uint8_t *buf)
{
	QSPI_CommandTypeDef s_command_cfg = {0};
	
	/* 1.指令阶段 */
	s_command_cfg.Instruction 		= W25Q64JV_GET_MANUFACTURER_ID_DUAL;
	s_command_cfg.InstructionMode 	= QSPI_INSTRUCTION_1_LINE;
	
	/* 2.地址阶段 */
	s_command_cfg.Address 			= 0x00;
	s_command_cfg.AddressSize		= QSPI_ADDRESS_24_BITS;
	s_command_cfg.AddressMode		= QSPI_ADDRESS_2_LINES;

	/* 3.交替字节阶段 */
	s_command_cfg.AlternateBytes	= 0x00;
	s_command_cfg.AlternateBytesSize= QSPI_ALTERNATE_BYTES_8_BITS;
	s_command_cfg.AlternateByteMode	= QSPI_ALTERNATE_BYTES_2_LINES;
	
	/* 4.空周期阶段 */
	s_command_cfg.DummyCycles		= 0x00;
	
	/* 5.数据阶段 */
	s_command_cfg.DataMode			= QSPI_DATA_2_LINES;
	s_command_cfg.NbData 			= 0x02;
	
	/* 6.其他设置 */
	s_command_cfg.DdrMode			= QSPI_DDR_MODE_DISABLE;
	s_command_cfg.DdrHoldHalfCycle 	= QSPI_DDR_HHC_ANALOG_DELAY;
	s_command_cfg.SIOOMode			= QSPI_SIOO_INST_EVERY_CMD;

    if (HAL_QSPI_Command(&hqspi, &s_command_cfg, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        return QSPI_ERROR;
    }

    if (HAL_QSPI_Receive(&hqspi, (uint8_t *)buf, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        return QSPI_ERROR;
    }

    return QSPI_OK;
}

int W25QXX_Read_Manufacturer_ID_Quad(uint8_t *buf)
{
	QSPI_CommandTypeDef s_command_cfg = {0};
	
	/* 1.指令阶段 */
	s_command_cfg.Instruction 		= W25Q64JV_GET_MANUFACTURER_ID_QUAD;
	s_command_cfg.InstructionMode 	= QSPI_INSTRUCTION_1_LINE;
	
	/* 2.地址阶段 */
	s_command_cfg.Address 			= 0x00;
	s_command_cfg.AddressSize		= QSPI_ADDRESS_24_BITS;
	s_command_cfg.AddressMode		= QSPI_ADDRESS_4_LINES;

	/* 3.交替字节阶段 */
	s_command_cfg.AlternateBytes	= 0xFF;
	s_command_cfg.AlternateBytesSize= QSPI_ALTERNATE_BYTES_8_BITS;
	s_command_cfg.AlternateByteMode	= QSPI_ALTERNATE_BYTES_4_LINES;
	
	/* 4.空周期阶段 */
	s_command_cfg.DummyCycles		= 0x04;
	
	/* 5.数据阶段 */
	s_command_cfg.DataMode			= QSPI_DATA_4_LINES;
	s_command_cfg.NbData 			= 0x02;
	
	/* 6.其他设置 */
	s_command_cfg.DdrMode			= QSPI_DDR_MODE_DISABLE;
	s_command_cfg.DdrHoldHalfCycle 	= QSPI_DDR_HHC_ANALOG_DELAY;
	s_command_cfg.SIOOMode			= QSPI_SIOO_INST_EVERY_CMD;

    if (HAL_QSPI_Command(&hqspi, &s_command_cfg, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        return QSPI_ERROR;
    }

    if (HAL_QSPI_Receive(&hqspi, (uint8_t *)buf, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        return QSPI_ERROR;
    }

    return QSPI_OK;
}

int W25QXX_Read_Unique_ID(uint8_t *buf)
{
	QSPI_CommandTypeDef s_command_cfg = {0};
	
	/* 1.指令阶段 */
	s_command_cfg.Instruction 		= W25Q64JV_GET_UNIQUE_ID;
	s_command_cfg.InstructionMode 	= QSPI_INSTRUCTION_1_LINE;
	
	/* 2.地址阶段 */
	s_command_cfg.Address 			= 0x00;
	s_command_cfg.AddressSize		= QSPI_ADDRESS_8_BITS;
	s_command_cfg.AddressMode		= QSPI_ADDRESS_NONE;

	/* 3.交替字节阶段 */
	s_command_cfg.AlternateBytes	= 0x00;
	s_command_cfg.AlternateBytesSize= QSPI_ALTERNATE_BYTES_8_BITS;
	s_command_cfg.AlternateByteMode	= QSPI_ALTERNATE_BYTES_NONE;
	
	/* 4.空周期阶段 */
	s_command_cfg.DummyCycles		= 0x1F;			// 数据手册写的32个clock, 但stm32h7最多可以设置31个, 大家如果了解的话, 欢迎给我一个解释
	
	/* 5.数据阶段 */
	s_command_cfg.DataMode			= QSPI_DATA_1_LINE;
	s_command_cfg.NbData 			= 0x04;
	
	/* 6.其他设置 */
	s_command_cfg.DdrMode			= QSPI_DDR_MODE_DISABLE;
	s_command_cfg.DdrHoldHalfCycle 	= QSPI_DDR_HHC_ANALOG_DELAY;
	s_command_cfg.SIOOMode			= QSPI_SIOO_INST_EVERY_CMD;

    if (HAL_QSPI_Command(&hqspi, &s_command_cfg, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        return QSPI_ERROR;
    }

    if (HAL_QSPI_Receive(&hqspi, (uint8_t *)buf, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        return QSPI_ERROR;
    }

    return QSPI_OK;
}

int W25QXX_Read_Jedec_ID(uint8_t *buf)
{
	QSPI_CommandTypeDef s_command_cfg = {0};
	
	/* 1.指令阶段 */
	s_command_cfg.Instruction 		= W25Q64JV_GET_JEDEC_ID;
	s_command_cfg.InstructionMode 	= QSPI_INSTRUCTION_1_LINE;
	
	/* 2.地址阶段 */
	s_command_cfg.Address 			= 0x00;
	s_command_cfg.AddressSize		= QSPI_ADDRESS_8_BITS;
	s_command_cfg.AddressMode		= QSPI_ADDRESS_NONE;

	/* 3.交替字节阶段 */
	s_command_cfg.AlternateBytes	= 0x00;
	s_command_cfg.AlternateBytesSize= QSPI_ALTERNATE_BYTES_8_BITS;
	s_command_cfg.AlternateByteMode	= QSPI_ALTERNATE_BYTES_NONE;
	
	/* 4.空周期阶段 */
	s_command_cfg.DummyCycles		= 0x00;
	
	/* 5.数据阶段 */
	s_command_cfg.DataMode			= QSPI_DATA_1_LINE;
	s_command_cfg.NbData 			= 0x03;
	
	/* 6.其他设置 */
	s_command_cfg.DdrMode			= QSPI_DDR_MODE_DISABLE;
	s_command_cfg.DdrHoldHalfCycle 	= QSPI_DDR_HHC_ANALOG_DELAY;
	s_command_cfg.SIOOMode			= QSPI_SIOO_INST_EVERY_CMD;

    if (HAL_QSPI_Command(&hqspi, &s_command_cfg, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        return QSPI_ERROR;
    }

    if (HAL_QSPI_Receive(&hqspi, (uint8_t *)buf, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        return QSPI_ERROR;
    }

    return QSPI_OK;
}

int W25QXX_Read_SFDP_Reg(uint8_t *buf, uint8_t sfdp_addr, uint16_t size)
{
	QSPI_CommandTypeDef s_command_cfg = {0};
	
	/* 1.指令阶段 */
	s_command_cfg.Instruction 		= W25Q64JV_GET_SFDP_REG;
	s_command_cfg.InstructionMode 	= QSPI_INSTRUCTION_1_LINE;
	
	/* 2.地址阶段 */
	s_command_cfg.Address 			= sfdp_addr;
	s_command_cfg.AddressSize		= QSPI_ADDRESS_24_BITS;
	s_command_cfg.AddressMode		= QSPI_ADDRESS_1_LINE;

	/* 3.交替字节阶段 */
	s_command_cfg.AlternateBytes	= 0x00;
	s_command_cfg.AlternateBytesSize= QSPI_ALTERNATE_BYTES_8_BITS;
	s_command_cfg.AlternateByteMode	= QSPI_ALTERNATE_BYTES_NONE;
	
	/* 4.空周期阶段 */
	s_command_cfg.DummyCycles		= 0x08;
	
	/* 5.数据阶段 */
	s_command_cfg.DataMode			= QSPI_DATA_1_LINE;
	s_command_cfg.NbData 			= size;
	
	/* 6.其他设置 */
	s_command_cfg.DdrMode			= QSPI_DDR_MODE_DISABLE;
	s_command_cfg.DdrHoldHalfCycle 	= QSPI_DDR_HHC_ANALOG_DELAY;
	s_command_cfg.SIOOMode			= QSPI_SIOO_INST_EVERY_CMD;

    if (HAL_QSPI_Command(&hqspi, &s_command_cfg, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        return QSPI_ERROR;
    }

    if (HAL_QSPI_Receive(&hqspi, (uint8_t *)buf, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        return QSPI_ERROR;
    }

    return QSPI_OK;
}

int W25QXX_Erase_Security_Reg(uint32_t addr)
{
	QSPI_CommandTypeDef s_command_cfg = {0};

	if(W25QXX_Write_Enable() != QSPI_OK)		return QSPI_ERROR;
	
	/* 1.指令阶段 */
	s_command_cfg.Instruction 		= W25Q64JV_ERASE_SECURITY_REG;
	s_command_cfg.InstructionMode 	= QSPI_INSTRUCTION_1_LINE;
	
	/* 2.地址阶段 */
	s_command_cfg.Address 			= addr;
	s_command_cfg.AddressSize		= QSPI_ADDRESS_24_BITS;
	s_command_cfg.AddressMode		= QSPI_ADDRESS_1_LINE;

	/* 3.交替字节阶段 */
	s_command_cfg.AlternateBytes	= 0x00;
	s_command_cfg.AlternateBytesSize= QSPI_ALTERNATE_BYTES_8_BITS;
	s_command_cfg.AlternateByteMode	= QSPI_ALTERNATE_BYTES_NONE;
	
	/* 4.空周期阶段 */
	s_command_cfg.DummyCycles		= 0x00;
	
	/* 5.数据阶段 */
	s_command_cfg.DataMode			= QSPI_DATA_NONE;
	s_command_cfg.NbData 			= 0x00;
	
	/* 6.其他设置 */
	s_command_cfg.DdrMode			= QSPI_DDR_MODE_DISABLE;
	s_command_cfg.DdrHoldHalfCycle 	= QSPI_DDR_HHC_ANALOG_DELAY;
	s_command_cfg.SIOOMode			= QSPI_SIOO_INST_EVERY_CMD;

    if (HAL_QSPI_Command(&hqspi, &s_command_cfg, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        return QSPI_ERROR;
    }

	if(W25QXX_Auto_Polling_Busy(HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != QSPI_OK)
	{
		return QSPI_ERROR;
	}

    return QSPI_OK;
}

int W25QXX_Program_Security_Reg(uint32_t addr, uint8_t *buf, uint16_t size)
{
	QSPI_CommandTypeDef s_command_cfg = {0};

	if(size == 0)		return QSPI_OK;
	if(W25QXX_Write_Enable() != QSPI_OK)		return QSPI_ERROR;
	
	/* 1.指令阶段 */
	s_command_cfg.Instruction 		= W25Q64JV_PROGRAM_SECURITY_REG;
	s_command_cfg.InstructionMode 	= QSPI_INSTRUCTION_1_LINE;
	
	/* 2.地址阶段 */
	s_command_cfg.Address 			= addr;
	s_command_cfg.AddressSize		= QSPI_ADDRESS_24_BITS;
	s_command_cfg.AddressMode		= QSPI_ADDRESS_1_LINE;

	/* 3.交替字节阶段 */
	s_command_cfg.AlternateBytes	= 0x00;
	s_command_cfg.AlternateBytesSize= QSPI_ALTERNATE_BYTES_8_BITS;
	s_command_cfg.AlternateByteMode	= QSPI_ALTERNATE_BYTES_NONE;
	
	/* 4.空周期阶段 */
	s_command_cfg.DummyCycles		= 0x00;
	
	/* 5.数据阶段 */
	s_command_cfg.DataMode			= QSPI_DATA_1_LINE;
	s_command_cfg.NbData 			= size;
	
	/* 6.其他设置 */
	s_command_cfg.DdrMode			= QSPI_DDR_MODE_DISABLE;
	s_command_cfg.DdrHoldHalfCycle 	= QSPI_DDR_HHC_ANALOG_DELAY;
	s_command_cfg.SIOOMode			= QSPI_SIOO_INST_EVERY_CMD;

    if (HAL_QSPI_Command(&hqspi, &s_command_cfg, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        return QSPI_ERROR;
    }

	if(HAL_QSPI_Transmit(&hqspi, (uint8_t *)buf, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
        return QSPI_ERROR;
	}

	if(W25QXX_Auto_Polling_Busy(HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != QSPI_OK)
	{
		return QSPI_ERROR;
	}

    return QSPI_OK;
}

int W25QXX_Read_Security_Reg(uint32_t addr, uint8_t *buf, uint16_t size)
{
	QSPI_CommandTypeDef s_command_cfg = {0};

	/* 1.指令阶段 */
	s_command_cfg.Instruction 		= W25Q64JV_READ_SECURITY_REG;
	s_command_cfg.InstructionMode 	= QSPI_INSTRUCTION_1_LINE;
	
	/* 2.地址阶段 */
	s_command_cfg.Address 			= addr;
	s_command_cfg.AddressSize		= QSPI_ADDRESS_24_BITS;
	s_command_cfg.AddressMode		= QSPI_ADDRESS_1_LINE;

	/* 3.交替字节阶段 */
	s_command_cfg.AlternateBytes	= 0x00;
	s_command_cfg.AlternateBytesSize= QSPI_ALTERNATE_BYTES_8_BITS;
	s_command_cfg.AlternateByteMode	= QSPI_ALTERNATE_BYTES_NONE;
	
	/* 4.空周期阶段 */
	s_command_cfg.DummyCycles		= 0x08;
	
	/* 5.数据阶段 */
	s_command_cfg.DataMode			= QSPI_DATA_1_LINE;
	s_command_cfg.NbData 			= size;
	
	/* 6.其他设置 */
	s_command_cfg.DdrMode			= QSPI_DDR_MODE_DISABLE;
	s_command_cfg.DdrHoldHalfCycle 	= QSPI_DDR_HHC_ANALOG_DELAY;
	s_command_cfg.SIOOMode			= QSPI_SIOO_INST_EVERY_CMD;

    if (HAL_QSPI_Command(&hqspi, &s_command_cfg, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        return QSPI_ERROR;
    }

	if(HAL_QSPI_Receive(&hqspi, (uint8_t *)buf, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		return QSPI_ERROR;
	}

    return QSPI_OK;
}

int W25QXX_Individual_Block_Sector_Lock(uint32_t addr)
{
	QSPI_CommandTypeDef s_command_cfg = {0};
	uint32_t lock_addr = 0x00;

	if((addr >= W25Q64JV_BLOCK1_START_ADDR) 
		&& (addr < W25Q64JV_BLOCK127_START_ADDR))
		lock_addr = ALIGN_DOWN(addr, W25Q64JV_64KB_BLOCK_SIZE);
	else if(addr < W25Q64JV_BLOCK1_START_ADDR)
		lock_addr = ALIGN_DOWN(addr, W25Q64JV_SECTOR_SIZE);
	else if((addr >= W25Q64JV_BLOCK127_START_ADDR) 
		&& (addr < W25Q64JV_BLOCK127_START_ADDR + W25Q64JV_64KB_BLOCK_SIZE))
		lock_addr = ALIGN_DOWN(addr, W25Q64JV_SECTOR_SIZE);
	else	return QSPI_ERROR;

	if(W25QXX_Write_Enable() != QSPI_OK)		return QSPI_ERROR;
	
	/* 1.指令阶段 */
	s_command_cfg.Instruction 		= W25Q64JV_INDIVIDUAL_BLOCK_SECTOR_LOCK;
	s_command_cfg.InstructionMode 	= QSPI_INSTRUCTION_1_LINE;
	
	/* 2.地址阶段 */
	s_command_cfg.Address 			= lock_addr;
	s_command_cfg.AddressSize		= QSPI_ADDRESS_24_BITS;
	s_command_cfg.AddressMode		= QSPI_ADDRESS_1_LINE;

	/* 3.交替字节阶段 */
	s_command_cfg.AlternateBytes	= 0x00;
	s_command_cfg.AlternateBytesSize= QSPI_ALTERNATE_BYTES_8_BITS;
	s_command_cfg.AlternateByteMode	= QSPI_ALTERNATE_BYTES_NONE;
	
	/* 4.空周期阶段 */
	s_command_cfg.DummyCycles		= 0x00;
	
	/* 5.数据阶段 */
	s_command_cfg.DataMode			= QSPI_DATA_NONE;
	s_command_cfg.NbData 			= 0;
	
	/* 6.其他设置 */
	s_command_cfg.DdrMode			= QSPI_DDR_MODE_DISABLE;
	s_command_cfg.DdrHoldHalfCycle 	= QSPI_DDR_HHC_ANALOG_DELAY;
	s_command_cfg.SIOOMode			= QSPI_SIOO_INST_EVERY_CMD;

    if (HAL_QSPI_Command(&hqspi, &s_command_cfg, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        return QSPI_ERROR;
    }

    return QSPI_OK;
}

int W25QXX_Individual_Block_Sector_UnLock(uint32_t addr)
{
	QSPI_CommandTypeDef s_command_cfg = {0};
	uint32_t unlock_addr = 0x00;

	if((addr >= W25Q64JV_BLOCK1_START_ADDR) 
		&& (addr < W25Q64JV_BLOCK127_START_ADDR))
		unlock_addr = ALIGN_DOWN(addr, W25Q64JV_64KB_BLOCK_SIZE);
	else if(addr < W25Q64JV_BLOCK1_START_ADDR)
		unlock_addr = ALIGN_DOWN(addr, W25Q64JV_SECTOR_SIZE);
	else if((addr >= W25Q64JV_BLOCK127_START_ADDR) 
		&& (addr < W25Q64JV_BLOCK127_START_ADDR + W25Q64JV_64KB_BLOCK_SIZE))
		unlock_addr = ALIGN_DOWN(addr, W25Q64JV_SECTOR_SIZE);
	else	return QSPI_ERROR;

	if(W25QXX_Write_Enable() != QSPI_OK)		return QSPI_ERROR;
	
	/* 1.指令阶段 */
	s_command_cfg.Instruction 		= W25Q64JV_INDIVIDUAL_BLOCK_SECTOR_UNLOCK;
	s_command_cfg.InstructionMode 	= QSPI_INSTRUCTION_1_LINE;
	
	/* 2.地址阶段 */
	s_command_cfg.Address 			= unlock_addr;
	s_command_cfg.AddressSize		= QSPI_ADDRESS_24_BITS;
	s_command_cfg.AddressMode		= QSPI_ADDRESS_1_LINE;

	/* 3.交替字节阶段 */
	s_command_cfg.AlternateBytes	= 0x00;
	s_command_cfg.AlternateBytesSize= QSPI_ALTERNATE_BYTES_8_BITS;
	s_command_cfg.AlternateByteMode	= QSPI_ALTERNATE_BYTES_NONE;
	
	/* 4.空周期阶段 */
	s_command_cfg.DummyCycles		= 0x00;
	
	/* 5.数据阶段 */
	s_command_cfg.DataMode			= QSPI_DATA_NONE;
	s_command_cfg.NbData 			= 0;
	
	/* 6.其他设置 */
	s_command_cfg.DdrMode			= QSPI_DDR_MODE_DISABLE;
	s_command_cfg.DdrHoldHalfCycle 	= QSPI_DDR_HHC_ANALOG_DELAY;
	s_command_cfg.SIOOMode			= QSPI_SIOO_INST_EVERY_CMD;

    if (HAL_QSPI_Command(&hqspi, &s_command_cfg, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        return QSPI_ERROR;
    }

    return QSPI_OK;
}

int W25QXX_Read_Block_Sector_Lock(uint32_t addr, uint8_t *buf)
{
	QSPI_CommandTypeDef s_command_cfg = {0};
	uint32_t lock_addr 	= 0x00;
	uint8_t lock_status	= 0x00;

	if((addr >= W25Q64JV_BLOCK1_START_ADDR) 
		&& (addr < W25Q64JV_BLOCK127_START_ADDR))
		lock_addr = ALIGN_DOWN(addr, W25Q64JV_64KB_BLOCK_SIZE);
	else if(addr < W25Q64JV_BLOCK1_START_ADDR)
		lock_addr = ALIGN_DOWN(addr, W25Q64JV_SECTOR_SIZE);
	else if((addr >= W25Q64JV_BLOCK127_START_ADDR) 
		&& (addr < W25Q64JV_BLOCK127_START_ADDR + W25Q64JV_64KB_BLOCK_SIZE))
		lock_addr = ALIGN_DOWN(addr, W25Q64JV_SECTOR_SIZE);
	else	return QSPI_ERROR;
	
	/* 1.指令阶段 */
	s_command_cfg.Instruction 		= W25Q64JV_READ_BLOCK_SECTOR_LOCK;
	s_command_cfg.InstructionMode 	= QSPI_INSTRUCTION_1_LINE;
	
	/* 2.地址阶段 */
	s_command_cfg.Address 			= lock_addr;
	s_command_cfg.AddressSize		= QSPI_ADDRESS_24_BITS;
	s_command_cfg.AddressMode		= QSPI_ADDRESS_1_LINE;

	/* 3.交替字节阶段 */
	s_command_cfg.AlternateBytes	= 0x00;
	s_command_cfg.AlternateBytesSize= QSPI_ALTERNATE_BYTES_8_BITS;
	s_command_cfg.AlternateByteMode	= QSPI_ALTERNATE_BYTES_NONE;
	
	/* 4.空周期阶段 */
	s_command_cfg.DummyCycles		= 0x00;
	
	/* 5.数据阶段 */
	s_command_cfg.DataMode			= QSPI_DATA_1_LINE;
	s_command_cfg.NbData 			= 0x01;
	
	/* 6.其他设置 */
	s_command_cfg.DdrMode			= QSPI_DDR_MODE_DISABLE;
	s_command_cfg.DdrHoldHalfCycle 	= QSPI_DDR_HHC_ANALOG_DELAY;
	s_command_cfg.SIOOMode			= QSPI_SIOO_INST_EVERY_CMD;

    if (HAL_QSPI_Command(&hqspi, &s_command_cfg, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        return QSPI_ERROR;
    }

	if(HAL_QSPI_Receive(&hqspi, (uint8_t *)&lock_status, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		return QSPI_ERROR;
	}

	if(lock_status & 0x01)		*buf = 0x01;
	else						*buf = 0x00;

    return QSPI_OK;
}

int W25QXX_Global_Block_Sector_Lock(void)
{
	QSPI_CommandTypeDef s_command_cfg = {0};

	if(W25QXX_Write_Enable() != QSPI_OK)		return QSPI_ERROR;
	
	/* 1.指令阶段 */
	s_command_cfg.Instruction 		= W25Q64JV_GLOBAL_BLOCK_SECTOR_LOCK;
	s_command_cfg.InstructionMode 	= QSPI_INSTRUCTION_1_LINE;
	
	/* 2.地址阶段 */
	s_command_cfg.Address 			= 0x00;
	s_command_cfg.AddressSize		= QSPI_ADDRESS_8_BITS;
	s_command_cfg.AddressMode		= QSPI_ADDRESS_NONE;

	/* 3.交替字节阶段 */
	s_command_cfg.AlternateBytes	= 0x00;
	s_command_cfg.AlternateBytesSize= QSPI_ALTERNATE_BYTES_8_BITS;
	s_command_cfg.AlternateByteMode	= QSPI_ALTERNATE_BYTES_NONE;
	
	/* 4.空周期阶段 */
	s_command_cfg.DummyCycles		= 0x00;
	
	/* 5.数据阶段 */
	s_command_cfg.DataMode			= QSPI_DATA_NONE;
	s_command_cfg.NbData 			= 0x00;
	
	/* 6.其他设置 */
	s_command_cfg.DdrMode			= QSPI_DDR_MODE_DISABLE;
	s_command_cfg.DdrHoldHalfCycle 	= QSPI_DDR_HHC_ANALOG_DELAY;
	s_command_cfg.SIOOMode			= QSPI_SIOO_INST_EVERY_CMD;

    if (HAL_QSPI_Command(&hqspi, &s_command_cfg, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        return QSPI_ERROR;
    }

	return QSPI_OK;
}

int W25QXX_Global_Block_Sector_Unlock(void)
{
	QSPI_CommandTypeDef s_command_cfg = {0};

	if(W25QXX_Write_Enable() != QSPI_OK)		return QSPI_ERROR;
	
	/* 1.指令阶段 */
	s_command_cfg.Instruction 		= W25Q64JV_GLOBAL_BLOCK_SECTOR_UNLOCK;
	s_command_cfg.InstructionMode 	= QSPI_INSTRUCTION_1_LINE;
	
	/* 2.地址阶段 */
	s_command_cfg.Address 			= 0x00;
	s_command_cfg.AddressSize		= QSPI_ADDRESS_8_BITS;
	s_command_cfg.AddressMode		= QSPI_ADDRESS_NONE;

	/* 3.交替字节阶段 */
	s_command_cfg.AlternateBytes	= 0x00;
	s_command_cfg.AlternateBytesSize= QSPI_ALTERNATE_BYTES_8_BITS;
	s_command_cfg.AlternateByteMode	= QSPI_ALTERNATE_BYTES_NONE;
	
	/* 4.空周期阶段 */
	s_command_cfg.DummyCycles		= 0x00;
	
	/* 5.数据阶段 */
	s_command_cfg.DataMode			= QSPI_DATA_NONE;
	s_command_cfg.NbData 			= 0x00;
	
	/* 6.其他设置 */
	s_command_cfg.DdrMode			= QSPI_DDR_MODE_DISABLE;
	s_command_cfg.DdrHoldHalfCycle 	= QSPI_DDR_HHC_ANALOG_DELAY;
	s_command_cfg.SIOOMode			= QSPI_SIOO_INST_EVERY_CMD;

    if (HAL_QSPI_Command(&hqspi, &s_command_cfg, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        return QSPI_ERROR;
    }

	return QSPI_OK;
}

int W25QXX_Reset(void)
{
	QSPI_CommandTypeDef s_command_cfg = {0};
	
	if(W25QXX_Auto_Polling_Busy(HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != QSPI_OK)
	{
		return QSPI_ERROR;
	}

	/* 1.指令阶段 */
	s_command_cfg.Instruction 		= W25Q64JV_ENABLE_RESET;
	s_command_cfg.InstructionMode 	= QSPI_INSTRUCTION_1_LINE;
	
	/* 2.地址阶段 */
	s_command_cfg.Address 			= 0x00;
	s_command_cfg.AddressSize		= QSPI_ADDRESS_8_BITS;
	s_command_cfg.AddressMode		= QSPI_ADDRESS_NONE;

	/* 3.交替字节阶段 */
	s_command_cfg.AlternateBytes	= 0x00;
	s_command_cfg.AlternateBytesSize= QSPI_ALTERNATE_BYTES_8_BITS;
	s_command_cfg.AlternateByteMode	= QSPI_ALTERNATE_BYTES_NONE;
	
	/* 4.空周期阶段 */
	s_command_cfg.DummyCycles		= 0x00;
	
	/* 5.数据阶段 */
	s_command_cfg.DataMode			= QSPI_DATA_NONE;
	s_command_cfg.NbData 			= 0x00;
	
	/* 6.其他设置 */
	s_command_cfg.DdrMode			= QSPI_DDR_MODE_DISABLE;
	s_command_cfg.DdrHoldHalfCycle 	= QSPI_DDR_HHC_ANALOG_DELAY;
	s_command_cfg.SIOOMode			= QSPI_SIOO_INST_EVERY_CMD;

    if (HAL_QSPI_Command(&hqspi, &s_command_cfg, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        return QSPI_ERROR;
    }
	
	if(W25QXX_Auto_Polling_Busy(HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != QSPI_OK)
	{
		return QSPI_ERROR;
	}

	s_command_cfg.Instruction 		= W25Q64JV_RESET_DEVICE;
	if (HAL_QSPI_Command(&hqspi, &s_command_cfg, HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        return QSPI_ERROR;
    }
	
	if(W25QXX_Auto_Polling_Busy(HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != QSPI_OK)
	{
		return QSPI_ERROR;
	}


	return QSPI_OK;
}

int W25QXX_Enable_Memory_Mapped(void)
{
	QSPI_CommandTypeDef s_command_cfg = {0};
	QSPI_MemoryMappedTypeDef s_mem_mapped_cfg = {0};

	if(W25QXX_Auto_Polling_Busy(HAL_QSPI_TIMEOUT_DEFAULT_VALUE) != QSPI_OK)
	{
		return QSPI_ERROR;
	}

	if(W25QXX_Quad_Enable() != QSPI_OK)	return QSPI_ERROR;

	/* 1.指令阶段 */
	s_command_cfg.Instruction 		= W25Q64JV_FAST_READ_QUAD_IO;
	s_command_cfg.InstructionMode 	= QSPI_INSTRUCTION_1_LINE;

	/* 2.地址阶段 */
	s_command_cfg.Address 			= 0x00;
	s_command_cfg.AddressSize		= QSPI_ADDRESS_24_BITS;
	s_command_cfg.AddressMode		= QSPI_ADDRESS_4_LINES;

	/* 3.交替字节阶段 */
	s_command_cfg.AlternateBytes	= 0xFF;
	s_command_cfg.AlternateBytesSize= QSPI_ALTERNATE_BYTES_8_BITS;
	s_command_cfg.AlternateByteMode	= QSPI_ALTERNATE_BYTES_4_LINES;
	
	/* 4.空周期阶段 */
	s_command_cfg.DummyCycles		= 0x04;
	
	/* 5.数据阶段 */
	s_command_cfg.DataMode			= QSPI_DATA_4_LINES;
	s_command_cfg.NbData 			= 0x00;
	
	/* 6.其他设置 */
	s_command_cfg.DdrMode			= QSPI_DDR_MODE_DISABLE;
	s_command_cfg.DdrHoldHalfCycle 	= QSPI_DDR_HHC_ANALOG_DELAY;
	s_command_cfg.SIOOMode			= QSPI_SIOO_INST_EVERY_CMD;

    s_mem_mapped_cfg.TimeOutActivation 	= QSPI_TIMEOUT_COUNTER_DISABLE;
	s_mem_mapped_cfg.TimeOutPeriod		= 0;

    if (HAL_QSPI_MemoryMapped(&hqspi, &s_command_cfg, &s_mem_mapped_cfg) != HAL_OK)
    {
        return QSPI_ERROR;
    }

    return QSPI_OK;
}

int W25QXX_Disable_Memory_Mapped(void)
{
	if(HAL_QSPI_Abort(&hqspi) != HAL_OK)	return QSPI_ERROR;
	
	return QSPI_OK;
}

int W25QXX_Write(uint32_t addr, uint8_t *buf, uint32_t size)
{
    uint32_t NumOfPage = 0, SizeOfFirst = 0, SizeOfEnd = 0, AddrOfPage = 0;
    uint32_t i;

    AddrOfPage = addr % W25Q64JV_PAGE_SIZE;

    /* 1. Address is W25Q64JV_PAGE_SIZE aligned */
    if(AddrOfPage == 0)
    {
        NumOfPage = size / W25Q64JV_PAGE_SIZE;
        SizeOfEnd = size % W25Q64JV_PAGE_SIZE;
        /* 1.1 Size is W25Q64JV_PAGE_SIZE aligned */
        if(SizeOfEnd == 0)
        {
            /* 1.1.1 Program */
            for(i = 0; i < NumOfPage; i++)
            {
                if(W25QXX_Quad_Input_Page_Program(addr, buf, W25Q64JV_PAGE_SIZE) != QSPI_OK)
                {
                    return QSPI_ERROR;
                }
                addr += W25Q64JV_PAGE_SIZE;
                buf += W25Q64JV_PAGE_SIZE;
            }
        }
        /* 1.2 Size is not W25Q64JV_PAGE_SIZE aligned */
        else    
        {
            /* 1.2.1 Program except the last page */
            for(i = 0; i < NumOfPage; i++)
            {
                if(W25QXX_Quad_Input_Page_Program(addr, buf, W25Q64JV_PAGE_SIZE) != QSPI_OK)
                {
                    return QSPI_ERROR;
                }
                addr += W25Q64JV_PAGE_SIZE;
                buf += W25Q64JV_PAGE_SIZE;
            }
            /* 1.2.2 Program the last page */
            if(W25QXX_Quad_Input_Page_Program(addr, buf, SizeOfEnd) != QSPI_OK)
            {
                return QSPI_ERROR;
            }
        }
    }
    /* 2. Address is not W25Q64JV_PAGE_SIZE aligned */
    else
    {
        /* 2.1 Program the first page */
        SizeOfFirst = W25Q64JV_PAGE_SIZE - AddrOfPage;
        if(W25QXX_Quad_Input_Page_Program(addr, buf, SizeOfFirst) != QSPI_OK)
        {
            return QSPI_ERROR;
        }
        addr += SizeOfFirst;
        buf += SizeOfFirst;

        /* 2.1 Program the remaining page */
        NumOfPage = (size - SizeOfFirst) / W25Q64JV_PAGE_SIZE;
        SizeOfEnd = (size - SizeOfFirst) % W25Q64JV_PAGE_SIZE;
        /* 2.1.1 The remaining size is W25Q64JV_PAGE_SIZE aligned */
        if(SizeOfEnd == 0)
        {
            for(i = 0; i < NumOfPage; i++)
            {
                if(W25QXX_Quad_Input_Page_Program(addr, buf, W25Q64JV_PAGE_SIZE) != QSPI_OK)
                {
                    return QSPI_ERROR;
                }
                addr += W25Q64JV_PAGE_SIZE;
                buf += W25Q64JV_PAGE_SIZE;
            }
        }
        /* 2.1.1 The remaining size is not W25Q64JV_PAGE_SIZE aligned */
        else    
        {
            /* 2.1.1.1 Program except the last page */
            for(i = 0; i < NumOfPage; i++)
            {
                if(W25QXX_Quad_Input_Page_Program(addr, buf, W25Q64JV_PAGE_SIZE) != QSPI_OK)
                {
                    return QSPI_ERROR;
                }
                addr += W25Q64JV_PAGE_SIZE;
                buf += W25Q64JV_PAGE_SIZE;
            }
            /* 2.1.1.2 Program the last page */
            if(W25QXX_Quad_Input_Page_Program(addr, buf, SizeOfEnd) != QSPI_OK)
            {
                return QSPI_ERROR;
            }
        }
    }
	
	return QSPI_OK; 
}