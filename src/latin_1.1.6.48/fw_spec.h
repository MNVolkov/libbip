/////////////////////////////////////////////////////////////
//
//  FUNCTIONS LIBRARY
//	for Amazfit Bip fw.latin 1.1.6.48
//  by Maxim Volkov Maxim.N.Volkov@yandex.ru 2021
//  
//
/////////////////////////////////////////////////////////////

#ifndef __LIBBIP_H__
#define __LIBBIP_H__

//#include "../common.h"

// шрифт
#define FONT_HEIGHT		16

// ресурсы	v85 (1.1.6.48)
#define	BIG_COLON		595		//	может отличаться на разных версиях ресурсов
#define ICON_RECYCLE	293		//	ресурсы до 559 включительно одинаковые для всех версий
#define ICON_ALARM		227
#define	ICON_CALEND		228
#define ARROW_UP		318
#define ARROW_DOWN		316
#define CHECK_BOX_ON	74
#define CHECK_BOX_OFF	73
#define PAGE_UP_ARROW	408
#define PAGE_DOWN_ARROW	407
#define ICON_CANCEL_RED	416
#define ICON_OK_GREEN	417
#define ICON_DISABLED	75
#define ICON_TOGGLE_ON	77
#define ICON_TOGGLE_OFF	76
#define MENU_ITEM_LOCKED 103
#define ICON_SUNNY		71
#define MENU_ITEM_ARROW	72

// Переменные в памяти 1.1.6.48
#define 	current_screen 				*((unsigned char *)	0x10000C7C)
#define 	swipe_screen_active_number	*((unsigned char *)	0x10000C7D)
#define 	menu_overlay				*((unsigned char *)	0x10000C7E)
#define 	menu_dispatch_func         	*((int *)			0x10000C80)          
#define 	key_press_func             	*((int *)			0x10000C84)          
#define 	screen_job_func            	*((int *)			0x10000C88)          
#define 	temp_buf_1                 	((void *)			0x10000C8C)          
#define 	show_menu_func             	*((int *)			0x10000C90)          
#define 	temp_buf_2                 	((void *)			0x10000C94)          
#define 	long_key_press_func        	*((int *)			0x10000C98)

#define		left_side_menu_active		*((unsigned char *)	0x10000089)		//	data_activity_page_action_source	

// свободная память
#define 	free_ram					*((unsigned int *)	0x20000B7C) 	//	data_heap_4_xFreeBytesRemaining
#define 	min_free_mem				*((unsigned int *)	0x20000B80) 	//	data_heap_4_xMinimumEverFreeBytesRemaining
#define 	free_list_p							((void*) 	0x20000B88) 	//	data_heap_4_xStart

// эта переменная используется как глобальная для AliPay, если алипэй не используется то можео взять ее
#define 	launch_elf_buffer			((void *)			0x10000184)		//	 data.qr_encode.nEncodeVersion
 
//буффер экрана
#define 	screen_memory				((void *)			0x10001040)		//	bss_LCDConf_local_lcd


#endif

