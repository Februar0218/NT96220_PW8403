#ifndef AUDEC_APIL_H
#define AUDEC_APIL_H


#define AUDEC_STATUS_OK                0

#define AUDEC_ID_NONE                0
#define AUDEC_ID_MP3D                1
#define AUDEC_ID_WMAD                2
#define AUDEC_ID_AACD                3


typedef enum {
    AUDEC_FLOW_CONTINUE = 0x0000,    /* continue normally */
      AUDEC_FLOW_STOP     = 0x0010,    /* stop decoding normally */
      AUDEC_FLOW_BREAK    = 0x0011,    /* stop decoding and signal an error */
      AUDEC_FLOW_IGNORE   = 0x0020    /* ignore the current frame */
} AUDECFlow_t;


typedef struct _AudioDecoderInfo_t{
    void     *decoder;
    //ai_object_t     *codec;
    unsigned long    dec_start;
    //void            *pDecoder;                // pointer to decoder
    //unsigned int    start;
    
    unsigned long     AudecID;                // audio decoder ID
    unsigned long     InputFileAddr;            // mp3 file addr
    unsigned long     InputFileSize;            // mp3 file size
    unsigned long     InputMp3DataBlockSize;    // 2007/09/27 Meg, mp3 block size, used for searching sync words
    unsigned long     BitStreamAddr;            // decoded bitstream addr
    unsigned long     OutputWAVFileAddr;        // 'WAVE' file output addr
    unsigned long     OutputWAVFileEnable;    // out wav-file mode
    unsigned long    CurrentTime;
    void             (*DecodeOneFrame_func)();
    void             (*DecodeAllFrame_func)();
} AudioDecoderInfo_t;

typedef struct {
    unsigned long InputFileAddr;            // mp3 file addr
    unsigned long InputFileSize;            // mp3 file size
    unsigned long InputMp3DataBlockSize;    // 2007/09/27 Meg, mp3 block size, used for searching sync words
    unsigned long BitStreamAddr;            // decoded bitstream addr            
    unsigned long OutputWAVFileAddr;        // 'WAVE' file output addr
    unsigned long OutputWAVFileEnable;    // out wav-file mode 
    //void     (*DecodeOneFrame_func)();
    //void     (*DecodeAllFrame_func)();
    unsigned long (*SkipOneFrame_func)();
} PlayMP3Info_t;

typedef struct _MemBlkInfo_t{
    int    align;
    int size;
    void *ptr;
} MemBlkInfo_t;
void     AUDEC_DecodeInit(void);
void     AUDEC_DecodeEnd(void);
AudioDecoderInfo_t     *AUDEC_CreateObject(unsigned codecID, PlayMP3Info_t *pInfoPlayMP3);
unsigned long     AUDEC_GetTotalFrames(void);
unsigned long     AUDEC_GetCurDecodedFrames(void);
unsigned long     AUDEC_GetNextFrameAddr(void);
unsigned long     AUDEC_GetDataLengthPerFrame(void);
unsigned long     AUDEC_GetTotalSamples(void);
unsigned long     AUDEC_GetCurParseFrames(void);
unsigned long     AUDEC_GetSampleRate(void);
unsigned long    AUDEC_GetFramesPerSecond(void);
unsigned long     AUDEC_GetBitRate(void);
unsigned long     AUDEC_GetChannelMode(void);
unsigned long     AUDEC_GetFrameOffsetInFile(void);


short     *AUDEC_GetOutputBufferAddr(void);
AUDECFlow_t     AUDEC_DecodePoll(void);
AUDECFlow_t     AUDEC_SkipOneFrame(void);

void     AUDEC_SetTotalFrames(unsigned long);
void     AUDEC_SetCurDecodedFrames(unsigned long);
void     AUDEC_SetNextFrameAddr(unsigned long addr);
void     AUDEC_SetOutputBufferAddr(short *ptr);
void     AUDEC_SetTotalSamples(unsigned long samples);
void     AUDEC_SetCurParseFrames(unsigned long frames);
void     AUDEC_SetFrameOffsetInFile(unsigned long offset);
void     AUDEC_SetOutputSize(unsigned long size);


#endif //AUDEC_APIL_H