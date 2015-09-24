;   Copyright   Novatek Microelectronics Corp. 2005.  All rights reserved.
;
;   @file       VersionInfo.s
;   @ingroup
;
;   @brief      Project version and build date information
;               Project version and build date information
;
;   @note       This file is generated by CreatePrjVer.bat
;
;   @version    V1.00.000
;   @author     Novatek FW Team
;   @date       2011/05/24

    AREA        VERSIONINFO, CODE, READONLY

;//#To know Part One Size for Partial Load and FW Compression.
    IMPORT      ||Load$$PART_TWO_RO$$Base||

    EXPORT      VersionNumber
    EXPORT      ReleaseDate
    EXPORT      ProjectCode
    EXPORT      PartLoad

LENGTH          EQU         0x0
CHECKSUM        EQU         0x0
CRCLENGTH       EQU         0x0
PARTLOAD        EQU         0x0
CRCTAG          EQU         0x0
RESERVEDSIZE    EQU         0x0
RESERVED1       EQU         0x0
CMPRESSTAG      EQU         0x0

ProjectCode
    DCB         "NT96220 "
VersionNumber
    DCB         "0.46.000"
ReleaseDate
    DCB         "20110211"
Length
    DCD         LENGTH
Checksum
    DCD         CHECKSUM
CRCLength
    DCD         CRCLENGTH   ; Length check (CRC32)
PartLoad
    IF _FWCOMPRESS_TYPE_ = _FWCOMPRESS_NONE_ :LOR: _FWCOMPRESS_TYPE_ = _FWCOMPRESS_FULL_
    DCD         PARTLOAD
    ELSE
	DCD         ||Load$$PART_TWO_RO$$Base||         ; 0x74 --> partial load address
    ENDIF
;//#NT#NAND Reserved size offset
ReservedSize                                        ; 0x78 --> Reserved data
    DCD         RESERVEDSIZE
CRCTag                                              ; 0x7C
    DCD         CRCTAG      ; Binary tag (CRC32)
Reserved1                                           ; 0x80
    DCD         RESERVED1
Reserved2
    DCD         0                                   ; 0x84
Reserved3
    DCD         0                                   ; 0x88

CompressTag                                         ; 0x8C
    DCD         CMPRESSTAG
FlashSizeTag                                        ; 0x90
    DCD         0
    END