#include "BG_Images.h"

//拓步中性
//#include "BG_tuobu_on.c"
//#include "BG_tuobu_off.c"

//#include "BG_tuobu11_on.c"
//#include "BG_tuobu11_off.c"

//#include "BG_tuobu22_on.c"
//#include "BG_tuobu22_off.c"

//#include "BG_tuobu33_on.c"
//#include "BG_tuobu33_off.c"

#include "welcome.c"
#include "Goodbye.c"

//开机logo
//#include "welcome.c"
//#include "Goodbye.c"

//互联移动
//#include "BG_HLYD_on.c"
//#include "BG_HLYD_off.c"

//奥尼中性
//#include "BG_aoni_on.c"
//#include "BG_aoni_off.c"
int Logo_getBGOpening_size(void)
{
	return sizeof(g_ucBGOpening);
}

int Logo_getBGGoodbye_size(void)
{
	return sizeof(g_ucBGGoodbye);
}
