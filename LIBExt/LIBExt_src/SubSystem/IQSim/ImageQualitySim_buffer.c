#include "ImageQualitySim_int.h"
#include "FileSysTsk.h"
#include "grph.h"

BUF_ALLOC* pBufAlloc = 0;

void BUF_ALLOC_Assign(BUF_ALLOC* pUserBufAlloc)
{
    pBufAlloc = pUserBufAlloc;
}

void BUF_ALLOC_Open(UINT32 BufAddr, UINT32 BufSize)
{
    pBufAlloc->tAddr = BufAddr;
    pBufAlloc->tSize = BufSize;
    pBufAlloc->tEnd = BufAddr+BufSize;
    pBufAlloc->cAddr = pBufAlloc->tAddr;
}

UINT32 BUF_ALLOC_New(UINT32 BufSize)
{
    UINT32 cAddr = pBufAlloc->cAddr;
    UINT32 nAddr = pBufAlloc->cAddr + BufSize;
    if(nAddr >= pBufAlloc->tEnd)
    {
        debug_err(("Buf alloc: %08x\r\n", cAddr));
        debug_err(("Buf alloc: %08x\r\n", nAddr));
        debug_err(("Buf alloc: %08x\r\n", pBufAlloc->tEnd));
        debug_err(("Buf alloc error: not enough free space!\r\n"));
        return 0;
    }
    pBufAlloc->cAddr = nAddr;
    return cAddr;
}

void BUF_ALLOC_Close(void)
{
}

void IQ_BUF_Attach(IQ_BUF* pBuf, IQ_BUF_FMT fmt, UINT32 w, UINT32 h, UINT32 lineoff,
    UINT32 size, UINT32 addr0, UINT32 addr1, UINT32 addr2)
{
    pBuf->fmt = fmt;
    pBuf->w = w;
    pBuf->h = h;
    //pBuf->lineoff = lineoff;
    pBuf->size = size;
    pBuf->addr[0] = addr0;
    pBuf->addr[1] = addr1;
    pBuf->addr[2] = addr2;
    switch(fmt)
    {
    case IQ_FMT_RAW8:
        if(lineoff == 0)lineoff = w;
        pBuf->lineoff = lineoff;
        break;
    case IQ_FMT_RAW10:
        if(lineoff == 0)lineoff = w*10/8;
        pBuf->lineoff = lineoff;
        break;
    case IQ_FMT_Y8:
        if(lineoff == 0)lineoff = w;
        lineoff = ((lineoff+3)>>2)<<2; //Y align to 4
        pBuf->lineoff = lineoff;
        break;
    case IQ_FMT_YCC444:
        if(lineoff == 0)lineoff = w;
        lineoff = ((lineoff+3)>>2)<<2; //Y align to 4, CC align to 4
        pBuf->lineoff = lineoff;
        break;
    case IQ_FMT_YCC422:
        if(lineoff == 0)lineoff = w;
        lineoff = ((lineoff+7)>>3)<<3; //Y align to 8, CC align to 4
        pBuf->lineoff = lineoff;
        break;
    case IQ_FMT_YCC420:
        if(lineoff == 0)lineoff = w;
        lineoff = ((lineoff+7)>>3)<<3; //Y align to 8, CC align to 4
        pBuf->lineoff = lineoff;
        break;
    case IQ_FMT_YCCJPG:
        pBuf->lineoff = 0;
        break;
    }
}

void IQ_BUF_New(IQ_BUF* pBuf, IQ_BUF_FMT fmt, UINT32 w, UINT32 h, UINT32 lineoff)
{
    if((w == 0) || (h == 0))
        return;
    switch(fmt)
    {
    case IQ_FMT_RAW8:
        if(lineoff == 0)lineoff = w;
        pBuf->lineoff = lineoff;
        pBuf->size = pBuf->lineoff*h;
        pBuf->addr[0] = BUF_ALLOC_New(pBuf->size);
        pBuf->addr[1] = 0;
        pBuf->addr[2] = 0;
        break;
    case IQ_FMT_RAW10:
        if(lineoff == 0)lineoff = w*10/8;
        pBuf->lineoff = lineoff;
        pBuf->size = pBuf->lineoff*h*10/8;
        pBuf->addr[0] = BUF_ALLOC_New(pBuf->size);
        pBuf->addr[1] = 0;
        pBuf->addr[2] = 0;
        break;
    case IQ_FMT_Y8:
        if(lineoff == 0)lineoff = w;
        lineoff = ((lineoff+3)>>2)<<2; //Y align to 4
        pBuf->lineoff = lineoff;
        pBuf->size = pBuf->lineoff*h;
        pBuf->addr[0] = BUF_ALLOC_New(pBuf->lineoff*h);
        pBuf->addr[1] = 0;
        pBuf->addr[2] = 0;
        break;
    case IQ_FMT_YCC444:
        if(lineoff == 0)lineoff = w;
        lineoff = ((lineoff+3)>>2)<<2; //Y align to 4, CC align to 4
        pBuf->lineoff = lineoff;
        pBuf->size = pBuf->lineoff*h*3;
        pBuf->addr[0] = BUF_ALLOC_New(pBuf->lineoff*h);
        pBuf->addr[1] = BUF_ALLOC_New(pBuf->lineoff*h);
        pBuf->addr[2] = BUF_ALLOC_New(pBuf->lineoff*h);
        break;
    case IQ_FMT_YCC422:
        if(lineoff == 0)lineoff = w;
        lineoff = ((lineoff+7)>>3)<<3; //Y align to 8, CC align to 4
        pBuf->lineoff = lineoff;
        pBuf->size = pBuf->lineoff*h*2;
        pBuf->addr[0] = BUF_ALLOC_New(pBuf->lineoff*h);
        pBuf->addr[1] = BUF_ALLOC_New(pBuf->lineoff*h/2);
        pBuf->addr[2] = BUF_ALLOC_New(pBuf->lineoff*h/2);
        break;
    case IQ_FMT_YCC420:
        if(lineoff == 0)lineoff = w;
        lineoff = ((lineoff+7)>>3)<<3; //Y align to 8, CC align to 4
        pBuf->lineoff = lineoff;
        pBuf->size = pBuf->lineoff*h*3/2;
        pBuf->addr[0] = BUF_ALLOC_New(pBuf->lineoff*h);
        pBuf->addr[1] = BUF_ALLOC_New(pBuf->lineoff*h/4);
        pBuf->addr[2] = BUF_ALLOC_New(pBuf->lineoff*h/4);
        break;
    case IQ_FMT_YCCJPG:
        pBuf->lineoff = 0;
        pBuf->size = w*h/2;
        pBuf->addr[0] = BUF_ALLOC_New(pBuf->size);
        pBuf->addr[1] = 0;
        pBuf->addr[2] = 0;
        break;
    }
    pBuf->fmt = fmt;
    pBuf->w = w;
    pBuf->h = h;
/*    
    debug_msg("buf w,h,fmt=%d,%d,%d\r\n", pBuf->w, pBuf->h, pBuf->fmt);
    debug_msg("buf lineoff,size=%d,%d\r\n", pBuf->lineoff, pBuf->size);
    debug_msg("buf addr[3]=%08x,%08x,%08x\r\n", pBuf->addr[0], pBuf->addr[1], pBuf->addr[2]);
*/    
}

void IQ_BUF_Clone(IQ_BUF* pBuf, IQ_BUF* pSrcBuf)
{
    *pBuf = *pSrcBuf;
}

void IQ_BUF_CloneByChannel(IQ_BUF* pBuf, IQ_BUF* pSrcBuf, IQ_CNL cnl)
{
    UINT32 wShift = 0;
    UINT32 hShift = 0;
    
    switch(pSrcBuf->fmt)
    {
    case IQ_FMT_YCC444:
        wShift = 0; hShift = 0; break;
    case IQ_FMT_YCC422:
        wShift = 1; hShift = 0; break;
    case IQ_FMT_YCC420:
        wShift = 1; hShift = 1; break;
    }
    if(cnl == IQ_CNL_Y)
    {
        pBuf->fmt = IQ_FMT_Y8;
        pBuf->w = pSrcBuf->w;
        pBuf->h = pSrcBuf->h;
        pBuf->lineoff = pSrcBuf->lineoff;
        pBuf->addr[0] = pSrcBuf->addr[0];
        pBuf->addr[1] = 0;
        pBuf->addr[2] = 0;
        pBuf->size = pBuf->lineoff*pBuf->h;
    }
    if(cnl == IQ_CNL_Cb)
    {
        pBuf->fmt = IQ_FMT_Y8;
        pBuf->w = pSrcBuf->w >> wShift;
        pBuf->h = pSrcBuf->h >> hShift;
        pBuf->lineoff = pSrcBuf->lineoff >> wShift;
        pBuf->addr[0] = pSrcBuf->addr[1];
        pBuf->addr[1] = 0;
        pBuf->addr[2] = 0;
        pBuf->size = pBuf->lineoff*pBuf->h;
    }
    if(cnl == IQ_CNL_Cr)
    {
        pBuf->fmt = IQ_FMT_Y8;
        pBuf->w = pSrcBuf->w >> wShift;
        pBuf->h = pSrcBuf->h >> hShift;
        pBuf->lineoff = pSrcBuf->lineoff >> wShift;
        pBuf->addr[0] = pSrcBuf->addr[2];
        pBuf->addr[1] = 0;
        pBuf->addr[2] = 0;
        pBuf->size = pBuf->lineoff*pBuf->h;
    }
}

void IQ_BUF_CloneByRect(IQ_BUF* pBuf, IQ_BUF* pSrcBuf, UINT32 x, UINT32 y, UINT32 w, UINT32 h)
{
    if((x == 0) && (y == 0) && (w == 0) && (h == 0))
    {
        IQ_BUF_Clone(pBuf, pSrcBuf);
        return;
    }
    
    if(x > pSrcBuf->w-1)
        return;
    if(x+w > pSrcBuf->w-1)
        return;
    if(y > pSrcBuf->h-1)
        return;
    if(y+h > pSrcBuf->h-1)
        return;

    switch(pBuf->fmt)
    {
    case IQ_FMT_Y8:
        pBuf->fmt = pSrcBuf->fmt;
        pBuf->w = w;
        pBuf->h = h;
        pBuf->lineoff = pSrcBuf->lineoff;
        pBuf->addr[0] = pSrcBuf->addr[0] + y*(pSrcBuf->lineoff) + x;
        pBuf->addr[1] = 0;
        pBuf->addr[2] = 0;
        pBuf->size = pBuf->lineoff*pBuf->h;
        break;
    case IQ_FMT_YCC444:
        pBuf->fmt = pSrcBuf->fmt;
        pBuf->w = w;
        pBuf->h = h;
        pBuf->lineoff = pSrcBuf->lineoff;
        pBuf->addr[0] = pSrcBuf->addr[0] + y*(pSrcBuf->lineoff) + x;
        pBuf->addr[1] = pSrcBuf->addr[1] + y*(pSrcBuf->lineoff) + x;
        pBuf->addr[2] = pSrcBuf->addr[2] + y*(pSrcBuf->lineoff) + x;
        pBuf->size = pBuf->lineoff*pBuf->h*3;
        break;
    case IQ_FMT_YCC422:
        pBuf->fmt = pSrcBuf->fmt;
        pBuf->w = w;
        pBuf->h = h;
        pBuf->lineoff = pSrcBuf->lineoff;
        pBuf->addr[0] = pSrcBuf->addr[0] + y*(pSrcBuf->lineoff) + x;
        pBuf->addr[1] = pSrcBuf->addr[1] + y*(pSrcBuf->lineoff >> 1) + (x >> 1);
        pBuf->addr[2] = pSrcBuf->addr[2] + y*(pSrcBuf->lineoff >> 1) + (x >> 1);
        pBuf->size = pBuf->lineoff*pBuf->h*2;
        break;
    case IQ_FMT_YCC420:
        pBuf->fmt = pSrcBuf->fmt;
        pBuf->w = w;
        pBuf->h = h;
        pBuf->lineoff = pSrcBuf->lineoff;
        pBuf->addr[0] = pSrcBuf->addr[0] + y*(pSrcBuf->lineoff) + x;
        pBuf->addr[1] = pSrcBuf->addr[1] + (y >> 1)*(pSrcBuf->lineoff >> 1) + (x >> 1);
        pBuf->addr[2] = pSrcBuf->addr[2] + (y >> 1)*(pSrcBuf->lineoff >> 1) + (x >> 1);
        pBuf->size = pBuf->lineoff*pBuf->h*3/2;
        break;
    }
}

void IQ_BUF_CloneEx(IQ_BUF* pBuf, IQ_BUF* pSrcBuf, UINT32 x, UINT32 y, UINT32 w, UINT32 h, IQ_CNL cnl)
{
    IQ_BUF tBuf;
    IQ_BUF_CloneByRect(&tBuf, pSrcBuf, x, y, w, h);
    IQ_BUF_CloneByChannel(pBuf, &tBuf, cnl);
}

UINT32 IQ_BUF_GetPixelAddr(IQ_BUF* pBuf, UINT32 x, UINT32 y, IQ_CNL cnl)
{
    UINT32 addr = 0;
    switch(pBuf->fmt)
    {
    case IQ_FMT_Y8:
        addr = pBuf->addr[0] + y*pBuf->lineoff + x;
        break;
    case IQ_FMT_YCC444:
        break;
    case IQ_FMT_YCC422:
        break;
    case IQ_FMT_YCC420:
        break;
    }
    return addr;
}

void IQ_BUF_FillRect(
    IQ_BUF* pBuf, UINT32 x, UINT32 y, UINT32 w, UINT32 h, UINT32 color)
{
    if((w == 0) && (h == 0))
    {
        return;
    }
    
    //if(x < 0)
    //    return;
    //if(x+w-1 > pBuf->w-1)
    if(x+w-1 > pBuf->lineoff-1)
        return;
    //if(y < 0)
    //    return;
    if(y+h-1 > pBuf->h-1)
        return;

    grph_open();
    grph_enableDMA();
        
    switch(pBuf->fmt)
    {
    case IQ_FMT_Y8:
        //debug_err(("FillRect x,y,w,h=%d,%d,%d,%d\r\n", x,y,w,h));
        grph_setImg1(pBuf->addr[0] + y*pBuf->lineoff + x, pBuf->lineoff, h, w);
        grph_setAOP(0, GRPH_DST_1, GRPH_AOP_TEXT_COPY, color);
        break;
    case IQ_FMT_YCC444:
        break;
    case IQ_FMT_YCC422:
        break;
    case IQ_FMT_YCC420:
        break;
    }
        
    grph_disableDMA();
    grph_swReset();
    grph_close();
}

void IQ_BUF_Copy(
    IQ_BUF* pBuf, 
    IQ_BUF* pSrcBuf)
{
    if(pBuf->fmt != pSrcBuf->fmt)
        return;
    if(pBuf->w != pSrcBuf->w)
        return;
    if(pBuf->h != pSrcBuf->h)
        return;
    IQ_BUF_CopyRect(pBuf, 0, 0, pSrcBuf, 0, 0, pSrcBuf->w, pSrcBuf->h);
}

void IQ_BUF_CopyRect(
    IQ_BUF* pBuf, UINT32 x, UINT32 y,
    IQ_BUF* pSrcBuf, UINT32 sx, UINT32 sy, UINT32 sw, UINT32 sh)
{
    if((sw == 0) && (sh == 0))
    {
        return;
    }
    
    //if(sx < 0)
    //    return;
    //if(sx+sw-1 > pSrcBuf->w-1)
    if(sx+sw-1 > pSrcBuf->lineoff-1)
        return;
    //if(sy < 0)
    //    return;
    if(sy+sh-1 > pSrcBuf->h-1)
        return;
    //if(x < 0)
    //    return;
    //if(x+sw-1 > pBuf->w-1)
    if(x+sw-1 > pBuf->lineoff-1)
        return;
    //if(y < 0)
    //    return;
    if(y+sh-1 > pBuf->h-1)
        return;

    grph_open();
    grph_enableDMA();
        
    switch(pBuf->fmt)
    {
    case IQ_FMT_Y8:
        //debug_err(("CopyRect x,y,w,h=%d,%d,%d,%d->sx,sy=%d,%d\r\n", sx,sy,sw,sh,x,y));
        grph_setImg1(pSrcBuf->addr[0] + sy*pSrcBuf->lineoff + sx, pSrcBuf->lineoff, sh, sw);
        grph_setImg2(pBuf->addr[0] + y*pBuf->lineoff + x, pBuf->lineoff);
        grph_setAOP(0, GRPH_DST_2, GRPH_AOP_A_COPY, 0);
        break;
    case IQ_FMT_YCC444:
        break;
    case IQ_FMT_YCC422:
        break;
    case IQ_FMT_YCC420:
        break;
    }
        
    grph_disableDMA();
    grph_swReset();
    grph_close();
}

extern int sprintf(char * /*s*/, const char * /*format*/, ...);

UINT32 IQ_BUF_Dump(IQ_BUF* pBuf, char* filename, UINT32 ci, UINT32 offset)
{
    char filename1[128];
    UINT32  length = 0;
    int fsStatus = 0;
    FilesysWaitCmdFinish(FST_TIME_INFINITE);
    switch(pBuf->fmt)
    {
    case IQ_FMT_RAW8:
        sprintf(filename1, "%s.RAW", filename);
        length = pBuf->size;
        fsStatus = FilesysReadWriteByName2(
            FST_CMD_WRITE_BY_NAME,
            filename1,
            (UINT8*)pBuf->addr[0],
            &length,
            offset,
            FST_TIME_INFINITE);
        break;
    case IQ_FMT_RAW10:
        sprintf(filename1, "%s.RAW", filename);
        length = pBuf->size;
        fsStatus = FilesysReadWriteByName2(
            FST_CMD_WRITE_BY_NAME,
            filename1,
            (UINT8*)pBuf->addr[0],
            &length,
            offset,
            FST_TIME_INFINITE);
        break;
    case IQ_FMT_Y8:
        sprintf(filename1, "%sY.RAW", filename);
        length = pBuf->lineoff*pBuf->h;
        fsStatus = FilesysReadWriteByName2(
            FST_CMD_WRITE_BY_NAME,
            filename1,
            (UINT8*)pBuf->addr[0],
            &length,
            offset,
            FST_TIME_INFINITE);
        break;
    case IQ_FMT_YCC444:
        if(ci == 0)
        {
        sprintf(filename1, "%sY.RAW", filename);
        length = pBuf->lineoff*pBuf->h;
        fsStatus = FilesysReadWriteByName2(
            FST_CMD_WRITE_BY_NAME,
            filename1,
            (UINT8*)pBuf->addr[0],
            &length,
            offset,
            FST_TIME_INFINITE);
        }
        if(ci == 1)
        {
        sprintf(filename1, "%sCB.RAW", filename);
        length = pBuf->lineoff*pBuf->h;
        fsStatus = FilesysReadWriteByName2(
            FST_CMD_WRITE_BY_NAME,
            filename1,
            (UINT8*)pBuf->addr[1],
            &length,
            offset,
            FST_TIME_INFINITE);
        }
        if(ci == 2)
        {
        sprintf(filename1, "%sCR.RAW", filename);
        length = pBuf->lineoff*pBuf->h;
        fsStatus = FilesysReadWriteByName2(
            FST_CMD_WRITE_BY_NAME,
            filename1,
            (UINT8*)pBuf->addr[2],
            &length,
            offset,
            FST_TIME_INFINITE);
        }
        break;
    case IQ_FMT_YCC422:
        if(ci == 0)
        {
        sprintf(filename1, "%sY.RAW", filename);
        length = pBuf->lineoff*pBuf->h;
        fsStatus = FilesysReadWriteByName2(
            FST_CMD_WRITE_BY_NAME,
            filename1,
            (UINT8*)pBuf->addr[0],
            &length,
            offset,
            FST_TIME_INFINITE);
        }
        if(ci == 1)
        {
        sprintf(filename1, "%sCB.RAW", filename);
        length = pBuf->lineoff*pBuf->h/2;
        fsStatus = FilesysReadWriteByName2(
            FST_CMD_WRITE_BY_NAME,
            filename1,
            (UINT8*)pBuf->addr[1],
            &length,
            offset,
            FST_TIME_INFINITE);
        }
        if(ci == 2)
        {
        sprintf(filename1, "%sCR.RAW", filename);
        length = pBuf->lineoff*pBuf->h/2;
        fsStatus = FilesysReadWriteByName2(
            FST_CMD_WRITE_BY_NAME,
            filename1,
            (UINT8*)pBuf->addr[2],
            &length,
            offset,
            FST_TIME_INFINITE);
        }
        break;
    case IQ_FMT_YCC420:
        if(ci == 0)
        {
        sprintf(filename1, "%sY.RAW", filename);
        length = pBuf->lineoff*pBuf->h;
        fsStatus = FilesysReadWriteByName2(
            FST_CMD_WRITE_BY_NAME,
            filename1,
            (UINT8*)pBuf->addr[0],
            &length,
            offset,
            FST_TIME_INFINITE);
        }
        if(ci == 1)
        {
        sprintf(filename1, "%sCB.RAW", filename);
        length = pBuf->lineoff*pBuf->h/4;
        fsStatus = FilesysReadWriteByName2(
            FST_CMD_WRITE_BY_NAME,
            filename1,
            (UINT8*)pBuf->addr[1],
            &length,
            offset,
            FST_TIME_INFINITE);
        sprintf(filename1, "%sCR.RAW", filename);
        }
        if(ci == 2)
        {
        length = pBuf->lineoff*pBuf->h/4;
        fsStatus = FilesysReadWriteByName2(
            FST_CMD_WRITE_BY_NAME,
            filename1,
            (UINT8*)pBuf->addr[2],
            &length,
            offset,
            FST_TIME_INFINITE);
        }
        break;
    case IQ_FMT_YCCJPG:
        sprintf(filename1, "%s.JPG", filename);
        length = pBuf->size;
        fsStatus = FilesysReadWriteByName2(
            FST_CMD_WRITE_BY_NAME,
            filename1,
            (UINT8*)pBuf->addr[0],
            &length,
            offset,
            FST_TIME_INFINITE);
        break;
    }
    return length;
}

