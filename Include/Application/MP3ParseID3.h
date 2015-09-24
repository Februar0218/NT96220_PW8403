#ifndef _MP3PARSEID3_H
#define _MP3PARSEID3_H
//-------------------------------------------------------
#define PARSEID3TAGNUMS      5

#define ID3TAGTIT2      0
#define ID3TAGTALB      1
#define ID3TAGTPE1      2
#define ID3TAGTYER      3
#define ID3TAGTCON      4

#define MP3_ID3V1_NO    0
#define MP3_ID3V1_TITLE    1
#define MP3_ID3V1_ARTIS    2
#define MP3_ID3V1_ALBUM 3
#define MP3_ID3V1_YEAR  4
#define MP3_ID3V1_COMM  5
#define MP3_ID3V1_TRACK 6
#define MP3_ID3V1_GENRE 7

    
typedef struct{
    UINT32  TagLength;
    UINT8   *pTagAddr;
} MP3ID3TAG_INFO, *PMP3ID3TAG_INFO;    

typedef struct{
    UINT8   TagOffset;
    UINT8   TagLength;
} MP3ID3TAG01_INFO, *PMP3ID3TAG01_INFO;    

/* Ver 1.0 Header
0   3   "TAG" identifier string. 
3   30  Song title string. 
33  30  Artist string. 
63  30  Album string. 
93  4   Year string. 
97  28  Comment string. 
125 1   Zero byte separator. 
126 1   Track byte. 
127 1   Genre byte. 
*/

UINT32 MP3ParseID3_Ver_01(CHAR *pSrcAddr);
UINT32 MP3ParseID3_Ver_02(CHAR *pSrcAddr);
UINT16 MP3ParseID3_Ver_0101(UINT32 fileidx, char *pDataAddr, char *pUserful, UINT8 mode);
UINT16 MP3ParseID3_GetInfo(UINT32 fileidx, char *pDataAddr, char *pUserful, UINT8 mode);

extern MP3ID3TAG_INFO     gMP3ID3TagInfo[]; 
//-------------------------------------------------------
#endif // _MP3PARSEID3_H