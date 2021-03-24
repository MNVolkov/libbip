/////////////////////////////////////////////////////////////
//
//  FUNCTIONS LIBRARY
//	for Amazfit Bip
//  by Maxim Volkov mr-volkov@yandex.ru 2019
//  
/////////////////////////////////////////////////////////////

#ifndef __COMMON_H__
#define __COMMON_H__

#include LIB_BIP_H

#define VIDEO_X     176
#define VIDEO_Y     176

#define NULL ((void*)0)

#ifndef min
#define min(x,y) ((x) < (y) ? (x) : (y))
#endif

#ifndef max
#define max(x,y) ((x) > (y) ? (x) : (y))
#endif

#ifndef abssub
#define abssub(x,y) ((x) > (y) ? (x)-(y) : (y)-(x))
#endif

#ifndef true
#define true 1
#endif

#ifndef false
#define false 0
#endif
/* 
Type    	  Size		Alignment
			(bytes)     (bytes)
-------------------------------------			
char        1 byte   1 byte  
short       2 bytes  2 bytes  
int         4 bytes  4 bytes  
unsigned    4 bytes  4 bytes  
long        4 bytes  4 bytes  
long long   8 bytes  8 bytes  
float       4 bytes  4 bytes  
double      8 bytes  8 bytes  
pointer     4 bytes  4 bytes  
-------------------------------------
*/

// структура экрана
 struct regmenu_ {
			char	 curr_scr;			//	раздел меню
			char	 swipe_scr;				//  подраздел меню
			char	 overlay;					//  
			void	*dispatch_func,				//	функция обработки жестов экрана
					*key_press,					//	функция обработки нажатия на боковую кнопку
					*scr_job_func,			//	колбэк функция таймера обновления экрана
					*scr_job_buff,			//	переменная для колбэка обновления экрана 
					*show_menu_funct,			//	функция формирования экрана
					*show_menu_buff,			//	переменная для функции формирования экрана 
					*long_key_press;			//	функция обработки долгого нажатия на боковую кнопку
};

//	структура жеста при обработке нажатий на экран и боковую кнопку
 struct gesture_	{
			char	gesture;		// тип жеста
			int		touch_pos_x,	// координаты нажатия 	X
					touch_pos_y;	//						Y
					
			int		touch_pos_x2,	// координаты нажатия 	X
					touch_pos_y2;	//						Y
};

// тип жеста
#define GESTURE_CLICK			1 // простое нажатие на экран, координаты нажатия в переменных touch_pos_x touch_pos_y
#define GESTURE_SWIPE_UP		2 // свайп снизу вверх
#define GESTURE_SWIPE_DOWN		3 // свайп сверху вниз
#define GESTURE_SWIPE_LEFT		4 // свайп справа налево
#define GESTURE_SWIPE_RIGHT		5 // свайп слева направо

// структура даты времени
struct datetime_ {
unsigned short year;
unsigned char 	month,
				day,
				hour,
				min,
				sec,
				weekday;
unsigned char	h24;
};


// Уникальный номер процессора - Uniquie device ID Register 96-bits
struct STM32_UUID_{
	unsigned short 	X_WAF; 		//	координата X на вафле формат BCD
	unsigned short 	Y_WAF; 		//	координата Y на вафле формат BCD
	unsigned char 	WAF_NUM;		//	номер вафли
			 char 	LOT_NUM[7];		//	номер лота ASCII
};
#define STM32_UUID ((struct STM32_UUID_ *) 0x1FFF7590)


//	секретная фраза берется из прошивки по нижеприведенному адресу
//	последовательность одинакова для всех прошивок Amazfit Bip международных версий
//	для Китайской версии последовательность может быть другой
//	сделано так чтобы сэкономить мето для мода, ничего не мешает для особых случаев использовать другую последовательность
#define SECRET_SEQUENCE ((char *) 0x8008A00)
#define SECRET_LEN 128

// Bluetooth адрес
#define BT_MAC_ADDRESS ((unsigned char *) 0x08007000 ) 

// вид анимации для функции show_menu_animate
#define ANIMATE_LEFT		0	// анимация перехода экрана справа налево
#define ANIMATE_RIGHT		1	// анимация перехода экрана слева направо
#define ANIMATE_UP			2	// анимация перехода экрана снизу вверх
#define ANIMATE_DOWN		3	// анимация перехода экрана сверху вниз

// цвета
#define	COLOR_BLACK		0x000000		//	черный
#define	COLOR_RED		0x0000FF		//	красный
#define	COLOR_GREEN		0x00FF00		//	зеленый
#define	COLOR_BLUE		0xFF0000		//	синий
#define	COLOR_YELLOW	0x00FFFF		//	желтый
#define	COLOR_AQUA		0xFFFF00		//	цвет морской волны
#define	COLOR_PURPLE	0xFF00FF		//	сиреневый
#define	COLOR_WHITE		0xFFFFFF		//	белый
#define	COLOR_MASK		0xFFFFFF		//	маска цвета

// цвета в коротком формате
#define	COLOR_SH_BLACK		0b0000		//	черный
#define	COLOR_SH_RED		0b0001		//	красный
#define	COLOR_SH_GREEN		0b0010		//	зеленый
#define	COLOR_SH_BLUE		0b0100		//	синий
#define	COLOR_SH_YELLOW		0b0011		//	желтый
#define	COLOR_SH_AQUA		0b0110		//	цвет морской волны
#define	COLOR_SH_PURPLE		0b0101		//	сиреневый
#define	COLOR_SH_WHITE		0b0111		//	белый
#define	COLOR_SH_MASK		0b1111		//	маска цвета

// языки текста https://www.science.co.il/language/Locale-codes.php
#define locale_ru_RU	1049	//	русский
#define	locale_en_US	1033	//	английский
#define	locale_it_IT	1040	//	итальянский
#define	locale_es_ES	3082	//	испанский
#define	locale_fr_FR	1036	//	французский
#define	locale_de_DE	1031	//	немецкий
#define	locale_el_GR	1032	//	греческий

// языки текста буквенное представление
#define locale_RU	"RU"	//	русский
#define	locale_US	"EN"	//	английский
#define	locale_IT	"IT"	//	итальянский
#define	locale_ES	"ES"	//	испанский
#define	locale_FR	"FR"	//	французский
#define	locale_DE	"DE"	//	немецкий
#define	locale_GR	"GR"	//	греческий

// язык установленный
#define	OPT_LANG_COUNT	7		//	количество языков, не считая AUTO
#define	OPT_LANG_AUTO	0x00	//	автовыбор по локали телефона
#define	OPT_LANG_RUS	0x01	//	русский
#define	OPT_LANG_ENG	0x02    //	английский
#define	OPT_LANG_ITA	0x03    //	итальянский
#define	OPT_LANG_SPA	0x04    //	испанский
#define	OPT_LANG_FRA	0x05    //	французский
#define	OPT_LANG_DEU	0x06    //	немецкий
#define	OPT_LANG_GRK	0x07    //	греческий// язык установленный

// опция отключения анимации
#define	OPT_ANIMATION_COUNT		1		//	количество вариантов, не считая AUTO
#define	OPT_ANIMATION_AUTO		0x01	//	автовыбор возможности анимации
#define	OPT_ANIMATION_DISABLED	0x00	//	анимация отключена

// адрес флэшпамяти для хранения настроек
#define OPTIONS_FLASH_ADDRESS	0x007810B0
// длина буфера в байтах
#define OPTIONS_FLASH_LIMIT			80
//	смещение адреса для хранения настроек календаря	- 1 байт
#define OPTIONS_OFFSET_CALEND		0
// смещение адреса для хранения настроек языка приложений	-	1 байт
#define OPTIONS_OFFSET_LANGUAGE		1
// смещение адреса для хранения настроек быстрого запуска приложения из левого меню	- !!!   MAX_ELF_NAME+1 байт	 (сейчас 32 байта)
#define OPTIONS_OFFSET_LEFT_SIDE	2
// смещение адреса для хранения настроек опции отключения анимации
#define OPTIONS_OFFSET_ANIMATION	2+MAX_ELF_NAME+1 +1


// данные настроек системы ( функция get_generic_data / set_generic_data )
#define GENERIC_DATA_LOCALE		36

//  структура меню
//	меню настроек 
#define MAX_MENU_ITEM_NAME_LEN	64
#define MAX_MENU_ENCLOSURE		15		//	максимальная вложенность меню
#define ITEMS_ON_PAGE			3
#define MAX_MENU_ITEMS_COUNT	55		//	максимальное количество пунктов меню
#define MAX_LINES_PER_ITEM		3
//	параметры отображения пунктов меню
#define MENU_H_MARGIN	16

// статусы пунктов меню
#define MENU_ITEM_STYLE_NORMAL		0		//	обычный пункт меню	значок ">". Запуск процедуры через show_animate c параметром функции возврата
#define MENU_ITEM_STYLE_LIST		1		//	обычный пункт меню	значок ">".	Запуск процедуры с параметрами menu и index
#define MENU_ITEM_STYLE_UNCHECKED	2		//	чекбокс не отмечен	значок "0".	Запуск процедуры с параметрами menu и index
#define MENU_ITEM_STYLE_CHECKED		3		//	чекбокс отмечен		значок "v".	Запуск процедуры с параметрами menu и index
#define MENU_ITEM_STYLE_TOGGLE_ON	4		//	переключатель включен	значок "( о)".	Запуск процедуры с параметрами menu и index
#define MENU_ITEM_STYLE_TOGGLE_OFF	5		//	переключатель включен	значок "(о )".	Запуск процедуры с параметрами menu и index
#define MENU_ITEM_STYLE_LOCKED		6		//	заблокирован		значок "замок".	
#define MENU_ITEM_STYLE_DISABLED	7		//	пункт в данный момент не доступен	значок "Х"



// коды ошибок загрузчика
#define ERROR_NONE						 0
#define ERROR_BAD_RES_MAGIC				-1
#define ERROR_BAD_ELF_MAGIC				-2
#define ERROR_SEC_NOT_FOUND 			-3
#define ERROR_ELF_NOT_FOUND 			-4
#define ERROR_RES_OUT_OF_BOUNDS 		-5
#define ERROR_RES_INV_FORMAT 			-6
#define ERROR_TOO_MACH_RUN 				-7
#define ERROR_TOO_MACH_PSEG	 			-8
#define ERROR_MALLOC_FAILED	 			-9
#define ERROR_LIB_FUNC_NOT_FOUND		-10
#define ERROR_UNKNOWN_RELOC_TYPE		-11
#define ERROR_UNKNOWN_PSEG_TYPE			-12
#define ERROR_FONT_INV_FORMAT			-13
#define ERROR_FONT_NO_RES				-14


struct menu_item_struct {
//unsigned char 	index;		//	номер пункта меню

//	char		name[MAX_MENU_ITEM_NAME_LEN+1]; 		//	название пункта меню
	char*		name; 			//	указатель на строку название пункта меню
	void* 		show_func;		//	функция запуска
	int 		item_color;		//	цвет пункта меню
	int			item_style;		//	статус строки меню	
	int 		item_param0;	//	произвольный параметр пункта меню
//unsigned char	premium;		//	пункт премиум версии
};

struct menu_struct {
struct 	menu_item_struct	items[MAX_MENU_ITEMS_COUNT];	//	структура меню
unsigned char 				page;							//	номер текущей отображаемой страницы
unsigned char				item_count;						//	количество пунктов меню
		void*				show_func;						//	адрес функции отображения текущего экрана
		void*				ret_f[MAX_MENU_ENCLOSURE];		//	указатель возврата в предыдущий экран
		int 				menu_level;						//	текущий уровень вложенности
};


#pragma pack(push, 1)		//	запретить выравнивание полей структуры
struct res_struct {// sizeof=32+4
char 			res_type[5];	//	NERES, HMRES
char 			version;		//	 
char 			magic[4];		//	E8 09 1A D7 = 0xD71A09E8
char			reserved_0[4];	//	03 00 00 00
char 			reserved_1[2];	//	FF FF
unsigned int	reserved[4];	//  FF..FF
		 int	count;			//		
};
#pragma pack(pop)

#define RES_MAGIC		0xD71A09E8	//	сигнатура ресурсов, шрифта и т.д. устанавливается при заливке в часы
#define NERES_ADDRESS	0x2A0000	//	стоковые ресурсы
#define NEZK_ADDRESS	0x0A0000	//	стоковый шрифт


// параметры графических ресурсов
#pragma pack(push, 1)		//	запретить выравнивание полей структуры
struct res_params_ {
  short width; 		//	ширина в рх
  short height; 	//	высота в рх 	
};
#pragma pack(pop)

#define FONT_TYPE_NOT_LATIN		01
#define FONT_TYPE_LATIN			02

#define CHAR_DATA_LEN_NOT_LATIN		33
#define CHAR_DATA_LEN_LATIN			32

#define CHAR_UNICODE_RES	(short) 0xFFF0

//	заголовок файла шрифта
#pragma pack(push, 1)		//	запретить выравнивание полей структуры
struct nezk_hdr_ {			//	Размер структуры 34 байта
	char	sig_nezk[4]; 	//	NEZK
	char	version;		//	9 - латин; 8 - нелатин
	int		magic;			//	в файле шрифта magic==0x00000000; в флэшпамяти часов magic== 0xD71A09E8 
	char	reserved;		//	не известно, равно FF
	int		latin;			//	скорее всего латин/нелатин; равно 02 - для латин, 01 - для нелатин
	short	reserved_1;		//	не известно, равно 0xFFFF
	int		reserved_2;		//	не известно, равно 0xFFFFFFFF
	int		reserved_3;		//	не известно, равно 0xFFFFFFFF
	int		reserved_4;		//	не известно, равно 0xFFFFFFFF
	int		reserved_5;		//	не известно, равно 0xFFFFFFFF
	short	count;			//	количество блоков символов
};
#pragma pack(pop)

//	после заголовка идет описание блоков данных, структура блока:
#pragma pack(push, 1)
struct char_block_hdr_ {	//	размер структуры 6 байт
	short start_char;		//	UNICODE код первого символа в блоке
	short end_char;			//	UNICODE код последнего символа в блоке
	short start_char_num;	//	сквозной номер первого символа в блоке в файле шрифта, отсчет номеров идет с 0
							//	т.е. если первый блок содержит 5 символов, то первый символ второго блока будет иметь start_char_num==5;
}	;
#pragma pack(pop)

#define INDEX_MAIN_RES	((int)0xFFFF0000)
#define INDEX_FONT_RES	((int)0xFFFF0001)
#define ELF_INDEX_SELF	((int)0xFFFFFFFF)

#define MAX_LOADED_ELF	5		//	максимальное количество одновременно запущенных процессов
#define MAX_LISTED_ELF	55		//	максимальное количество эльфов для запуска

//#define MAX_ELF_LABEL	64
#define MAX_ELF_NAME	32

#define	ELF_LOADER_MAGIC	0x4C464C45	//	ELFL - ELF Loader

typedef struct {				//	структура запущенного процесса
	unsigned int 	process_id;	//	идентификатор процесса, присваивается загрузчиком
	int				index_listed;	//	индекс эльфа в списке загрузчика
	void* 			base;		//	указатель на выделенную под процесс память
	unsigned int	size;		//	количество выделенной под процесс памяти
	void*			ret_f;		//	точка возврата процесса
	unsigned int	ret_param0;	//	параметр функции возврата
	void(*elf_finish)(void* param0);	//	указатель на процедуру завершения эльфа, сюда надо передать 
	int (*entry_point)(void* param0, char** argv);	//	указатель на процедуру запуска эльфа, точка входа. Param0 = указатель на структуру proc запущенного процесса 
	void*			param;		//	пользовательский параметр, можно хранить что угодно, например указатели вместо temp_buf_2 для фоновых процессов				
	int				argc;		//	количество параметров при запуске эльфа
	char**			argv;		//	параметры при запуске эльфа
} Elf_proc_;

typedef struct  {				//	структура загружаемого эльфа
	char*	lang_labels[OPT_LANG_COUNT+1]; 	//	указатели на строки названия приложения на разных языках	
	int		lang_codes[OPT_LANG_COUNT+1];	//	значения локалей строк в массиве labels
	int		lang_count;						//	количество имеющихся строк для различных языков
	char	name[MAX_ELF_NAME];				//	служебное название эльфа
	unsigned int 	adr_symtab;				//	адрес в флэш памяти секции .symtab
	unsigned int 	sz_symtab;				//	размер секции .symtab 
	unsigned int 	adr_settings;			//	адрес в флэш памяти секции .elf.settings
	unsigned int 	sz_settings;			//	размер секции .elf.settings
	unsigned int 	adr_elfres;				//	адрес в флэш памяти секции .elf.res	
	unsigned int 	sz_elfres;				//	размер секции .elf.res
	unsigned int 	adr_elfdata;			//	адрес в флэш памяти секции .data	
	unsigned int 	sz_elfdata;				//	размер секции .data	
	unsigned int 	elf_addr;				//	адрес в флэш памяти ресурса содержащего данный ELF
	char			visible;				//	если не равно 0 эльф отображается в списке загрузчика
} Elf_listed_;

typedef struct  {
unsigned int 	magic;					//	идентификатор структуры, если не совпадает перетираем и заводим новую
		int  	loaded_count;			//	количество загруженных процессов
		int		listed_count;			//	количество эльфов обнаруженных в ресурсах
Elf_proc_ 		procs [MAX_LOADED_ELF];	//	структуры процессов
Elf_listed_*	listed[MAX_LISTED_ELF];	//	эльфы к загрузке 
		int		reloc_version;			//	версия таблицы релокации, при сканировании ресурсов выбирается таблица с наибольшей версией 
		int 	reloc_adr;				//	адрес ресурса во флэш памяти, сожержащий таблицу релокации прошивочных функций
		int 	reloc_size;				//	размер таблицы релокации прошивочных функций
} 	Elf_loader_;


// версии прошивок

#define latin_1_1_5_12 		LATIN_1_1_5_12
#define latin_1_1_5_36 		LATIN_1_1_5_36
#define not_latin_1_1_2_05 	NOT_LATIN_1_1_2_05

#define LATIN_1_1_5_12		11512
#define LATIN_1_1_5_16		11516
#define LATIN_1_1_5_36		11536
#define LATIN_1_1_6_48		11648
#define LATIN_1_1_5_56		11556
#define NOT_LATIN_1_1_2_05	11205
#define UNI_LATIN			55555


// статусы функции get_app_state
#define APP_STATE_CHARGE_PLUGGED	0x10
#define APP_STATE_BT_CONNECTED		0x200
#define APP_STATE_CHARGE_COMPLETE	0x1000


// структура данных содержащая информацию будильника
struct alarm_struct {
unsigned char index; 		//  bit_7 - включен/выключен; bit_0..4 - номер будильника
unsigned char hours;        //  часы
unsigned char minutes;      //  минуты
unsigned char repeat;       //  bit_0 - bit_7 - дни недели с понедельника по пятницу
                            //  значение 	128 - однократно; 127 - каждыйдень;
							//	31 - по рабочим; 0   - не создан;   60 - по выходным
};

// структура данных содержащая информацию о номере выбранной страницы и данные 10 будильников
struct alarm_10{
unsigned char 	page; 		// текущая странца установки будильников	
unsigned char 	field_0;	//	неизвестное поле структуры, возможно не используемое
struct 			alarm_struct alarms[10] ;	//	массив 10 будильников
};

// структура свободного блока памяти
struct mem_struct{
void* 	next_addr;	//	указатель на следующий блок
int 	size;		//	размер текущего свободного блока
};

// структуры данных датчика сердца
// 1.1.5.12, 1.1.5.36
typedef struct {
int 			field_0;
short 			field_1;
unsigned char 	field_2;
unsigned char 	field_3;
unsigned char 	field_4;
unsigned char 	heart_rate_1;		//	частота, ударов/мин; >200 - значение не доступно
unsigned char 	retCode;			//	статус измерения 0-закончено, >0 измерение не закончено
unsigned char 	field_5;
} hrm_data_struct ;

// 1.1.2.05
typedef struct {
int 			field_0;
short 			field_1;
unsigned char 	heart_rate_1;		///<	частота, ударов/мин; >200 - значение не доступно
unsigned char 	retCode;			///<	статус измерения 0-закончено, >0 измерение не закончено
}  hrm_data_struct_legacy;

/*!
	@brief \~russian навгационные данные (для функции get_navi_data) \~english navigation data (for get_navi_data function)
	@details \~russian Структура содержит поля данных геолокации такие как широта, долгота, высота и давление. Данные заполняются в функции \code get_navi_data(struct navi_struct_*); \endcode
*/
//	навгационные данные (для функции get_navi_data)
typedef struct {
	int ready 		                        ; ///< Готовность данных: bit 0: давление ; bit 1: высота  ; bit 2: широта  ; bit 3: долгота
	unsigned int pressure					; ///< значение давления в Паскалях
	float altitude							; ///< значение высоты в метрах
	signed int latitude						; ///< модуль значения долготы в градусах, умноженное на 3000000
	int ns									; ///< ns: 0-северное полушарие; 1-южное
	signed int longitude					; ///< модуль знаения долготы в градусах, умноженное на 3000000
	int ew									; ///< ew: 2-восточное полушарие; 3-западное; 
} navi_struct_;

// макросы для проверки значения готовности
#define IS_NAVI_PRESS_READY(navi) 		(navi & 0x01)
#define IS_NAVI_GPS_READY(navi) 		(navi & 0x0E)
#define IS_NAVI_GPS_ALT_READY(navi) 	(navi & 0x02)
#define IS_NAVI_GPS_LAT_READY(navi) 	(navi & 0x04)
#define IS_NAVI_GPS_LONG_READY(navi) 	(navi & 0x08)

// тип шрифта для функции text_out_font
#define FONT_DIGIT_LED_0					0
#define FONT_DIGIT_SQUARE_MINI_1			1
#define FONT_DIGIT_MIDDLE_2					2
#define FONT_DIGIT_BIG_3					3
#define FONT_DIGIT_BIG_RED_4				4
#define FONT_LETTER_MIDDLE_5				5
#define FONT_LETTER_BIG_6					6
#define FONT_DIGIT_ROUND_BIG_7				7
#define FONT_DIGIT_MIDDLE_8					8
#define FONT_DIGIT_BIG_9					9
#define FONT_FONT_DIGIT_SQUARE_MIDDLE_10	10
#define FONT_DIGIT_SMALL_11					11
#define FONT_DIGIT_TINY_12					12
#define FONT_DIGIT_MICRO_13					13
#define FONT_DIGIT_TINY_14					14
#define FONT_DIGIT_SQUARE_MICRO_15			15

// структура хранения номеров ресурсов при передаче в качестве параметров 
struct icons_{
	short icon_main;
	short icon_label_ru;
	short icon_label_en;	
};

// структура отрисовки значков главного меню
struct menu_items_draw_ {
		struct regmenu_* regmenu;	//	указатель на структуру нового элемента
		int	prev_screen;			//	предыдущий экран
		int	prev_sscreen;			//	предыдущий подэкран
		int	screen;					//	текущий подэкран
		struct icons_ icon_res;	//	указатель на номера ресурсов иконок меню
};


// Глобальные переменные
unsigned char get_var_current_screen();                                  //	активный номер экрана
unsigned char set_var_current_screen(unsigned char val); 			     //	
unsigned char get_var_swipe_screen_active_number();                      //	активный подномер экрана
unsigned char set_var_swipe_screen_active_number(unsigned char val);     //	
unsigned char get_var_menu_overlay();		                             //	активно оверлейный экран (уведомление, входящий звонок и т.д.)
unsigned char set_var_menu_overlay(unsigned char val);			         //	
		void* get_ptr_menu_dispatch_func();                              //	указатель на обработчик тачскрина и догого нажатия кнопки
		void* set_ptr_menu_dispatch_func(void* val);                     //	
		void* get_ptr_key_press_func();                                  //	указатель на обработчик нажатия кнопки
		void* set_ptr_key_press_func(void* val);                         //	
		void* get_ptr_screen_job_func();                                 //	указатель на обработчик таймера экрана
		void* set_ptr_screen_job_func(void* val);                        //	
		void* get_ptr_temp_buf_1();                                      //	указатель на данные экрана (утрачивается при появлении оверлейного экрана) 
		void* get_ptr_show_menu_func();                                  //	указатель на функцию создания текущего экрана
		void* set_ptr_show_menu_func(void* val);                         //	
		void* get_ptr_temp_buf_2();                                      //	указатель на данные экрана (не утрачивается при появлении оверлейного экрана)
		void* get_ptr_long_key_press_func();                             //	указатель на обработчик долгого нажатия на кнопку (действует не на всех экранах)
		void* set_ptr_long_key_press_func(void* val);                    //	
unsigned char get_var_left_side_menu_active(); 		                     //	значение переменной 1-вызов через левое меню; 0-через главное меню
		  int get_var_free_ram(); 	                                     //	количество свободной памяти
		void* get_ptr_screen_memory();                                   //	указатель на память экрана
		void* get_ptr_free_list();										 // указатель на дерево свободных блоков памяти

// import.s
extern void ARM0(void*);
extern void ARM1(void*,int);
extern void ARM2(void*,int,int);
extern void ARM3(void*,int,int,int);

extern void reg_menu(void *regmenu_, int param);
extern	int	show_menu_animate(void* show_menu_function, int param, int animate);
extern 	int show_menu (void* show_menu_function, int param);
extern 	int animate_screen(void *show_menu_function, int param, int animate, int line_from, int line_to);

extern  int	text_width(const char *text);
extern void text_out(const char *text, int pos_x, int pos_y);
extern void text_out_center(const char *text, int pos_center_x, int pos_y);
extern	int	get_text_height();
extern int 	get_last_text_pos_x();	

extern void set_bg_color(long color);
extern void set_fg_color (long color);

extern void fill_screen_bg();
extern int	set_graph_callback_to_ram_1();

extern void repaint_screen_lines(int from, int to);
extern void repaint_screen();
extern void draw_horizontal_line(int pos_y, int from_x, int to_x);
extern void draw_rect(int from_x, int from_y, int to_x, int to_y);
extern void draw_vertical_line(int pos_x, int from_y, int to_y);
extern void draw_filled_rect(int from_x, int from_y, int to_x, int to_y);
extern void draw_filled_rect_bg(int from_x, int from_y, int to_x, int to_y);
extern void draw_rect_or_line(int from_x, int from_y, int to_x, int to_y);

extern	void	load_font ();
extern 	int		get_res_ret_language();
extern	void	show_res_by_id(int res_ID,int pos_x, int pos_y);
extern	int		read_flash_by_offset(int *base_addres, void *buffer_p, int len, int offset);
extern 	int		draw_res_by_address(void *read_flash_func, void* res_addr, int pos_x, int pos_y);


extern	void	show_poweroff_message ();
extern	void	show_options_menu_2nd_screen ();
extern	void	show_menu_options ();
extern	void	show_options_menu_1st_screen ();

extern 	int		_sprintf (char *buf, const char *format, ...);

extern	int 	_strlen (const char *str);
extern	int 	_strcmp (const char * string1, const char * string2);
extern	int 	_strncmp (const char * string1, const char * string2, int num);
extern	int 	_strcpy ( char * destptr, const char * srcptr);
extern	int 	_strncpy (char * destptr, const char * srcptr, int num );

extern 	int		_memclr (void *buf, int len);
extern 	int		_memset (void *buf, int len, int val);
extern	int 	_memcpy (void *dest, const void *srcptr, int num);
extern	int		_memcmp	(const void* p1, const void* p2, int size);

extern 	int		get_tick_count();
extern 	int		set_update_period (int enable, int period); // cmd=0 остановить таймер, cmd=1 взвод таймера на количество тиков равное period 
extern	int 	set_display_state_value(int state_1, int state);
extern	int 	set_close_timer(int delay_s);

extern 	int 	log_printf(int debug_level, const char *format, ...); // обычно debug_level = 5
extern  int 	f_log_flush(char* msg);		//	сброс буффера лога на флэш, можно указать сообщение
extern	int 	vibrate(int count, int on_ms, int off_ms);

extern	void* 	_pvPortMalloc (int size);	//	прошивочная функция
extern  void	vPortFree( void *pv );

extern	void 	show_menu_touch_test (unsigned char enabled);
extern	void 	show_main_menu_item (unsigned char gesture);
extern 	void 	show_menu_alipay();
extern	int		dispatch_menu_alipay_1st_screen(void *param);
extern	void	keypress_menu_alipay_1st_screen();
extern 	int 	dispatch_left_side_menu(struct gesture_* gest);
extern	int		get_left_side_menu_active();
extern	int		set_left_side_menu_active(int val);

extern	unsigned int get_current_date_time(struct datetime_* datetime);
extern	int		get_current_timestamp(); 

extern  int 	show_watchface(); 
extern  void 	show_big_digit(int color, char * digits, int pos_x, unsigned int pos_y, int space); 
extern  void 	text_out_font(int font, const char * text, int pos_x, unsigned int pos_y, int space); 	//	отображение цифр альтернативным шрифтом (алиас к функции show_big_digit), примеры шрифтов см. https://github.com/freebip/fwhack/blob/master/show_big_digit.md
extern	void 	vTaskDelay(int delay_ms);

extern	int		read_flash (int addr, void *data, int size);
extern	int		write_flash(int addr, const void *data, int size);

extern	int 	get_generic_data(int info, void* buffer);
extern	int		set_generic_data(int info, void *buffer);

extern	int		set_backlight_state(int state);
extern	int		set_backlight_percent(int percent);
extern 	int		get_backlight_value();

extern	int 	show_alarm_set_screen(struct alarm_10 *alarms);
extern 	int		store_and_enable_alarm(struct alarm_struct *alarms, unsigned int cnt, unsigned int smart_disable);
extern	void 	dispatch_alarm_set(void *param);
extern	void	stm32_soft_reset();
extern	int 	iwdg_reboot();		
extern 	unsigned char 	get_last_heartrate();															//		получение измеренного последнего значения с датчика сердцебиения
extern 	int		set_hrm_mode(int hrm_mode);													//	установка режима измерения пульса 0x20 - однократный
extern  hrm_data_struct* get_hrm_struct();															//	получение указателя на данные датчика сердца
extern	char* 	get_res_string_by_id(int res_id);	

// Функции навигации
extern	void switch_gps_pressure_sensors(int mode);				//	включение/отключение сенсоров GPS и барометра
extern	navi_struct_* get_navi_data(navi_struct_ *navi_data);		//	получение данных GPS и барометра
extern	int is_gps_fixed();										//	проверка готовности GPS

// Функции создания нотификаций (всплывающих уведомлений)
extern int add_notification(int notif_type, int timestamp, char *title, char *msg, char *app_name);	//	создание и добавление в список уведомлений нового уведомления
extern int create_and_show_notification(int notif_type, char *title, char *msg, char *app_name);	//	создание и отображение созданного уведомления (остается в списке)

// Функция проверки заряда батареи
int get_battery_charge();	//	возвращает заряд батареи в %
#define IS_CHARGE_PLUGGED	get_app_state(APP_STATE_CHARGE_PLUGGED)		//	макрос, возвращает 0 если часы не заряжаются, 1 если заряжаются
#define IS_CHARGE_COMPLETE	get_app_state(APP_STATE_CHARGE_COMPLETE)	//	макрос, возвращает 0 если часы не до конца заряжены, 1 если часы заряжены полностью и зарядное устройство не отключено


// случайные числа
extern int _rand();
extern void _srand(unsigned int seed);

// протокол общения с телефоном
extern void send_host_app_msg (int msg);
extern void send_host_app_data (int a1, int channel, unsigned int size, void *data, int a2);

// проверка состояния системы
extern int check_app_state(long long state);

//	экран DND
extern void silence_page_create_frame();

// common.c
// работа с зыком
extern 	int 	get_selected_locale();
extern 	int		get_stored_lang();
extern 	int		store_selected_lang(int lang);
extern	int		get_system_locale();

// прочие функции
extern 	void 	draw_progressbar(int pos_y, int width, int height, int color_bg, int color_fg, int progress_max, int progress, int border, int repaint, const char* text);
extern 	void	dump_mem (void * address, unsigned long len);
extern 	int 	get_res_count();
extern 	int 	read_res_by_id (int res_id, int offset, void* buffer, int len);
extern  int 	read_elf_res_by_id (int index_listed, int res_id, int offset, void* buffer, int len);
extern	int 	getColorFromShort(unsigned char short_color);

// меню
extern	int 	add_menu_item(struct menu_struct *	menu, char* name, void* show_func, int color, int style);
extern	int 	clear_menu (struct menu_struct *	menu);
extern 	void 	draw_menu(struct menu_struct *	menu);
extern	int 	dispatch_menu (struct menu_struct *	menu, void *param);
extern	int 	push_ret_f(struct menu_struct *	menu, void* ret_f);
extern	void* 	pop_ret_f(struct menu_struct *	menu);
extern	void* 	pvPortMalloc (int size);  //	выделение памяти библиотечная функция (рекомендуется вместо _pvPortMalloc)
extern 	int 	check_free_mem_block(int size);	//	проверка наличия свободного блока памяти необходимого размера
// loader.c

#endif
