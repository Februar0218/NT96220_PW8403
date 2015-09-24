#include "OV5653.h"
#include "debug.h"
const SENSOR_CMD OV5653_INI_REG[]=
{
    {0x3008 ,0x82},
    {0x3008 ,0x42},
    {0x300f ,0x8e},
    {0x3011 ,0x10},
    {0x3017 ,0x7f},
    {0x3018 ,0xfc},
    {0x3103 ,0x93},
    {0x3500 ,0x00},
    {0x3501 ,0x3},//{0x3501 ,0x15},//
    {0x3502 ,0xf0},
    {0x350b ,0x4},
    {0x3503 ,0x37},//;17//agc active timing
    {0x350A ,0x0},
    {0x350B ,0x10},//{0x350B ,0x7f},//
    {0x3600 ,0x50},
    {0x3601 ,0x0d},
    {0x3603 ,0xa7},
    {0x3604 ,0x50},
    {0x3605 ,0x4},
    {0x3606 ,0x3f},
    {0x3612 ,0x1a},
    {0x3615 ,0x50},
    {0x3620 ,0x56},
    {0x3630 ,0x22},
    {0x3631 ,0x22},
    {0x3632 ,0x55},
    {0x3702 ,0x3a},
    {0x3704 ,0x18},
    {0x3706 ,0x41},
    {0x370b ,0x40},
    {0x370e ,0x0},
    {0x3710 ,0x28},
    {0x3712 ,0x13},
    {0x3800 ,0x2},
    {0x3801 ,0x54},
    {0x3810 ,0x40},
    {0x3815 ,0x82},
    {0x3830 ,0x50},
    {0x3836 ,0x41},
    {0x3a00 ,0x38},
    {0x3a18 ,0x0},
    {0x3a19 ,0xf8},
    {0x3a1a ,0x6},
    {0x3c01 ,0x80},
    {0x4000 ,0x1},//{0x4000 ,0x1},
    {0x401c ,0x48},
    {0x401d ,0x28}, //**
    {0x5000 ,0x06},
    {0x5001 ,0x0},
    {0x503d ,0x0},
    {0x5046 ,0x1},
    {0x3406 ,0x1},
    {0x3400 ,0x4},
    {0x3401 ,0x0},
    {0x3402 ,0x4},
    {0x3403 ,0x0},
    {0x3404 ,0x4},
    {0x3405 ,0x0},
    {0x3010 ,0x30},
    {0x3613 ,0xc4}, //**
    {0x3621 ,0xef},//[6] =0,Hor bin sum,[6]=1,hor skip
    {0x3703 ,0x9a},
    {0x3705 ,0xdc},
    {0x370a ,0x81},
    {0x370c ,0xc8},
    {0x370d ,0x42},
    {0x3713 ,0x2d},
    {0x3803 ,0xa},
    {0x3804 ,0x5},
    {0x3805 ,0x0},
    {0x3806 ,0x1},
    {0x3807 ,0xe4},//;e0
    {0x3808 ,0x2},
    {0x3809 ,0x80},
    {0x380a ,0x1},
    {0x380b ,0xe4},//;e0
    {0x380C ,0x9},//;8
    {0x380D ,0x90},//;38
    {0x380E ,0x2},
    {0x380F ,0x8e},//;F8
    {0x3818 ,0xc2},
    {0x381a ,0x0},
    {0x381c ,0x20},
    {0x381d ,0xa},
    {0x381e ,0x1},
    {0x381f ,0x20},
    {0x3820 ,0x0},
    {0x3821 ,0x0},
    {0x3824 ,0x23},
    {0x3825 ,0x26},
    {0x5002 ,0x2},
    {0x5901 ,0x4},
    {0x4700 ,0x4},//;Sync mode
    //{0x4708 ,0x7},//;Vsync, HREF, PCLK polarity, inverse signal
    {0x3008 ,0x2},
    {0x350d ,0x0},
    {0x350c ,0x0},
    {0x3604 ,0x40},
    {0x3631 ,0x36},
    {0x3632 ,0x5f},
    {0x3711 ,0x24},
    {0x3713 ,0x92},
    {0x3714 ,0x17},
};


const SENSOR_CMD OV5653_REG_VGA[]=
{
    {0x3011, 0x10 },
    {0x401d, 0x08 },
    {0x3613, 0x44 },
    {0x3621, 0xaf },
    {0x3703, 0x9a },
    {0x3705, 0xdc },
    {0x370a, 0x81 },
    {0x370c, 0xc8 },
    {0x370d, 0x42 },
    {0x3713, 0x2d },
    {0x3803, 0xa  },
    {0x3804, 0x2  },
    {0x3805, 0x80 },
    {0x3806, 0x1  },
    {0x3807, 0xe4 },
    {0x3808, 0x2  },
    {0x3809, 0x80 },
    {0x380a, 0x1  },
    {0x380b, 0xe4 },
    {0x380C, 0x9  },
    {0x380D, 0x90 },
    {0x380E, 0x2  },
    {0x380F, 0x8e },
    {0x3818, 0xc2 },
    {0x381a, 0x0  },
    {0x381c, 0x20 },
    {0x381d, 0xa  },
    {0x381e, 0x1  },
    {0x381f, 0x20 },
    {0x3820, 0x0  },
    {0x3821, 0x0  },
    {0x3824, 0x23 },
    {0x3825, 0x26 },
    {0x3010, 0x10 },
    {0x5002, 0x2 },
    {0x5901, 0x4 },
    {0x3713, 0x92},
    {0x3714, 0x17},
};

const SENSOR_CMD OV5653_REG_1280X720[]=
{
    {0x3011, 0x17 },
    {0x401d, 0x08 },
    {0x3613, 0x44 },
    {0x3621, 0xaf },
    {0x3703, 0x9a },
    {0x3705, 0xda },
    {0x370a, 0x80 },
    {0x370c, 0xc5 },
    {0x370d, 0x42 },
    {0x3713, 0x2d },
    {0x3803, 0xe  },
    {0x3804, 0x5  },
    {0x3805, 0x00 },
    {0x3806, 0x2  },
    {0x3807, 0xd0 },
    {0x3808, 0x5  },
    {0x3809, 0x00 },
    {0x380a, 0x2  },
    {0x380b, 0xd0 },
    {0x380C, 0x8  },
    {0x380D, 0x60 },
    {0x380E, 0x4  },
    {0x380F, 0x2a },
    {0x3818, 0xc1 },
    {0x381a, 0x3c },
    {0x381c, 0x30 },
    {0x381d, 0xee },
    {0x381e, 0x5  },
    {0x381f, 0xc8 },
    {0x3820, 0x0  },
    {0x3821, 0x20 },
    {0x3824, 0x23 },
    {0x3825, 0x2A },
    {0x3010, 0x30 },
    {0x5002, 0x0 },
    {0x5901, 0x0 },
    {0x3713, 0x92},
    {0x3714, 0x17},
};

const SENSOR_CMD OV5653_REG_1280X720P60[]=
{
//PLL setting
    {0x3008, 0x82},
    {0x3008, 0x42},
    {0x3103, 0x93},
    {0x3b07, 0x0c},
    {0x3017, 0xff},
    {0x3018, 0xfc},
    {0x3706, 0x41},
    {0x3630, 0x22},
    {0x3605, 0x4},
    {0x3606, 0x3f},
    {0x3712, 0x13},
    {0x370e, 0x0},
    {0x370b, 0x40},
    {0x3600, 0x54},
    {0x3601, 0x5},
    {0x3631, 0x22},
    {0x3612, 0x1a},
    {0x3604, 0x40},
    {0x3710, 0x28},
    {0x3702, 0x3a},
    {0x3704, 0x18},
    {0x3a18, 0x0},
    {0x3a19, 0xf8},
    {0x3a00, 0x38},
    {0x3830, 0x50},
    {0x3a08, 0x12},
    {0x3a09, 0x70},
    {0x3a0a, 0x0f},
    {0x3a0b, 0x60},
    {0x3a0d, 0x6},
    {0x3a0e, 0x6},
    {0x3a13, 0x54},
    {0x3815, 0x82},
    {0x5059, 0x80},
    {0x3615, 0x52},
    {0x3a08, 0x16},
    {0x3a09, 0x48},
    {0x3a0a, 0x12},
    {0x3a0b, 0x90},
    {0x3a0d, 0x3},
    {0x3a0e, 0x3},
    {0x3a1a, 0x6},
    {0x3623, 0x1},
    {0x3633, 0x24},
    {0x3c01, 0x34},
    {0x3c04, 0x28},
    {0x3c05, 0x98},
    {0x3c07, 0x7},
    {0x3c09, 0xc2},
    {0x4000, 0x5},
    {0x401d, 0x28},
    {0x4001, 0x2},
    {0x401c, 0x46},
    {0x5046, 0x1},
    {0x3810, 0x40},
    {0x3836, 0x41},
    {0x505f, 0x4},
    {0x5000, 0x0},
    {0x5001, 0x0},
    {0x503d, 0x0},
    {0x585a, 0x1},
    {0x585b, 0x2c},
    {0x585c, 0x1},
    {0x585d, 0x93},
    {0x585e, 0x1},
    {0x585f, 0x90},
    {0x5860, 0x1},
    {0x5861, 0x0d},
    {0x5180, 0xc0},
    {0x5184, 0x0},
    {0x470a, 0x0},
    {0x470b, 0x0},
    {0x470c, 0x0},
    {0x300f, 0x8e},
    {0x3603, 0xa7},
    {0x3632, 0x55},
    {0x3620, 0x56},
    {0x3631, 0x36},
    {0x3632, 0x5f},
    {0x3711, 0x24},
    {0x401f, 0x3},
    {0x3010, 0x10},
    {0x3011, 0x10},
    {0x3406, 0x1},
    {0x3400, 0x4},
    {0x3401, 0x0},
    {0x3402, 0x4},
    {0x3403, 0x0},
    {0x3404, 0x4},
    {0x3405, 0x0},
    {0x3503, 0x37},
    {0x3500, 0x0},
    {0x3501, 0x18},
    {0x3502, 0x0},
    {0x350a, 0x0},
    {0x350b, 0x38},
    {0x4704, 0x0},
    {0x4708, 0x1},
    {0x3008, 0x2},
// size setting
    {0x3800,0x2},
    {0x3801,0x22},
    {0x3803,0xc},
    {0x3804,0x5},
    {0x3805,0x0},
    {0x3806,0x2},
    {0x3807,0xD0},
    {0x3808,0x5},
    {0x3809,0x0},
    {0x380A,0x2},
    {0x380B,0xD0},
    {0x380C,0x8},
    {0x380D,0x7D},
    {0x380E,0x2},
    {0x380F,0xE0},
    {0x3815,0x81},
    {0x3818,0xC1},
    {0x381A,0x0},
    {0x381C,0x31},
    {0x381D,0x2},
    {0x381E,0x5},
    {0x381F,0xC0},
    {0x3820,0x0},
    {0x3821,0x20},
    {0x3824,0x23},
    {0x3825,0x2A},
    {0x3613,0xC4},
    {0x3621,0xAF},
    {0x3703,0x9A},
    {0x3705,0xDB},
    {0x370A,0x81},
    {0x370C,0x0},
    {0x370D,0x42},
    {0x3713,0x92},
    {0x3714,0x17},
    {0x401C,0x42},
    {0x5002,0x0},
    {0x505A,0xA},
    {0x505B,0x2E},
    {0x5901,0x0},
    {0x3010,0x10},
    {0x3011,0x10},
};

const SENSOR_CMD OV5653_REG_1280X720P50[]=
{
//PLL setting
    {0x3008, 0x82},
    {0x3008, 0x42},
    {0x3103, 0x93},
    {0x3b07, 0x0c},
    {0x3017, 0xff},
    {0x3018, 0xfc},
    {0x3706, 0x41},
    {0x3630, 0x22},
    {0x3605, 0x4},
    {0x3606, 0x3f},
    {0x3712, 0x13},
    {0x370e, 0x0},
    {0x370b, 0x40},
    {0x3600, 0x54},
    {0x3601, 0x5},
    {0x3631, 0x22},
    {0x3612, 0x1a},
    {0x3604, 0x40},
    {0x3710, 0x28},
    {0x3702, 0x3a},
    {0x3704, 0x18},
    {0x3a18, 0x0},
    {0x3a19, 0xf8},
    {0x3a00, 0x38},
    {0x3830, 0x50},
    {0x3a08, 0x12},
    {0x3a09, 0x70},
    {0x3a0a, 0x0f},
    {0x3a0b, 0x60},
    {0x3a0d, 0x6},
    {0x3a0e, 0x6},
    {0x3a13, 0x54},
    {0x3815, 0x82},
    {0x5059, 0x80},
    {0x3615, 0x52},
    {0x3a08, 0x16},
    {0x3a09, 0x48},
    {0x3a0a, 0x12},
    {0x3a0b, 0x90},
    {0x3a0d, 0x3},
    {0x3a0e, 0x3},
    {0x3a1a, 0x6},
    {0x3623, 0x1},
    {0x3633, 0x24},
    {0x3c01, 0x34},
    {0x3c04, 0x28},
    {0x3c05, 0x98},
    {0x3c07, 0x7},
    {0x3c09, 0xc2},
    {0x4000, 0x5},
    {0x401d, 0x28},
    {0x4001, 0x2},
    {0x401c, 0x46},
    {0x5046, 0x1},
    {0x3810, 0x40},
    {0x3836, 0x41},
    {0x505f, 0x4},
    {0x5000, 0x0},
    {0x5001, 0x0},
    {0x503d, 0x0},
    {0x585a, 0x1},
    {0x585b, 0x2c},
    {0x585c, 0x1},
    {0x585d, 0x93},
    {0x585e, 0x1},
    {0x585f, 0x90},
    {0x5860, 0x1},
    {0x5861, 0x0d},
    {0x5180, 0xc0},
    {0x5184, 0x0},
    {0x470a, 0x0},
    {0x470b, 0x0},
    {0x470c, 0x0},
    {0x300f, 0x8e},
    {0x3603, 0xa7},
    {0x3632, 0x55},
    {0x3620, 0x56},
    {0x3631, 0x36},
    {0x3632, 0x5f},
    {0x3711, 0x24},
    {0x401f, 0x3},
    {0x3010, 0x10},
    {0x3011, 0x10},
    {0x3406, 0x1},
    {0x3400, 0x4},
    {0x3401, 0x0},
    {0x3402, 0x4},
    {0x3403, 0x0},
    {0x3404, 0x4},
    {0x3405, 0x0},
    {0x3503, 0x37},
    {0x3500, 0x0},
    {0x3501, 0x18},
    {0x3502, 0x0},
    {0x350a, 0x0},
    {0x350b, 0x38},
    {0x4704, 0x0},
    {0x4708, 0x1},
    {0x3008, 0x2},
// size setting
    {0x3800,0x2},
    {0x3801,0x22},
    {0x3803,0xc},
    {0x3804,0x5},
    {0x3805,0x0},
    {0x3806,0x2},
    {0x3807,0xD0},
    {0x3808,0x5},
    {0x3809,0x0},
    {0x380A,0x2},
    {0x380B,0xD0},
    {0x380C,0xA},
    {0x380D,0x30},
    {0x380E,0x2},
    {0x380F,0xE0},
    {0x3815,0x81},
    {0x3818,0xC1},
    {0x381A,0x0},
    {0x381C,0x31},
    {0x381D,0x2},
    {0x381E,0x5},
    {0x381F,0xC0},
    {0x3820,0x0},
    {0x3821,0x20},
    {0x3824,0x23},
    {0x3825,0x2A},
    {0x3613,0xC4},
    {0x3621,0xAF},
    {0x3703,0x9A},
    {0x3705,0xDB},
    {0x370A,0x81},
    {0x370C,0x0},
    {0x370D,0x42},
    {0x3713,0x92},
    {0x3714,0x17},
    {0x401C,0x42},
    {0x5002,0x0},
    {0x505A,0xA},
    {0x505B,0x2E},
    {0x5901,0x0},
    {0x3010,0x10},
    {0x3011,0x10},
};


const SENSOR_CMD OV5653_REG_1280X960[]=
{
    {0x3011, 0x10},
    {0x401d, 0x08},
    {0x3613, 0x44},
    {0x3621, 0xaf},
    {0x3703, 0xb0},
    {0x3705, 0xda},
    {0x370a, 0x80},
    {0x370c, 0xc5},
    {0x370d, 0x42},
    {0x3713, 0x2f},
    {0x3803, 0x8 },
    {0x3804, 0x5 },
    {0x3805, 0x00},
    {0x3806, 0x3 },
    {0x3807, 0xc0},
    {0x3808, 0x5 },
    {0x3809, 0x00},
    {0x380a, 0x3 },
    {0x380b, 0xc0},
    {0x380C, 0x8 },
    {0x380D, 0x98},
    {0x380E, 0x3 },
    {0x380F, 0xca},
    {0x3818, 0xc1},
    {0x381a, 0x3c},
    {0x381c, 0x20},
    {0x381d, 0xa },
    {0x381e, 0x1 },
    {0x381f, 0x20},
    {0x3820, 0x0 },
    {0x3821, 0x0 },
    {0x3824, 0x23},
    {0x3825, 0x7A},
    {0x3010, 0x20},
    {0x5002, 0x0 },
    {0x5901, 0x0 },
    {0x3713, 0x92},
    {0x3714, 0x17},
};

const SENSOR_CMD OV5653_REG_1280X480[]=
{
    {0x3011, 0x0e},
    {0x401d, 0X08},
    {0x3613, 0xc4},
    {0x3621, 0xaf},
    {0x3703, 0xb0},
    {0x3705, 0xda},
    {0x370a, 0x80},
    {0x370c, 0xc5},
    {0x370d, 0x42},
    {0x3713, 0x2f},
    {0x3803, 0x08},
    {0x3804, 0x5 },
    {0x3805, 0x00},
    {0x3806, 0x1 },
    {0x3807, 0xe0},
    {0x3808, 0x5 },
    {0x3809, 0x00},
    {0x380a, 0x1 },
    {0x380b, 0xe0},
    {0x380C, 0x8 },
    {0x380D, 0x98},
    {0x380E, 0x1 },
    {0x380F, 0xea},
    {0x3818, 0xc2},
    {0x381a, 0x3c},
    {0x381c, 0x20},
    {0x381d, 0xa },
    {0x381e, 0x1 },
    {0x381f, 0x20},
    {0x3820, 0x0 },
    {0x3821, 0x0 },
    {0x3824, 0x23},
    {0x3825, 0x7A},
    {0x3010, 0x10},
    {0x5002, 0x0 },
    {0x5901, 0x0 },
    {0x3713, 0x92},
    {0x3714, 0x17},
    {0x3200, 0x0 },
    {0x3201, 0xff},
    {0x3202, 0xff},
    {0x3203, 0xff},
    {0x3212, 0x0 },
    {0x3613, 0x44},
    {0x3621, 0xaf},
    {0x3703, 0x98},
    {0x3705, 0xdc},
    {0x370a, 0x81},
    {0x370c, 0xc8},
    {0x370d, 0x42},
    {0x3713, 0x92},
    {0x3714, 0x17},
    {0x3803, 0x6 },
    {0x3804, 0x5 },
    {0x3805, 0x0 },
    {0x3806, 0x1 },
    {0x3807, 0xe0},
    {0x3808, 0x5 },
    {0x3809, 0x0 },
    {0x380a, 0x1 },
    {0x380b, 0xe0},
    {0x380C, 0x8 },
    {0x380D, 0xe7},
    {0x380E, 0x1 },
    {0x380F, 0xee},
    {0x3818, 0xc2},
    {0x381a, 0x0 },
    {0x381c, 0x20},
    {0x381d, 0xa },
    {0x381e, 0x1 },
    {0x381f, 0x20},
    {0x3820, 0x0 },
    {0x3821, 0x0 },
    {0x3824, 0x23},
    {0x3825, 0x18},
    {0x3010, 0x0 },
    {0x3011, 0x0e},
    {0x5002, 0x0 },
    {0x5901, 0x0 },
    {0x3010, 0x10},
    {0x3212, 0x10},
    {0x3212, 0xa0},
};
const SENSOR_CMD OV5653_REG_2560X960[]=
{
    {0x3011, 0x0E},
    {0x401d, 0X08},
    {0x3613, 0x44},
    {0x3621, 0x2f},
    {0x3703, 0xe6},
    {0x3705, 0xda},
    {0x370a, 0x80},
    {0x370c, 0xa0},
    {0x370d, 0x4 },
    {0x3713, 0x2f},
    {0x3803, 0xe },
    {0x3804, 0xa },
    {0x3805, 0x00},
    {0x3806, 0x3 },
    {0x3807, 0xc0},
    {0x3808, 0xa },
    {0x3809, 0x00},
    {0x380a, 0x3 },
    {0x380b, 0xc0},
    {0x380C, 0x13},
    {0x380D, 0x20},
    {0x380E, 0x3 },
    {0x380F, 0xd0},
    {0x3818, 0xc1},
    {0x381a, 0x3c},
    {0x381c, 0x20},
    {0x381d, 0xa },
    {0x381e, 0x1 },
    {0x381f, 0x20},
    {0x3820, 0x0 },
    {0x3821, 0x0 },
    {0x3824, 0x22},
    {0x3825, 0x52},
    {0x3010, 0x10},
    {0x5002, 0x0 },
    {0x5901, 0x0 },
    {0x3713, 0x22},
    {0x3714, 0x27},
};

const SENSOR_CMD OV5653_REG_FULL[]=
{
    {0x3011, 0x10 },
    {0x401d, 0x08 },
    {0x3613, 0x44 },
    {0x3621, 0x2f },
    {0x3703, 0xe6 },
    {0x3705, 0xda },
    {0x370a, 0x80 },
    {0x370c, 0xa0 },
    {0x370d, 0x4  },
    {0x3713, 0x2f },
    {0x3803, 0xe  },
    {0x3804, 0xA  },
    {0x3805, 0x20 },
    {0x3806, 0x7  },
    {0x3807, 0x98 },
    {0x3808, 0xA  },
    {0x3809, 0x20 },
    {0x380a, 0x7  },
    {0x380b, 0x98 },
    {0x380C, 0xc  },
    {0x380D, 0xb4 },
    {0x380E, 0x7  },
    {0x380F, 0xb0 },
    {0x3818, 0xc0 },
    {0x381a, 0x3c },
    {0x381c, 0x20 },
    {0x381d, 0xa  },
    {0x381e, 0x1  },
    {0x381f, 0x20 },
    {0x3820, 0x0  },
    {0x3821, 0x0  },
    {0x3824, 0x22 },
    {0x3825, 0x52 },
    {0x3010, 0x20 },
    {0x5002, 0x0  },
    {0x5901, 0x0  },
    {0x3713, 0x22 },
    {0x3714, 0x27 },
};

const SENSOR_CMD OV5653_REG_1920X1080[]=
{
    {0x3008,0x82},
    {0x3008,0x42},
    {0x3103,0x93},
    {0x3b07,0x0c},
    {0x3017,0xff},
    {0x3018,0xfc},
    {0x3706,0x41},
    {0x3703,0xe6},
    {0x3613,0x44},
    {0x3630,0x22},
    {0x3605,0x04},
    {0x3606,0x3f},
    {0x3712,0x13},
    {0x370e,0x00},
    {0x370b,0x40},
    {0x3600,0x54},
    {0x3601,0x05},
    {0x3713,0x22},
    {0x3714,0x27},
    {0x3631,0x22},
    {0x3612,0x1a},
    {0x3604,0x40},
    {0x3705,0xda},
    {0x370a,0x80},
    {0x370c,0x00},
    {0x3710,0x28},
    {0x3702,0x3a},
    {0x3704,0x18},
    {0x3a18,0x00},
    {0x3a19,0xf8},
    {0x3a00,0x38},
    {0x3800,0x02},
    {0x3801,0x54},
    {0x3803,0x0c},
    {0x380c,0x0c},
    {0x380d,0xb4},
    {0x380e,0x07},
    {0x380f,0xb0},
    {0x3830,0x50},
    {0x3a08,0x12},
    {0x3a09,0x70},
    {0x3a0a,0x0f},
    {0x3a0b,0x60},
    {0x3a0d,0x06},
    {0x3a0e,0x06},
    {0x3a13,0x54},
    {0x3815,0x82},
    {0x5059,0x80},
    {0x3615,0x52},
    {0x505a,0x0a},
    {0x505b,0x2e},
    {0x3803,0x0c},
    {0x3804,0x07},
    {0x3805,0x80},
    {0x3806,0x04},
    {0x3807,0x38},
    {0x3808,0x07},
    {0x3809,0x80},
    {0x380a,0x04},
    {0x380b,0x38},
    {0x380c,0x0D},// 0xa84 f0r P30, 0xD27 for P24
    {0x380d,0x27},
    {0x380e,0x04},
    {0x380f,0xa4},
    {0x3801,0x94},
    {0x381c,0x31},
    {0x381d,0xa4},
    {0x381e,0x04},
    {0x381f,0x60},
    {0x3820,0x03},
    {0x3821,0x1a},
    {0x3a08,0x16},
    {0x3a09,0x48},
    {0x3a0a,0x12},
    {0x3a0b,0x90},
    {0x3a0d,0x03},
    {0x3a0e,0x03},
    {0x3a1a,0x06},
    {0x3503,0x37},
    {0x3623,0x01},
    {0x3633,0x24},
    {0x3c01,0x34},
    {0x3c04,0x28},
    {0x3c05,0x98},
    {0x3c07,0x07},
    {0x3c09,0xc2},
    {0x4000,0x05},
    {0x401d,0x28},
    {0x4001,0x02},
    {0x401c,0x46},
    {0x5046,0x01},
    {0x3810,0x40},
    {0x3836,0x41},
    {0x505f,0x04},
    {0x5000,0x00},
    {0x5001,0x00},
    {0x5002,0x00},
    {0x503d,0x00},
    {0x5901,0x00},
    {0x585a,0x01},
    {0x585b,0x2c},
    {0x585c,0x01},
    {0x585d,0x93},
    {0x585e,0x01},
    {0x585f,0x90},
    {0x5860,0x01},
    {0x5861,0x0d},
    {0x5180,0xc0},
    {0x5184,0x00},
    {0x470a,0x00},
    {0x470b,0x00},
    {0x470c,0x00},
    {0x300f,0x8e},
    {0x3603,0xa7},
    {0x3632,0x55},
    {0x3620,0x56},
    {0x3621,0x2f},
    {0x381a,0x1c},
    {0x3818,0xc0},
    {0x3631,0x36},
    {0x3632,0x5f},
    {0x3711,0x24},
    {0x401f,0x03},
    {0x3008,0x02},
};

/**
* ISO to AGC usage
*/
#define AGC_ISO50       1100
#define ADJ_ISO         0
#define ISOBASE         50

/************** MONITOR_MODE_1(VGA) **************/

#define VD_Sync_Prv_1                 1
#define HD_Sync_Prv_1                 640
#define VD_Total_Prv_1                653//688//830
#define HD_Total_Prv_1                1220//1606

#define ACT_STRX_Prv_1                  8// 7
#define ACT_STRY_Prv_1                  2// 1

#define IMAGESIZE_H_PRV_1         624
#define IMAGESIZE_V_PRV_1         468
#define SIE_PRV_XCENTER_1         (ACT_STRX_Prv_1+(IMAGESIZE_H_PRV_1/2))
#define SIE_PRV_YCENTER_1         (ACT_STRY_Prv_1+(IMAGESIZE_V_PRV_1/2))

#define Padding_Prv_1               0

/************** MONITOR_MODE_2(720p) **************/
#define VD_Sync_Prv_2               1
#define HD_Sync_Prv_2               1281
#define VD_Total_Prv_2              1066//1060
#define HD_Total_Prv_2              2144//2120

#define ACT_STRX_Prv_2              8
#define ACT_STRY_Prv_2              2

#define IMAGESIZE_H_PRV_2           1272
#define IMAGESIZE_V_PRV_2           716
#define SIE_PRV_XCENTER_2       (ACT_STRX_Prv_2+(IMAGESIZE_H_PRV_2/2))
#define SIE_PRV_YCENTER_2       (ACT_STRY_Prv_2+(IMAGESIZE_V_PRV_2/2))

#define Padding_Prv_2               0

/************** MONITOR_MODE_3(1280*960)**************/
#define VD_Sync_Prv_3               1
#define HD_Sync_Prv_3               1281

#define VD_Total_Prv_3              970
#define HD_Total_Prv_3              2200//2177


#define ACT_STRX_Prv_3              8
#define ACT_STRY_Prv_3              2

#define IMAGESIZE_H_PRV_3           1272
#define IMAGESIZE_V_PRV_3           956
#define SIE_PRV_XCENTER_3       (ACT_STRX_Prv_3+(IMAGESIZE_H_PRV_3/2))
#define SIE_PRV_YCENTER_3       (ACT_STRY_Prv_3+(IMAGESIZE_V_PRV_3/2))


#define Padding_Prv_3               0

/************** MONITOR_MODE_4(1280*480)**************/

#define VD_Sync_Prv_4               1
#define HD_Sync_Prv_4               1300

#define VD_Total_Prv_4              494//485
#define HD_Total_Prv_4              2279//2245


#define ACT_STRX_Prv_4              8
#define ACT_STRY_Prv_4              2

#define IMAGESIZE_H_PRV_4           1272
#define IMAGESIZE_V_PRV_4           476
#define SIE_PRV_XCENTER_4       (ACT_STRX_Prv_4+(IMAGESIZE_H_PRV_4/2))
#define SIE_PRV_YCENTER_4       (ACT_STRY_Prv_4+(IMAGESIZE_V_PRV_4/2))

#define Padding_Prv_4               0
/************** MONITOR_MODE_5(2560*960)**************/
#define VD_Sync_Prv_5               1
#define HD_Sync_Prv_5               2572

#define VD_Total_Prv_5              976
#define HD_Total_Prv_5              4880//4924


#define ACT_STRX_Prv_5              8
#define ACT_STRY_Prv_5              2

#define IMAGESIZE_H_PRV_5           2556
#define IMAGESIZE_V_PRV_5           956
#define SIE_PRV_XCENTER_5       (ACT_STRX_Prv_5+(IMAGESIZE_H_PRV_5/2))
#define SIE_PRV_YCENTER_5       (ACT_STRY_Prv_5+(IMAGESIZE_V_PRV_5/2))

#define Padding_Prv_5               0

/************** MONITOR_MODE_6(720p60) **************/
#define VD_Sync_Prv_6               1
#define HD_Sync_Prv_6               10
#define VD_Total_Prv_6              736//1060
#define HD_Total_Prv_6              2173//2120

#define ACT_STRX_Prv_6              8
#define ACT_STRY_Prv_6              4

#define IMAGESIZE_H_PRV_6           1264
#define IMAGESIZE_V_PRV_6           712
#define SIE_PRV_XCENTER_6       (ACT_STRX_Prv_6+(IMAGESIZE_H_PRV_6/2))
#define SIE_PRV_YCENTER_6       (ACT_STRY_Prv_6+(IMAGESIZE_V_PRV_6/2))

#define Padding_Prv_6               0

/************** MONITOR_MODE_7(720p60) **************/
#define VD_Sync_Prv_7               1
#define HD_Sync_Prv_7               10
#define VD_Total_Prv_7              736//1060
#define HD_Total_Prv_7              2608//2120

#define ACT_STRX_Prv_7              8
#define ACT_STRY_Prv_7              4

#define IMAGESIZE_H_PRV_7           1264
#define IMAGESIZE_V_PRV_7           712
#define SIE_PRV_XCENTER_7       (ACT_STRX_Prv_7+(IMAGESIZE_H_PRV_7/2))
#define SIE_PRV_YCENTER_7       (ACT_STRY_Prv_7+(IMAGESIZE_V_PRV_7/2))

#define Padding_Prv_7               0

/************** MONITOR_MODE_8(1080p24) **************/
#define VD_Sync_Prv_8               1
#define HD_Sync_Prv_8               10
#define VD_Total_Prv_8              1188
#define HD_Total_Prv_8              3367// 2692(P30)

#define ACT_STRX_Prv_8              8
#define ACT_STRY_Prv_8              4

#define IMAGESIZE_H_PRV_8           1904
#define IMAGESIZE_V_PRV_8           1068
#define SIE_PRV_XCENTER_8       (ACT_STRX_Prv_8+(IMAGESIZE_H_PRV_8/2))
#define SIE_PRV_YCENTER_8       (ACT_STRY_Prv_8+(IMAGESIZE_V_PRV_8/2))

#define Padding_Prv_8               0

/************** STILL_MODE **************/
#define VD_Sync_Cap             2
#define HD_Sync_Cap             2592
#define VD_Total_Cap            1968//1975
#define HD_Total_Cap            3252//3247

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

#define OB_OFS_PRV      0x18//10 bit
#define OB_OFS_CAP      0x18//10 bit

/************** SIE CFA pattern selection **************/
#define CFAPAT_PRV    SIECFA_Gr
#define CFAPAT_CAP    SIECFA_Gr

/*********************************************/
//unit:mhz
#define PCLK_1      48
#define PCLK_2      69
#define PCLK_3      64
#define PCLK_4      69
#define PCLK_5      70
#define PCLK_6      96
#define PCLK_7      96
#define PCLK_8      96
#define PCLK_STILL  64

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

/* 1280x720 P60*/
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

/* 1280x720 P50*/
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

/* 1920x1080 P30*/
SYNC_INFO prv_mode8 = {
    ACT_STRX_Prv_8, // act_start_x;
    ACT_STRY_Prv_8, // act_start_y;
    SIE_PRV_XCENTER_8, // act_center_x;
    SIE_PRV_YCENTER_8, // act_center_y;
    IMAGESIZE_H_PRV_8  , // Act_SizeX;
    IMAGESIZE_V_PRV_8, // Act_SizeY;

    // HDVD_Sync
    HD_Sync_Prv_8, //    HD_sync
    VD_Sync_Prv_8, //    VD_sync
    HD_Total_Prv_8, //    HD_total
    VD_Total_Prv_8, //    VD_total

    // ob_area
    PRV_OB_X0, //    OB_X0
    PRV_OB_Y0, //    OB_Y0
    PRV_OB_X1, //    OB_X1
    PRV_OB_Y1, //    OB_Y1
    PRV_OB_THRES, //    OB_Thres
    PRV_OB_SUBSAMPLE, //    OB_SubSample
    PRV_OB_GAIN, // OB_Gain
    OB_OFS_PRV, // OB_Offset;

    Padding_Prv_8 // padding;
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

PREVIEW_DATA_INFO OV5653_prv_mode[] = {
    { // SEN_MODE_VGA
        2, //  frame_rate
        CFAPAT_PRV, //  bayer_pattern
        2, // bin_max
        1, // skip_max
        PCLK_1, // pclk
        //&prv_mode1, // prv_sync_info
        //(SENSOR_CMD*)OV5653_REG_VGA,
        //sizeof(OV5653_REG_VGA)/sizeof(SENSOR_CMD)
    },

    { // SEN_MODE_720P
        2, //  frame_rate
        CFAPAT_PRV, //  bayer_pattern
        2, // bin_max
        1, // skip_max
        PCLK_2, // pclk
        //&prv_mode2, // prv_sync_info
        //(SENSOR_CMD*)OV5653_REG_1280X720,
        //sizeof(OV5653_REG_1280X720)/sizeof(SENSOR_CMD)
    },

    { // SEN_MODE_1280_960
        2, //  frame_rate
        CFAPAT_PRV, //  bayer_pattern
        2, // bin_max
        1, // skip_max
        PCLK_3, // pclk
        //&prv_mode3, // prv_sync_info
        //(SENSOR_CMD*)OV5653_REG_1280X960,
        //sizeof(OV5653_REG_1280X960)/sizeof(SENSOR_CMD)
    },

    { // SEN_MODE_1280_480
        2, //  frame_rate
        CFAPAT_PRV, //  bayer_pattern
        2, // bin_max
        1, // skip_max
        PCLK_4, // pclk
        //&prv_mode4, // prv_sync_info
        //(SENSOR_CMD*)OV5653_REG_1280X480,
        //sizeof(OV5653_REG_1280X480)/sizeof(SENSOR_CMD)
    },

    { // SEN_MODE_2560_960
        2, //  frame_rate
        CFAPAT_PRV, //  bayer_pattern
        1, // bin_max
        1, // skip_max
        PCLK_5, // pclk
        //&prv_mode5, // prv_sync_info
        //(SENSOR_CMD*)OV5653_REG_2560X960,
        //sizeof(OV5653_REG_2560X960)/sizeof(SENSOR_CMD)
    },

    { // SEN_MODE_720P60
        2, //  frame_rate
        CFAPAT_PRV, //  bayer_pattern
        2, // bin_max
        1, // skip_max
        PCLK_6, // pclk
        //&prv_mode6, // prv_sync_info
        //(SENSOR_CMD*)OV5653_REG_1280X720P60,
        //sizeof(OV5653_REG_1280X720P60)/sizeof(SENSOR_CMD)
    },

    { // SEN_MODE_720P50
        2, //  frame_rate
        CFAPAT_PRV, //  bayer_pattern
        2, // bin_max
        1, // skip_max
        PCLK_7, // pclk
        //&prv_mode6, // prv_sync_info
        //(SENSOR_CMD*)OV5653_REG_1280X720P60,
        //sizeof(OV5653_REG_1280X720P60)/sizeof(SENSOR_CMD)
    },
    
    { // SEN_MODE_720P50
        2, //  frame_rate
        CFAPAT_PRV, //  bayer_pattern
        1, // bin_max
        1, // skip_max
        PCLK_8, // pclk
        //&prv_mode6, // prv_sync_info
        //(SENSOR_CMD*)OV5653_REG_1280X720P60,
        //sizeof(OV5653_REG_1280X720P60)/sizeof(SENSOR_CMD)
    }

};

CAPTURE_DATA_INFO OV5653_still_mode[] = {
    {
        2592, // cap_width
        1944, // cap_height
        CFAPAT_CAP, // bayer_pattern
        PCLK_STILL, // pclk
        //&still_mode, // cap_sync_info;
        //(SENSOR_CMD*)OV5653_REG_FULL,
        //sizeof(OV5653_REG_FULL)/sizeof(SENSOR_CMD)
    }
};

extern void OV5653_GetRegInfo(OV5653_MODE mode,UINT32* addr,UINT32* size)
{
    switch(mode)
    {
        case OV5653_INIT:
            *addr = (UINT32)OV5653_INI_REG;
            *size = (sizeof(OV5653_INI_REG)/sizeof(SENSOR_CMD));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(INIT)));
            break;
        case OV5653_VGA:
            *addr = (UINT32)OV5653_REG_VGA;
            *size = (sizeof(OV5653_REG_VGA)/sizeof(SENSOR_CMD));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(VGA)));
            break;
        case OV5653_1280X720:
            *addr = (UINT32)OV5653_REG_1280X720;
            *size = (sizeof(OV5653_REG_1280X720)/sizeof(SENSOR_CMD));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(1280X720)));
            break;
        case OV5653_1280X720P50:
            *addr = (UINT32)OV5653_REG_1280X720P50;
            *size = (sizeof(OV5653_REG_1280X720P50)/sizeof(SENSOR_CMD));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(1280X720)));
            break;
        case OV5653_1280X720P60:
            *addr = (UINT32)OV5653_REG_1280X720P60;
            *size = (sizeof(OV5653_REG_1280X720P60)/sizeof(SENSOR_CMD));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(1280X720)));
            break;
        case OV5653_1280X960:
            *addr = (UINT32)OV5653_REG_1280X960;
            *size = (sizeof(OV5653_REG_1280X960)/sizeof(SENSOR_CMD));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(1280X960)));
            break;
        case OV5653_1280X480:
            *addr = (UINT32)OV5653_REG_1280X480;
            *size = (sizeof(OV5653_REG_1280X480)/sizeof(SENSOR_CMD));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(1280X480)));
            break;
        case OV5653_2560X960:
            *addr = (UINT32)OV5653_REG_2560X960;
            *size = (sizeof(OV5653_REG_2560X960)/sizeof(SENSOR_CMD));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(2560X960)));
            break;
        case OV5653_1920X1080:
            *addr = (UINT32)OV5653_REG_1920X1080;
            *size = (sizeof(OV5653_REG_1920X1080)/sizeof(SENSOR_CMD));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(2560X960)));
            break;
        case OV5653_STILL:
            *addr = (UINT32)OV5653_REG_FULL;
            *size = (sizeof(OV5653_REG_FULL)/sizeof(SENSOR_CMD));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(FULL)));
            break;
        default:
            debug_err(("^R %s: mode error(default)",__func__));
            break;
    }
}

extern void OV5653_GetSigInfo(OV5653_MODE mode,UINT32* addr,UINT32* size)
{
    switch(mode)
    {
        case OV5653_VGA:
            *addr = (UINT32)&prv_mode1;
            //*size = (sizeof(prv_mode1)/sizeof(SYNC_INFO));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(VGA)));
            break;
        case OV5653_1280X720:
            *addr = (UINT32)&prv_mode2;
            //*size = (sizeof(prv_mode2)/sizeof(SYNC_INFO));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(1280X720)));
            break;
        case OV5653_1280X720P50:
            *addr = (UINT32)&prv_mode7;//(UINT32)&prv_mode6;
            //*size = (sizeof(prv_mode2)/sizeof(SYNC_INFO));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(1280X720)));
            break;
        case OV5653_1280X720P60:
            *addr = (UINT32)&prv_mode6;
            //*size = (sizeof(prv_mode2)/sizeof(SYNC_INFO));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(1280X720)));
            break;
        case OV5653_1280X960:
            *addr = (UINT32)&prv_mode3;
            //*size = (sizeof(prv_mode3)/sizeof(SYNC_INFO));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(1280X960)));
            break;
        case OV5653_1280X480:
            *addr = (UINT32)&prv_mode4;
            //*size = (sizeof(prv_mode4)/sizeof(SYNC_INFO));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(1280X480)));
            break;
        case OV5653_2560X960:
            *addr = (UINT32)&prv_mode5;
            //*size = (sizeof(prv_mode5)/sizeof(SYNC_INFO));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(2560X960)));
            break;
        case OV5653_1920X1080:
            *addr = (UINT32)&prv_mode8;
            //*size = (sizeof(prv_mode8)/sizeof(SYNC_INFO));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(1920X1080)));
            break;
        case OV5653_STILL:
            *addr = (UINT32)&still_mode;
            //*size = (sizeof(still_mode)/sizeof(SYNC_INFO));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(FULL)));
            break;
        case OV5653_INIT:
            //debug_err(("^R %s: mode error(%s)",__func__,Token(INIT)));
        default:
            debug_err(("^R %s: mode error(default)",__func__));
            break;
    }
}

extern void OV5653_GetDataInfo(OV5653_MODE mode,UINT32* addr,UINT32* size)
{
    switch(mode)
    {
        case OV5653_VGA:
            *addr = (UINT32)&OV5653_prv_mode[0];
            //*size = (sizeof(OV5653_prv_mode[0])/sizeof(PREVIEW_DATA_INFO));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(VGA)));
            break;
        case OV5653_1280X720:
            *addr = (UINT32)&OV5653_prv_mode[1];
            //*size = (sizeof(OV5653_prv_mode)/sizeof(PREVIEW_DATA_INFO));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(1280X720)));
            break;
        case OV5653_1280X720P50:
            *addr = (UINT32)&OV5653_prv_mode[6];//(UINT32)&OV5653_prv_mode[5];
            //*size = (sizeof(OV5653_prv_mode)/sizeof(PREVIEW_DATA_INFO));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(1280X720)));
            break;
        case OV5653_1280X720P60:
            *addr = (UINT32)&OV5653_prv_mode[5];
            //*size = (sizeof(OV5653_prv_mode)/sizeof(PREVIEW_DATA_INFO));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(1280X720)));
            break;
        case OV5653_1280X960:
            *addr = (UINT32)&OV5653_prv_mode[2];
            //*size = (sizeof(OV5653_prv_mode)/sizeof(PREVIEW_DATA_INFO));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(1280X960)));
            break;
        case OV5653_1280X480:
            *addr = (UINT32)&OV5653_prv_mode[3];
            //*size = (sizeof(OV5653_prv_mode)/sizeof(PREVIEW_DATA_INFO));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(1280X480)));
            break;
        case OV5653_2560X960:
            *addr = (UINT32)&OV5653_prv_mode[4];
            //*size = (sizeof(OV5653_prv_mode)/sizeof(PREVIEW_DATA_INFO));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(2560X960)));
            break;
        case OV5653_1920X1080:
            *addr = (UINT32)&OV5653_prv_mode[7];
            //*size = (sizeof(OV5653_prv_mode)/sizeof(PREVIEW_DATA_INFO));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(1920X1080)));
            break;
        case OV5653_STILL:
            *addr = (UINT32)&OV5653_still_mode;
            //*size = (sizeof(OV5653_still_mode)/sizeof(CAPTURE_DATA_INFO));
            //debug_err(("^R %s: mode error(%s)",__func__,Token(FULL)));
            break;
        case OV5653_INIT:
            //debug_err(("^R %s: mode error(%s)",__func__,Token(INIT)));
        default:
            debug_err(("^R %s: mode error(default)",__func__));
            break;
    }
}

extern void OV5653_GetISOInfo(UINT32 *ISOBase,UINT32 *AGCBase)
{
    *ISOBase = ISOBASE;
    *AGCBase = AGC_ISO50;
}

