#ifndef MP3D_MAIN_H
#define MP3D_MAIN_H
#include "mp3d_symbol.h"
#include "mp3d_decoder.h"

#define OUT_REAL_TIME        0
#define OUT_DUMP_FILE        0
#define OUT_WAV_HEADER        0
#define TEST_LOOPBACK        0

#if ((!OUT_REAL_TIME) || (OUT_DUMP_FILE))
    #define    OUT_STRUCT_OUTBUF    0
#else // ((!OUT_REAL_TIME) || (OUT_DUMP_FILE))
    #define    OUT_STRUCT_OUTBUF    0
#endif // ((!OUT_REAL_TIME) || (OUT_DUMP_FILE))


typedef struct _mp3_buffer {
    unsigned char const *bstart;
    unsigned long length;
    unsigned int  filesize;    //2007/09/27Meg Lin, add for lost_sync more than 5 times or decode end

} mp3_buffer;

typedef struct al_variables
{
    MP3DObject_t     decoder;
    //ai_object_t     *codec;
    unsigned int    start;
} al_variables_t;





typedef struct {
    UINT32    groupID;            // 'RIFF'
    UINT32    totalSize;            // audio size + 44 - 8
    UINT32    riffType;            // 'WAVE'
    UINT32    fmtChunkID;            // 'fmt '
    UINT32    fmtChunkSize;        // 16
    INT16    hwFormatTag;
    UINT16    hwChannels;
    UINT32    wSamplesPerSec;
    UINT32    wAvgBytesPerSec;
    UINT16    hwBlockAlign;
    UINT16    hwBitsPerSample;
    UINT32    datChunkID;
    UINT32    datChunkSize;
} test_wav_header_t;


#define GROUP_ID                0x46464952    //'RIFF'
#define RIFF_TYPE                0x45564157  //'WAVE'
#define FORMAT_ID                0x20746D66  //'fmt '
#define DATA_ID                    0x61746164  //'data'
#define FORMAT_SIZE                16


//int DecodeMP3(UINT8 const *start, UINT32 length);
AUDECFlow_t input(void *pdecoder, void *data);
AUDECFlow_t output(void *, void *data);
AUDECFlow_t filter(void *cb_data, void *);
AUDECFlow_t error(void *data, void *);
AUDECFlow_t header(void *pdecoder,void *pheader);

void al_output_finish(void);


#endif    // MP3D_MAIN_H