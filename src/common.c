/////////////////////////////////////////////////////////////
//
//  COMMON FUNCTIONS LIBRARY
//	for Amazfit Bip
//  by Maxim Volkov mr-volkov@yandex.ru 2019
//  
/////////////////////////////////////////////////////////////

#include "common.h"
#include LIB_BIP_H

//#define DEBUG_LOG

#if FW_VERSION != UNI_LATIN

//========================================================================================================
//		Работа с языком
//========================================================================================================
int get_selected_locale(){

unsigned char flash_lang; 	//	язык во флэшпамяти

flash_lang = (unsigned char) get_stored_lang();

switch(flash_lang){	
	case OPT_LANG_RUS:  return	locale_ru_RU;
	case OPT_LANG_ENG:  return	locale_en_US;
	case OPT_LANG_ITA:  return	locale_it_IT;
	case OPT_LANG_SPA:  return	locale_es_ES;
	case OPT_LANG_FRA:  return  locale_fr_FR;
	case OPT_LANG_DEU:  return  locale_de_DE;
	case OPT_LANG_GRK:  return  locale_el_GR;
	
	case OPT_LANG_AUTO:
	default: return get_system_locale();
}
}

int get_system_locale(){
char locale[10];		//	ru_RU - язык телефона

get_generic_data(GENERIC_DATA_LOCALE, &locale);	

if ( !_memcmp(&locale, "ru_RU", 5) ){
	return locale_ru_RU;	
} else 
if ( !_memcmp(&locale, "it_IT", 5) ){
	return locale_it_IT;	
} else 
if ( !_memcmp(&locale, "es_ES", 5) ){
	return locale_es_ES;	
} else 
if ( !_memcmp(&locale, "fr_FR", 5) ){
	return locale_fr_FR;	
} else 
if ( !_memcmp(&locale, "en_EN", 5) ){
	return locale_en_US;	
} else 
if ( !_memcmp(&locale, "de_DE", 5) ){
	return locale_de_DE;	
} else
if ( !_memcmp(&locale, "el_GR", 5) ){
	return locale_el_GR;	
} else 
	 return locale_en_US;	

}

int get_stored_lang(){
unsigned char flash_lang; 	//	язык во флэшпамяти

// считаем опции из flash памяти, если значение в флэш-памяти некорректное то берем автовыбор
read_flash(OPTIONS_FLASH_ADDRESS + OPTIONS_OFFSET_LANGUAGE, &flash_lang, sizeof(flash_lang));

#ifdef DEBUG_LOG
	log_printf(5, "[get_stored_lang] Lang in flash %d; ", flash_lang);
#endif

// если язык во флэш памяти установлен неверно берем автонастройку
if (flash_lang > OPT_LANG_COUNT) 
	flash_lang = OPT_LANG_AUTO;

#ifdef DEBUG_LOG
	log_printf(5, "get_stored_lang = %d\r\n", flash_lang);
#endif

return flash_lang;
}


int store_selected_lang(int lang){
unsigned char flash_lang; 	//	язык во флэшпамяти

#ifdef DEBUG_LOG
	log_printf(5, "[store_selected_lang] Selected lang %d; ", lang);
#endif


// если язык во флэш памяти установлен неверно берем автонастройку
if (lang > OPT_LANG_COUNT) 
	flash_lang = OPT_LANG_AUTO;
else	
	flash_lang = (unsigned char) lang;

#ifdef DEBUG_LOG
	log_printf(5, "writing to flash %d\r\n", flash_lang);
#endif

// запись опций во flash память
return write_flash(OPTIONS_FLASH_ADDRESS + OPTIONS_OFFSET_LANGUAGE, &flash_lang, sizeof(flash_lang));

}


//==========================================================================================================
//		Прочие функции
//==========================================================================================================

void draw_progressbar(int pos_y, int width, int height, int color_bg, int color_fg, int progress_max, int progress, int border, int repaint, const char* text){
	
	progress = min(progress, progress_max);

	// рассчитаем ширину прогрессбара в пикселях
	int gauge = progress*width/progress_max;

	if (border){
		// нарисуем рамку прогрессбара
		set_fg_color(COLOR_WHITE);
		draw_filled_rect(88-(width+4)/2, pos_y-2, 88+(width+4)/2, pos_y-2+height+4);
	}
	// нарисуем фон прогрессбара
	set_fg_color(color_bg);
	draw_filled_rect(88-(width)/2, pos_y, 88+(width)/2, pos_y+height);

	// нарисуем сам прогрессбар
	set_fg_color(color_fg);
	draw_filled_rect(88-(width)/2, pos_y, 88-(width)/2+gauge, pos_y+height);

	if (text){
	set_bg_color(COLOR_BLACK);
	draw_filled_rect_bg(0,pos_y+height+2, 176, pos_y+height+4+get_text_height());
	set_fg_color(COLOR_AQUA);
	text_out_center(text, 88, pos_y+height+4);
	}

	if (repaint)
		repaint_screen_lines(pos_y-2, pos_y+height+2+get_text_height());
}
/*
// возвращает количество ресурсов NERES
int get_res_count(){
struct res_struct res;

read_flash(NERES_ADDRESS, &res, sizeof(struct res_struct));
return res.count;	
}
*/

/*
// чтение данных ресурса начиная с offset длиной len по его номеру res_id
int read_res_by_id (int res_id, int offset, void* buffer, int len){

if ( (offset<0) || (len<0) )
	return -4;

int res_addr, adr;
int res_count = get_res_count();

// считаем адрес ресурса из таблицы ресурсов
adr = NERES_ADDRESS+sizeof(struct res_struct)+res_id*4;
read_flash(adr, &res_addr, 4);
adr = res_addr + offset + NERES_ADDRESS+sizeof(struct res_struct)+res_count*4;
return read_flash(adr, buffer, len);
}

*/

// дамп памяти в лог в формате ti-txt (конвертация через srec_cat)

/*
TODO: переделать в формат base64 

пример кода:
static char encoding_table[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
                                'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
                                'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
                                'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
                                'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
                                'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
                                'w', 'x', 'y', 'z', '0', '1', '2', '3',
                                '4', '5', '6', '7', '8', '9', '+', '/'};
static char *decoding_table = NULL;
static int mod_table[] = {0, 2, 1};


char *base64_encode(const unsigned char *data,
                    size_t input_length,
                    size_t *output_length) {

    *output_length = 4 * ((input_length + 2) / 3);

    char *encoded_data = malloc(*output_length);
    if (encoded_data == NULL) return NULL;

    for (int i = 0, j = 0; i < input_length;) {

        uint32_t octet_a = i < input_length ? (unsigned char)data[i++] : 0;
        uint32_t octet_b = i < input_length ? (unsigned char)data[i++] : 0;
        uint32_t octet_c = i < input_length ? (unsigned char)data[i++] : 0;

        uint32_t triple = (octet_a << 0x10) + (octet_b << 0x08) + octet_c;

        encoded_data[j++] = encoding_table[(triple >> 3 * 6) & 0x3F];
        encoded_data[j++] = encoding_table[(triple >> 2 * 6) & 0x3F];
        encoded_data[j++] = encoding_table[(triple >> 1 * 6) & 0x3F];
        encoded_data[j++] = encoding_table[(triple >> 0 * 6) & 0x3F];
    }

    for (int i = 0; i < mod_table[input_length % 3]; i++)
        encoded_data[*output_length - 1 - i] = '=';

    return encoded_data;
}


*/

void dump_mem (void * address, unsigned long len){
 unsigned long addr = ( unsigned long )address;

	// SRAM1 0x20000000 - 0x20018000
	// SRAM2 0x10000000 - 0x10008000
	// FLASH 0x08000000 - 0x08007FFF
	
	 unsigned long ticks = get_tick_count();
	log_printf(5, "[%d] Dump memory at 0x%X, lenght %d block_size 64b, format ti-txt\r\n", ticks, addr, len);
	
	// unsigned long perc = len / 64 / 176;
	
	log_printf(5, "@0000\r\n");
	
	for (unsigned long i=addr; i<(addr+len); i+=1){
		//log_printf(5, "%X: ", i);
	    log_printf(5, "%.2X ", (unsigned char) *((unsigned char *)	(i)) );
		if ((i-addr+1)%32 == 0) 
			log_printf(5, "\r\n");
	}
	log_printf(5, "\r\nq\r\n");
	
	set_bg_color(COLOR_YELLOW);
	set_fg_color(COLOR_RED);
	draw_filled_rect_bg(0, 64, 176, 100);
	text_out_center("Дамп памяти", 88, 65);
	text_out_center(" завершен! ", 88, 80);
	repaint_screen_lines(65, 96);
	vibrate(1, 500, 0);
};

/**
	getColorFromShort
	Возвращает значение цвета из короткого формата вида COLOR_SH_WHITE
**/

int getColorFromShort(unsigned char short_color){
int color;
color  = (((unsigned int)short_color&(unsigned char)COLOR_SH_MASK)&COLOR_SH_RED)  ?COLOR_RED   :0;	//	составляющая красного цвета
color |= (((unsigned int)short_color&(unsigned char)COLOR_SH_MASK)&COLOR_SH_GREEN)?COLOR_GREEN :0;	//	составляющая зеленого цвета
color |= (((unsigned int)short_color&(unsigned char)COLOR_SH_MASK)&COLOR_SH_BLUE) ?COLOR_BLUE  :0;	//	составляющая синего цвета
return color;
}




//==========================================================================================================
//==========================================================================================================
//		Процедуры доступа к глобальным переменным прошивки
//=========================================================================================================
/*
#define __VAR(var_name, type)	type get_var_##var_name (){ return *(type*)var_name;};\
								type set_var_##var_name (type value){ *(type*)var_name=value;return value;};
#define __PTR(ptr_name, type)	type get_ptr_##ptr_name (){ return *(type*)ptr_name;};

__VAR(current_screen, unsigned char);
__VAR(swipe_screen_active_number, unsigned char);
__VAR(menu_overlay, unsigned char);
__PTR(menu_dispatch_func, int);
__PTR(key_press_func, int);
__PTR(screen_job_func, int);
__PTR(temp_buf_1, void*);
__PTR(show_menu_func, int);
__PTR(temp_buf_2, void*);
__PTR(long_key_press_func, int);
__VAR(left_side_menu_active, unsigned char);
__VAR(free_ram, unsigned int);

#undef GET_VAR
#undef GET_PTR
*/
unsigned char get_var_current_screen(){ return current_screen;}
unsigned char set_var_current_screen(unsigned char val){ return current_screen=val;} 			
unsigned char get_var_swipe_screen_active_number(){ return swipe_screen_active_number;}
unsigned char set_var_swipe_screen_active_number(unsigned char val){ return swipe_screen_active_number=val;}
unsigned char get_var_menu_overlay(){ return menu_overlay;}				
unsigned char set_var_menu_overlay(unsigned char val){ return menu_overlay=val;}				
		void* get_ptr_menu_dispatch_func(){ return (void*)menu_dispatch_func;}        
		void* set_ptr_menu_dispatch_func(void* val){ return (void*)(menu_dispatch_func=(int)val);}        
		void* get_ptr_key_press_func(){ return (void*)key_press_func;}            
		void* set_ptr_key_press_func(void* val){ return (void*)(key_press_func=(int)val);}            
		void* get_ptr_screen_job_func(){ return (void*)screen_job_func;}           
		void* set_ptr_screen_job_func(void* val){ return (void*)(screen_job_func=(int)val);}           
		void* get_ptr_temp_buf_1(){ return temp_buf_1;}                
		void* get_ptr_show_menu_func(){ return (void*)show_menu_func;}            
		void* set_ptr_show_menu_func(void* val){ return (void*)(show_menu_func=(int)val);}            
		void* get_ptr_temp_buf_2(){ return (void*)temp_buf_2;}                
		void* get_ptr_long_key_press_func(){ return (void*)long_key_press_func;}       
		void* set_ptr_long_key_press_func(void* val){ return (void*)(long_key_press_func=(int)val);}       
unsigned char get_var_left_side_menu_active(){ return left_side_menu_active;} 		
		  int get_var_free_ram(){ return free_ram;} 
		void* get_ptr_screen_memory(){ return (void*)screen_memory;}     
		void* get_ptr_free_list(){ return (void*)free_list_p;} 
		
//===========================================================================================================

/***
check_free_mem_block	-	проверяет наличие свободного блока памяти необходимого размера. память не выделяется

входные параметры:	
	size	-	размер блока памяти для проверки.
возыращаемое значение:
	0	-	необходимого блока нет
	1	-	необходмый блок есть

**/
int check_free_mem_block(int size){
struct mem_struct* block;

// цикл перебора свободных блоков памяти, в поисках блока достаточного размера
for (block = get_ptr_free_list() ; block ;block = block->next_addr ){
	if (block->size >= size) return 1;
}
return 0;
};


/***

void* 	pvPortMalloc (int size) - выделение блока памяти из кучи, с проверкой доступности блока необходимого размера
size - требуемый размер выделяемой памяти

выходные параметры: указатель на выделенный блок памяти, либо NULL в случае неудачи

***/


void* 	pvPortMalloc (int size){

// проверка наличия свободного блока памяти необходимого размера
if (check_free_mem_block(size)){
	//	в случае если необходимый блок найден выделяем память прошивочной функцией, возвращая указатель на выделенную память
	return _pvPortMalloc(size);
}

// если блока необходимого размера нет - выход с результатом NULL
return NULL;
}


//========================================================================================================
//		Работа с меню
//========================================================================================================
/***
Меню позволяет осуществлять навигацию по различным пунктам меню, каждый пункт которого может быть одним из нижеследующих типов:
	-	обычный пункт меню	значок ">". Запуск нового экрана через show_animate c параметром функции возврата
	-	обычный пункт меню	значок ">".	Запуск колбэк-функции с параметрами menu и index
	-	чекбокс не отмечен	значок "0".	Запуск колбэк-функции с параметрами menu и index
	-	чекбокс отмечен		значок "v".	Запуск колбэк-функции с параметрами menu и index
	-	переключатель включен	значок "( о)".	Запуск колбэк-функции с параметрами menu и index
	-	переключатель включен	значок "(о )".	Запуск колбэк-функции с параметрами menu и index
	-	заблокирован	значок "замок".	
	-	пункт в данный момент не доступен	значок "Х"

Для работы с меню необходимо создать новый экран функцией reg_menu, а также переменную struct_menu, остающуюся действующей в данном экране. 
Если переменная создается в динамическеой памяти, необходимо очистить память под struct_menu перед использованием. 
Функцией clear_menu можно очистить пункты меню. При этом история вложенности меню сохраняется.
Переменная menu.show_func содержит адрес функции текущего экрана меню. При создании меню необходимо присвоить show_func адрес функции создания
экрана. Для отображения меню необходимо создать новые пункты меню функцией add_menu_item. Меню отображается функцией draw_menu.
Для обработки нажатий на экран, свайпов и нажатий кнопки необходимо в эуранном обработчике dispatch_screen вызвать dispatch_menu с указанием параметра gesture.
При вызове dispatch_menu обрабатываются перелистывания страниц меню, выбор пунктов меню с вызовом назначенных колбэк функций.
В случае вложенных меню в колбэк функции вызывается clear_menu, затем создаются новые пункты меню, меню отображается функцией draw_menu.



**/
int add_menu_item(struct menu_struct *	menu, char* name, void* show_func, int color, int style){

// если превышен лимит количества пунктов меню выходим с ошибкой
if (menu->item_count >= MAX_MENU_ITEMS_COUNT)
	return -1;

// копируем имя пунка меню
/*
//	устаревший вариант
int str_len = _strlen(name);
_memcpy(&menu->items[menu->item_count].name, name, min(str_len, MAX_MENU_ITEM_NAME_LEN)+1);
menu->items[menu->item_count].name[min(str_len, MAX_MENU_ITEM_NAME_LEN)] = 0;
*/

// новый(старый) вариант, зодно учтем управляющие символы \n
//_sprintf(&menu->items[menu->item_count].name[0], name);

// новый динамический вариант хранения строк пунктов меню
int name_size = max(_strlen(name), MAX_MENU_ITEM_NAME_LEN-1)+1;	//	вычислим необходимый объем памяти под строку пункта меню и завершающий 0
menu->items[menu->item_count].name = pvPortMalloc(name_size);	//	выделение необходимой памяти
//_sprintf(menu->items[menu->item_count].name, name);			//	копирование наименования пункта в выделенную память
_strncpy(menu->items[menu->item_count].name, name, name_size);	//	копирование наименования пункта в выделенную память


// копируем функцию запуска
menu->items[menu->item_count].show_func = show_func;

// копируем цвет пункта меню
menu->items[menu->item_count].item_color = color;

// копируем стиль
menu->items[menu->item_count].item_style = style;

// увеличиваем счетчик количества пунктов
menu->item_count++;

#ifdef DEBUG_LOG
log_printf(5, "[add_menu_item] menu=0x%X; name=%s; show_func=0x%X; index=%d\r\n", 
				 menu, 
				 menu->items[menu->item_count-1].name, 
				 menu->items[menu->item_count-1].show_func, 
				 menu->item_count-1);
#endif

return menu->item_count-1;
}

int clear_menu (struct menu_struct *	menu){

// высвободить память под строковые ресурсы меню
for (int i = 0; i<menu->item_count; i++){
	if (menu->items[i].name){
		vPortFree(menu->items[i].name);
		menu->items[i].name = NULL;
	}
}

// очистим память под данные
//_memclr(menu, sizeof(struct menu_struct ));	
menu->page = 0;
menu->item_count = 0;

#ifdef DEBUG_LOG
log_printf(5, "[clear_menu] menu=0x%X; menu_level=%d \r\n", menu, menu->menu_level);
#endif

return 0;
}

int push_ret_f(struct menu_struct *	menu, void* ret_f){

if (menu->menu_level < MAX_MENU_ENCLOSURE){
	menu->ret_f[menu->menu_level] = ret_f;
	menu->menu_level++;	
	}

#ifdef DEBUG_LOG
log_printf(5, "[push_ret_f] menu=0x%X; menu_level=%d; ret_f=0x%X \r\n", menu, menu->menu_level, menu->ret_f[menu->menu_level-1]);
#endif

return menu->menu_level;
}

void* pop_ret_f(struct menu_struct *	menu){

void* result = NULL;
if (menu->menu_level > 0){
	result = menu->ret_f[menu->menu_level-1];
	menu->menu_level--;	
	}
	
#ifdef DEBUG_LOG
log_printf(5, "[pop_ret_f] menu=0x%X; menu_level=%d; ret_f=0x%X \r\n", menu, menu->menu_level, result);
#endif
	
return result;
}

void draw_menu(struct menu_struct *	menu){
//	подготовка главного меню 
set_bg_color(COLOR_BLACK);
fill_screen_bg();
set_fg_color(COLOR_WHITE);
set_graph_callback_to_ram_1();
load_font();

#ifdef DEBUG_LOG
log_printf(5, "[draw_menu] menu=0x%X; page=%d; count=%d\r\n", menu, menu->page, menu->item_count);
#endif

for (int i = 0; i < ITEMS_ON_PAGE; i++)
{	
	if ( (menu->page*ITEMS_ON_PAGE + i) < menu->item_count){		//	если есть такой пункт меню
		
#ifdef DEBUG_LOG
		log_printf(5, "	#%d; name=%s;\r\n", menu->page*ITEMS_ON_PAGE+i, menu->items[menu->page*ITEMS_ON_PAGE+i].name);
#endif
		
		
		// найдем все переводы строк и выведем каждую строку с новой строки
		char new_str[MAX_LINES_PER_ITEM][MAX_MENU_ITEM_NAME_LEN];
		_memclr(&new_str, sizeof(new_str));
		
		int string_count 	= 0;
		int str_start 		= 0;
		int str_end 		= 0;
			
		while (menu->items[menu->page*ITEMS_ON_PAGE+i].name[str_end]){
			if ( 	(menu->items[menu->page*ITEMS_ON_PAGE+i].name[str_end] == 10)||
					(menu->items[menu->page*ITEMS_ON_PAGE+i].name[str_end] == 13) ){
				if (string_count >= MAX_LINES_PER_ITEM-1){ 
					menu->items[menu->page*ITEMS_ON_PAGE+i].name[str_end] = 0x20; // заменяем перевод строки на пробел
				} else {
					_memcpy(&new_str[string_count][0], &menu->items[menu->page*ITEMS_ON_PAGE+i].name[str_start] , str_end - str_start);
					new_str[string_count][str_end] = 0;
					str_start = str_end+1;
					string_count++;
				}
				
			}
			str_end++;
		}
		
		_memcpy(&new_str[string_count][0], &menu->items[menu->page*ITEMS_ON_PAGE+i].name[str_start] , str_end - str_start);
		new_str[string_count][str_end] = 0;
		string_count++;
		
		//	вставляем название пункта меню
		set_fg_color(menu->items[menu->page*ITEMS_ON_PAGE+i].item_color);
	
		//	нумерация пунктов меню
		if (menu->item_count > ITEMS_ON_PAGE){
				char text[4];
				_sprintf(text, "%d", menu->page*ITEMS_ON_PAGE+i+1);
				text_out_font(	FONT_DIGIT_MICRO_13, text, 
								3, 
								4 + i*(176/ITEMS_ON_PAGE)+((176/ITEMS_ON_PAGE)-get_text_height()*string_count)/2,
								0);
			}
	
		for (int j=0;j<string_count;j++){
			text_out(	&new_str[j][0], 
						MENU_H_MARGIN, 		
						i*(176/ITEMS_ON_PAGE)+((176/ITEMS_ON_PAGE)-get_text_height()*string_count)/2 + j*get_text_height());
					
		}
		
		
		int 	icon_height = 0;
		int 	icon_width = 0;
		char 	menu_item_icon_res = 0;
		
		switch (menu->items[menu->page*ITEMS_ON_PAGE+i].item_style){
		case 	MENU_ITEM_STYLE_LIST:
		case	MENU_ITEM_STYLE_NORMAL:{
			menu_item_icon_res = MENU_ITEM_ARROW;// значок стрелочка вправо (пункт меню) 7*14
			icon_width	= 7;
			icon_height	= 14;
			break;
		}
		case	MENU_ITEM_STYLE_UNCHECKED:{
			menu_item_icon_res = CHECK_BOX_OFF;
			icon_width	= 25;
			icon_height	= 25;
			break;
		}
		case    MENU_ITEM_STYLE_CHECKED	:{
			menu_item_icon_res = CHECK_BOX_ON;
			icon_width	= 25;
			icon_height	= 25;
			break;
		}
		case    MENU_ITEM_STYLE_TOGGLE_ON:{
			menu_item_icon_res = ICON_TOGGLE_ON;
			icon_width	= 53;
			icon_height	= 26;
			break;
		}
		case    MENU_ITEM_STYLE_TOGGLE_OFF:{
			menu_item_icon_res = ICON_TOGGLE_OFF;
			icon_width	= 53;
			icon_height	= 26;
			break;
		}
		case    MENU_ITEM_STYLE_LOCKED:{
			menu_item_icon_res = MENU_ITEM_LOCKED;
			icon_width	= 15;
			icon_height	= 15;
			break;
		}
		case    MENU_ITEM_STYLE_DISABLED:{
			menu_item_icon_res = ICON_DISABLED;
			icon_width	= 27;
			icon_height	= 37;
			break;
		}
		default: {
			menu_item_icon_res = 0;
			icon_width	= 0;
			icon_height	= 0;
			break;
		}
		}
		
		show_res_by_id(	menu_item_icon_res, 
						176 - icon_width - MENU_H_MARGIN, 	
						i*(176/ITEMS_ON_PAGE)+((176/ITEMS_ON_PAGE)-icon_height)/2); 
	}
	
}

//	стрелки свайп вверх вних 9х5
if (menu->page > 0) 
	show_res_by_id(PAGE_UP_ARROW,   176-9-2, 		2); // 	вверх 

if (menu->page*ITEMS_ON_PAGE+ITEMS_ON_PAGE < menu->item_count) 
	show_res_by_id(PAGE_DOWN_ARROW, 176-9-2,  176-5-2);	//	вниз

// нумерация страниц
if (menu->item_count > ITEMS_ON_PAGE){
	char text[4];
	_sprintf(text, "%d/%d", menu->page+1, (menu->item_count-1)/ITEMS_ON_PAGE+1);
	text_out_font(	FONT_DIGIT_MICRO_13, text, 
					3, 
					176-10-2,
					0);
}

//repaint_screen_lines(0, 176);
}


int dispatch_menu (struct menu_struct *	menu, void *param){
	 struct gesture_ *gest = param;
	 int result = 0;
	 
	#ifdef DEBUG_LOG
	log_printf(5, "[dispatch_menu] menu=0x%X; menu_level=%d; ", menu, menu->menu_level);
	#endif
	 
	switch (gest->gesture){
		case GESTURE_CLICK: {
			
			int row = min( (gest->touch_pos_y / (176/ITEMS_ON_PAGE)), ITEMS_ON_PAGE-1 );

			if ( (menu->page*ITEMS_ON_PAGE + row) < menu->item_count){		//	если есть такой пункт меню
	
			#ifdef DEBUG_LOG
			log_printf(5, "GESTURE_CLICK: index=%d; name=%s; show_func=0x%X", 
							menu->page*ITEMS_ON_PAGE+row, 
							menu->items[menu->page*ITEMS_ON_PAGE+row].name, 
							menu->items[menu->page*ITEMS_ON_PAGE+row].show_func);
			#endif

			
			switch (menu->items[menu->page*ITEMS_ON_PAGE+row].item_style){
				case MENU_ITEM_STYLE_NORMAL:{
					int(*show_func)(int) = menu->items[menu->page*ITEMS_ON_PAGE+row].show_func;
					void* ret_f = menu->show_func;
					push_ret_f(menu, menu->show_func);
					
					if (show_func) {
						menu->show_func = show_func;
						clear_menu(menu);	//	очистим пункты меню
						//	в качестве параметра функции show_func передаем адрес возврата в предыдущий экран
						result = show_menu_animate(	show_func, (int) ret_f, ANIMATE_LEFT);
					}
					break;
				};
				case    MENU_ITEM_STYLE_LIST:{
					int(*show_func)(struct menu_struct *	menu, int index) = menu->items[menu->page*ITEMS_ON_PAGE+row].show_func;
					
					push_ret_f(menu, menu->show_func);
					if (show_func) {
						menu->show_func = show_func;
						result = show_func(menu,  (int)(menu->page*ITEMS_ON_PAGE+row));
					}
					break;
				}				
				case	MENU_ITEM_STYLE_UNCHECKED:
				case    MENU_ITEM_STYLE_CHECKED:
				case    MENU_ITEM_STYLE_TOGGLE_ON:
				case    MENU_ITEM_STYLE_TOGGLE_OFF:{
					
					int(*show_func)(struct menu_struct *	menu, int index) = menu->items[menu->page*ITEMS_ON_PAGE+row].show_func;
					if (show_func) 
						result = show_func(menu,  (int)(menu->page*ITEMS_ON_PAGE+row));
					
					break;
				}				
				case    MENU_ITEM_STYLE_LOCKED:
				case    MENU_ITEM_STYLE_DISABLED:{
					vibrate (3, 70, 70);
					break;
				}
				default: {
				
					break;
				}	
			}
			
			
			}
			break;
		};
		case GESTURE_SWIPE_UP: {
			if (menu->page*ITEMS_ON_PAGE+ITEMS_ON_PAGE < menu->item_count) {
				menu->page++;
				show_menu_animate(draw_menu, (int) menu, ANIMATE_UP);
			}
			break;
		};
		case GESTURE_SWIPE_DOWN: {			
			if (menu->page > 0) {
				menu->page--;
				show_menu_animate(draw_menu, (int) menu, ANIMATE_DOWN);
			}
			break;
		};
		case GESTURE_SWIPE_RIGHT: {	//	свайп направо
			#ifdef DEBUG_LOG
			log_printf(5, "GESTURE_SWIPE_RIGHT: ret_f=0x%X;", menu->ret_f[menu->menu_level-1] );
			#endif
		
			void* ret_f = pop_ret_f(menu);
			if (!ret_f)
				ret_f = show_watchface;
			
			//show_menu_animate(ret_f, 0, ANIMATE_RIGHT);
			
			clear_menu(menu);	//	очистим пункты меню
			
			//	в качестве параметра функции возврата передадим значение по указателю temp_buf_2
			show_menu_animate(ret_f,  *(int*)get_ptr_temp_buf_2(), ANIMATE_RIGHT);
			
			break;
		};
		case GESTURE_SWIPE_LEFT: {	// справа налево
				
			break;
		};

		default:{	// что-то пошло не так...
			break;
		};		
		
	}
	
	#ifdef DEBUG_LOG
	log_printf(5, "\r\n");
	#endif
	
	return result;
};

//==============================================================================================
#endif // if FW_VERSION == uni_latin

