/*********************************************************
*     File Name : Hardware.h
*     YIN  @ 2016.12.03
*     Description  : Initial create
**********************************************************/
#ifndef _HARDWARE_H_
#define _HARDWARE_H_

#include <stdint.h>

#define HARDWARE_INTERFACE

typedef struct _NANDFLASH_PARAM
{
    uint16_t mSizePerPage;
    uint16_t mBytesPerPage;
    uint16_t mBytesPerSpare;
    uint16_t mPagesPerBlock;
    uint16_t mBlocksPerChip;
    uint16_t mPlanesPerChip;
    uint16_t mBlocksPerPlane;
    uint16_t mMultiPlaneEnable;
    uint16_t mSectorsPerPage;
} NANDFLASH_PARAM_S;

typedef struct _LAYOUT_PARAM
{
    uint8_t  mDiesPerChip;
    uint16_t mChipsPerChannel;
    uint8_t  mChannelsPerArray;
} LAYOUT_PARAM_S;

LAYOUT_PARAM_S    gHW_LayoutParam;
NANDFLASH_PARAM_S gHW_NandFlashParam;

extern HARDWARE_INTERFACE   const char* gHW_NandFlashID = "SSD_NANDFLASHID";
extern HARDWARE_INTERFACE   const char* gHW_NandFlashDir = "SSD_NANDFLASH";
extern HARDWARE_INTERFACE   const char* gHW_NandFlashBBIMG = "SSD_NANDFLASHBBIMG";

extern HARDWARE_INTERFACE   LAYOUT_PARAM_S   gHW_LayoutParam;


#endif
