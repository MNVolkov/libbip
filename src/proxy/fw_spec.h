/////////////////////////////////////////////////////////////
//
//  FUNCTIONS LIBRARY v.1.0 
//	for Amazfit Bip
//  (C) Maxim Volkov  2019 <Maxim.N.Volkov@ya.ru>
//	Универсальная библиотека функций для загрузчика приложений
//  
/////////////////////////////////////////////////////////////
/****
v.1.0 
-	создана первая версия библиотеки

*****/



#ifndef __LIBBIP_H__
#define __LIBBIP_H__

//#include "../common.h"

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


// loader.c
extern int 		load_elf_by_index(int index_listed, void* ret_f, unsigned int ret_param0, int argc, void** argv);
extern int 		load_elf_by_name(char* name, void* ret_f, unsigned int ret_param0, int argc, void** argv);
extern void* 	init_elf_loader();
extern void 	elf_finish(void* addr);				


#endif

