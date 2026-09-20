/******************************************************************************
* File Name: slld.h
*
* Description: This is a header file for SPI based external flash
*              
********************************************************************************
* (c) 2026, Infineon Technologies AG, or an affiliate of Infineon
* Technologies AG. All rights reserved.
* This software, associated documentation and materials ("Software") is
* owned by Infineon Technologies AG or one of its affiliates ("Infineon")
* and is protected by and subject to worldwide patent protection, worldwide
* copyright laws, and international treaty provisions. Therefore, you may use
* this Software only as provided in the license agreement accompanying the
* software package from which you obtained this Software. If no license
* agreement applies, then any use, reproduction, modification, translation, or
* compilation of this Software is prohibited without the express written
* permission of Infineon.
*
* Disclaimer: UNLESS OTHERWISE EXPRESSLY AGREED WITH INFINEON, THIS SOFTWARE
* IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
* INCLUDING, BUT NOT LIMITED TO, ALL WARRANTIES OF NON-INFRINGEMENT OF
* THIRD-PARTY RIGHTS AND IMPLIED WARRANTIES SUCH AS WARRANTIES OF FITNESS FOR A
* SPECIFIC USE/PURPOSE OR MERCHANTABILITY.
* Infineon reserves the right to make changes to the Software without notice.
* You are responsible for properly designing, programming, and testing the
* functionality and safety of your intended application of the Software, as
* well as complying with any legal requirements related to its use. Infineon
* does not guarantee that the Software will be free from intrusion, data theft
* or loss, or other breaches ("Security Breaches"), and Infineon shall have
* no liability arising out of any Security Breaches. Unless otherwise
* explicitly approved by Infineon, the Software may not be used in any
* application where a failure of the Product or any consequences of the use
* thereof can reasonably be expected to result in personal injury.
*******************************************************************************/

#ifndef __INC_lldh
#define __INC_lldh

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define SLLD_VERSION          "16.2.1"

// ==================
// Status error flags
// ==================
#define HAS_NOTHING          0x00
#define HAS_STATBIT5_ERROR   0x01
#define HAS_STATBIT6_ERROR   0x02



#include "slld_fll.h"

// =======================
// SPI Flash Commands info
// =======================
#define SPI_WRR_CMD                 (0x01)
#define SPI_PP_CMD                  (0x02)
#define SPI_READ_CMD                (0x03)
#define SPI_WRDI_CMD                (0x04)
#define SPI_RDSR_CMD                (0x05)
#define SPI_WREN_CMD                (0x06)
#define SPI_RDSR2_CMD               (0x07)  // Read Status Register-2
#define SPI_FAST_READ_CMD           (0x0B)
#define SPI_FAST_READ_4B_CMD        (0x0C)
#define SPI_PP_4B_CMD               (0x12)
#define SPI_READ_4B_CMD             (0x13)
#define SPI_RDCR2_CMD               (0x15) // Read configuration register-2
#define SPI_IRPRD_CMD               (0x2B) // IRP register Read
#define SPI_IRPP_CMD                  (0x2F) // IRP register program
#define SPI_CLSR_CMD                (0x30)
#define SPI_QPP_CMD                 (0x32)
#define SPI_RDCR3_CMD           (0x33) // Read Configuration Register-3
#define SPI_QPP_4B_CMD              (0x34)
#define SPI_RCR_CMD                 (0x35)
#define SPI_IBL_CMD                   (0x36) // IBL lock
#define SPI_QPIEN_CMD               (0x38) // Enter QPI
#define SPI_IBUL_CMD                 (0x39) // IBL unlock
#define SPI_DOR_CMD             (0x3B) // Dual output read
#define SPI_4DOR_CMD              (0x3C) // Dual output read 4 Byte address
#define SPI_IBLRD_CMD           (0x3D) // IBL read
#define SPI_DLPRD_CMD               (0x41)  // Read Data Learning Pattern
#define SPI_SECRP_CMD           (0x42) // security region program
#define SPI_PNVDLR_CMD              (0x43)  // Program NV Data Learning Register
#define SPI_SECRE_CMD             (0x44) // security region erase
#define SPI_SECRR_CMD             (0x48) // security region read
#define SPI_WVDLR_CMD               (0x4A)  // Write Volatile Data Learning Register
#define SPI_RUID_CMD                (0x4B)
#define SPI_WRENV_CMD            (0x50) // Write enable for Volatile status and configureation registers
#define SPI_HBE_CMD             (0x52) // Half Block Erase
#define SPI_4HBE_CMD            (0x53) // Half Block Erase 4B address
#define SPI_READ_SFDP_CMD           (0x5A)  // Read Serial Flash Discoverable Parameter Register
#define SPI_CE_CMD              (0x60) // chip erase
#define SPI_RDAR_CMD                (0x65)  // Read Any Register
#define SPI_RSTEN_CMD               (0x66)  // Software Reset Enable
#define SPI_QOR_CMD                 (0x6B) // Quad Output Read
#define SPI_4QOR_CMD                  (0x6C) // Quad Output Read 4B address
#define SPI_WRAR_CMD                (0x71)  // Write Any Register
#define SPI_SETBURSTWRAP_CMD        (0x77)  // Set Burst with Wrap
#define SPI_GBL_CMD               (0x7E) // Global IBL lock
//#define SPI_CLSR2_CMD               (0x82)  //Clear Status Register 1 (alternate instruction) - Erase/Prog. Fail Reset
#define SPI_RDID_90_CMD  (0x90)
#define SPI_GBUL_CMD                (0x98) // Global IBL unlock
#define SPI_RESET_CMD               (0x99)  // Software Reset
#define SPI_RDID_9F_CMD          (0x9F)
#define SPI_PRL_CMD                (0xA6) // Protection Register lock
#define SPI_PRRD_CMD                  (0xA7) // Protection Register read
#define SPI_RDID_AB_CMD             (0xAB)
#define SPI_RES_CMD                 (0xAB)
#define SPI_RDQID_CMD               (0xAF)  //Read Quad ID
#define SPI_EPS_CMD                 (0x75)  // for FLL
#define SPI_EPR_CMD                 (0x7A) // for FLL
#define SPI_4BEN_CMD                (0xB7) // Enter 4-bytes Address Mode
#define SPI_DP_CMD                  (0xB9)
#define SPI_DIOR_CMD            (0xBB) // Dual IO read
#define SPI_4DIOR_CMD          (0xBC) // Dual IO read 4B address
#define SPI_CE1_CMD              (0xC7) // chip erase
#define SPI_SE_CMD          (0x20)  // Sector Erase
#define SPI_SE_4B_CMD               (0x21)
#define SPI_BLOCK_ERASE_CMD         (0xD8)  // BLOCK ERASE
#define SPI_4BE_CMD             (0xDC) // Block erase 4B address
#define SPI_4IBLRD_CMD              (0xE0) // IBL read 4B address
#define SPI_4IBL_CMD                   (0xE1) // IBL lock 4B address
#define SPI_4IBUL_CMD                 (0xE2) // IBL unlock 4B address
#define SPI_4SPRP_CMD               (0xE3) // Set Point Region Protection 4B address
#define SPI_PASSRD_CMD              (0xE7)  // Password Read
#define SPI_PASSP_CMD               (0xE8)  // Password Program
#define SPI_4BEX_CMD                (0xE9) // Exit 4 Byte Address mode
#define SPI_PASSU_CMD            (0xEA) // Password Unlock
#define SPI_QIOR_CMD            (0xEB) // Quad IO read
#define SPI_4QIOR_CMD          (0xEC) // Quad IO read 4B address
#define SPI_DDRQIOR_CMD         (0xED) // DDR Quad IO read
#define SPI_4DDRQIOR_CMD       (0xEE) // DDR Quad IO read 4B address
#define SPI_QPIEX_CMD               (0xF5) // Exit QPI
#define SPI_SPRP_CMD                (0xFB) // Set Pointer Region Protection
#define SPI_MBR_CMD                 (0xFF) // Mode Bit Reset
#define SPI_DUALIO_RD_CMD           (0x3B)
#define SPI_DUALIO_RD_4B_CMD        (0x3C)
#define SPI_QUADIO_RD_CMD           (0x6B)
#define SPI_QUADIO_RD_4B_CMD        (0x6C)
#define SPI_OTPR_CMD                (0x4B)
#define SPI_DUALIO_HPRD_CMD         (0xBB)
#define SPI_DUALIO_HPRD_4B_CMD      (0xBC)
#define SPI_QUADIO_HPRD_CMD         (0xEB)
#define SPI_QUADIO_HPRD_4B_CMD      (0xEC)
#define SPI_P8E_4B_CMD              (0x4C)

/* Voltage States */
#define SLLD_P_VIL                  (0x0001)
#define SLLD_P_VIH                  (0x0002)

/* LLD System Specific Typedefs */
typedef unsigned short LLD_UINT16;  /* 16 bits wide */
typedef unsigned long  LLD_UINT32;  /* 32 bits wide */
typedef LLD_UINT32     ADDRESS;     /* Used for system level addressing */

/* SLLD System Specific Typedefs */
typedef unsigned char  BYTE;   /* 8 bits wide */
typedef unsigned short WORD;   /* 16 bits wide */
typedef unsigned long  DWORD;  /* 32 bits wide */
typedef BYTE FLASHDATA;

/* SLLD Internal Data Types */
//typedef DWORD PARAM;            /* MUST be at least 32 bits wide */
typedef unsigned long BYTECOUNT;  /* used for multi-byte operations */

/* boolean macros */
#ifndef TRUE
#define TRUE  (1)
#endif
#ifndef FALSE
#define FALSE (0)
#endif

typedef enum
{
    VOLATILE_WR = 0,
    NON_VOLATILE_WR
}WRR_mode;

/* data mask */
#define B0_MASK                  (0x01)
#define B1_MASK                  (0x02)
#define B2_MASK                  (0x04)
#define B3_MASK                  (0x08)
#define B5_MASK                  (0x20)
#define B6_MASK                  (0x40)
#define B7_MASK                  (0x80)

#define LLD_DEV_READ_MASK        (0xFF)
#define LLD_BYTES_PER_OP         (0x01)

/*RDAR read any register address definition*/
// Non-Volatile Status and Configuration Registers
#define    SR1NV         0x00000000
#define    CR1NV         0x00000002
#define    CR2NV         0x00000003
#define    CR3NV         0x00000004
#define    CR4NV         0x00000005
#define    NVDLP         0x00000005
// Non-Volatile Data Learning Register
#define    NVDLR         0x00000010
#define    PASS7_0       0x00000020
//Non-Volatile Password Register
#define    PASS15_8      0x00000021
#define    PASS23_160    0x00000022
#define    PASS31_24     0x00000023
#define    PASS39_32     0x00000024
#define    PASS47_40     0x00000025
#define    PASS55_48     0x00000026
#define    PASS63_56     0x00000027
//#ifdef FL_L
#define    IRP7_0           0x00000030
#define    IRP15_8         0x00000031
//#else
#define    ASPR7_0       0x00000030
#define    ASPR15_8      0x00000031
//#endif
#define    PRPRA15_A8       0x00000039
#define    PRPRA23_A16     0x0000003A
#define    PRPRA31_A24     0x0000003B
#define    SR1V          0x00800000
//Volatile Status and Configuration Registers
#define    SR2V          0x00800001
#define    CR1V          0x00800002
#define    CR2V          0x00800003
#define    CR3V          0x00800004
#define    CR4V          0x00800005
#define    VDLP          0x00800005
// Volatile Data Learning Register
#define    VDLR          0x00800010
//Volatile PPB Lock Register
#define    PPBL          0x00800040
#define    PR              0x00800040

/*Device status */
/*Flash Software protect status */
typedef enum {
    FLASH_SOFTWARE_UNPROTECTED = 0,
    FLASH_SOFTWARE_PROTECTED
} DEV_SOFTWARE_PROTECT_STATUS;

// Flash embedded operation status
typedef enum {
    dev_status_unknown = 0,
    dev_not_busy,
    dev_program_error,
    dev_erase_error,
    dev_suspend,
    dev_busy
} DEVSTATUS;

// SLLD Returned values
typedef enum {
    SLLD_OK = 0x0,
    SLLD_E_DEVICE_SOFTWARE_PROTECTED,
    SLLD_E_HAL_ERROR = 0x200,
    SLLD_ERROR = 0xFFFF
} SLLD_STATUS;

// FLASH_RD and FLASH_WR functions needed defines
#define ADDRESS_NOT_USED 0xFFFFFFFF
#define BUFFER_NOT_USED  (BYTE*)0

extern BYTE modebit_char;

// Include Nor Super Tests
//#define NST_TESTS

/* Include for Competitive Analysis Test */
/* #define CA_TEST */

/* public function prototypes */

/* Operation Functions */



SLLD_STATUS slld_Read_IDCmd(BYTE device_num, BYTE* target);
SLLD_STATUS slld_RDIDCmd(BYTE device_num, BYTE* target, BYTECOUNT len_in_bytes);
SLLD_STATUS slld_RDSRCmd(BYTE device_num, BYTE* target);
SLLD_STATUS slld_WRENCmd(BYTE device_num);
SLLD_STATUS slld_WRDICmd(BYTE device_num);
void slld_VersionCmd(void);
SLLD_STATUS slld_DPCmd(BYTE device_num);
SLLD_STATUS slld_RESCmd(BYTE device_num);
SLLD_STATUS slld_ClearStatusRegisterCmd(BYTE device_num);
SLLD_STATUS slld_WRRCmd(BYTE device_num, BYTE* status_val, BYTE* config_val, BYTE* status2_val);
SLLD_STATUS slld_WRROp(BYTE device_num, BYTE* status_val, BYTE* config_val, BYTE* status2_val, DEVSTATUS* dev_status_ptr);
SLLD_STATUS slld_RCRCmd(BYTE device_num, BYTE* target);
SLLD_STATUS slld_ReadCmd(BYTE device_num, ADDRESS sys_addr, BYTE* target, BYTECOUNT len_in_bytes);
SLLD_STATUS slld_Read_4BCmd(BYTE device_num, ADDRESS sys_addr, BYTE* target, BYTECOUNT len_in_bytes);
SLLD_STATUS slld_Fast_ReadCmd(BYTE device_num, ADDRESS sys_addr, BYTE* target, BYTECOUNT len_in_bytes);
SLLD_STATUS slld_Fast_Read_4BCmd(BYTE device_num, ADDRESS sys_addr, BYTE* target, BYTECOUNT len_in_bytes);
SLLD_STATUS slld_DORCmd(BYTE device_num, ADDRESS sys_addr, BYTE* target, BYTECOUNT len_in_bytes);
SLLD_STATUS slld_4DORCmd(BYTE device_num, ADDRESS sys_addr, BYTE* target, BYTECOUNT len_in_bytes);
SLLD_STATUS slld_QORCmd(BYTE device_num, ADDRESS sys_addr, BYTE* target, BYTECOUNT len_in_bytes);
SLLD_STATUS slld_4QORCmd(BYTE device_num, ADDRESS sys_addr, BYTE* target, BYTECOUNT len_in_bytes);
SLLD_STATUS slld_DIORCmd(BYTE device_num, ADDRESS sys_addr, BYTE* target, BYTE modebit, BYTECOUNT len_in_bytes);
SLLD_STATUS slld_4DIORCmd(BYTE device_num, ADDRESS sys_addr, BYTE* target, BYTE modebit, BYTECOUNT len_in_bytes);
SLLD_STATUS slld_QIORCmd(BYTE device_num, ADDRESS sys_addr, BYTE* target, BYTE modebit, BYTECOUNT len_in_bytes);
SLLD_STATUS slld_4QIORCmd(BYTE device_num, ADDRESS sys_addr, BYTE* target, BYTE modebit, BYTECOUNT len_in_bytes);
SLLD_STATUS slld_DDRQIORCmd(BYTE device_num, ADDRESS sys_addr, BYTE* target, BYTE modebit, BYTECOUNT len_in_bytes);
SLLD_STATUS slld_4DDRQIORCmd(BYTE device_num, ADDRESS sys_addr, BYTE* target, BYTE modebit, BYTECOUNT len_in_bytes);
SLLD_STATUS slld_SECmd(BYTE device_num, ADDRESS sys_addr);
SLLD_STATUS slld_SEOp(BYTE device_num, ADDRESS sys_addr, DEVSTATUS* dev_status_ptr);
SLLD_STATUS slld_SE_4BCmd(BYTE device_num, ADDRESS sys_addr);
SLLD_STATUS slld_SE_4BOp(BYTE device_num, ADDRESS sys_addr, DEVSTATUS* dev_status_ptr);
SLLD_STATUS slld_HBECmd(BYTE device_num, ADDRESS sys_addr);
SLLD_STATUS slld_HBEOp(BYTE device_num, ADDRESS sys_addr, DEVSTATUS* dev_status_ptr);
SLLD_STATUS slld_4HBECmd(BYTE device_num, ADDRESS sys_addr);
SLLD_STATUS slld_4BECmd(BYTE device_num, ADDRESS sys_addr);
SLLD_STATUS slld_CECmd(BYTE device_num);
SLLD_STATUS slld_CEOp(BYTE device_num, DEVSTATUS* dev_status_ptr);
SLLD_STATUS slld_CE1Cmd(BYTE device_num);
SLLD_STATUS slld_CE1Op(BYTE device_num, DEVSTATUS* dev_status_ptr);
SLLD_STATUS slld_BlockEraseCmd(BYTE device_num, ADDRESS sys_addr);
SLLD_STATUS slld_BlockEraseOp(BYTE device_num, ADDRESS sys_addr, DEVSTATUS* dev_status_ptr);
SLLD_STATUS slld_PPCmd(BYTE device_num, ADDRESS sys_addr, BYTE* data_buf, BYTECOUNT len_in_bytes);
SLLD_STATUS slld_PPOp(BYTE device_num, ADDRESS sys_addr, BYTE* data_buf, BYTECOUNT len_in_bytes, DEVSTATUS* dev_status_ptr);
SLLD_STATUS slld_PP_4BCmd(BYTE device_num, ADDRESS sys_addr, BYTE* data_buf, BYTECOUNT len_in_bytes);
SLLD_STATUS slld_PP_4BOp(BYTE device_num, ADDRESS sys_addr, BYTE* data_buf, BYTECOUNT len_in_bytes, DEVSTATUS* dev_status_ptr);
SLLD_STATUS slld_QPPCmd(BYTE device_num, ADDRESS sys_addr, BYTE* data_buf, BYTECOUNT len_in_bytes);
SLLD_STATUS slld_QPPOp(BYTE device_num, ADDRESS sys_addr, BYTE* data_buf, BYTECOUNT len_in_bytes, DEVSTATUS* dev_status_ptr);
SLLD_STATUS slld_QPP_4BCmd(BYTE device_num, ADDRESS sys_addr, BYTE* data_buf, BYTECOUNT len_in_bytes);
SLLD_STATUS slld_QPP_4BOp(BYTE device_num, ADDRESS sys_addr, BYTE* data_buf, BYTECOUNT len_in_bytes, DEVSTATUS* dev_status_ptr);
SLLD_STATUS slld_Poll(BYTE device_num, DEVSTATUS* dev_status_ptr);
SLLD_STATUS slld_StatusGet(BYTE device_num, DEVSTATUS* dev_status_ptr);
SLLD_STATUS slld_BufferedProgramOp(BYTE device_num, ADDRESS sys_addr, BYTE* data_buf, BYTECOUNT len_in_bytes, DEVSTATUS* dev_status_ptr);
SLLD_STATUS slld_BufferedProgram_4BOp(BYTE device_num, ADDRESS sys_addr, BYTE* data_buf, BYTECOUNT len_in_bytes, DEVSTATUS* dev_status_ptr);
SLLD_STATUS slld_BlockProtectOp(BYTE device_num, BYTE bpb_value, DEVSTATUS* dev_status_ptr);
SLLD_STATUS slld_QuadWriteOp(BYTE device_num, ADDRESS sys_addr, BYTE* data_buf, BYTECOUNT len_in_bytes, DEVSTATUS* dev_status_ptr);
SLLD_STATUS slld_RDSR2Cmd(BYTE device_num, BYTE* target);
SLLD_STATUS slld_DLPRDCmd(BYTE device_num, BYTE* data_buf, BYTECOUNT len_in_bytes);
SLLD_STATUS slld_PNVDLRCmd(BYTE device_num, BYTE* data_buf, BYTECOUNT len_in_bytes);
SLLD_STATUS slld_WVDLRCmd(BYTE device_num, BYTE* data_buf, BYTECOUNT len_in_bytes);
SLLD_STATUS slld_PASSRDCmd(BYTE device_num, BYTE* data_buf);
SLLD_STATUS slld_PASSPCmd(BYTE device_num, BYTE* data_buf);
SLLD_STATUS slld_PASSUCmd(BYTE device_num, BYTE* data_buf);
SLLD_STATUS slld_RDARCmd(BYTE device_num, ADDRESS reg_addr, BYTE* target);
SLLD_STATUS slld_WRARCmd(BYTE device_num, ADDRESS reg_addr, BYTE* data_buf);
SLLD_STATUS slld_WRAR_Op(BYTE device_num, ADDRESS reg_addr, BYTE* data_buf, DEVSTATUS* dev_status_ptr);
SLLD_STATUS slld_RSTENCmd(BYTE device_num);
SLLD_STATUS slld_RSTCmd(BYTE device_num);
SLLD_STATUS slld_RDQIDCmd(BYTE device_num, BYTE* target, BYTECOUNT len_in_bytes);
SLLD_STATUS slld_EPR_Cmd(BYTE device_num);
SLLD_STATUS slld_EPS_Cmd(BYTE device_num);
SLLD_STATUS slld_ReadSFDPCmd(BYTE device_num, ADDRESS sys_addr, BYTE* read_buf);
SLLD_STATUS slld_RDCR2Cmd(BYTE device_num, BYTE* target);
SLLD_STATUS slld_RDCR3Cmd(BYTE device_num, BYTE* target);
SLLD_STATUS slld_IRPRDCmd(BYTE device_num, WORD* target);
SLLD_STATUS slld_IRPPCmd(BYTE device_num, WORD* irp_val);
SLLD_STATUS slld_IRPPOp(BYTE device_num, WORD* irp_val, DEVSTATUS* dev_status_ptr);
SLLD_STATUS slld_QPIENCmd(BYTE device_num);
SLLD_STATUS slld_QPIEXCmd(BYTE device_num);
SLLD_STATUS slld_4BENCmd(BYTE device_num);
SLLD_STATUS slld_4BEXCmd(BYTE device_num);
SLLD_STATUS slld_SECRRCmd(BYTE device_num, ADDRESS sys_addr, BYTE* read_buf, BYTECOUNT len_in_bytes);
SLLD_STATUS slld_SECRPCmd(BYTE device_num, ADDRESS sys_addr, BYTE* program_buf, BYTECOUNT len_in_bytes);
SLLD_STATUS slld_SECRPOp(BYTE device_num, ADDRESS sys_addr, BYTE* program_buf, BYTECOUNT len_in_bytes, DEVSTATUS* dev_status_ptr);
SLLD_STATUS slld_SECRECmd(BYTE device_num, ADDRESS sys_addr);
SLLD_STATUS slld_SECREOp(BYTE device_num, ADDRESS sys_addr, DEVSTATUS* dev_status_ptr);
SLLD_STATUS slld_RUIDCmd(BYTE device_num, BYTE* read_buf);
SLLD_STATUS slld_WRENVCmd(BYTE device_num);
SLLD_STATUS slld_IBLRDCmd(BYTE device_num, ADDRESS sys_addr, BYTE* target);
SLLD_STATUS slld_4IBLRDCmd(BYTE device_num, ADDRESS sys_addr, BYTE* target);
SLLD_STATUS slld_IBLCmd(BYTE device_num, ADDRESS sys_addr);
SLLD_STATUS slld_4IBLCmd(BYTE device_num, ADDRESS sys_addr);
SLLD_STATUS slld_IBLOp(BYTE device_num, ADDRESS sys_addr, DEVSTATUS* dev_status_ptr);
SLLD_STATUS slld_IBULCmd(BYTE device_num, ADDRESS sys_addr);
SLLD_STATUS slld_4IBULCmd(BYTE device_num, ADDRESS sys_addr);
SLLD_STATUS slld_IBULOp(BYTE device_num, ADDRESS sys_addr, DEVSTATUS* dev_status_ptr);
SLLD_STATUS slld_GBLCmd(BYTE device_num);
SLLD_STATUS slld_GBLOp(BYTE device_num, DEVSTATUS* dev_status_ptr);
SLLD_STATUS slld_GBULCmd(BYTE device_num);
SLLD_STATUS slld_GBULOp(BYTE device_num, DEVSTATUS* dev_status_ptr);
SLLD_STATUS slld_SPRPCmd(BYTE device_num, ADDRESS sys_addr);
SLLD_STATUS slld_4SPRPCmd(BYTE device_num, ADDRESS sys_addr);
SLLD_STATUS slld_SPRPOp(BYTE device_num, ADDRESS sys_addr, DEVSTATUS* dev_status_ptr);
SLLD_STATUS slld_PRLCmd(BYTE device_num);
SLLD_STATUS slld_PRRDCmd(BYTE device_num, BYTE* data_buf);
SLLD_STATUS slld_SBLCmd(BYTE device_num, BYTE* wrapbit_buf);
SLLD_STATUS slld_MBRCmd(BYTE device_num);



/*****************************************************
 * Define Flash read/write macro to be used by SLLD   *
 *****************************************************/
#ifdef TRACE
#define FLASH_WR(dn, c,a,d,n)    FlashWrite(dn, c,a,d,n)
#define FLASH_RD(dn, c,a,d,n)    FlashRead(dn, c,a,d,n)
#elif defined BOARD226
#define FLASH_WR(dn, c,a,d,n)    FLASH_WRITE(dn, c,a,d,n)
#define FLASH_RD(dn, c,a,d,n)    FLASH_READ(dn, c,a,d,n)
#else
#define FLASH_WR(dn,c,a,d,n)    FLASH_WRITE(dn,c,a,d,n)
#define FLASH_RD(dn,c,a,d,n)    FLASH_READ(dn,c,a,d,n)
#endif

#ifdef UCHAR
typedef unsigned char   UCHAR;
#endif
#ifdef USHORT
typedef unsigned short  USHORT;
#endif
#ifdef UINT
typedef unsigned int    UINT;
#endif
#ifdef ULONG
typedef unsigned long   ULONG;
#endif
#ifdef BOARD226
#include "slld_hal_226.h"
#endif

SLLD_STATUS slld_ReadOp(BYTE device_num, ADDRESS sys_addr, BYTE* target, BYTECOUNT len_in_bytes);
SLLD_STATUS slld_WriteOp(BYTE device_num, ADDRESS sys_addr, BYTE* data_buf, BYTECOUNT len_in_bytes, DEVSTATUS* dev_status_ptr);
SLLD_STATUS slld_GetDevNumFromAddr(ADDRESS sys_addr, BYTE* device_num);
#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* __INC_lldh */

