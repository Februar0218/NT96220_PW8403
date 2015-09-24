/*
    Public header file for IFE module.

    This file is the header file that define the API and data type for IFE
    module.

    @file       ife_lib.h
    @ingroup    mIIPPIFE

    Copyright   Novatek Microelectronics Corp. 2010.  All rights reserved.
*/
#ifndef _SRA_LIB_220_H
#define _SRA_LIB_220_H

/**
    @addtogroup mIIPPSRA
*/
//@{

/**
    IFE Function Selection.

    IFE Function Selection.
\n  Used for ife_setMode()
*/
//@{
typedef enum
{
    SRA_READBY_SIE = 0,       ///< SRA is read by SIE
    SRA_READBY_IFE = 1,       ///< SRA is read by IFE
    ENUM_DUMMY4WORD(_SRA_READSEL)
}_SRA_READSEL;
//@}

//#NT#2011/05/20#Connie Yu -begin
//#NT#for protection of state machine
extern ER sra_setGammaTable(_SRA_READSEL sel, UINT32* gamma);
//#NT#2011/05/20#Connie Yu -end

#endif

//@}
