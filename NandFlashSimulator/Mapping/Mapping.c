/*********************************************************
*     File Name : Mapping.h
*     YIN  @ 2016.12.15
*     Description  : Initial create
**********************************************************/
#include <string.h>
#include <stdio.h>
#include "Mapping.h"
#include "NandFlash_sim.h"

/* 逻辑块号映射表。好块总数的2%用于备份区，因此数组维数低于1024。 LUT = Look Up Table */
static uint16_t m_LUT[NAND_BLOCK_COUNT];

static uint16_t m_ValidDataBlockCount;	/* 有效的数据块个数 */

static uint8_t  gDataPageBuffer[MAX_BYTES_PER_PAGE];

uint32_t Nand_Mapping_ReadID(uint32_t PageNo, uint8_t* pID)
{
    uint32_t ret;
    uint32_t mChipNo;
    uint32_t mChipAddr;

    mChipNo = PageNo / PAGES_PER_CHIP;
    mChipAddr = ((mChipNo / 8)<<24) + ((mChipNo % 8)<<8);

    ret = NandReadID(mChipAddr, pID);

    return 0;
}

uint32_t Nand_Mapping_ReadData(uint8_t* pDataBuffer, uint32_t PageNo, uint16_t AddrInPage, uint16_t ByteCount)
{
    uint16_t i;
    uint32_t ret;
    uint32_t mChipNo;
    uint32_t mChipAddr;
    uint32_t mPageAddr;

    memset(gDataPageBuffer, 0x00, MAX_BYTES_PER_PAGE);

    /*逻辑页号 转换为 实际存储地址*/
    mChipNo = PageNo / PAGES_PER_CHIP;
    mChipAddr = ((mChipNo / 8)<<24) + ((mChipNo % 8)<<8);

    mPageAddr = PageNo - (mChipNo * PAGES_PER_CHIP);
    mPageAddr += AddrInPage;

    ret = NandSimRead(mChipAddr, mPageAddr, gDataPageBuffer, 0);

    for(i = 0; i < ByteCount; i++)
	{
		pDataBuffer[i] = gDataPageBuffer[i];
	}

    return ret;
}

uint32_t Nand_Mapping_ReadSpare(uint8_t* pDataBuffer, uint32_t PageNo, uint16_t AddrInPage, uint16_t ByteCount)
{
    uint16_t i;
    uint32_t ret;
    uint32_t mChipNo;
    uint32_t mChipAddr;
    uint32_t mPageAddr;

    memset(gDataPageBuffer, 0x00, MAX_BYTES_PER_PAGE);

    mChipNo = PageNo / PAGES_PER_CHIP;
    mChipAddr = ((mChipNo / 8)<<24) + ((mChipNo % 8)<<8);

    mPageAddr = PageNo - (mChipNo * PAGES_PER_CHIP);
    mPageAddr += AddrInPage;

    ret = NandSimRead(mChipAddr, mPageAddr, pDataBuffer, 1);

    for(i = 0; i < ByteCount; i++)
	{
		pDataBuffer[i] = gDataPageBuffer[i];
	}

   return ret;
}

uint32_t Nand_Mapping_WriteData(uint8_t* pDataBuffer, uint32_t PageNo, uint16_t AddrInPage, uint16_t ByteCount)
{
    uint16_t i;
    uint32_t ret;
    uint32_t mChipNo;
    uint32_t mChipAddr;
    uint32_t mPageAddr;

    memset(gDataPageBuffer, 0x00, MAX_BYTES_PER_PAGE);

    mChipNo = PageNo / PAGES_PER_CHIP;
    mChipAddr = ((mChipNo / 8)<<24) + ((mChipNo % 8)<<8);

    mPageAddr = PageNo - (mChipNo * PAGES_PER_CHIP);
    mPageAddr += AddrInPage;

    ret = NandSimWrite(mChipAddr, mPageAddr, pDataBuffer, 0);

    for(i = 0; i < ByteCount; i++)
	{
        gDataPageBuffer[i] = pDataBuffer[i];
	}

    return ret;
}

uint32_t Nand_Mapping_WriteSpare(uint8_t* pDataBuffer, uint32_t PageNo, uint16_t AddrInPage, uint16_t ByteCount)
{
    uint16_t i;
    uint32_t ret;
    uint32_t mChipNo;
    uint32_t mChipAddr;
    uint32_t mPageAddr;

    memset(gDataPageBuffer, 0x00, MAX_BYTES_PER_PAGE);

    mChipNo = PageNo / PAGES_PER_CHIP;
    mChipAddr = ((mChipNo / 8)<<24) + ((mChipNo % 8)<<8);

    mPageAddr = PageNo - (mChipNo * PAGES_PER_CHIP);
    mPageAddr += AddrInPage;

    ret = NandSimWrite(mChipAddr, mPageAddr, pDataBuffer, 1);

    for(i = 0; i < ByteCount; i++)
	{
        gDataPageBuffer[i] = pDataBuffer[i];
	}

    return ret;
}

uint32_t Nand_Mapping_PageCopy(uint8_t* pDataBuffer, uint32_t SrcPageNo, uint32_t TarPageNo, uint16_t AddrInPage, uint16_t ByteCount)
{
    uint16_t m16i;
    uint32_t ret;
    uint32_t mrChipNo, mrChipAddr, mrPageAddr;
    uint32_t mwChipNo, mwChipAddr, mwPageAddr;

    memset(gDataPageBuffer, 0x00, MAX_BYTES_PER_PAGE);

    /*ÂßŒ­Ò³ºÅ ×ª»»Îª ÊµŒÊŽæŽ¢µØÖ·*/
    mrChipNo = SrcPageNo / PAGES_PER_CHIP;
    mrChipAddr = ((mrChipNo / 8)<<24) + ((mrChipNo % 8)<<8);
    mrPageAddr = SrcPageNo - (mrChipNo * PAGES_PER_CHIP);
    mrPageAddr += AddrInPage;/*Ò³ÄÚÆ«ÒÆ*/

    mwChipNo = TarPageNo / PAGES_PER_CHIP;
    mwChipAddr = ((mwChipNo / 8)<<24) + ((mwChipNo % 8)<<8);

    mwPageAddr = TarPageNo - (mwChipNo * PAGES_PER_CHIP);/*ÐÂ¿éŽÓÆä µÚ0¿é ¿ªÊŒÐŽÈë*/

    ret = NandSimRead(mrChipAddr, mrPageAddr, gDataPageBuffer, 0);

    for(m16i = 0; m16i < ByteCount; m16i++)
	{
		pDataBuffer[m16i] = gDataPageBuffer[m16i];
	}

	if(ret == 0)
	{
	    memset(gDataPageBuffer, 0x00, MAX_BYTES_PER_PAGE);

	    ret = NandSimWrite(mwChipAddr, mwPageAddr, pDataBuffer, 0);

	    for(m16i = 0; m16i < ByteCount; m16i++)
	    {
	        gDataPageBuffer[m16i] = pDataBuffer[m16i];
	    }
	}

	return ret;
}

/**
PS: 找到该页对应的块，删除此块
*/
uint32_t Nand_Mapping_EraseBlock(uint32_t PageNo)
{
    uint32_t ret;
    uint32_t mChipNo;
    uint32_t mChipAddr;
    uint32_t mPageAddr;

    mChipNo = PageNo / BLOCKS_PER_CHIP;
    mChipAddr = ((mChipNo / 8)<<24) + ((mChipNo % 8)<<8);

    mPageAddr = PageNo - (mChipNo * PAGES_PER_CHIP);

    ret = NandSimErase(mChipAddr, mPageAddr);

    return ret;
}

/**
*	函 数 名: NAND_BuildLUT
*	功能说明: 创建坏块管理表
*	返 回 值: NAND_OK： 成功； 	NAND_FAIL：失败
*/
uint8_t NAND_BuildLUT(void)
{
    uint8_t  CHECK_SPARE_BUFFER[CHECK_SPARE_SIZE];
	uint16_t uLBN;
	uint32_t i;

	for (i = 0; i < NAND_BLOCK_COUNT; i++)
	{
	    /* 填充无效值，用于重建LUT后，判断LUT是否合理 */
		m_LUT[i] = 0xFFFF;
	}
	for (i = 0; i < NAND_BLOCK_COUNT; i++)
	{
	    /* 读每个块的第1个PAGE，偏移地址为LBN0_OFFSET的数据 */
		Nand_Mapping_ReadSpare(CHECK_SPARE_BUFFER, i * MAX_PAGES_PER_BLOCK, 0, CHECK_SPARE_SIZE);

        /* 如果是好块，则记录LBN0 LBN1 */
		if (CHECK_SPARE_BUFFER[BB_OFFSET] == 0xFF)
		{
			uLBN = CHECK_SPARE_BUFFER[LBN0_OFFSET] + CHECK_SPARE_BUFFER[LBN1_OFFSET] * 256;
			if (uLBN < NAND_BLOCK_COUNT)
			{
			    /* 如果已经登记过了，则判定为异常 */
				if (m_LUT[uLBN] != 0xFFFF)
				{
					return NAND_FAIL;
				}

				m_LUT[uLBN] = i;/* 更新LUT表 */
			}
		}
	}

    /* LUT建立完毕，检查是否合理 */
	for (i = 0; i < NAND_BLOCK_COUNT; i++)
	{
		if (m_LUT[i] >= NAND_BLOCK_COUNT)
		{
			m_ValidDataBlockCount = i;
			break;
		}
	}

	if (m_ValidDataBlockCount < 100)
	{
	    /* 错误： 最大的有效逻辑块号小于100。可能是没有格式化 */
		return NAND_FAIL;
	}

	for (; i < m_ValidDataBlockCount; i++)
	{
		if (m_LUT[i] != 0xFFFF)
		{
            /* 错误：LUT表逻辑块号存在跳跃现象，可能是没有格式化 */
			return NAND_FAIL;
		}

	}

    /* 重建LUT正常 */
	return NAND_OK;
}

/**
*	函 数 名:  NAND_AddrToPhyBlockNo
*	功能说明:  内存逻辑地址转换为物理块号
*	形    参:  aLbn：逻辑块号；
*	返 回 值:  物理页号， 如果是 0xFFFF 则表示错误
*/
uint16_t LbnToPbn(uint32_t aLbn)
{
	uint16_t uPBN;		/* 物理块号 */

	/* 如果逻辑块号大于有效的数据块个数则固定返回0xFFFF, 调用该函数的代码应该检查出这种错误 */
	if (aLbn >= m_ValidDataBlockCount)
	{
		return 0xFFFF;
	}
	/* 查询坏块表，获得物理块号 */
	uPBN = m_LUT[aLbn];

	return uPBN;
}
