/////////////////////////////////////////////////////////////
//
//  FUNCTIONS LIBRARY
//	for Amazfit Bip fw.latin 1.1.2.05
//  by Maxim Volkov mr-volkov@yandex.ru 2019
//  
//
/////////////////////////////////////////////////////////////

#ifndef __LIBBIP_H__
#define __LIBBIP_H__

#include "../common.h"

// шрифт
#define FONT_HEIGHT		20

// ресурсы
#define	BIG_COLON		573
#define ICON_RECYCLE	293
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
#define MENU_ITEM_ARROW	72
#define ICON_DISABLED	75
#define ICON_TOGGLE_ON	77
#define ICON_TOGGLE_OFF	76
#define MENU_ITEM_LOCKED 103
#define ICON_SUNNY		71

// Переменные в памяти 1.1.2.05
#define 	current_screen 				*((unsigned char *)	0x20009768)
#define 	swipe_screen_active_number	*((unsigned char *)	0x20009769)
#define 	menu_overlay				*((unsigned char *)	0x2000976A)
#define 	menu_dispatch_func         	*((int *)			0x2000976C)          
#define 	key_press_func             	*((int *)			0x20009770)          
#define 	screen_job_func            	*((int *)			0x20009774)          
#define 	temp_buf_1                 	((void *)			0x20009778)          
#define 	show_menu_func             	*((int *)			0x2000977C)          
#define 	temp_buf_2                 	((void *)			0x20009780)          
#define 	long_key_press_func        	*((int *)			0x20009784)


#define		left_side_menu_active		*((unsigned char *)	0x10000619)

// свободная память
#define 	free_ram					*((unsigned int *)	0x1000074C) 
#define 	min_free_mem				*((unsigned int *)	0x10000750) 
#define 	free_list_p							((void*) 	0x10000758) 

// эта переменная используется как глобальная для AliPay, если алипэй не используется то можео взять ее
#define 	launch_elf_buffer			((void *)			0x2000055C)
//буффер экрана
#define 	screen_memory				((void *)			0x100026C8)



#endif

