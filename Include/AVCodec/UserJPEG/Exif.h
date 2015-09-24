/**
    Copyright   Novatek Microelectronics Corp. 2004.  All rights reserved.

    @file       Exif.h
    @ingroup    mIAVJPEG

    @brief      JPEG EXIF(DCF2.2) header file.

*/

#ifndef _EXIF_H
#define _EXIF_H

#include "Type.h"

/** \addtogroup mIAVJPEG
*/
//@{

//#define _Support_QV5_AVI_   1           //If you want to support AVI format for Apple QuicklyView 5.0, set this 1

#define    HiWord(data)     (((int)data) & 0xffff0000)>>16              //Bug inside, (USHORT)HiWord(-20) is not 0xFFFF
#define    LoWord(data)     (((int)data) & 0x0000ffff)
#define    HiByte(data)     (((int)data) & 0x0000ff00) >> 8
#define    LoByte(data)     (((int)data) & 0x000000ff)

#define    EXIF_DATE_STR    0x01
#define    DPOF_DATE_STR    0x02

//----- JPEG Marker (inverse) -----------
#define JPEGMarkerSOI    0xD8FF
#define JPEGMarkerAPP1   0xE1FF
#define JPEGMarkerAPP2   0xE2FF
#define JPEGMarkerAPP3   0xE3FF

#define JPEGMarkerDHT    0xC4FF
#define JPEGMarkerDQT    0xDBFF
#define JPEGMarkerSOF    0xC0FF
#define JPEGMarkerSOS    0xDAFF
#define JPEGMarkerCOM    0xFEFF
#define JPEGMarkerEOI    0xD9FF

#define ByteOrderLittleEndian    0x4949
#define ByteOrderBigEndian       0x4D4D
#define TIFFIdentify             42
//-----------------------------------------
//      TIFF IFD Type
//-----------------------------------------
#define TypeBYTE                 1
#define TypeASCII                2
#define TypeSHORT                3
#define TypeLONG                 4
#define TypeRATIONAL             5
#define TypeUNDEFINED            7
#define TypeSLONG                9
#define TypeSRATIONAL            10

//-----------------------------------------
//      TIFF IFD Tag Numbers
//-----------------------------------------
//#NT#2009/03/18#Photon Lin -begin
//#GPS feature
#define _Support_GPSInfo_Tag    0
//#NT#2009/03/18#Photon Lin -end
//#NT#2011/06/01#Ben Wang -begin
//#NT#Don't use "#ifdef"
#define _Support_Brightness_Tag    0
//#NT#2011/06/01#Ben Wang -end

//#NT#2009/03/18#Photon Lin -begin
//#Do not use #ifdef
#if _Support_GPSInfo_Tag
//#NT#2009/03/18#Photon Lin -end
    #define TagNumsOf0thIFD          12                     //Add GPS IFD pointer.
#else
    #define TagNumsOf0thIFD          11
#endif

#if _Support_Brightness_Tag

    #define TagNumsOfExifIFD         35

#else

    #define TagNumsOfExifIFD         34

#endif

#define TagNumsOfGPSIFD          5                      //Change this value if you suppport more.

#define TagNumsOfInterOpIFD      2
#define TagNumsOf1thIFD          6

//-----------------------------------------
//      TIFF IFD Tag ID
//-----------------------------------------
//----- 0th IFD -----------------------
#define TagImageDescription       0x010E  //ASCII
#define TagMake                   0x010F  //ASCII
#define TagModel                  0x0110  //ASCII
#define TagOrientation            0x0112  //SHORT
#define TagXResolution            0x011A  //RATIONAL
#define TagYResolution            0x011B  //RATIONAL
#define TagResolutionUnit         0x0128  //SHORT
#define TagSoftVer                0x0131  //ASCII
#define TagDataTime               0x0132  //ASCII
#define TagYCbCrPositioning       0x0213  //SHORT
//#NT#2009/06/03#Scottie -begin
//#NT#Add PrintIM tag
#define TagPrintIM                0xC4A5
//#NT#2009/06/03#Scottie -end

//#NT#2008/03/28#Jades Shih - Begin
// Add complete TIFF tags
#define TagImageWidth                  0x0100
#define TagImageLength                 0x0101
#define TagBitsPerSample               0x0102
#define TagPhotometricInterpretation   0x0106
#define TagStripOffsets                0x0111
#define TagSamplesPerPixel             0x0115
#define TagRowsPerStrip                0x0116
#define TagStripByteCounts             0x0117
#define TagPlanarConfiguration         0x011C
#define TagTransferFunction            0x012D
#define TagSoftware                    0x0131
#define TagArtist                      0x013B
#define TagWhitePoint                  0x013E
#define TagPrimaryChromaticities       0x013F
#define TagYCbCrCoefficients           0x0211
#define TagYCbCrSubSampling            0x0212
#define TagReferenceBlackWhite         0x0214
#define TagCopyright                   0x8298
//#NT#2008/03/28#Jades Shih - End

#define TagGPSIFDPtr              0x8825
#define TagExifIFDPtr             0x8769

//----- Exif IFD -----------------------
// If you want to to add/delete tags into ExifSubIFD,
// You should also modify TagNumsOfExifIFD value.
#define TagExposureTime           0x829A  //RATIONAL
#define TagFNumber                0x829D  //RATIONAL
#define TagExposureProgram        0x8822  //SHORT
#define TagISOSpeedRatings        0x8827  //SHORT
#define TagExifVersion            0x9000  //UNDEFINED
#define TagDateTimeOriginal       0x9003  //ASCII
#define TagDateTimeDigitized      0x9004  //ASCII
#define TagComponentsCfg          0x9101  //UNDEFINED
#define TagShutterSpeed           0x9201  //SRATIONAL
#define TagAperture               0x9202  //RATIONAL
#define TagBrightness             0x9203  //SRATIONAL
#define TagExposureBias           0x9204  //SRATIONAL
#define TagMaxAperture            0x9205  //RATIONAL
#define TagMeteringMode           0x9207  //SHORT
#define TagLightSource            0x9208  //SHORT
#define TagStrobeFlash            0x9209  //SHORT
#define TagFocalLength            0x920A  //RATIONAL
#define TagMakerNote              0x927C  //UNDEFINED
#define TagUserComment            0x9286  //UNDEFINED
#define TagFlashPixVersion        0xA000  //UNDEFINED
#define TagColorspace             0xA001  //SHORT
#define TagPixelXDimension        0xA002  //LONG
#define TagPixelYDimension        0xA003  //LONG
#define TagInterOperateIFDPtr     0xA005
#define TagSensingMethod          0xA217  //SHORT
#define TagFileSource             0xA300  //UNDEFINED
#define TagCustomRendered         0xA401  //SHORT
#define TagExposureMode           0xA402  //SHORT
#define TagWhiteBalance           0xA403  //SHORT
#define TagSceneCaptureType       0xA406  //SHORT
#define TagDigitalZoomRatio       0xA404
#define TagGainControl            0xA407
#define TagContrast               0xA408
#define TagSaturation             0xA409
#define TagSharpness              0xA40A  //SHORT
#define TagSpectralSensitivity      0x8824
#define TagOECF                     0x8828
#define TagCompressedBitsPerPixel   0x9102
#define TagSubjectDistance          0x9206
#define TagSubjectArea              0x9214
#define TagSubSecTime               0x9290
#define TagSubSecTimeOriginal       0x9291
#define TagSubSecTimeDigitized      0x9292
#define TagRelatedSoundFile         0xA004
#define TagFlashEnergy              0xA20B
#define TagSpatialFrequencyResponse 0xA20C
#define TagFocalPlaneXResolution    0xA20E
#define TagFocalPlaneYResolution    0xA20F
#define TagFocalPlaneResolutionUnit 0xA210
#define TagSubjectLocation          0xA214
#define TagExposureIndex            0xA215
#define TagSensingMethod            0xA217
#define TagSceneType                0xA301
#define TagCFAPattern               0xA302
#define TagFocalLengthIn35mmFilm    0xA405
#define TagSceneCaptureType         0xA406
#define TagDeviceSettingDescription 0xA40B
#define TagSubjectDistanceRange     0xA40C
#define TagImageUniqueID            0xA420


//----- GPS IFD  -----
// Until 2006/12, We still do NOT have GPS module. This information is defined for future used.
// If you required more information, please refer to EXIF Spec.
#define TagGPSVersionID           0x0000  //BYTE
#define TagGPSLatitudeRef         0x0001  //ASCII
#define TagGPSLatitude            0x0002  //RATIONAL
#define TagGPSLongitudeRef        0x0003  //ASCII
#define TagGPSLongitude           0x0004  //RATIONAL
#define TagGPSAltitudeRef         0x0005  //BYTE
#define TagGPSAltitude            0x0006  //RATIONAL
#define TagGPSTimeStamp           0x0007  //RATIONAL
#define TagGPSSatellites          0x0008
#define TagGPSStatus              0x0009
#define TagGPSMeasureMode         0x000A
#define TagGPSDOP                 0x000B
#define TagGPSSpeedRef            0x000C
#define TagGPSSpeed               0x000D
#define TagGPSTrackRef            0x000E
#define TagGPSTrack               0x000F
#define TagGPSImgDirectionRef     0x0010
#define TagGPSImgDirection        0x0011
#define TagGPSMapDatum            0x0012
#define TagGPSDestLatitudeRef     0x0013
#define TagGPSDestLatitude        0x0014
#define TagGPSDestLongitudeRef    0x0015
#define TagGPSDestLongitude       0x0016
#define TagGPSDestBearingRef      0x0017
#define TagGPSDestBearing         0x0018
#define TagGPSDestDistanceRef     0x0019
#define TagGPSDestDistance        0x001A
#define TagGPSProcessingMethod    0x001B
#define TagGPSAreaInformation     0x001C
#define TagGPSDateStamp           0x001D  //ASCII
#define TagGPSDifferential        0x001E

//----- InterOperability IFD ----------
#define TagInterOpIndex            0x0001
#define TagExifR98Ver              0x0002
//----- 1th IFD -------------------------------
#define TagCompression                     0x0103
#define TagJPEGInterchangeFormat           0x0201
#define TagJPEGInterchangeFormatLength     0x0202


//-----------------------------------------
//      TIFF IFD Tag Length
//-----------------------------------------
//----- 0th IFD -----------------------
#define TagImgDescLen             16
#define TagMakeLen                24
#define TagModelLen               16
#define TagOrientLen              1
#define TagXResolLen              1
#define TagYResolLen              1
#define TagResolUnitLen           1
#define TagSoftVerLen             32
#define TagDataTimeLen            20
#define TagYCbCrPosLen            1
#define TagGPSPtrLen              1
#define TagExifPtrLen             1
//----- Exif IFD -----------------------
#define TagExpTimeLen             1
#define TagFNumberLen             1
#define TagExpProgLen             1
#define TagISOSpeedLen            1
#define TagExifVerLen             4
#define TagDatTimOriLen           20
#define TagDatTimDigLen           20
#define TagCompCfgLen             4
#define TagShutterLen             1
#define TagApertureLen            1
#define TagBrightnessLen          1
#define TagExpoBiasLen            1
#define TagMaxApertureLen         1
#define TagMeteringLen            1
#define TagLightSrcLen            1
#define TagStrobeLen              1
#define TagFocalLenLen            1

// Until 2006/12, We still do NOT have GPS module. This information is defined for future used.
#define TagGPSVersionLen          4
#define TagGPSLatitudeRefLen      2
#define TagGPSLatitudeLen         3
#define TagGPSLongitudeRefLen     TagGPSLatitudeRefLen
#define TagGPSLongitudeLen        TagGPSLatitudeLen
//#NT#2011/06/08#Meg Lin -begin
#define TagGPSAltitudeRefLen      1
#define TagGPSAltitudeLen         1
#define TagGPSTimeStampLen        3
#define TagGPSDateStampLen        11

//#Add User Comment tag
#define TagUserCommentLen         32
#define TagUserCommentSNIDLen     7     // length of Social Network identifier in User Comment (7 bytes)
//#NT#2011/06/08#Meg Lin -end

///////////////////////////////////////////////////////////////////////////
//we reserve 128 bytes for normal data (only 64 bytes in real),
//4096 for debug message.
#define MakeNoteLen_DEBUGBUFSIZE  1024
#define MakeNoteLen_Real          63
#define EXIFW_MAKENOTELEN_REAL    64//2011/07/12 Meg Lin adds
//#NT#2009/03/18#Photon Lin -begin
//#Do not use #ifdef
#if _Support_GPSInfo_Tag
//#NT#2009/03/18#Photon Lin -end
    //Since GPS IFD is 4n+2... To make the whole EXIF is 4n, we use 62 byte for Uninit area.
    #define MakeNoteLen_UnInit        62
#else
    #define MakeNoteLen_UnInit        64
#endif

#define TagMakeNoteLen            MakeNoteLen_Real+MakeNoteLen_UnInit+MakeNoteLen_DEBUGBUFSIZE

#define TagFlashVerLen            4
#define TagColorSpcLen            1
#define TagPXDimenLen             1
#define TagPYDimenLen             1
#define TagOpIFDPtrLen            1
#define TagSensingLen             1
#define TagFileSrcLen             1
#define TagCMRenderLen            1
#define TagExpModeLen             1
#define TagWhiteBNLen             1
#define TagSNECaptLen             1
//#NT#2011/06/01#Ben Wang -begin
//#NT#modified length to make sizeof(EXIF_HEAD_SIZE+EXIF_HEAD_AFTERMAKERNOTE+(JPGHeaderSize-1)) word-alignment
#define TagUserCmtLen             43//125
//#NT#2011/06/01#Ben Wang -end
#define TagDZRatioLen             1
#define TagGainCtlLen             1
#define TagContrastLen            1
#define TagSaturateLen            1
#define TagSharpLen               1

//----- MakerNote IFD ------------------
#define TagBodyFWVer            0x0104
#define TagSpecialFunc          0x0200
#define TagMacroMode            0x0202
#define TagColorMode            0x0203
#define TagDigitalZoom          0x0204
#define TagAngleOfView          0x0205
#define TagAberration           0x0206
#define TagCameraType           0x0207
#define TagCameraID             0x0209
#define TagEditType             0x020A
#define TagJPEGCSRate           0x020B
#define TagAFLockData           0x0401
#define TagAFMode               0x0402
#define TagScenePrg             0x0403
#define TagWBMode               0x0500
#define TagCameraMode           0x2020
//#NT#2008/10/30#Scottie -begin
//#Modify for the modificaion of MakerNote v3.0 spec
#define TagHH_Local             0x5000//0x4010
//#NT#2008/10/30#Scottie -end

#define TagNumsOfMakerNoteIFD   16

//----- CameraMode IFD -----------------
#define TagCMVersion            0x0000
#define TagPlayPicFormat        0x0100
#define TagPlayPicOffset        0x0101
#define TagPlayPicLength        0x0102
//#NT#2009/04/14#Scottie -begin
//#NT#Add for OLY 2009Autumn project
#define TagJPEGCSRate_CM        0x050D

//#define TagNumsOfCameraModeIFD  4
#define TagNumsOfCameraModeIFD  5
//#NT#2009/04/14#Scottie -end

//#NT#2008/09/19#Scottie -begin
//#Add for MakerNote v3.0
//----- HH_Local IFD -------------------
//#NT#2009/04/14#Scottie -begin
//#NT#Modify for OLY 2009Autumn project
#if 0
#define TagColorMode_HH         0x0200
#define TagDigitalZoom_HH       0x0201
#define TagEditType_HH          0x0300
#define TagAFLockData_HH        0x0400
#define TagAFMode_HH            0x0401
#define TagJPEGCSRate_HH        0x0500
#endif
//#define TagColorMode_HH         0xF200
//#define TagDigitalZoom_HH       0xF201
#define TagEditType_HH          0xF300
#define TagAFLockData_HH        0xF400
#define TagAFMode_HH            0xF401
//#define TagJPEGCSRate_HH        0xF500
//#NT#2009/04/14#Scottie -end
//#NT#2008/09/19#Scottie -end

//----- InterOperability IFD ----------
#define TagInterOpIndexLen        4
#define TagExifR98VerLen          4
//----- 1th IFD -----------------------
#define TagCompressionLen         1
#define TagJPEGInterLen           1
#define TagJPEGLengthLen          1

//-----------------------------------------
// Exif file header data structures
//-----------------------------------------
typedef struct {
    USHORT    Tag;
    USHORT    Type;
    USHORT    Count_LoWord;
    USHORT    Count_HiWord;
    USHORT    Offset_LoWord;
    USHORT    Offset_HiWord;
} FIELD;

typedef struct {
    USHORT    NumeratorLoWord;
    USHORT    NumeratorHiWord;
    USHORT    DenominatorLoWord;
    USHORT    DenominatorHiWord;
} RATIONAL;

typedef struct {
    SHORT    SNumeratorLoWord;
    SHORT    SNumeratorHiWord;
    SHORT    SDenominatorLoWord;
    SHORT    SDenominatorHiWord;
} SRATIONAL;

typedef struct {
    USHORT    IFDLoWord;
    USHORT    IFDHiWord;
} NextIFD;

typedef struct {
    UCHAR     year[4];
    UCHAR     colon1;        // :
    UCHAR     month[2];
    UCHAR     colon2;        // :
    UCHAR     day[2];
    UCHAR     space;
    UCHAR     hour[2];
    UCHAR     colon3;        // :
    UCHAR     minute[2];
    UCHAR     colon4;        // :
    UCHAR     second[2];
    UCHAR     endmark;
} RTCStruct;



typedef struct {
    USHORT   TagNumbers;
    FIELD    ImageDescription;
    FIELD    Maker;
    FIELD    Model;
    FIELD    Orientation;
    FIELD    XResolution;
    FIELD    YResolution;
    FIELD    ResolutionUnit;
    FIELD    SoftVersion;
    FIELD    DateTime;
    FIELD    YCbCrPositioning;
    FIELD    ExifIFDPtr;

    // Until 2006/12, We still do NOT have GPS module. This information is defined for future used.
//#NT#2009/03/18#Photon Lin -begin
//#Do not use #ifdef
#if _Support_GPSInfo_Tag
//#NT#2009/03/18#Photon Lin -end
    FIELD    GPSIFDPtr;
#endif

    NextIFD  NextIFDOffset;
} IFD_0TH;

typedef struct {
    USHORT   TagNumbers;
    FIELD    ExposureTime;             //unsigned rational
    FIELD    FNumber;                  //unsigned rational
    FIELD    ExposureProgram;          //unsigned short
    FIELD    ISOSpeedRatings;          //unsigned short
    FIELD    ExifVersion;              //undefined 4 units: 48 50 50 48
    FIELD    DateTimeOri;              //ascii string
    FIELD    DateTimeDig;              //ascii string
    FIELD    ComponentCfg;             //undefined 4 units: 01 02 03 00
    FIELD    ShutterSpeed;             //signed rational
    FIELD    Aperture;                 //unsigned rational
    #if _Support_Brightness_Tag
    FIELD    Brightness;               //signed ration.
    #endif
    FIELD    ExposureBias;             //signed rational
    FIELD    MaxAperture;              //unsigned rational
    FIELD    MeteringMode;             //unsigned short
    FIELD    LightSource;              //unsigned short
    FIELD    StrobeFlash;              //unsigned short
    FIELD    FocalLength;              //unsigned rational
    FIELD    DigitalZoomRatio;         //unsigned rational
    FIELD    UserComment;              //undefined
    FIELD    MakerNote;                //undefined
    FIELD    FlashPixVersion;          //undefined: 48 49 48 48
    FIELD    ColorSpace;               //unsigned short
    FIELD    PixelXDimemsion;          //unsigned short
    FIELD    PixelYDimemsion;          //unsigned short
    FIELD    InterOperateIFDPtr;       //unsigned long
    FIELD    SensingMethod;            //unsigned short
    FIELD    FileSource;               //undefined. 3 (DSC)
    FIELD    CustomRenderedMode;       //unsigned short
    FIELD    ExposureMode;             //unsigned short
    FIELD    WhiteBalance;             //unsigned short
    FIELD    SceneCaptureType;         //unsigned short
    FIELD    GainControl;              //unsigned short
    FIELD    Contrast;                 //unsigned short
    FIELD    Saturation;               //unsigned short
    FIELD    Sharpness;                //unsigned short
    NextIFD  NextIFDOffset;
} IFD_EXIF;

typedef struct {
    USHORT   TagNumbers;
    FIELD    InterOpIndex;
    FIELD    ExifR98Ver;
    NextIFD  NextIFDOffset;
} IFD_INTEROP;

typedef struct {
    USHORT   TagNumbers;
    FIELD    Compression;
    FIELD    XResolution;
    FIELD    YResolution;
    FIELD    ResolutionUnit;
    FIELD    JPEGInterchangeFormat;
    FIELD    JPEGInterchangeFormatLength;
    NextIFD  NextIFDOffset;
} IFD_1TH;

/**
    IFD0 for Primary image, and IFD1 for thumbnail image.
    The size of EXIF_HEAD is 0x34C (if TagMakeNoteLen is 128).
*/
typedef struct {
    USHORT     SOI;
    USHORT     AppMarker1;
    UCHAR      AppMarker1SizeH;
    UCHAR      AppMarker1SizeL;
    CHAR       ExifID[6];
    // TIFF header
    USHORT     TiffOrder;
    USHORT     TiffID;
    NextIFD    NextIFD0th;
    // 0th IFD
    IFD_0TH    IFD0th;
    // Value longer than 4bytes of 0th IFD
    CHAR       ImageDescriptionString[TagImgDescLen];
    CHAR       MakerString[TagMakeLen];
    CHAR       ModelString[TagModelLen];
    CHAR       SoftwareVer[TagSoftVerLen];
    RTCStruct  DateTime;
    RATIONAL   XResolValue;
    RATIONAL   YResolValue;
    // Exif IFD
    IFD_EXIF   IFDexif;
    // Value longer than 4bytes of exif IFD
    RATIONAL   ExposureTimeValue;
    RATIONAL   FNumberValue;

    RTCStruct  OriDateTime;
    RTCStruct  DigDateTime;

    SRATIONAL  ShutterSpeed;
    RATIONAL   ApertureValue;

    #if _Support_Brightness_Tag
    SRATIONAL  BrightnessValue;
    #endif

    SRATIONAL  ExposureBias;
    RATIONAL   MaxApertureValue;
    RATIONAL   FocalLengthValue;
    RATIONAL   DigitalZoomRatioValue;
    CHAR       UserCommentString[TagUserCmtLen];
    //*****************************************************************************************************
    // Since Debug message will be put into TagMakeNoteLen, It is large size, however, we can't
    // allocate too many space in header. We should remember the difference in physical and logical.
    //*****************************************************************************************************
    //CHAR       MakeNoteString[TagMakeNoteLen];
    CHAR       MakeNoteString[MakeNoteLen_Real];
} EXIF_HEAD, *PEXIF_HEAD;

//#NT#2009/03/18#Photon Lin -begin
//#Do not use #ifdef
#if _Support_GPSInfo_Tag
//#NT#2009/03/18#Photon Lin -end
/**
    GPS tag information.
*/
typedef struct {

    USHORT   TagNumbers;
    FIELD    GPSVersion;
    FIELD    GPSLatitudeRef;
    FIELD    GPSLatitude;
    FIELD    GPSLongitudeRef;
    FIELD    GPSLongitude;
    NextIFD  NextIFDOffset;

}GPS_IFD, *PGPS_IFD;
#endif


/**
    EXIF header information.

    @note
        In order to add the second thumbnail image inside MakeNoteString, we should separate
        the original EXIF HEAD structure. The data member in structure EXIF_HEAD which is after
        MakeNoteString should be moved to this new structure. Since we will insert an "UNFIXED"
        JPG thumbnail image inside MakeNoteString.
        Why do we insert an "UNFIXED" JPG thumbnail image? If we allocate too small space, it can't save
        the data. If we allocate too large space, it will waste Memory card space.
*/
typedef struct {

    //#NT#2009/03/18#Photon Lin -begin
    //#Do not use #ifdef
    #if _Support_GPSInfo_Tag
    //#NT#2009/03/18#Photon Lin -end
    GPS_IFD        GPSIFD;

    RATIONAL       LatitudeDegVal;
    RATIONAL       LatitudeMinVal;
    RATIONAL       LatitudeSecVal;
    RATIONAL       LongitudeDegVal;
    RATIONAL       LongitudeMinVal;
    RATIONAL       LongitudeSecVal;
    #endif


    // InterOperability IFD
    IFD_INTEROP    IFDInterOperability;
    // 1th IFD
    IFD_1TH        IFD1th;
    // Value longer than 4bytes of 1th IFD
    RATIONAL       XResolValue1th;
    RATIONAL       YResolValue1th;
    // (SOI) Thumbnail
} EXIF_HEAD_AFTERMAKERNOTE, *PEXIF_HEAD_AFTERMAKERNOTE;

//----- Exposure Program ---------------------------
#define ExpPrgNoDef            0
#define ExpPrgManual           1
#define ExpPrgNormal           2
#define ExpPrgAperture         3    // Aperture Priority
#define ExpPrgShutter          4    // Shutter Priority
#define ExpPrgCreative         5
#define ExpPrgAction           6
#define ExpPrgPortrait         7
#define ExpPrgLandscape        8
//----- ISO Speed -----------------------------------
#define ISOSpeed50             50
#define ISOSpeed100            100
#define ISOSpeed200            200
#define ISOSpeed400            400
//----- Metering Mode -------------------------------
#define MeterUnknown           0
#define MeterAverage           1
#define MeterCenterWeightedAvg 2
#define MeterSpot              3
#define MeterMultiSpot         4
#define MeterPattern           5
#define MeterPartial           6
//----- Light Source -------------------------------
//#NT#2011/06/14#Hideo Lin -begin
//#NT#Modify EXIF tag value according to EXIF 2.2 spec.
#define LightUnknown                0
#define LightDaylight               1
#define LightFluorescent            2
#define LightTungsten               3
#define LightFlash                  4
#define LightFineWeather            9
#define LightCloudy                 10
#define LightShade                  11
#define LightStandardA              17
#define LightStandardB              18
#define LightStandardC              19
//----- Strobe Flash -------------------------------
#define StrobeNoFired               0
#define StrobeFired                 1
#define StrobeNoDetectFunction      (0 << 1) // bit[2:1] = 0
#define StrobeReturnNotDetected     (2 << 1) // bit[2:1] = 2
#define StrobeReturnDetected        (3 << 1) // bit[2:1] = 3
#define StrobeModeUnknown           (0 << 3) // bit[3:4] = 0
#define StrobeForceOn               (1 << 3) // bit[3:4] = 1
#define StrobeForceOff              (2 << 3) // bit[3:4] = 2
#define StrobeModeAuto              (3 << 3) // bit[3:4] = 3
#define StrobeFunctionPresent       (0 << 5) // bit[5] = 0
#define StrobeNoFunction            (1 << 5) // bit[5] = 1
#define StrobeRedEyeOff             (0 << 6) // bit[6] = 0
#define StrobeRedEyeOn              (1 << 6) // bit[6] = 1
#define StrobeFlashAuto             ((1<<4)+(1<<3))
#define StrobeFlashForceOFF         ((1<<4))
#define StrobeFlashForceON          ((1<<3))
#define StrobeFlashAutoRedEye       ((1<<6)+(1<<4)+(1<<3))
#define StrobeFlashForceONRedEye    ((1<<6)+(1<<3))
#define StrobeFlashNightStrobe      ((1<<6)+(1<<4)+(1<<3))
//#NT#2011/06/14#Hideo Lin -end
//----- Color Space --------------------------------
#define ColorSpaceSRGB         1
//----- Sensing Mode -------------------------------
#define SensingNoDef           1
#define SensingOneChip         2    // OneChipColorArea sensor
#define SensingTwoChip         3
#define SensingThreeChip       4
#define SensingColorSeqArea    5
#define SensingTriLinear       7
#define SensingColorSeqLinear  8
//----- CustomRendered ----------------------------
#define NormalRendered         0
#define CustomRendered         1
//-----Exposure Mode-------------------------------
#define AutoExposure           0
#define ManualExposure         1
#define AutoBracket            2
//-----White Balance ------------------------------
#define AutoWhite              0
#define ManualWhite            1

//-----SceneCapture Type --------------------------
#define StandardScene          0
#define LandScapeScene         1
#define PortraitScene          2
#define NightScene             3

//-----Gain Control--------------------------------
#define NONE_GAIN              0
#define LOW_GAIN_UP            1
#define HIGH_GAIN_UP           2
#define LOW_GAIN_DOWN          3
#define HIGH_GAIN_DOWN         4

//-----Contrast------------------------------------
#define NORMAL_CONTRAST        0
#define SOFT_CONTRAST          1
#define HARD_CONTRAST          2

//-----Saturation----------------------------------
#define NORMAL_SATURATION      0
#define LOW_SATURATION         1
#define HIGH_SATURATION        2

//-----Sharpness-----------------------------------
#define NORMAL_SHARPNESS       0
#define SOFT_SHARPNESS         1
#define HARD_SHARPNESS         2

//----- Compression -------------------------------
#define UnCompressed           1
#define JPEGCompression        6

//--------------------------------------------------------------
//#define Commentdatalength        17
//#NT#2011/06/08#Meg Lin -begin
//----- GPS IFD Value --------------------------------------------
#define TagGPSVersionVal        0x00000202                  //2.2.0.0
#define GPSLatitudeRefVal       0x0000004E                  //Default "N"
#define GPSLongitudeRefVal      0x00000045                  //Default "E"
#define GPSAltitudeRefVal       0x00000000                  //Default 0, 0->Above sea level, 1->Below sea level
//#NT#2011/06/08#Meg Lin -end

typedef __packed struct {
    UCHAR    TblIndex;
    UCHAR    symbol_nums[16];
    UCHAR    symbol_data[12];
} HuffmanTblDC;

typedef __packed struct {
    UCHAR    TblIndex;
    UCHAR    symbol_nums[16];
    UCHAR    symbol_data[162];
} HuffmanTblAC;

typedef __packed struct {
    UCHAR    TblIndex;
    UCHAR    q_data[64];
} QuantTbl;

typedef __packed struct {
    UCHAR    ComponentID;
    UCHAR    SampleHVRate;        // Horizontal & Vertical rate
    UCHAR    ComponentQTblIndex;    // Q-table index
} SOF_COMPINFO;

typedef __packed struct {
    UCHAR    ComponentID;
    UCHAR    ComponentHufTblIndex;    // AC & DC Huffman table index
} SOS_COMPINFO;

typedef __packed struct {
    UCHAR           TagID[2];
    UCHAR           TagLength[2];
    HuffmanTblDC    HuffDC_0th;        // 00->0th DC Huffman table (Y)
    HuffmanTblAC    HuffAC_0th;        // 10->0th AC Huffman table (Y)
    HuffmanTblDC    HuffDC_1th;        // 01->1th DC Huffman table (UV)
    HuffmanTblAC    HuffAC_1th;        // 11->1th AC Huffman table (UV)
} Tag_DHT;    // Huffman tables

typedef __packed struct {
    UCHAR           TagID[2];
    UCHAR           TagLength[2];
    QuantTbl        Qtbl_0th;        // 00: 0th Q-table
    QuantTbl        Qtbl_1th;        // 01: 1th Q-table
} Tag_DQT;    // Quantization tables

typedef __packed struct {
    UCHAR           TagID[2];
    UCHAR           TagLength[2];
    UCHAR           BitsPerPixel;
    UCHAR           ImgHeight[2];
    UCHAR           ImgWidth[2];
    UCHAR           ComponentNums;
    SOF_COMPINFO    Component1;
    SOF_COMPINFO    Component2;
    SOF_COMPINFO    Component3;
} Tag_SOF;    // Start of Frame

typedef __packed struct {
    UCHAR           TagID[2];
    UCHAR           TagLength[2];
    UCHAR           ComponentNums;
    SOS_COMPINFO    Component1;
    SOS_COMPINFO    Component2;
    SOS_COMPINFO    Component3;
    UCHAR           OtherData[3];
} Tag_SOS;    // Start of Scan

//#Add for supporting restart interval header and JPEG gray format header
typedef __packed struct {
    UCHAR           TagID[2];
    UCHAR           TagLength[2];
    UCHAR           RestartNums[2];
} Tag_DRI;    // Distance of restart interval

typedef __packed struct {
    USHORT        SOI;
    Tag_DQT       MarkDQT;
    Tag_SOF       MarkSOF;
    Tag_DHT       MarkDHT;
    Tag_DRI       MarkDRI;
    Tag_SOS       MarkSOS;
} JPG_HEADER, *PJPG_HEADER;

//#NT#2008/11/11#Daniel Lu - begin
typedef __packed struct {
    UCHAR           TagID[2];
    UCHAR           TagLength[2];
    UCHAR           BitsPerPixel;
    UCHAR           ImgHeight[2];
    UCHAR           ImgWidth[2];
    UCHAR           ComponentNums;
    SOF_COMPINFO    Component1;
} Tag_SOF_GRAY;    // Start of Frame for gray format

typedef __packed struct {
    UCHAR           TagID[2];
    UCHAR           TagLength[2];
    UCHAR           ComponentNums;
    SOS_COMPINFO    Component1;
    UCHAR           OtherData[3];
} Tag_SOS_GRAY;    // Start of Scan for gray format

typedef __packed struct {
    USHORT        SOI;
    Tag_DQT       MarkDQT;
    Tag_SOF       MarkSOFGray;
    Tag_DHT       MarkDHT;
    Tag_SOS       MarkSOSGray;
} JPG_HEADER_GRAY, *PJPG_HEADER_GRAY;
//#NT#2008/11/11#Daniel Lu - begin

//For QV 5.0 AVI
typedef __packed struct {
    UCHAR           TagID[2];
    UCHAR           TagLength[2];
    UCHAR           Comment[14];
} Tag_CMAVI;    // Start of Scan

/**
    Quicktime version 5 AVI header.
*/
typedef __packed struct {
    USHORT        SOI;
    Tag_CMAVI     MarkCME0;
    Tag_DQT       MarkDQT;
    Tag_SOF       MarkSOF;
    Tag_SOS       MarkSOS;
} QV50AVI_HEADER, *PQV50AVI_HEADER;

//#NT#2007/10/17#Meg Lin -begin
//add MovMjpg
#define  QV5MOVMJPGHDRSIZE    0x299
                          //FFD8+ FFE0+ FFFE+ FFE1+ FFDB+ FFC4 + FFC0 +FFDA
                          //0x02+ 0x12+ 0x0E+ 0x2C+ 0x86+ 0x1A4+ 0x13 +0x0E

typedef __packed struct {
    UCHAR           TagID[2];
    UCHAR           TagLength[2];
    UCHAR           Comment[10];
} Tag_CMFE;    // Start of Scan
//For QV 5.0 AVI
typedef __packed struct {
    UCHAR           TagID[2];
    UCHAR           TagLength[2];
    UCHAR           Comment[8];
    UINT32           FieldSize;
    UINT32           PaddedSize;
    UINT32           Offset2Next;
    UCHAR           Offset1[4];
    UCHAR           Offset2[4];
    UCHAR           Offset3[4];
    UCHAR           Offset4[4];
    UCHAR           Offset5[4];

} Tag_CME1;    // Start of Scan


/**
    Quicktime version 5 AVI header.
*/
typedef __packed struct {
    USHORT        SOI;
    Tag_CMAVI     MarkCME0;
    Tag_CMFE      MarkCMFE;
    Tag_CME1      MarkCME1;
    Tag_DQT       MarkDQT;
    Tag_DHT       MarkDHT;
    Tag_SOF       MarkSOF;
    Tag_SOS       MarkSOS;
} MOVMJPG_HEADER, *PMOVMJPG_HEADER;

//#NT#2007/10/17#Meg Lin -end


//@}

#endif    // _EXIF_H
