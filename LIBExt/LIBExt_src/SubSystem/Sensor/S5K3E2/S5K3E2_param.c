#include "S5K3E2.h"
#include "debug.h"

const SENSOR_CMD S5K3E2_INI_REG[]=
{
{0x0103,0x01},      // SW reset
};


const SENSOR_CMD S5K3E2_REG_VGA[]=
{
{0x0103,0x01},      // SW reset
{0x0100,0x00},      // SW stand-by

// PLL setting for 800Mhz,  vt_pix_clk 80Mhz
{0x0305,0x06},
{0x0306,0x00},
{0x0307,0xa0},      // pll multiplier  160d
{0x0301,0x08},      // vt_pix_clk_div : 8d
{0x0303,0x01},  // vt_sys_clk_div : 1d
{0x0309,0x08},      // op_pix_clk_div : 8d     //default   value
{0x030b,0x01},  // op_sys_clk_div : 1d
#if(_S5K3E2_FLIP_H_V_ )
{0x0101,0x03},      // flip H V
#endif
{0x0344,0x00},      // x_addr_start_H
{0x0345,0x18},      // x_addr_start_L
{0x0346,0x00},      // y_addr_start_H
{0x0347,0x14},      // y_addr_start_L
{0x0348,0x0a},      // x_addr_end_H
{0x0349,0x17},      // x_addr_end_L
{0x034a,0x07},      // y_addr_end_H
{0x034b,0x93},      // y_addr_end_L
//frame format (min blanking)
{0x0340,0x01},      //frame length line 490 (min frame blanking line 10) //Paul
{0x0341,0xEA},
{0x0342,0x0a},      //line length pck 2730 (min line blanking pck 122)
{0x0343,0xac},
//Binning
{0x0381,0x01},      //x_even_inc
{0x0383,0x07},      //x_odd_inc, Paul
{0x0385,0x01},      //y_even_inc
{0x0387,0x07},      //y_odd_inc, Paul
{0x3014,0x00},      //binning enable, Paul

//MSR setting
{0x30c4,0x01},// MIPI/CCP PHY sel  1:CCP  0:MIPI
{0x3000,0x03},// Analog Register Setting
{0x3001,0x94},// Analog Register Setting
{0x3002,0x02},// Analog Register Setting
{0x3003,0x95},// Analog Register Setting
{0x3004,0x0f},// Analog Register Setting
{0x3005,0x05},// Analog Register Setting
{0x3006,0x3c},// Analog Register Setting
{0x3007,0x8c},// Analog Register Setting
{0x3008,0x93},// Analog Register Setting
{0x3009,0x05},// Analog Register Setting
{0x300a,0x3a},// Analog Register Setting
//default   value 300b10 // Analog Register Setting
{0x300c,0x02},// Analog Register Setting
{0x300d,0x3e},// Analog Register Setting
//default   value 300e02 // Analog Register Setting
{0x300f,0x0e},// Analog Register Setting
{0x3010,0x46},// Analog Register Setting
{0x3011,0x64},// Analog Register Setting
{0x3012,0x1e},// Analog Register Setting
//default   value 301500 // Analog Register Setting
{0x301d,0x3f},// Analog Register Setting
{0x3024,0x04},// Analog Register Setting
{0x3028,0x40},// Analog Register Setting
{0x3070,0xdf},// Analog Register Setting
//default   value 307220 // Analog Register Setting
{0x301b,0x73},// Analog Register Setting
{0x307e,0x02},// Analog Register Setting
{0x30bd,0x06},// Analog Register Setting
{0x30c2,0x0b},// Analog Register Setting
{0x30ac,0x81},// Analog Register Setting
{0x3151,0xe6},// Analog Register Setting
{0x3029,0x02},// Analog Register Setting
{0x30BF,0x00},// Analog Register Setting
{0x3022,0x87},// Analog Register Setting
{0x3019,0x60},// Analog Register Setting    //Rick 090525
//{0x31463C }// AF current //greg090610

{0x3112,0x0a},//used VCM calibration current start }//greg 090610
{0x3112,0x09},
//p5000 //delay 5ms
{0x3145,0x04},
{0x3146,0x80},//used VCM calibration current end

{0x3152,0x08},
{0x3159,0x00},//             Gordon 090204 Driving strength H V
//{0x315a,0x20},//Rick 080922  Gordon 090204Driving strength Data PCLK
{0x315a,0x30},//Rick 080922  Gordon 090204Driving strength Data PCLK
//default   value s020400
{0x0205,0x80},
//default   value s020002
{0x0202,0x01},      // Paul

{0x0100,0x01},      //streaming on,
};

const SENSOR_CMD S5K3E2_REG_1280X720[]=
{
{0x0103,0x01},      // SW reset
{0x0100,0x00},      // SW stand-by

// PLL setting for 800Mhz,  vt_pix_clk 80Mhz
{0x0305,0x06},
{0x0306,0x00},
#if(_S5K3E2_PRV_PCLK_60M_)
{0x0307,0x78},      // pll multiplier  160d
#else
{0x0307,0xa0},      // pll multiplier  160d
#endif
{0x0301,0x08},      // vt_pix_clk_div : 8d
{0x0303,0x01},  // vt_sys_clk_div : 1d
{0x0309,0x08},      // op_pix_clk_div : 8d     //default   value
{0x030b,0x01},  // op_sys_clk_div : 1d

#if(_S5K3E2_FLIP_H_V_ )
{0x0101,0x03},      // flip H V
#endif

//output size   (1280x 720)
{0x034c,0x05},          // x size 1280, Paul
{0x034d,0x00},
{0x034e,0x02},          // y size 720, Paul
{0x034f,0xD0},
{0x0344,0x00},      // x_addr_start_H   Paul
{0x0345,0x18},      // x_addr_start_L, 24d
{0x0346,0x01},      // y_addr_start_H
{0x0347,0x04},      // y_addr_start_L, 260d
{0x0348,0x0a},      // x_addr_end_H
{0x0349,0x17},      // x_addr_end_L, 2583d
{0x034a,0x06},      // y_addr_end_H
{0x034b,0xA3},      // y_addr_end_L, 1699d
//frame format (min blanking)
#if(_S5K3E2_PRV_PCLK_60M_)
{0x0340,0x02},  //07        //frame length line 730 (min framee blanking line 10)//Rick 090116
{0x0341,0xda},  //b6
#else
{0x0340,0x03},  //07        //frame length line 1970 (min framee blanking line 10)//Rick 090116
{0x0341,0xe2},  //b6
#endif
{0x0342,0x0a},      //line length pck 2730 (min line blanking pck 122)
{0x0343,0xac},
//Binning                                                                             ,
{0x0381,0x01},      //x_even_inc
{0x0383,0x01},      //x_odd_inc
{0x0385,0x01},      //y_even_inc
{0x0387,0x03},      //y_odd_inc
{0x3014,0x06},      //binning enable

//MSR setting
{0x30c4,0x01},// MIPI/CCP PHY sel  1:CCP  0:MIPI
{0x3000,0x03},// Analog Register Setting
{0x3001,0x94},// Analog Register Setting
{0x3002,0x02},// Analog Register Setting
{0x3003,0x95},// Analog Register Setting
{0x3004,0x0f},// Analog Register Setting
{0x3005,0x05},// Analog Register Setting
{0x3006,0x3c},// Analog Register Setting
{0x3007,0x8c},// Analog Register Setting
{0x3008,0x93},// Analog Register Setting
{0x3009,0x05},// Analog Register Setting
{0x300a,0x3a},// Analog Register Setting
//default   value 300b10 // Analog Register Setting
{0x300c,0x02},// Analog Register Setting
{0x300d,0x3e},// Analog Register Setting
//default   value 300e02 // Analog Register Setting
{0x300f,0x0e},// Analog Register Setting
{0x3010,0x46},// Analog Register Setting
{0x3011,0x64},// Analog Register Setting
{0x3012,0x1e},// Analog Register Setting
//default   value 301500 // Analog Register Setting
{0x301d,0x3f},// Analog Register Setting
{0x3024,0x04},// Analog Register Setting
{0x3028,0x40},// Analog Register Setting
{0x3070,0xdf},// Analog Register Setting
//default   value 307220 // Analog Register Setting
{0x301b,0x73},// Analog Register Setting
{0x307e,0x02},// Analog Register Setting
{0x30bd,0x06},// Analog Register Setting
{0x30c2,0x0b},// Analog Register Setting
{0x30ac,0x81},// Analog Register Setting
{0x3151,0xe6},// Analog Register Setting
{0x3029,0x02},// Analog Register Setting
{0x30BF,0x00},// Analog Register Setting
{0x3022,0x87},// Analog Register Setting
{0x3019,0x60},// Analog Register Setting    //Rick 090525
//{0x31463C }// AF current //greg090610

{0x3112,0x0a},//used VCM calibration current start }//greg 090610
{0x3112,0x09},
//p5000 //delay 5ms
{0x3145,0x04},
{0x3146,0x80},//used VCM calibration current end

{0x3152,0x08},
{0x3159,0x00},//             Gordon 090204 Driving strength H V
//{0x315a,0x20},//Rick 080922  Gordon 090204Driving strength Data PCLK
{0x315a,0x30},//Rick 080922  Gordon 090204Driving strength Data PCLK

//default   value s020400
{0x0205,0x80},
//default   value s020002
{0x0202,0x03},      // Paul

{0x0100,0x01},      //streaming on,


};
//1296x972
const SENSOR_CMD S5K3E2_REG_1280X960[]=
{
 //{0x0103,0x01},     // SW reset
 {0x0100,0x00},     // SW stand-by

// PLL setting for 800Mhz,  vt_pix_clk 80Mhz
{0x0305,0x06},
{0x0306,0x00},
#if(_S5K3E2_PRV_PCLK_60M_)
{0x0307,0x78},      // pll multiplier  160d
#else
{0x0307,0xa0},      // pll multiplier  160d
#endif
{0x0301,0x08},      // vt_pix_clk_div : 8d
{0x0303,0x01},  // vt_sys_clk_div : 1d
{0x0309,0x08},      // op_pix_clk_div : 8d     //default   value
{0x030b,0x01},  // op_sys_clk_div : 1d

#if(_S5K3E2_FLIP_H_V_ )
{0x0101,0x03},      // flip H V
#endif
// Data Format
//default   value s01120a       // CCP data format_MSB : 0808
//default   value s01130a       // CCP data format_LSB : 0808

//output size   (1296x 972)
{0x034c,0x05},  //0a        // x size 1296 //Rick 080922
{0x034d,0x10},  //30                      //Rick 080922
{0x034e,0x03},  //07        // y size 972  //Rick 081229
{0x034f,0xcc},  //a8                      //Rick 081229


{0x0344,0x00},      // x_addr_start_H
{0x0345,0x08},      // x_addr_start_L
{0x0346,0x00},      // y_addr_start_H
{0x0347,0x08},      // y_addr_start_L
{0x0348,0x0a},      // x_addr_end_H
{0x0349,0x27},      // x_addr_end_L
{0x034a,0x07},      // y_addr_end_H
{0x034b,0x9f},      // y_addr_end_L

//frame format (min blanking)
#if(_S5K3E2_PRV_PCLK_60M_)
{0x0340,0x02},  //07        //frame length line 730 (min framee blanking line 10)//Rick 090116
{0x0341,0xda},  //b6
#else
{0x0340,0x03},  //07        //frame length line 1970 (min framee blanking line 10)//Rick 090116
{0x0341,0xe2},  //b6
#endif
{0x0342,0x0a},      //line length pck 2730 (min line blanking pck 122)
{0x0343,0xac},

//Binning
{0x0381,0x01},      //x_even_inc                                                                };
{0x0383,0x01},      //x_odd_inc
{0x0385,0x01},      //y_even_inc                                                                const SENSOR_CMD S5K3E2_REG_1280X480[]=
{0x0387,0x03},      //y_odd_inc                                                                 {
{0x3014,0x06},      //binning enable                                                                {0x3011, 0x0e},

//MSR setting
{0x30c4,0x01},// MIPI/CCP PHY sel  1:CCP  0:MIPI
{0x3000,0x03},// Analog Register Setting
{0x3001,0x94},// Analog Register Setting
{0x3002,0x02},// Analog Register Setting
{0x3003,0x95},// Analog Register Setting
{0x3004,0x0f},// Analog Register Setting
{0x3005,0x05},// Analog Register Setting
{0x3006,0x3c},// Analog Register Setting
{0x3007,0x8c},// Analog Register Setting
{0x3008,0x93},// Analog Register Setting
{0x3009,0x05},// Analog Register Setting
{0x300a,0x3a},// Analog Register Setting
//default   value 300b10 // Analog Register Setting
{0x300c,0x02},// Analog Register Setting
{0x300d,0x3e},// Analog Register Setting
//default   value 300e02 // Analog Register Setting
{0x300f,0x0e},// Analog Register Setting
{0x3010,0x46},// Analog Register Setting
{0x3011,0x64},// Analog Register Setting
{0x3012,0x1e},// Analog Register Setting
//default   value 301500 // Analog Register Setting
{0x301d,0x3f},// Analog Register Setting
{0x3024,0x04},// Analog Register Setting
{0x3028,0x40},// Analog Register Setting
{0x3070,0xdf},// Analog Register Setting
//default   value 307220 // Analog Register Setting
{0x301b,0x73},// Analog Register Setting
{0x307e,0x02},// Analog Register Setting
{0x30bd,0x06},// Analog Register Setting
{0x30c2,0x0b},// Analog Register Setting
{0x30ac,0x81},// Analog Register Setting
{0x3151,0xe6},// Analog Register Setting
{0x3029,0x02},// Analog Register Setting
{0x30BF,0x00},// Analog Register Setting
{0x3022,0x87},// Analog Register Setting
{0x3019,0x60},// Analog Register Setting    //Rick 090525
//{0x31463C }// AF current //greg090610

{0x3112,0x0a},//used VCM calibration current start }//greg 090610
{0x3112,0x09},
//p5000 //delay 5ms
{0x3145,0x04},
{0x3146,0x80},//used VCM calibration current end

{0x3152,0x08},
{0x3159,0x00},//             Gordon 090204 Driving strength H V
//{0x315a,0x20},//Rick 080922  Gordon 090204Driving strength Data PCLK
{0x315a,0x30},//Rick 080922  Gordon 090204Driving strength Data PCLK

//default   value s020400
{0x0205,0x80},
//default   value s020002
{0x0202,0x03},      // Paul

{0x0100,0x01},      //streaming on,


};
const SENSOR_CMD S5K3E2_REG_1920X1080[]=
{

{0x0103,0x01},      // SW reset
{0x0100,0x00},      // SW stand-by

// PLL setting for 800Mhz,  vt_pix_clk 80Mhz
{0x0305,0x06},
{0x0306,0x00},
{0x0307,0xa0},      // pll multiplier  160d
{0x0301,0x08},      // vt_pix_clk_div : 8d
{0x0303,0x01},  // vt_sys_clk_div : 1d
{0x0309,0x08},      // op_pix_clk_div : 8d     //default   value
{0x030b,0x01},  // op_sys_clk_div : 1d

// Data Format
//default   value s01120a       // CCP data format_MSB : 0808
//default   value s01130a       // CCP data format_LSB : 0808
#if(_S5K3E2_FLIP_H_V_ )
{0x0101,0x03},      // flip H V
#endif
//output size   (1280x 720)
{0x034c,0x07},          // x size 1920, Paul
{0x034d,0x80},
{0x034e,0x04},          // y size 1080, Paul
{0x034f,0x38},


{0x0344,0x01},      // x_addr_start_H   Paul
{0x0345,0x58},      // x_addr_start_L, 344d
{0x0346,0x01},      // y_addr_start_H
{0x0347,0xB8},      // y_addr_start_L, 440d
{0x0348,0x08},      // x_addr_end_H
{0x0349,0xD7},      // x_addr_end_L, 2263d
{0x034a,0x05},      // y_addr_end_H
{0x034b,0xEF},      // y_addr_end_L, 1519d


//frame format (min blanking)
{0x0340,0x04},  //07        //frame length line 1970 (min framee blanking line 10)//Rick 090116
{0x0341,0x94},  //b6
{0x0342,0x0a},      //line length pck 2730 (min line blanking pck 122)
{0x0343,0xac},

//Binning
{0x0381,0x01},      //x_even_inc                                                                };
{0x0383,0x01},      //x_odd_inc
{0x0385,0x01},      //y_even_inc                                                                const SENSOR_CMD S5K3E2_REG_1280X480[]=
{0x0387,0x01},      //y_odd_inc                                                                 {
{0x3014,0x00},      //binning enable                                                                {0x3011, 0x0e},

//MSR setting
{0x30c4,0x01},// MIPI/CCP PHY sel  1:CCP  0:MIPI
{0x3000,0x03},// Analog Register Setting
{0x3001,0x94},// Analog Register Setting
{0x3002,0x02},// Analog Register Setting
{0x3003,0x95},// Analog Register Setting
{0x3004,0x0f},// Analog Register Setting
{0x3005,0x05},// Analog Register Setting
{0x3006,0x3c},// Analog Register Setting
{0x3007,0x8c},// Analog Register Setting
{0x3008,0x93},// Analog Register Setting
{0x3009,0x05},// Analog Register Setting
{0x300a,0x3a},// Analog Register Setting
//default   value 300b10 // Analog Register Setting
{0x300c,0x02},// Analog Register Setting
{0x300d,0x3e},// Analog Register Setting
//default   value 300e02 // Analog Register Setting
{0x300f,0x0e},// Analog Register Setting
{0x3010,0x46},// Analog Register Setting
{0x3011,0x64},// Analog Register Setting
{0x3012,0x1e},// Analog Register Setting
//default   value 301500 // Analog Register Setting
{0x301d,0x3f},// Analog Register Setting
{0x3024,0x04},// Analog Register Setting
{0x3028,0x40},// Analog Register Setting
{0x3070,0xdf},// Analog Register Setting
//default   value 307220 // Analog Register Setting
{0x301b,0x73},// Analog Register Setting
{0x307e,0x02},// Analog Register Setting
{0x30bd,0x06},// Analog Register Setting
{0x30c2,0x0b},// Analog Register Setting
{0x30ac,0x81},// Analog Register Setting
{0x3151,0xe6},// Analog Register Setting
{0x3029,0x02},// Analog Register Setting
{0x30BF,0x00},// Analog Register Setting
{0x3022,0x87},// Analog Register Setting
{0x3019,0x60},// Analog Register Setting    //Rick 090525
//{0x31463C }// AF current //greg090610

{0x3112,0x0a},//used VCM calibration current start }//greg 090610
{0x3112,0x09},
//p5000 //delay 5ms
{0x3145,0x04},
{0x3146,0x80},//used VCM calibration current end

{0x3152,0x08},
{0x3159,0x00},//             Gordon 090204 Driving strength H V
//{0x315a,0x20},//Rick 080922  Gordon 090204Driving strength Data PCLK
{0x315a,0x30},//Rick 080922  Gordon 090204Driving strength Data PCLK

//default   value s020400
{0x0205,0x80},
//default   value s020002
{0x0202,0x03},      // Paul

{0x0100,0x01},      //streaming on,
};

const SENSOR_CMD S5K3E2_REG_FULL[]=
{
//{0x0103,0x01},        // SW reset
{0x0100,0x00},      // SW stand-by

// PLL setting for 800Mhz,  vt_pix_clk 80Mhz
{0x0305,0x06},
{0x0306,0x00},
{0x0307,0xa0},      // pll multiplier  160d
{0x0301,0x08},      // vt_pix_clk_div : 8d
{0x0303,0x01},  // vt_sys_clk_div : 1d
{0x0309,0x08},      // op_pix_clk_div : 8d     //default   value
{0x030b,0x01},  // op_sys_clk_div : 1d

#if(_S5K3E2_FLIP_H_V_ )
{0x0101,0x03},      // flip H V
#endif
// Data Format
//default   value       s01120a     // CCP data format_MSB : 0808
//default   value       s01130a     // CCP data format_LSB : 0808

//output size   (2608 x 1960)
{0x034c,0x0a},//default   value       s034c0a       // x size
{0x034d,0x30},//default   value       s034d30
{0x034e,0x07},//default   value       s034e07       // y size
{0x034f,0xa8},//default   value       s034fa8

{0x0344,0x00},      // x_addr_start_H   Paul
{0x0345,0x00},      // x_addr_start_L, 344d
{0x0346,0x00},      // y_addr_start_H
{0x0347,0x00},      // y_addr_start_L, 440d
{0x0348,0x0a},      // x_addr_end_H
{0x0349,0x2f},      // x_addr_end_L, 2263d
{0x034a,0x07},      // y_addr_end_H
{0x034b,0xa7},      // y_addr_end_L, 1519d

//frame format (min blanking)
{0x0340,0x07},//default   value       s034007       //frame length line 1970 (min framee blanking line 10)
{0x0341,0xb6},//default   value       s0341b6
{0x0342,0x0a},//default   value       s03420a       //line length pck 2730 (min line blanking pck 122)
{0x0343,0xac},//default   value       s0343ac

//Binning
{0x0381,0x01},      //x_even_inc                                                                };
{0x0383,0x01},      //x_odd_inc
{0x0385,0x01},      //y_even_inc                                                                const SENSOR_CMD S5K3E2_REG_1280X480[]=
{0x0387,0x01},      //y_odd_inc                                                                 {
{0x3014,0x00},      //binning enable                                                                {0x3011, 0x0e},

//MSR setting
{0x30c4,0x01},// MIPI/CCP PHY sel  1:CCP  0:MIPI
{0x3000,0x03},// Analog Register Setting
{0x3001,0x94},// Analog Register Setting
{0x3002,0x02},// Analog Register Setting
{0x3003,0x95},// Analog Register Setting
{0x3004,0x0f},// Analog Register Setting
{0x3005,0x05},// Analog Register Setting
{0x3006,0x3c},// Analog Register Setting
{0x3007,0x8c},// Analog Register Setting
{0x3008,0x93},// Analog Register Setting
{0x3009,0x05},// Analog Register Setting
{0x300a,0x3a},// Analog Register Setting
//default   value 300b10 // Analog Register Setting
{0x300c,0x02},// Analog Register Setting
{0x300d,0x3e},// Analog Register Setting
//default   value 300e02 // Analog Register Setting
{0x300f,0x0e},// Analog Register Setting
{0x3010,0x46},// Analog Register Setting
{0x3011,0x64},// Analog Register Setting
{0x3012,0x1e},// Analog Register Setting
//default   value 301500 // Analog Register Setting
{0x301d,0x3f},// Analog Register Setting
{0x3024,0x04},// Analog Register Setting
{0x3028,0x40},// Analog Register Setting
{0x3070,0xdf},// Analog Register Setting
//default   value 307220 // Analog Register Setting
{0x301b,0x73},// Analog Register Setting
{0x307e,0x02},// Analog Register Setting
{0x30bd,0x06},// Analog Register Setting
{0x30c2,0x0b},// Analog Register Setting
{0x30ac,0x81},// Analog Register Setting
{0x3151,0xe6},// Analog Register Setting
{0x3029,0x02},// Analog Register Setting
{0x30BF,0x00},// Analog Register Setting
{0x3022,0x87},// Analog Register Setting
{0x3019,0x60},// Analog Register Setting    //Rick 090525
//{0x31463C }// AF current //greg090610

{0x3112,0x0a},//used VCM calibration current start }//greg 090610
{0x3112,0x09},
//p5000 //delay 5ms
{0x3145,0x04},
{0x3146,0x80},//used VCM calibration current end

{0x3152,0x08},
{0x3159,0x00},//             Gordon 090204 Driving strength H V
{0x315a,0x20},//Rick 080922  Gordon 090204Driving strength Data PCLK
//{0x315a,0x30},//Rick 080922  Gordon 090204Driving strength Data PCLK

//default   value s020400
{0x0205,0x80},
//default   value s020002
{0x0202,0x03},      // Paul

{0x0100,0x01},      //streaming on,

};

const SENSOR_CMD S5K3E2_REG_VGAP60[]=
{

{0x0103,0x01},      // SW reset
{0x0100,0x00},      // SW stand-by


// PLL setting for 800Mhz,  vt_pix_clk 80Mhz

{0x0305,0x06},      // Chris MCLK=24MHz
{0x0306,0x00},
{0x0307,0xA0},      // pll multiplier  160d  , Paul
{0x0301,0x08},      // vt_pix_clk_div : 8d
{0x0303,0x01},      // vt_sys_clk_div : 1d, Paul
{0x0309,0x08},      // op_pix_clk_div : 8d
{0x030b,0x01},      // op_sys_clk_div : 1d, Paul
#if(_S5K3E2_FLIP_H_V_ )
{0x0101,0x03},      // flip H V
#endif

// Data Format
//default   value s01120a       // CCP data format_MSB : 0808
//default   value s01130a       // CCP data format_LSB : 0808

//output size   (1280x480)
{0x034c,0x05},      // x size 1280, chris 20120420
{0x034d,0x00},
{0x034e,0x01},      // y size 480
{0x034f,0xE0},


{0x0344,0x00},      // x_addr_start_H
{0x0345,0x18},      // x_addr_start_L  24
{0x0346,0x00},      // y_addr_start_H
{0x0347,0x14},      // y_addr_start_L  20
{0x0348,0x0a},      // x_addr_end_H
{0x0349,0x17},      // x_addr_end_L    2583
{0x034a,0x07},      // y_addr_end_H
{0x034b,0x93},      // y_addr_end_L    1939


//frame format (min blanking)
{0x0340,0x01},      //frame length line 490 (min frame blanking line 10) //Paul
{0x0341,0xEA},
{0x0342,0x0a},      //line length pck 2730 (min line blanking pck 122)
{0x0343,0xac},

//Binning
{0x0381,0x01},      //x_even_inc
{0x0383,0x03},      //x_odd_inc,  chris 20120420
{0x0385,0x01},      //y_even_inc
{0x0387,0x07},      //y_odd_inc, Paul
{0x3014,0x00},      //binning enable, Paul


//MSR setting
{0x30c4,0x01}, // MIPI/CCP PHY sel  1:CCP  0:MIPI
{0x3000,0x03}, // Analog Register Setting
{0x3001,0x94}, // Analog Register Setting
{0x3002,0x02}, // Analog Register Setting
{0x3003,0x95}, // Analog Register Setting
{0x3004,0x0f}, // Analog Register Setting
{0x3005,0x05}, // Analog Register Setting
{0x3006,0x3c}, // Analog Register Setting
{0x3007,0x8c}, // Analog Register Setting
{0x3008,0x93}, // Analog Register Setting
{0x3009,0x05}, // Analog Register Setting
{0x300a,0x3a}, // Analog Register Setting
//default   value s300b10 // Analog Register Setting
{0x300c,0x02}, // Analog Register Setting
{0x300d,0x3e}, // Analog Register Setting
//default   value s300e02 // Analog Register Setting
{0x300f,0x0e}, // Analog Register Setting
{0x3010,0x46}, // Analog Register Setting
{0x3011,0x64}, // Analog Register Setting
{0x3012,0x1e}, // Analog Register Setting
//default   value s301500 // Analog Register Setting
{0x301d,0x3f}, // Analog Register Setting
{0x3024,0x04}, // Analog Register Setting
{0x3028,0x40}, // Analog Register Setting
{0x3070,0xdf}, // Analog Register Setting
//default   value s307220 // Analog Register Setting
{0x301b,0x73}, // Analog Register Setting
{0x307e,0x02}, // Analog Register Setting
{0x30bd,0x06}, // Analog Register Setting
{0x30c2,0x0b}, // Analog Register Setting
{0x30ac,0x81}, // Analog Register Setting
{0x3151,0xe6}, // Analog Register Setting
{0x3029,0x02}, // Analog Register Setting
{0x30BF,0x00}, // Analog Register Setting
{0x3022,0x87}, // Analog Register Setting
{0x3019,0x60}, // Analog Register Setting    //Rick 090525
//s31463C // AF current //greg090610

{0x3112,0x0a}, //used VCM calibration current start //greg 090610
{0x3112,0x09},
//p5000 //delay 5ms
{0x3145,0x04},
{0x3146,0x80}, //used VCM calibration current end

{0x3152,0x08},
{0x3159,0x00},  //             Gordon 090204 Driving Strength H V
{0x315a,0x20},  //Rick 080922  Gordon 090204Driving strength Data PCLK


//default   value s020400
{0x0205,0x80},
//default   value s020002
{0x0202,0x01},      // Paul


{0x0100,0x01},      //streaming on
};


const SENSOR_CMD S5K3E2_REG_QVGAP120[]=
{

{0x0103,0x01},      // SW reset
{0x0100,0x00},      // SW stand-by


// PLL setting , vt_pix_clk 82Mhz

{0x0305,0x06},      // Chris MCLK=24MHz
{0x0306,0x00},
{0x0307,0xA4},      // pll multiplier  164d  , chris 20120424
{0x0301,0x08},      // vt_pix_clk_div : 8d
{0x0303,0x01},      // vt_sys_clk_div : 1d, Paul
{0x0309,0x08},      // op_pix_clk_div : 8d
{0x030b,0x01},      // op_sys_clk_div : 1d, Paul
#if(_S5K3E2_FLIP_H_V_ )
{0x0101,0x03},      // flip H V
#endif

// Data Format
//default   value s01120a       // CCP data format_MSB : 0808
//default   value s01130a       // CCP data format_LSB : 0808

//output size   (1280x240)
{0x034c,0x05},      // x size 1280, chris 20120420
{0x034d,0x00},
{0x034e,0x00},      // y size 240   chris 20120424
{0x034f,0xF0},


{0x0344,0x00},      // x_addr_start_H
{0x0345,0x18},      // x_addr_start_L  24
{0x0346,0x00},      // y_addr_start_H
{0x0347,0x14},      // y_addr_start_L  20
{0x0348,0x0a},      // x_addr_end_H
{0x0349,0x17},      // x_addr_end_L    2583
{0x034a,0x07},      // y_addr_end_H
{0x034b,0x93},      // y_addr_end_L    1939


//frame format (min blanking)
{0x0340,0x00},      //frame length line 250 (min frame blanking line 10) //chris
{0x0341,0xFA},
{0x0342,0x0a},      //line length pck 2730 (min line blanking pck 122)
{0x0343,0xac},

//Binning
{0x0381,0x01},      //x_even_inc
{0x0383,0x03},      //x_odd_inc,  chris 20120420
{0x0385,0x01},      //y_even_inc
{0x0387,0x0F},      //y_odd_inc, chris 20120424
{0x3014,0x00},      //binning disable  chris 20120424


//MSR setting
{0x30c4,0x01}, // MIPI/CCP PHY sel  1:CCP  0:MIPI
{0x3000,0x03}, // Analog Register Setting
{0x3001,0x94}, // Analog Register Setting
{0x3002,0x02}, // Analog Register Setting
{0x3003,0x95}, // Analog Register Setting
{0x3004,0x0f}, // Analog Register Setting
{0x3005,0x05}, // Analog Register Setting
{0x3006,0x3c}, // Analog Register Setting
{0x3007,0x8c}, // Analog Register Setting
{0x3008,0x93}, // Analog Register Setting
{0x3009,0x05}, // Analog Register Setting
{0x300a,0x3a}, // Analog Register Setting
//default   value s300b10 // Analog Register Setting
{0x300c,0x02}, // Analog Register Setting
{0x300d,0x3e}, // Analog Register Setting
//default   value s300e02 // Analog Register Setting
{0x300f,0x0e}, // Analog Register Setting
{0x3010,0x46}, // Analog Register Setting
{0x3011,0x64}, // Analog Register Setting
{0x3012,0x1e}, // Analog Register Setting
//default   value s301500 // Analog Register Setting
{0x301d,0x3f}, // Analog Register Setting
{0x3024,0x04}, // Analog Register Setting
{0x3028,0x40}, // Analog Register Setting
{0x3070,0xdf}, // Analog Register Setting
//default   value s307220 // Analog Register Setting
{0x301b,0x73}, // Analog Register Setting
{0x307e,0x02}, // Analog Register Setting
{0x30bd,0x06}, // Analog Register Setting
{0x30c2,0x0b}, // Analog Register Setting
{0x30ac,0x81}, // Analog Register Setting
{0x3151,0xe6}, // Analog Register Setting
{0x3029,0x02}, // Analog Register Setting
{0x30BF,0x00}, // Analog Register Setting
{0x3022,0x87}, // Analog Register Setting
{0x3019,0x60}, // Analog Register Setting    //Rick 090525
//s31463C // AF current //greg090610

{0x3112,0x0a}, //used VCM calibration current start //greg 090610
{0x3112,0x09},
//p5000 //delay 5ms
{0x3145,0x04},
{0x3146,0x80}, //used VCM calibration current end

{0x3152,0x08},
{0x3159,0x00},  //             Gordon 090204 Driving strength H V
{0x315a,0x20},  //Rick 080922  Gordon 090204Driving strength Data PCLK


//default   value s020400
{0x0205,0x80},
//default   value s020002
{0x0202,0x00},      // chris


{0x0100,0x01},      //streaming on
};


/**
* ISO to AGC usage
*/
#define AGC_ISO50       1000
#define ADJ_ISO         0
#define ISOBASE         50

/************** MONITOR_MODE_1(VGA) **************/

#define VD_Sync_Prv_1                 10
#define HD_Sync_Prv_1                 122
#define VD_Total_Prv_1                490//688//830
#define HD_Total_Prv_1                2730//1606

#define ACT_STRX_Prv_1                  8// 7
#define ACT_STRY_Prv_1                  2// 1

#define IMAGESIZE_H_PRV_1         640
#define IMAGESIZE_V_PRV_1         480
#define SIE_PRV_XCENTER_1         (ACT_STRX_Prv_1+(IMAGESIZE_H_PRV_1/2))
#define SIE_PRV_YCENTER_1         (ACT_STRY_Prv_1+(IMAGESIZE_V_PRV_1/2))

#define Padding_Prv_1               0

/************** MONITOR_MODE_2(720p) **************/
#define VD_Sync_Prv_2               10
#define HD_Sync_Prv_2               122
#define VD_Total_Prv_2              994//1060
#define HD_Total_Prv_2              2730//2120

#define ACT_STRX_Prv_2              8
#define ACT_STRY_Prv_2              2

#define IMAGESIZE_H_PRV_2           1280
#define IMAGESIZE_V_PRV_2           720
#define SIE_PRV_XCENTER_2       (ACT_STRX_Prv_2+(IMAGESIZE_H_PRV_2/2))
#define SIE_PRV_YCENTER_2       (ACT_STRY_Prv_2+(IMAGESIZE_V_PRV_2/2))

#define Padding_Prv_2               0

/************** MONITOR_MODE_3(1280*960)**************/
#define VD_Sync_Prv_3               10
#define HD_Sync_Prv_3               122

#define VD_Total_Prv_3              994
#define HD_Total_Prv_3              2730//2177


#define ACT_STRX_Prv_3              8
#define ACT_STRY_Prv_3              2

#define IMAGESIZE_H_PRV_3           1296
#define IMAGESIZE_V_PRV_3           972
#define SIE_PRV_XCENTER_3       (ACT_STRX_Prv_3+(IMAGESIZE_H_PRV_3/2))
#define SIE_PRV_YCENTER_3       (ACT_STRY_Prv_3+(IMAGESIZE_V_PRV_3/2))


#define Padding_Prv_3               0

/************** MONITOR_MODE_4(1280*480)**************/

#define VD_Sync_Prv_4               1
#define HD_Sync_Prv_4               1300

#define VD_Total_Prv_4              994//485
#define HD_Total_Prv_4              2730//2245


#define ACT_STRX_Prv_4              8
#define ACT_STRY_Prv_4              2

#define IMAGESIZE_H_PRV_4           1272
#define IMAGESIZE_V_PRV_4           476
#define SIE_PRV_XCENTER_4       (ACT_STRX_Prv_4+(IMAGESIZE_H_PRV_4/2))
#define SIE_PRV_YCENTER_4       (ACT_STRY_Prv_4+(IMAGESIZE_V_PRV_4/2))

#define Padding_Prv_4               0
/************** MONITOR_MODE_5(1920*1080)**************/
#define VD_Sync_Prv_5               10
#define HD_Sync_Prv_5               122

#define VD_Total_Prv_5              1221
#define HD_Total_Prv_5              2730//4924


#define ACT_STRX_Prv_5              8
#define ACT_STRY_Prv_5              2

#define IMAGESIZE_H_PRV_5           1920
#define IMAGESIZE_V_PRV_5           1080
#define SIE_PRV_XCENTER_5       (ACT_STRX_Prv_5+(IMAGESIZE_H_PRV_5/2))
#define SIE_PRV_YCENTER_5       (ACT_STRY_Prv_5+(IMAGESIZE_V_PRV_5/2))

#define Padding_Prv_5               0

/************** MONITOR_MODE_6(1280*480 P60)**************/
#define VD_Sync_Prv_6                 10
#define HD_Sync_Prv_6                 122
#define VD_Total_Prv_6                490//688//830
#define HD_Total_Prv_6                2732//1606

#define ACT_STRX_Prv_6                  2// 7
#define ACT_STRY_Prv_6                  2// 1

#define IMAGESIZE_H_PRV_6         1280
#define IMAGESIZE_V_PRV_6         480
#define SIE_PRV_XCENTER_6         (ACT_STRX_Prv_1+(IMAGESIZE_H_PRV_6/2))
#define SIE_PRV_YCENTER_6         (ACT_STRY_Prv_1+(IMAGESIZE_V_PRV_6/2))

#define Padding_Prv_6               0


/************** MONITOR_MODE_7(1280*240 P120)**************/
#define VD_Sync_Prv_7                 10
#define HD_Sync_Prv_7                 122
#define VD_Total_Prv_7                250//688//830
#define HD_Total_Prv_7                2732//1606

#define ACT_STRX_Prv_7                  2// 7
#define ACT_STRY_Prv_7                  2// 1

#define IMAGESIZE_H_PRV_7         1280
#define IMAGESIZE_V_PRV_7         240
#define SIE_PRV_XCENTER_7         (ACT_STRX_Prv_1+(IMAGESIZE_H_PRV_7/2))
#define SIE_PRV_YCENTER_7         (ACT_STRY_Prv_1+(IMAGESIZE_V_PRV_7/2))

#define Padding_Prv_7               0


/************** STILL_MODE **************/
#define VD_Sync_Cap             10
#define HD_Sync_Cap             122
#define VD_Total_Cap            1974//1975
#define HD_Total_Cap            2730//3247

#define IMAGESIZE_H_CAP         2560
#define IMAGESIZE_V_CAP         1920

#define ACT_STRX_Cap            10
#define ACT_STRY_Cap            4
#define ACT_STRY_Cap2           0xff
#define ACT_STRY_Cap3           0xff
#define ACT_STRY_Cap4           0xff
#define ACT_STRY_Cap5           0xff
#define ACT_STRY_Cap6           0xff
#define ACT_STRY_Cap7           0xff
#define ACT_STRY_Cap8           0xff

#define SensorCapTotalFld       1
#define Field_Num1              1
#define Field_Num2              0xff
#define Field_Num3              0xff
#define Field_Num4              0xff
#define Field_Num5              0xff
#define Field_Num6              0xff
#define Field_Num7              0xff
#define Field_Num8              0xff

#define Padding_Cap             0
#define SIE_CAP_XCENTER     (ACT_STRX_Cap+(IMAGESIZE_H_CAP/2))
#define SIE_CAP_YCENTER     (ACT_STRY_Cap+(IMAGESIZE_V_CAP/2))
/**************Default Sensor mode selection for preview**************/
//#define SENSOR_PRV_MODE             MONITOR_MODE_3
//#define SENSOR_AF_MODE              MONITOR_MODE_4
//#define SENSOR_PRV_VGATYPE          SENSOR_OUT_PROGRESSIVE

/************** OB Setting **************/
#define PRV_OB_GAIN         0x20
#define PRV_OB_THRES        0xF0
#define PRV_OB_SUBSAMPLE    0x02
#define PRV_OB_X0           1264
#define PRV_OB_X1           4900
#define PRV_OB_Y0           9
#define PRV_OB_Y1           9

#define CAP_OB_GAIN         0x20
#define CAP_OB_THRES        0xF0
#define CAP_OB_SUBSAMPLE    0x05
#define CAP_OB_X0           1200
#define CAP_OB_X1           4900
#define CAP_OB_Y0           17
#define CAP_OB_Y1           17

#define OB_OFS_PRV      0x30//0x40//10 bit
#define OB_OFS_CAP      0x30//0x40//10 bit

/************** SIE CFA pattern selection **************/
#if(_S5K3E2_FLIP_H_V_ )
#define CFAPAT_PRV    SIECFA_R//flip H V
#define CFAPAT_CAP    SIECFA_R//flip H V
#else
#define CFAPAT_PRV    SIECFA_B
#define CFAPAT_CAP    SIECFA_B
#endif


/*********************************************/
//unit:mhz
#define PCLK_1      80
#define PCLK_2      80
#define PCLK_3      80
#define PCLK_4      80
#define PCLK_5      80
#define PCLK_6      80
#define PCLK_7      82
#define PCLK_STILL  80

SYNC_INFO prv_mode1 = {
    ACT_STRX_Prv_1, // Act_StartX;
    ACT_STRY_Prv_1, // Act_StartY;
    SIE_PRV_XCENTER_1, // Act_CenterX;
    SIE_PRV_YCENTER_1, // Act_CenterY;
    IMAGESIZE_H_PRV_1  , // Act_SizeX;
    IMAGESIZE_V_PRV_1, // Act_SizeY;

    // HDVD_Sync
    HD_Sync_Prv_1, //    HD_sync
    VD_Sync_Prv_1, //    VD_sync
    HD_Total_Prv_1, //    HD_total
    VD_Total_Prv_1, //    VD_total

    // ob_area
    PRV_OB_X0, //    OB_X0
    PRV_OB_Y0, //    OB_Y0
    PRV_OB_X1, //    OB_X1
    PRV_OB_Y1, //    OB_Y1
    PRV_OB_THRES, //    OB_Thres
    PRV_OB_SUBSAMPLE, //    OB_SubSample
    PRV_OB_GAIN, // OB_Gain
    OB_OFS_PRV, // OB_Offset;

    Padding_Prv_1 // padding;
};

SYNC_INFO prv_mode2 = {
    ACT_STRX_Prv_2, // act_start_x;
    ACT_STRY_Prv_2, // act_start_y;
    SIE_PRV_XCENTER_2, // act_center_x;
    SIE_PRV_YCENTER_2, // act_center_y;
    IMAGESIZE_H_PRV_2  , // Act_SizeX;
    IMAGESIZE_V_PRV_2, // Act_SizeY;

    // HDVD_Sync
    HD_Sync_Prv_2, //    HD_sync
    VD_Sync_Prv_2, //    VD_sync
    HD_Total_Prv_2, //    HD_total
    VD_Total_Prv_2, //    VD_total

    // ob_area
    PRV_OB_X0, //    OB_X0
    PRV_OB_Y0, //    OB_Y0
    PRV_OB_X1, //    OB_X1
    PRV_OB_Y1, //    OB_Y1
    PRV_OB_THRES, //    OB_Thres
    PRV_OB_SUBSAMPLE, //    OB_SubSample
    PRV_OB_GAIN, // OB_Gain
    OB_OFS_PRV, // OB_Offset;

    Padding_Prv_2 // padding;
};

SYNC_INFO prv_mode3 = {
    ACT_STRX_Prv_3, // act_start_x;
    ACT_STRY_Prv_3, // act_start_y;
    SIE_PRV_XCENTER_3, // act_center_x;
    SIE_PRV_YCENTER_3, // act_center_y;
    IMAGESIZE_H_PRV_3  , // Act_SizeX;
    IMAGESIZE_V_PRV_3, // Act_SizeY;

    // HDVD_Sync
    HD_Sync_Prv_3, //    HD_sync
    VD_Sync_Prv_3, //    VD_sync
    HD_Total_Prv_3, //    HD_total
    VD_Total_Prv_3, //    VD_total

    // ob_area
    PRV_OB_X0, //    OB_X0
    PRV_OB_Y0, //    OB_Y0
    PRV_OB_X1, //    OB_X1
    PRV_OB_Y1, //    OB_Y1
    PRV_OB_THRES, //    OB_Thres
    PRV_OB_SUBSAMPLE, //    OB_SubSample
    PRV_OB_GAIN, // OB_Gain
    OB_OFS_PRV, // OB_Offset;

    Padding_Prv_3 // padding;
};

SYNC_INFO prv_mode4 = {
    ACT_STRX_Prv_4, // act_start_x;
    ACT_STRY_Prv_4, // act_start_y;
    SIE_PRV_XCENTER_4, // act_center_x;
    SIE_PRV_YCENTER_4, // act_center_y;
    IMAGESIZE_H_PRV_4  , // Act_SizeX;
    IMAGESIZE_V_PRV_4, // Act_SizeY;

    // HDVD_Sync
    HD_Sync_Prv_4, //    HD_sync
    VD_Sync_Prv_4, //    VD_sync
    HD_Total_Prv_4, //    HD_total
    VD_Total_Prv_4, //    VD_total

    // ob_area
    PRV_OB_X0, //    OB_X0
    PRV_OB_Y0, //    OB_Y0
    PRV_OB_X1, //    OB_X1
    PRV_OB_Y1, //    OB_Y1
    PRV_OB_THRES, //    OB_Thres
    PRV_OB_SUBSAMPLE, //    OB_SubSample
    PRV_OB_GAIN, // OB_Gain
    OB_OFS_PRV, // OB_Offset;

    Padding_Prv_4 // padding;
};

SYNC_INFO prv_mode5 = {
    ACT_STRX_Prv_5, // act_start_x;
    ACT_STRY_Prv_5, // act_start_y;
    SIE_PRV_XCENTER_5, // act_center_x;
    SIE_PRV_YCENTER_5, // act_center_y;
    IMAGESIZE_H_PRV_5  , // Act_SizeX;
    IMAGESIZE_V_PRV_5, // Act_SizeY;

    // HDVD_Sync
    HD_Sync_Prv_5, //    HD_sync
    VD_Sync_Prv_5, //    VD_sync
    HD_Total_Prv_5, //    HD_total
    VD_Total_Prv_5, //    VD_total

    // ob_area
    PRV_OB_X0, //    OB_X0
    PRV_OB_Y0, //    OB_Y0
    PRV_OB_X1, //    OB_X1
    PRV_OB_Y1, //    OB_Y1
    PRV_OB_THRES, //    OB_Thres
    PRV_OB_SUBSAMPLE, //    OB_SubSample
    PRV_OB_GAIN, // OB_Gain
    OB_OFS_PRV, // OB_Offset;

    Padding_Prv_5 // padding;
};

SYNC_INFO prv_mode6 = {
    ACT_STRX_Prv_6, // act_start_x;
    ACT_STRY_Prv_6, // act_start_y;
    SIE_PRV_XCENTER_6, // act_center_x;
    SIE_PRV_YCENTER_6, // act_center_y;
    IMAGESIZE_H_PRV_6  , // Act_SizeX;
    IMAGESIZE_V_PRV_6, // Act_SizeY;

    // HDVD_Sync
    HD_Sync_Prv_6, //    HD_sync
    VD_Sync_Prv_6, //    VD_sync
    HD_Total_Prv_6, //    HD_total
    VD_Total_Prv_6, //    VD_total

    // ob_area
    PRV_OB_X0, //    OB_X0
    PRV_OB_Y0, //    OB_Y0
    PRV_OB_X1, //    OB_X1
    PRV_OB_Y1, //    OB_Y1
    PRV_OB_THRES, //    OB_Thres
    PRV_OB_SUBSAMPLE, //    OB_SubSample
    PRV_OB_GAIN, // OB_Gain
    OB_OFS_PRV, // OB_Offset;

    Padding_Prv_6 // padding;
};

SYNC_INFO prv_mode7 = {
    ACT_STRX_Prv_7, // act_start_x;
    ACT_STRY_Prv_7, // act_start_y;
    SIE_PRV_XCENTER_7, // act_center_x;
    SIE_PRV_YCENTER_7, // act_center_y;
    IMAGESIZE_H_PRV_7  , // Act_SizeX;
    IMAGESIZE_V_PRV_7, // Act_SizeY;

    // HDVD_Sync
    HD_Sync_Prv_7, //    HD_sync
    VD_Sync_Prv_7, //    VD_sync
    HD_Total_Prv_7, //    HD_total
    VD_Total_Prv_7, //    VD_total

    // ob_area
    PRV_OB_X0, //    OB_X0
    PRV_OB_Y0, //    OB_Y0
    PRV_OB_X1, //    OB_X1
    PRV_OB_Y1, //    OB_Y1
    PRV_OB_THRES, //    OB_Thres
    PRV_OB_SUBSAMPLE, //    OB_SubSample
    PRV_OB_GAIN, // OB_Gain
    OB_OFS_PRV, // OB_Offset;

    Padding_Prv_7 // padding;
};


SYNC_INFO still_mode = {
    ACT_STRX_Cap, // act_start_x;
    ACT_STRY_Cap, // act_start_y;
    SIE_CAP_XCENTER, // act_center_x;
    SIE_CAP_YCENTER, // act_center_y;
    IMAGESIZE_H_CAP  , // Act_SizeX;
    IMAGESIZE_V_CAP, // Act_SizeY;

    // HDVD_Sync
    HD_Sync_Cap, //    HD_sync
    VD_Sync_Cap, //    VD_sync
    HD_Total_Cap, //    HD_total
    VD_Total_Cap, //    VD_total

    // ob_area
    CAP_OB_X0, //    OB_X0
    CAP_OB_Y0, //    OB_Y0
    CAP_OB_X1, //    OB_X1
    CAP_OB_Y1, //    OB_Y1
    CAP_OB_THRES, //    OB_Thres
    CAP_OB_SUBSAMPLE, //    OB_SubSample
    CAP_OB_GAIN, // OB_Gain
    OB_OFS_CAP, // OB_Offset;

    Padding_Cap // padding;
};

PREVIEW_DATA_INFO S5K3E2_prv_mode[] = {
    { // SEN_MODE_VGA
        2, //  frame_rate
        CFAPAT_PRV, //  bayer_pattern
        2, // bin_max
        1, // skip_max
        PCLK_1, // pclk
        //&prv_mode1, // prv_sync_info
        //(SENSOR_CMD*)S5K3E2_REG_VGA,
        //sizeof(S5K3E2_REG_VGA)/sizeof(SENSOR_CMD)
    },

    { // SEN_MODE_720P
        2, //  frame_rate
        CFAPAT_PRV, //  bayer_pattern
        2, // bin_max
        1, // skip_max
        PCLK_2, // pclk
        //&prv_mode2, // prv_sync_info
        //(SENSOR_CMD*)S5K3E2_REG_1280X720,
        //sizeof(S5K3E2_REG_1280X720)/sizeof(SENSOR_CMD)
    },

    { // SEN_MODE_1280_960
        2, //  frame_rate
        CFAPAT_PRV, //  bayer_pattern
        1, // bin_max
        1, // skip_max
        PCLK_3, // pclk
        //&prv_mode3, // prv_sync_info
        //(SENSOR_CMD*)S5K3E2_REG_1280X960,
        //sizeof(S5K3E2_REG_1280X960)/sizeof(SENSOR_CMD)
    },

    { // SEN_MODE_1280_480
        2, //  frame_rate
        CFAPAT_PRV, //  bayer_pattern
        2, // bin_max
        1, // skip_max
        PCLK_4, // pclk
        //&prv_mode4, // prv_sync_info
        //(SENSOR_CMD*)S5K3E2_REG_1280X480,
        //sizeof(S5K3E2_REG_1280X480)/sizeof(SENSOR_CMD)
    },

    { // SEN_MODE_2560_960
        2, //  frame_rate
        CFAPAT_PRV, //  bayer_pattern
        1, // bin_max
        1, // skip_max
        PCLK_5, // pclk
        //&prv_mode5, // prv_sync_info
        //(SENSOR_CMD*)S5K3E2_REG_2560X960,
        //sizeof(S5K3E2_REG_2560X960)/sizeof(SENSOR_CMD)
    },

    { // SEN_MODE_2560_960
        2, //  frame_rate
        CFAPAT_PRV, //  bayer_pattern
        1, // bin_max
        1, // skip_max
        PCLK_6, // pclk
        //&prv_mode5, // prv_sync_info
        //(SENSOR_CMD*)S5K3E2_REG_2560X960,
        //sizeof(S5K3E2_REG_2560X960)/sizeof(SENSOR_CMD)
    },

    { // SEN_MODE_2560_960
        2, //  frame_rate
        CFAPAT_PRV, //  bayer_pattern
        1, // bin_max
        1, // skip_max
        PCLK_7, // pclk
        //&prv_mode5, // prv_sync_info
        //(SENSOR_CMD*)S5K3E2_REG_2560X960,
        //sizeof(S5K3E2_REG_2560X960)/sizeof(SENSOR_CMD)
    }
};

CAPTURE_DATA_INFO S5K3E2_still_mode[] = {
    {
        4336, // cap_width
        3240, // cap_height
        CFAPAT_CAP, // bayer_pattern
        PCLK_STILL, // pclk
        //&still_mode, // cap_sync_info;
        //(SENSOR_CMD*)S5K3E2_REG_FULL,
        //sizeof(S5K3E2_REG_FULL)/sizeof(SENSOR_CMD)
    }
};

extern void S5K3E2_GetRegInfo(S5K3E2_MODE mode,UINT32* addr,UINT32* size)
{
    switch(mode)
    {
        case S5K3E2_INIT:
            *addr = (UINT32)S5K3E2_INI_REG;
            *size = (sizeof(S5K3E2_INI_REG)/sizeof(SENSOR_CMD));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(INIT)));
            break;
        case S5K3E2_VGA:
            *addr = (UINT32)S5K3E2_REG_VGA;
            *size = (sizeof(S5K3E2_REG_VGA)/sizeof(SENSOR_CMD));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(VGA)));
            break;
        case S5K3E2_VGAP60:
            *addr = (UINT32)S5K3E2_REG_VGAP60;
            *size = (sizeof(S5K3E2_REG_VGAP60)/sizeof(SENSOR_CMD));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(VGAP60)));
            break;
        case S5K3E2_QVGAP120:
            *addr = (UINT32)S5K3E2_REG_QVGAP120;
            *size = (sizeof(S5K3E2_REG_QVGAP120)/sizeof(SENSOR_CMD));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(QVGAP120)));
            break;
        case S5K3E2_1280X720:
            *addr = (UINT32)S5K3E2_REG_1280X720;
            *size = (sizeof(S5K3E2_REG_1280X720)/sizeof(SENSOR_CMD));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(1280X720)));
            break;
        case S5K3E2_1280X960:
            *addr = (UINT32)S5K3E2_REG_1280X960;
            *size = (sizeof(S5K3E2_REG_1280X960)/sizeof(SENSOR_CMD));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(1280X960)));
            break;
        case S5K3E2_1280X480://tmp
            *addr = (UINT32)S5K3E2_REG_1280X960;
            *size = (sizeof(S5K3E2_REG_1280X960)/sizeof(SENSOR_CMD));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(1280X480)));
            break;
        case S5K3E2_1920X1080://for 1080p
            *addr = (UINT32)S5K3E2_REG_1920X1080;
            *size = (sizeof(S5K3E2_REG_1920X1080)/sizeof(SENSOR_CMD));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(2560X960)));
            break;
        case S5K3E2_STILL:
            *addr = (UINT32)S5K3E2_REG_FULL;
            *size = (sizeof(S5K3E2_REG_FULL)/sizeof(SENSOR_CMD));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(FULL)));
            break;
        default:
            debug_err(("^R %s: mode error(default)",__func__));
            break;
    }
}

extern void S5K3E2_GetSigInfo(S5K3E2_MODE mode,UINT32* addr,UINT32* size)
{
    switch(mode)
    {
        case S5K3E2_VGA:
            *addr = (UINT32)&prv_mode1;
            //*size = (sizeof(prv_mode1)/sizeof(SYNC_INFO));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(VGA)));
            break;
        case S5K3E2_VGAP60:
            *addr = (UINT32)&prv_mode6;
            //*size = (sizeof(prv_mode1)/sizeof(SYNC_INFO));
            //debug_err(("^R %s: mode (%s)",__func__,Token(VGAP60)));
            break;
        case S5K3E2_QVGAP120:
            *addr = (UINT32)&prv_mode7;
            //*size = (sizeof(prv_mode1)/sizeof(SYNC_INFO));
            //debug_err(("^R %s: mode (%s)",__func__,Token(QVGAP120)));
            break;
        case S5K3E2_1280X720:
            *addr = (UINT32)&prv_mode2;
            //*size = (sizeof(prv_mode2)/sizeof(SYNC_INFO));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(1280X720)));
            break;
        case S5K3E2_1280X960:
            *addr = (UINT32)&prv_mode3;
            //*size = (sizeof(prv_mode3)/sizeof(SYNC_INFO));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(1280X960)));
            break;
        case S5K3E2_1280X480:
            *addr = (UINT32)&prv_mode4;
            //*size = (sizeof(prv_mode4)/sizeof(SYNC_INFO));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(1280X480)));
            break;
        case S5K3E2_1920X1080:
            *addr = (UINT32)&prv_mode5;
            //*size = (sizeof(prv_mode5)/sizeof(SYNC_INFO));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(2560X960)));
            break;
        case S5K3E2_STILL:
            *addr = (UINT32)&still_mode;
            //*size = (sizeof(still_mode)/sizeof(SYNC_INFO));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(FULL)));
            break;
        case S5K3E2_INIT:
            //debug_err(("^R %s: mode error(%s)",__func__,Token(INIT)));
        default:
            debug_err(("^R %s: mode error(default)",__func__));
            break;
    }
}

extern void S5K3E2_GetDataInfo(S5K3E2_MODE mode,UINT32* addr,UINT32* size)
{
    switch(mode)
    {
        case S5K3E2_VGA:
            *addr = (UINT32)&S5K3E2_prv_mode[0];
            //*size = (sizeof(S5K3E2_prv_mode[0])/sizeof(PREVIEW_DATA_INFO));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(VGA)));
            break;
        case S5K3E2_1280X720:
            *addr = (UINT32)&S5K3E2_prv_mode[1];
            //*size = (sizeof(S5K3E2_prv_mode)/sizeof(PREVIEW_DATA_INFO));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(1280X720)));
            break;
        case S5K3E2_1280X960:
            *addr = (UINT32)&S5K3E2_prv_mode[2];
            //*size = (sizeof(S5K3E2_prv_mode)/sizeof(PREVIEW_DATA_INFO));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(1280X960)));
            break;
        case S5K3E2_1280X480:
            *addr = (UINT32)&S5K3E2_prv_mode[3];
            //*size = (sizeof(S5K3E2_prv_mode)/sizeof(PREVIEW_DATA_INFO));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(1280X480)));
            break;
        case S5K3E2_1920X1080:
            *addr = (UINT32)&S5K3E2_prv_mode[4];
            //*size = (sizeof(S5K3E2_prv_mode)/sizeof(PREVIEW_DATA_INFO));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(2560X960)));
            break;
        case S5K3E2_VGAP60:
            *addr = (UINT32)&S5K3E2_prv_mode[5];
            //*size = (sizeof(S5K3E2_prv_mode[0])/sizeof(PREVIEW_DATA_INFO));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(VGA)));
            break;
        case S5K3E2_QVGAP120:
            *addr = (UINT32)&S5K3E2_prv_mode[6];
            //*size = (sizeof(S5K3E2_prv_mode[0])/sizeof(PREVIEW_DATA_INFO));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(VGA)));
            break;
        case S5K3E2_STILL:
            *addr = (UINT32)&S5K3E2_still_mode;
            //*size = (sizeof(S5K3E2_still_mode)/sizeof(CAPTURE_DATA_INFO));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(FULL)));
            break;
        case S5K3E2_INIT:
            //debug_err(("^R %s: mode error(%s)",__func__,Token(INIT)));
        default:
            debug_err(("^R %s: mode error(default)",__func__));
            break;
    }
}

extern void S5K3E2_GetISOInfo(UINT32 *ISOBase,UINT32 *AGCBase)
{
    *ISOBase = ISOBASE;
    *AGCBase = AGC_ISO50;
}

