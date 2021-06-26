/*!
\file

/////////////////////////////////////////////////////////////
//
//  \brief FUNCTIONS LIBRARY v.1.5 //	for Amazfit Bip
//  \author (C) Maxim Volkov  2021 <Maxim.N.Volkov@ya.ru>
//
//	SDK BipOS. Библиотека функций для загрузчика приложений BipOS
//  
/////////////////////////////////////////////////////////////
**/

/****
v.1.5. - 03.2021
+	добавлен макрос IS_BT_CONNECTED возвращает 0 если часы не подключены к телефону и 1 если подключены
+	добавлены определения констрант для определения статуса зарядки часов, а также команд bluetooth для функции send_host_app_msg
+	добавлены прошивочные функции f_log_flush, set_backlight 
+	изменена работа загрузчика в части подмены прокси-адресов функций на реальные прошивочные адреса. Теперь реальные адреса прошивочных функций 
	не храняся в загрузчике, а размещены в специальном ресурсе, который можно разместить в ресурсах или шрифтах часов. Это позволит во-первых
	высвободить место для кода BipOS для дальнейшего развития, а также позволит добавлять новые прошивочные функции в BipOS без дополнительного релиза 
	патча BipOS. Для добавления новых функций, которые можно использовать в своих приложениях, достаточно будет обновить ресурс с прошивочными функциями.


v.1.4. - 11.2020
*	добавлена функция text_out_font - алиас к прощивочной функции show_big_digit с прототипом более соотвествующем её функциональности (спасибо x27)
	функция show_big_digit объявлена устаревшей и будет удалена в будущих версиях библиотеки
*	в функциях работы с ресурсами, такими как ElfGetSettingsSize, ElfReadSettings, ElfWriteSettings, show_elf_res_by_id, get_res_count, get_res_params и т.д. 
	добавлен новый источник ресурсов - Шрифт (index_listed == INDEX_FONT_RES)
+	добавлена прошивоная функция int get_battery_charge(), производящая замер и возвращающая заряд батареи в %, а также два макроса 
	IS_CHARGE_PLUGGED - возвращает 0 если часы не заряжаются, 1 если заряжаются
	IS_CHARGE_COMPLETE	- возвращает 0 если часы не до конца заряжены, 1 если часы заряжены полностью и зарядное устройство не отключено
+	добавлены структуры данных передаваемые BipOS в приложение при отрисовке значков главного меню

v.1.3. - 12.07.2020 
+	добавлены новые функции прошивки: 
		switch_gps_pressure_sensors, get_navi_data, is_gps_fixed	-	функции для раоты с  GPS и атмосферным давлением
		add_notification, create_and_show_notification				-	функции создания нотификаций (всплывающих уведомлений)
		get_current_timestamp	    						        -	значение текущего UNIX timestamp
+	доработаны функции работы с ресурсами ElfGetSettingsSize, ElfReadSettings, ElfWriteSettings, show_elf_res_by_id, get_res_count, get_res_params и т.д. 
	теперь в качестве аргумента index_listed можно передавать ELF_INDEX_SELF для указания собственного индекса приложения.
+	добавлена поддержка греческого языка приложениями BipOS

v.1.2 - 06.01.2020
+	добавлены функции прошивки:
	send_host_app_msg, send_host_app_data	-	передача сообщений из часов в телефон
	get_ptr_screen_memory,  				-	получение указателя на область видеопамяти
	show_elf_res_by_id, get_res_params, 	-	функции обращения с ресурсами часов (собственными и стоковыми)
	get_res_count, read_elf_res_by_id
	
*	исправлена работа следующих функций:
	pvPortMalloc - при нехватке памяти возвращает NULL (раньше была перезагрузка)
	read_res_by_id - исправлена ошибка в реализации, добалена возможность чтения стоковых ресурсов

*	

v.1.1 - 12.10.2019
+	добавлены функции прошивки:
	+	_rand, _srand (спасибо @Rainberd)
+	добавлены функции библиотеки:
	+	ElfReadSettings, 
	+	ElfWriteSettings, 
	+	ElfGetSettingsSize, 
	+	show_elf_res_by_id
*	дополнена структура Elf_proc_ (добавлен параметр index_listed)	

v.1.0 
-	первая версия библиотеки

*****/


#ifndef __LIBBIP_H__
#define __LIBBIP_H__

#define LIBBIP_VERSION "1.5"

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

#ifndef offsetof
#define offsetof(s, m) (int)&(((s *)0)->m)
#endif

#ifndef sizeof_member
#define sizeof_member(s,m) ((int) sizeof(((s *)0)->m))
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

/// структура данных создаваемого экрана
 struct regmenu_ {
unsigned 	char	 curr_scr;					//!<	номер экрана
unsigned	char	 swipe_scr;					//!< 	дополниетльный номер экрана
unsigned	char	 overlay;					//!< 	признак оверлейного экрана
			void	*dispatch_func,				//!<	функция обработки жестов экрана
					*key_press,					//!<	функция обработки нажатия на боковую кнопку
					*scr_job_func,				//!<	колбэк функция таймера обновления экрана
					*scr_job_buff,				//!<	переменная для колбэка обновления экрана 
					*show_menu_func,			//!<	функция формирования экрана
					*show_menu_buff,			//!<	переменная для функции формирования экрана 
					*long_key_press;			//!<	функция обработки долгого нажатия на боковую кнопку
};

//!	структура жеста при обработке нажатий на экран и боковую кнопку
 struct gesture_	{
			char	gesture;		//!< тип жеста
			int		touch_pos_x,	//!< координаты нажатия 	X
					touch_pos_y;	//!< координата нажатия		Y
};

//! тип жеста
#define GESTURE_CLICK			1 //!< простое нажатие на экран, координаты нажатия в переменных touch_pos_x touch_pos_y
#define GESTURE_SWIPE_UP		2 //!< свайп снизу вверх
#define GESTURE_SWIPE_DOWN		3 //!< свайп сверху вниз
#define GESTURE_SWIPE_LEFT		4 //!< свайп справа налево
#define GESTURE_SWIPE_RIGHT		5 //!< свайп слева направо

//! структура даты времени
struct datetime_ {
unsigned short year;		//!<	год
unsigned char 	month,      //!<	месяц
				day,        //!<	день месяца
				hour,       //!<	часы
				min,        //!<	минуты
				sec,        //!<	секунды
				weekday;    //!<	день недели
unsigned char	h24;        //!<	формат времени 12/24
};

//! вид анимации для функции show_menu_animate
#define ANIMATE_LEFT		0	//!< анимация перехода экрана справа налево
#define ANIMATE_RIGHT		1	//!< анимация перехода экрана слева направо
#define ANIMATE_UP			2	//!< анимация перехода экрана снизу вверх
#define ANIMATE_DOWN		3	//!< анимация перехода экрана сверху вниз

//! цвета
#define	COLOR_BLACK		0x000000		//!<	черный
#define	COLOR_RED		0x0000FF		//!<	красный
#define	COLOR_GREEN		0x00FF00		//!<	зеленый
#define	COLOR_BLUE		0xFF0000		//!<	синий
#define	COLOR_YELLOW	0x00FFFF		//!<	желтый
#define	COLOR_AQUA		0xFFFF00		//!<	цвет морской волны
#define	COLOR_PURPLE	0xFF00FF		//!<	сиреневый
#define	COLOR_WHITE		0xFFFFFF		//!<	белый
#define	COLOR_MASK		0xFFFFFF		//!<	маска цвета

//! цвета в коротком формате
#define	COLOR_SH_BLACK		0b0000		//!<	черный
#define	COLOR_SH_RED		0b0001		//!<	красный
#define	COLOR_SH_GREEN		0b0010		//!<	зеленый
#define	COLOR_SH_BLUE		0b0100		//!<	синий
#define	COLOR_SH_YELLOW		0b0011		//!<	желтый
#define	COLOR_SH_AQUA		0b0110		//!<	цвет морской волны
#define	COLOR_SH_PURPLE		0b0101		//!<	сиреневый
#define	COLOR_SH_WHITE		0b0111		//!<	белый
#define	COLOR_SH_MASK		0b1111		//!<	маска цвета

//! языки текста (локали)
#define locale_ru_RU	1049	//!<	русский
#define	locale_en_US	1033	//!<	английский
#define	locale_it_IT	1040	//!<	итальянский
#define	locale_es_ES	3082	//!<	испанский
#define	locale_fr_FR	1036	//!<	французский
#define	locale_de_DE	1031	//!<	немецкий
#define	locale_el_GR	1032	//!<	греческий

//! язык установленный для меню мода, возвращаемый функцией get_stored_lang
#define	OPT_LANG_COUNT	7		//!<	количество языков
#define	OPT_LANG_AUTO	0x00	//!<	автовыбор по локали телефона
#define	OPT_LANG_RUS	0x01	//!<	русский
#define	OPT_LANG_ENG	0x02    //!<	английский
#define	OPT_LANG_ITA	0x03    //!<	итальянский
#define	OPT_LANG_SPA	0x04    //!<	испанский
#define	OPT_LANG_FRA	0x05    //!<	французский
#define	OPT_LANG_DEU	0x06    //!<	немецкий
#define	OPT_LANG_GRK	0x07    //!<	греческий

//! данные настроек системы ( функция get_generic_data / set_generic_data )
#define GENERIC_DATA_LOCALE		36		//!<	локаль телефона
#define GENERIC_DATA_DND_MODE	17		//!<	настройка режима "Не беспокоить" (DND)

//! структура меню
//!	меню настроек 
#define MAX_MENU_ITEM_NAME_LEN	64		//!<	максимальная длина названия пункта меню
#define MAX_MENU_ENCLOSURE		15		//!<	максимальная вложенность меню
#define ITEMS_ON_PAGE			3		//!<	количество пунктов меню на одном экране
#define MAX_MENU_ITEMS_COUNT	55      //!<	максимальное количество пунктов меню 
#define MAX_LINES_PER_ITEM		3       //!<	максимальное количество строк текста для одного пункта меню

//!	параметры отображения пунктов меню
#define MENU_H_MARGIN	10				//!<	горизонтальный отступ пункта меню

//! статусы пунктов меню
#define MENU_ITEM_STYLE_NORMAL		0		//!<	обычный пункт меню	значок ">". Запуск процедуры через show_animate c параметром функции возврата
#define MENU_ITEM_STYLE_LIST		1		//!<	обычный пункт меню	значок ">".	Запуск процедуры с параметрами menu и index
#define MENU_ITEM_STYLE_UNCHECKED	2		//!<	чекбокс не отмечен	значок "0".	Запуск процедуры с параметрами menu и index
#define MENU_ITEM_STYLE_CHECKED		3		//!<	чекбокс отмечен		значок "v".	Запуск процедуры с параметрами menu и index
#define MENU_ITEM_STYLE_TOGGLE_ON	4		//!<	переключатель включен	значок "( о)".	Запуск процедуры с параметрами menu и index
#define MENU_ITEM_STYLE_TOGGLE_OFF	5		//!<	переключатель включен	значок "(о )".	Запуск процедуры с параметрами menu и index
#define MENU_ITEM_STYLE_LOCKED		6		//!<	заблокирован		значок "замок".	
#define MENU_ITEM_STYLE_DISABLED	7		//!<	пункт в данный момент не доступен	значок "Х"


//! коды ошибок загрузчика
#define ERROR_NONE						 0		//!<	ошибки нет
#define ERROR_BAD_RES_MAGIC				-1      //!<	ошибка в структуре ресурсов (некорректная сигнатура заголовка)
#define ERROR_BAD_ELF_MAGIC				-2      //!<	ошибка в структуре ELF файла (некорректная сигнатура заголовка)
#define ERROR_SEC_NOT_FOUND 			-3      //!<	ошибка в структуре ELF файла (отсутствует необходимая секция)
#define ERROR_ELF_NOT_FOUND 			-4      //!<	ELF файл не найден
#define ERROR_RES_OUT_OF_BOUNDS 		-5      //!<	ошибка в структуре ресурсов (попытка доступа за пределами ресурса)
#define ERROR_RES_INV_FORMAT 			-6      //!<	ошибка в структуре ресурсов (некорректный формат)
#define ERROR_TOO_MACH_RUN 				-7      //!<	ошибка загрузчика (превышено максимальное количество одновременно запущенных приложений)
#define ERROR_TOO_MACH_PSEG	 			-8      //!<	ошибка загрузка (превышено максимальное значение количество программных сегментов)
#define ERROR_MALLOC_FAILED	 			-9      //!<	ошибка системы (недостаточно свободной памяти)
#define ERROR_LIB_FUNC_NOT_FOUND		-10     //!<	ошибка загрузчика (не найдена фунция в библиотеке подменных адресов)
#define ERROR_UNKNOWN_RELOC_TYPE		-11     //!<	ошибка загрузчика (неизвестный тип релокации)
#define ERROR_UNKNOWN_PSEG_TYPE			-12     //!<	ошибка загрузчика (не известный тип программного сегмента)
#define ERROR_FONT_INV_FORMAT			-13     //!<	ошибка шрифта (не верный формат шрифта)
#define ERROR_FONT_NO_RES				-14     //!<	ошибка шрифта (шрифт не содержит ресурсов)

// ресурсы
//! ресурсы с номерами до 559 включительно одинаковые для всех версий прошивок и ресурсов
#define ICON_SUNNY			71
#define MENU_ITEM_ARROW		72
#define CHECK_BOX_OFF		73
#define CHECK_BOX_ON		74
#define ICON_DISABLED		75
#define ICON_TOGGLE_OFF		76
#define ICON_TOGGLE_ON		77
#define MENU_ITEM_LOCKED	103
#define ICON_ALARM			227
#define	ICON_CALEND			228
#define ICON_RECYCLE		293
#define ARROW_DOWN			316
#define ARROW_UP			318
#define PAGE_UP_ARROW		408
#define PAGE_DOWN_ARROW		407
#define ICON_CANCEL_RED		416
#define ICON_OK_GREEN		417

//!	структура пункта меню
struct menu_item_struct {
	char*		name; 			//!<	указатель на строку название пункта меню
	void* 		show_func;		//!<	функция запуска
	int 		item_color;		//!<	цвет пункта меню
	int			item_style;		//!<	статус строки меню	
	int 		item_param0;	//!<	произвольный параметр пункта меню
};

//! структура меню
struct menu_struct {
struct 	menu_item_struct	items[MAX_MENU_ITEMS_COUNT];	//!<	массив структур пунктов меню
unsigned char 				page;							//!<	номер текущей отображаемой страницы
unsigned char				item_count;						//!<	количество пунктов меню
		void*				show_func;						//!<	адрес функции отображения текущего экрана
		void*				ret_f[MAX_MENU_ENCLOSURE];		//!<	массив указателей функций предыдущих экранов
		int 				menu_level;						//!<	текущий уровень вложенности
};

#pragma pack(push, 1)		//	запретить выравнивание полей структуры
//!	структура заголовка файла ресурса
struct res_struct {// sizeof=32+4
char 			res_type[5];	//!	сигнатура, содержащая символы NERES для новой структуры, HMRES - старая структура (не используется)
char 			version;		//!	 версия файла ресурсов
char 			magic[4];		//!	сигнатура файла ресурсов: E8 09 1A D7 = 0xD71A09E8 - в памяти часов; 0xFFFFFFFF - в файле ресурсов до загрузки в часы
char			reserved_0[4];	//!	03 00 00 00 
char 			reserved_1[2];	//!	FF FF
unsigned int	reserved[4];	//! FF..FF
		 int	count;			//!	количество ресурсов в файле ресурсов	
};
#pragma pack(pop)

#define RES_MAGIC		0xD71A09E8
#define NERES_ADDRESS	0x2A0000

//! параметры графических ресурсов
#pragma pack(push, 1)		//	запретить выравнивание полей структуры
struct res_params_ {
  short width; 		//!<	ширина в рх
  short height; 	//!<	высота в рх 	
};
#pragma pack(pop)		//	запретить выравнивание полей структуры

#define INDEX_MAIN_RES	((int)0xFFFF0000)
#define INDEX_FONT_RES	((int)0xFFFF0001)
#define ELF_INDEX_SELF	((int)0xFFFFFFFF)

//!	структура запущенного процесса
typedef struct {				
	unsigned int 	process_id;						//!<	идентификатор процесса, присваивается загрузчиком
	int				index_listed;					//!<	индекс эльфа в списке загрузчика
	void* 			base;							//!<	указатель на выделенную под процесс память
	unsigned int	size;							//!<	количество выделенной под процесс памяти
	void*			ret_f;							//!<	точка возврата процесса
	unsigned int	ret_param0;						//!<	параметр функции возврата
	void(*elf_finish)(void* param0);				//!<	указатель на процедуру завершения эльфа, сюда надо передать 
	int (*entry_point)(void* param0, char** argv);	//!<	указатель на процедуру запуска эльфа, точка входа. Param0 = указатель на структуру proc запущенного процесса 
	void*			param;							//!<	пользовательский параметр, можно хранить что угодно, например указатели вместо temp_buf_2 для фоновых процессов				
	int				argc;							//!<	количество параметров при запуске эльфа
	char**			argv;							//!<	параметры при запуске эльфа
} Elf_proc_;

//! версии прошивок
#define LATIN_1_1_5_12		11512
#define LATIN_1_1_5_16		11516
#define LATIN_1_1_5_36		11536
#define LATIN_1_1_6_48		11648
#define LATIN_1_1_5_56		11556
#define NOT_LATIN_1_1_2_05	11205
#define UNI_LATIN			55555


//! структура данных содержащая информацию будильника
struct alarm_struct {
unsigned char index; 		//!  bit_7 - включен/выключен; bit_0..4 - номер будильника
unsigned char hours;        //!  часы
unsigned char minutes;      //!  минуты
unsigned char repeat;       //!  bit_0 - bit_7 - дни недели с понедельника по пятницу; значение 128 - однократно; 127 - каждыйдень;	31 - по рабочим; 0   - не создан;   60 - по выходным							
};

//! структура данных содержащая информацию о номере выбранной страницы и данные 10 будильников
struct alarm_10{
unsigned char 	page; 						//!<	текущая странца установки будильников	
unsigned char 	field_0;					//!<	неизвестное поле структуры, возможно не используемое
struct 			alarm_struct alarms[10] ;	//!<	массив 10 будильников
};


//! структуры данных датчика сердца для прошивок 1.1.5.12, 1.1.5.36
typedef struct {
int 			t_start;
short 			last_hr;		
unsigned char 	field_2;
unsigned char 	field_3;
unsigned char 	field_4;
unsigned char 	heart_rate;			//!<	частота, ударов/мин; >200 - значение не доступно
unsigned char 	ret_code;			//!<	статус измерения 0-закончено, >0 измерение не закончено
unsigned char 	field_5;
} hrm_data_struct ;

//! структуры данных датчика сердца для прошивки 1.1.2.05
typedef struct{
int 			t_start;
short 			last_hr;
unsigned char 	heart_rate;			//!<	частота, ударов/мин; >200 - значение не доступно
unsigned char 	ret_code;			//!<	статус измерения 0-закончено, >0 измерение не закончено
}  hrm_data_struct_legacy;


//!	навгационные данные (для функции get_navi_data)
typedef struct {
	int ready 		                        ; //!< Готовность данных: bit 0: давление ; bit 1: высота  ; bit 2: широта  ; bit 3: долгота
	unsigned int pressure					; //!< значение давления в Паскалях
	float altitude							; //!< значение высоты в метрах
	signed int latitude						; //!< модуль значения долготы в градусах, умноженное на 3000000
	int ns									; //!< ns: 0-северное полушарие; 1-южное
	signed int longitude					; //!< модуль знаения долготы в градусах, умноженное на 3000000
	int ew									; //!< ew: 2-западное полушарие; 3-восточное; 
} navi_struct_;

//! полушария
#define NAVI_NORTH_HEMISPHERE	0	//!<	северное полушарие
#define NAVI_SOUTH_HEMISPHERE	1	//!<	южное полушарие
#define NAVI_WEST_HEMISPHERE	2	//!<	западное полушарие
#define NAVI_EAST_HEMISPHERE	3	//!<	воточное полушарие


//! макросы для проверки значения готовности
#define IS_NAVI_PRESS_READY(navi) 		(navi & 0x01)	//!<	готовность данных атмосферного давления: 0 - не готов, 1 - готов
#define IS_NAVI_GPS_READY(navi) 		(navi & 0x0E)	//!<	готовность координат: 0 - не готов, 1 - готов	
#define IS_NAVI_GPS_ALT_READY(navi) 	(navi & 0x02)	//!<	готовность данных высоты (GPS): 0 - не готов, 1 - готов
#define IS_NAVI_GPS_LAT_READY(navi) 	(navi & 0x04)	//!<	готовность данных широты (GPS): 0 - не готов, 1 - готов
#define IS_NAVI_GPS_LONG_READY(navi) 	(navi & 0x08)	//!<	готовность данных долготы (GPS): 0 - не готов, 1 - готов

// как правило данные высоты, широты и долготы готовы при фиксации GPS приемника, так что достаточно проверять 
//	статус готовности GPS  - IS_NAVI_GPS_READY(navi) 

//	Типы уведомлений
#define NOTIFY_TYPE_NONE		0
#define NOTIFY_TYPE_SMS			5
#define NOTIFY_TYPE_EMAIL		6
#define NOTIFY_TYPE_MICHAT		7
#define NOTIFY_TYPE_FACEBOOK	8
#define NOTIFY_TYPE_TWITTER		9
#define NOTIFY_TYPE_MI			10
#define NOTIFY_TYPE_WHATSAPP	12
#define NOTIFY_TYPE_ALARM		16
#define NOTIFY_TYPE_INSTAGRAM	18
#define NOTIFY_TYPE_ALIPAY		22
#define NOTIFY_TYPE_CALENDAR	27
#define NOTIFY_TYPE_VIBER		29
#define NOTIFY_TYPE_TELEGRAM	31
#define NOTIFY_TYPE_SKYPE		33
#define NOTIFY_TYPE_VK			34
#define NOTIFY_TYPE_CALL		39
#define NOTIFY_TYPE_LOW_BAT		42

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


//! структура хранения номеров ресурсов при передаче в качестве параметров 
struct icons_{
	short icon_main;
	short icon_label_ru;
	short icon_label_en;	
};

//! структура отрисовки значков главного меню
struct menu_items_draw_{
		struct regmenu_* regmenu;	//!<	указатель на структуру нового элемента
		int	prev_screen;			//!<	предыдущий экран
		int	prev_sscreen;			//!<	предыдущий подэкран
		int	screen;					//!<	текущий подэкран
		struct icons_ icon_res;		//!<	указатель на номера ресурсов иконок меню
};

//! значки главного меню
#define MAIN_MENU_STATUS	2
#define MAIN_MENU_ACTIVITY	3
#define MAIN_MENU_WEATHER	4
#define MAIN_MENU_ALARM		5
#define MAIN_MENU_TIMER		6
#define MAIN_MENU_COMPASS	7
#define MAIN_MENU_OPTIONS	8
#define MAIN_MENU_ALIPAY	9

// статусы функции check_app_state
#define APP_STATE_CHARGE_PLUGGED	0x10
#define APP_STATE_BT_CONNECTED		0x200
#define APP_STATE_CHARGE_COMPLETE	0x1000

//! команды bluetooth для функции send_host_app_msg
#define	CMD_FELL_ASLEEP			0x01
#define	CMD_WOKE_UP				0x02
#define	CMD_STEPSGOAL_REACHED	0x03
#define	CMD_BUTTON_PRESSED 		0x04
#define	CMD_FIND_PHONE_START	0x08
#define	CMD_ALARM_TOGGLED 		0x0A
#define	CMD_BUTTON_PRESSED_LONG 0x0B
#define	CMD_TICK_30MIN			0x0E
#define	CMD_FIND_PHONE_STOP 	0x0F
#define	CMD_MUSIC_CONTROL		0xFE

//! команды управления плеером для CMD_MUSIC_CONTROL
#define	CMD_MUSIC_PLAY			0
#define	CMD_MUSIC_PAUSE			1
#define	CMD_MUSIC_NEXT			3
#define	CMD_MUSIC_PREV			4
#define	CMD_MUSIC_VOL_UP		5
#define	CMD_MUSIC_VOL_DOWN		6
#define	CMD_MUSIC_AMC_ENABLE	0xE0
#define	CMD_MUSIC_AMC_DISABLE 	0xE1	


#define IS_CHARGE_PLUGGED	check_app_state(APP_STATE_CHARGE_PLUGGED)		//	макрос, возвращает 0 если часы не заряжаются, 1 если заряжаются
#define IS_CHARGE_COMPLETE	check_app_state(APP_STATE_CHARGE_COMPLETE)	//	макрос, возвращает 0 если часы не до конца заряжены, 1 если часы заряжены полностью и зарядное устройство не отключено
#define IS_BT_CONNECTED	check_app_state(APP_STATE_BT_CONNECTED)			//	макрос, возвращает 0 если часы не подключены к bluetooth, 1 если подключены 

//!	режим DND Не беспокоить для функций get_generic_data и set_generic_data
#define DND_MODE_OFF	0
#define DND_MODE_ON		1
#define DND_MODE_AUTO	2


//!	определения номеров сенсоров для функции sensors_enable
#define SENSOR_TYPE_ACCEL		1
#define SENSOR_TYPE_COMPASS		3
#define SENSOR_TYPE_PRESSURE	7
#define SENSOR_TYPE_GPS			8


//! Глобальные переменные
unsigned char get_var_current_screen();                                  //!<	получение активного номера экрана
unsigned char set_var_current_screen(unsigned char val); 			     //!<	запись активного номера экрана
unsigned char get_var_swipe_screen_active_number();                      //!<	получение активного дополнительного номера экрана
unsigned char set_var_swipe_screen_active_number(unsigned char val);     //!<	запись активного дополнительного номера экрана
unsigned char get_var_menu_overlay();		                             //!<	получение значения активности оверлейного экрана (уведомление, входящий звонок и т.д.)
unsigned char set_var_menu_overlay(unsigned char val);			         //!<	запись значения активности оверлейного экрана (уведомление, входящий звонок и т.д.)
		void* get_ptr_menu_dispatch_func();                              //!<	получения указателя на обработчик тачскрина и догого нажатия кнопки
		void* set_ptr_menu_dispatch_func(void* val);                     //!<	запись указателя на обработчик тачскрина и догого нажатия кнопки
		void* get_ptr_key_press_func();                                  //!<	получение указателя на обработчик нажатия кнопки
		void* set_ptr_key_press_func(void* val);                         //!<	запись указателя на обработчик нажатия кнопки
		void* get_ptr_screen_job_func();                                 //!<	получение указателя на обработчик таймера экрана
		void* set_ptr_screen_job_func(void* val);                        //!<	запись указателя на обработчик таймера экрана
		void* get_ptr_temp_buf_1();                                      //!<	получение указателя на данные экрана (утрачивается при появлении оверлейного экрана) 
		void* get_ptr_show_menu_func();                                  //!<	получение указателя на функцию создания текущего экрана
		void* set_ptr_show_menu_func(void* val);                         //!<	запись указателя на функцию создания текущего экрана
		void* get_ptr_temp_buf_2();                                      //!<	получение указателя на данные экрана (не утрачивается при появлении оверлейного экрана)
		void* get_ptr_long_key_press_func();                             //!<	получение указателя на обработчик долгого нажатия на кнопку (действует не на всех экранах)
		void* set_ptr_long_key_press_func(void* val);                    //!<	запись указателя на обработчик долгого нажатия на кнопку (действует не на всех экранах)
unsigned char get_var_left_side_menu_active(); 		                     //!<	получение значения активности бокового меню: 1-вызов через левое меню; 0-через главное меню
		  int get_var_free_ram(); 	                                     //!<	получение количества свободной памяти
		void* get_ptr_screen_memory();                                   //!<	получение указателя на память экрана
		void* get_ptr_free_list();										 //!<	получение указателя на дерево свободных блоков памяти
		
// Функции прошивки
/*!	\brief Регистрация создаваемого экрана в операционной системе. 
	
	\details Функция reg_menu регистрирует в системе создание нового экрана (активность).  Значения структуры reg_menu_ копируются, 
	после вызова reg_menu память по указателю не используется).
	Оверлейнаый экран создается без замещения предыдущего активного экрана. Данный режим используется для создания временной активности, такой как
	уведомление, будильник и т.д.
	
	\param[in] 	regmenu_ 	Указатель на структуру меню reg_menu_ создаваемого экрана
	\param[in] 	param 		Значение режима создания нового экрана: 0 - обычный; 1 - оверлейный 
	
*/
extern void 	reg_menu(void *regmenu_, int param);		                              

/*!	\brief Отображение содержимого видеопамяти на LCD с анимацией сдвига экрана

	
	\details Функция reg_menu регистрирует в системе создание нового экрана (активность).  Значения структуры reg_menu_ копируются, 
	после вызова reg_menu память по указателю не используется).
	Оверлейнаый экран создается без замещения предыдущего активного экрана. Данный режим используется для создания временной активности, такой как
	уведомление, будильник и т.д.
	
	\param [in]		show_menu_function	указатель на функцию создания экрана или отрисовки интерфейса
	\param [in] 	param	параметр передаваемый функции show_menu_function
	\param [in] 	animate	направление анимации
	
*/
extern	int		show_menu_animate(void* show_menu_function, int param, int animate);      
extern 	int 	show_menu (void* show_menu_function, int param);                          //	отображение меню без анимации, param - параметр передаваемый функции show_menu_function
extern  int		text_width(const char *text);                                             //	возвращает ширину текста, который будетвыведен текущим шрифтом
extern void 	text_out(const char *text, int pos_x, int pos_y);                               //	вывод текста по координатам, pos_x, pos_y координаты верхнего левого угла надписи
extern void 	text_out_center(const char *text, int pos_center_x, int pos_y);                 //	вывод текста по координатам, pos_x, pos_y координаты середины верхнего края надписи
extern	int		get_text_height();                                                        //	возвращает высоту текста, текущего шрифта
extern int 		get_last_text_pos_x();													  //	возвращает координату X сразу после последнего выведенного текста
	                                                                                      //	
extern void 	set_bg_color(long color);                                                 //	выбор фонового цвета для последующих графических опреаций
extern void 	set_fg_color (long color);                                                //	выбор цвета для последующих графических опреаций
extern void 	fill_screen_bg();                                                         //	заливка экрана цветом фона
extern int		__attribute__ ((deprecated("set_graph_callback_to_ram_1 is deprecated use GUI_UC_SetEncodeUTF8 instead"))) set_graph_callback_to_ram_1();                                            //	подключение кодировки UTF8 для функций вывода текста
extern int		GUI_UC_SetEncodeUTF8();                                         		 //	подключение кодировки UTF8 для функций вывода текста
extern void 	repaint_screen_lines(int from, int to);                                   //	обновить на экране строки (копирование из видеопамяти в дисплей)
extern void 	repaint_screen();                                                         //	обновить весь экран ((копирование из видеопамяти в дисплей))
extern void 	draw_horizontal_line(int pos_y, int from_x, int to_x);                    //	отрисовка горизонтальной линии
extern void 	draw_rect(int from_x, int from_y, int to_x, int to_y);                    //	отрисовка прямоугольника
extern void 	draw_vertical_line(int pos_x, int from_y, int to_y);                      //	отрисовка вериткальной линии
extern void 	draw_filled_rect(int from_x, int from_y, int to_x, int to_y);             //	отрисовка закрашенного прямоугольника
extern void 	draw_filled_rect_bg(int from_x, int from_y, int to_x, int to_y);          //	отрисовка закрашенного цветом фона прямоугольника 
extern void 	draw_rect_or_line(int from_x, int from_y, int to_x, int to_y);            //	отрисовка прямоугольника ???
                                                                                          //	
extern	void	load_font ();                                                             //	подгрузка шрифта
extern 	int		get_res_ret_language();                                                   //	получение языка интерфейса часов
extern	void	show_res_by_id(int res_ID,int pos_x, int pos_y);                          //	отображение графического ресурса по его номеру
                                                                                          //	
extern 	int		_sprintf (char *buf, const char *format, ...);                            //	встроенная в прошивку функция sprintf
                                                                                          //	
extern	int 	_strlen (const char *str);                                                      //	встроенная в прошивку функция strlen
extern	int 	_strcmp (const char * string1, const char * string2);                     //	встроенная в прошивку функция strcmp
extern	int 	_strncmp (const char * string1, const char * string2, int num);           //	встроенная в прошивку функция strncmp
extern	int 	_strcpy ( char * destptr, const char * srcptr);                           //	встроенная в прошивку функция strcpy
extern	int 	_strncpy (char * destptr, const char * srcptr, int num );                 //	встроенная в прошивку функция strncpy
                                                                                          //	
extern 	int		_memclr (void *buf, int len);                                             //	встроенная в прошивку функция memclr
extern 	int		_memset (void *buf, int len, int val);                                    //	встроенная в прошивку функция memset
extern	int 	_memcpy (void *dest, const void *srcptr, int num);                        //	встроенная в прошивку функция memcpy
extern	int		_memcmp	(const void* p1, const void* p2, int size);                       //	встроенная в прошивку функция memcmp

extern 	int		get_tick_count();														  //	возвращает количество тиков системного таймера с момента перезагрузки (примерно 510 тиков в секунду)
extern 	int		set_update_period (int enable, int period); 								  //    запуск таймера текущего экрана. колбэк таймера screen_job_func; cmd=0 остановить таймер, cmd=1 взвод таймера на количество мс равное period 
extern	int 	set_display_state_value(int state_1, int state);						  //	установка параметров экрана
extern	int 	set_close_timer(int delay_s);											  //	

extern 	int 	log_printf(int debug_level, const char *format, ...); 					  //	запись в лог устройства, debug_level=5
extern  int 	f_log_flush(char* msg);													  //	сброс буффера лога на флэш, можно указать сообщение
extern	int 	vibrate(int count, int on_ms, int off_ms);								  //	вибрация (фоновая)

extern	void* 	_pvPortMalloc (int size);                                                 //	выделение памяти прошивочная функция
extern  void	vPortFree( void *pv );                                                    //	освобождение выделенной памяти
                                                                                          //	
extern	int		get_left_side_menu_active();                                              //	возвращает переменную left_side_menu_active
                                                                                          //	
extern	int		get_current_date_time(struct datetime_* datetime);                        //	заполняет данные о текущей дате/времени, возвращает милисекунды текущей секунды
extern	int		get_current_timestamp();    						          	          //	возвращает значение текущего UNIX timestamp
extern	int		get_system_ms();    						          	        			  //	возвращает количество мс, прошедших с 01.01.1970

extern  int 	show_watchface();                                                         //	процедура отображения циферблата
extern  void 	__attribute__ ((deprecated("show_big_digit is deprecated use text_out_font instead"))) show_big_digit(int color, const char * digits, int pos_x, unsigned int pos_y, int space); 	//	отображение цифр большим шрифтом
extern  void 	text_out_font(int font, const char * text, int pos_x, unsigned int pos_y, int space); 	//	отображение цифр альтернативным шрифтом (алиас к функции show_big_digit), примеры шрифтов см. https://github.com/freebip/fwhack/blob/master/show_big_digit.md
extern	void 	vTaskDelay(int delay_ms);			                                      //	приостановить текущий процесс на время мс
                                                                                          //	
extern	int		read_flash (int addr, void *data, int size);                              //	чтение из флэш памяти
extern	int		_write_flash(int addr, void *data, int size);                              //	запись во флэш память (работать аккуратно)
//	прошивочная функция write_flash удалена из-за проблем безопасности, вместо неё используется одноименная функция BipOS
                                                                                          //	
extern	int 	get_generic_data(int info, void* buffer);                                 //	получение данных настроек часов
extern	int		set_generic_data(int info, void *buffer);                                 //	запись данных настроек часов
                                                                                          //	
extern	int		set_backlight_state(int state);                                           //	включение/отключение подсветки дисплея
extern	int		set_backlight_percent(int percent);                                       //	установка яркости дисплея в % (из ряда значений 0, 10, 20, 40, 60, 90)
extern 	int		get_backlight_value();                                                    //	получение из настроек уровня яркости (0..5)
extern	void	set_backlight();														  //	установка уровн яркости из настроек и включение подсветки

// случайные числа
extern int 		_rand();																//	получение случайного числа	
extern void 	_srand(unsigned int seed);												  // 	инициализация генератора случайных чисел

// протокол общения с телефоном
extern void		send_host_app_msg (int msg);
extern void 	send_host_app_data (short a1, short channel, unsigned int size, void *data, int a2);

// проверка состояния системы
extern int 		check_app_state(long long state);										//		проверка состояния системы
 
extern int dispatch_left_side_menu(struct gesture_ * gest);								//		процедура разбора свайпов быстрого меню

extern	void	stm32_soft_reset();														//		программная перезагрузка, включение часов длительным нажатием
extern	int 	iwdg_reboot();															//		программная перезагрузка часов, часы включаются

extern 	unsigned char 	get_last_heartrate();											//		получение измеренного последнего значения с датчика сердцебиения
extern 	int		set_hrm_mode(int hrm_mode);												//		установка режима измерения пульса 0x20 - однократный
extern  void* 	get_hrm_struct();														//		получение указателя на данные датчика сердца

// Функции навигации
extern	void switch_gps_pressure_sensors(int mode);										//	включение/отключение сенсоров GPS и барометра
extern	navi_struct_* get_navi_data(navi_struct_ *navi_data);							//	получение данных GPS и барометра
extern	int is_gps_fixed();																//	проверка готовности GPS

// Функции компаса
extern	short	compass_get_degree();													//	получение текущего азимута компаса
extern	int		compass_need_calibration();												//	получение статуса необходимости калибровки компаса
extern	void	compass_task_resume();													//	выполнение инициализации компаса
extern	int 	sensors_enable(int sensor, int enable);									//	включение/отключение сенсоров


// Функции создания нотификаций (всплывающих уведомлений)
extern int add_notification(int notif_type, int timestamp, char *title, char *msg, char *app_name);	//	создание и добавление в список уведомлений нового уведомления
extern int create_and_show_notification(int notif_type, char *title, char *msg, char *app_name);	//	создание и отображение созданного уведомления (остается в списке)

// Функция проверки заряда батареи
extern int get_battery_charge();	//	возвращает заряд батареи в %


//	экран DND
extern void silence_page_create_frame();

// Функции библиотеки
// работа с зыком
extern 	int 	get_selected_locale();	                                                  //	получение локали на основе выбранного языка меню мода
extern 	int		get_stored_lang();                                                        //	получение выбранного в меню мода языка меню
extern 	int		store_selected_lang(int lang);                                            //	запись значения языка меню мода
extern	int		get_system_locale();                                                      //	получение системной локали

// прочие функции
//	рисование прогрессбара по середине экрана
extern 	void 	draw_progressbar(int pos_y, int width, int height, int color_bg, int color_fg, int progress_max, int progress, int border, int repaint, const char* text);
extern 	void 	dump_mem (void * address, int len);                                       //	дамп памяти в лог устройства в формате ti-txt
extern 	int 	get_res_count();                                                          //	получение количества ресурсов
extern	void* 	pvPortMalloc (int size);                                                 	//	выделение памяти библиотечная функция (рекомендуется вместо _pvPortMalloc)
extern	int		write_flash(int addr, void *data, int size);                              	//	запись во флэш память (работать аккуратно)
																						 
// меню
extern	int 	add_menu_item(struct menu_struct *	menu, char* name, void* show_func, int color, int style);		//	добавление пункта меню
extern	int 	clear_menu (struct menu_struct *	menu);								  //	очистить пользовательское меню
extern 	void 	draw_menu(struct menu_struct *	menu);                                    //	отобразить пользовательское меню
extern	int 	dispatch_menu (struct menu_struct *	menu, void *param);                   //	обработчик пользовательского меню
extern	int 	push_ret_f(struct menu_struct *	menu, void* ret_f);                       //	сохранение в стёке цепочки меню функции воврата на предыдущий пункт меню
extern	void* 	pop_ret_f(struct menu_struct *	menu);                                    //	изъятие из стека цепочки меню функции воврата на предыдущий пункт меню

// функции загрузчика
extern int 		load_elf_by_index(int index_listed, void* ret_f, unsigned int ret_param0, int argc, void** argv);      //	загрузка и запуск приложения по его индексу
extern int 		load_elf_by_name(char* name, void* ret_f, unsigned int ret_param0, int argc, void** argv);             //	загрузка и запуск приложения по его имени
extern void* 	init_elf_loader(int force_scan);                                                                       //	инициализация загрузчика приложений. force_scan - принудительное сканирование ресурсов в поисках эльфов
extern Elf_proc_* get_proc_by_addr(void* addr);												//	возвращает указатель на структуру запущенного процесса вычисляя по адрсу символа
extern void 	elf_finish(void* addr);				                                                                   //	функция завершения приложения, в качестве параметра необходимо передать адрес основной функции отображения экрана приложения
extern int 		ElfReadSettings (int index_listed, void* buffer, int offset, int len);		//	чтение секции настроек конкретного эльфа
extern int 		ElfWriteSettings (int index_listed, void* buffer, int offset, int len); 	//	запись секции настроек конкретного эльфа 
extern int 		ElfGetSettingsSize (int index_listed);                                  	//	возвращает размер секции настроек .elf.settings, хранящихся в ресурсах  
extern int 		show_elf_res_by_id(int index_listed, int res_id, int pos_x, int pos_y); 	//	отображает на экране графический ресурс конкретного эльфа, содержащийся в секции .elf.resources
																							//	нумерация ресурсов начинается с 0 для каждого эльфа  
extern int 		get_res_params(int index_listed, int res_id, struct res_params_* res_params);//	возвращает структуру res_params_ с размерами графического ресурса																						
extern int 		get_res_count(int index_listed);									// возвращает количество доступных ресурсов в источнике
extern int 		read_res_by_id (int res_id, int offset, void* buffer, int len); // чтение данных стандартного ресурса начиная с offset длиной len по его номеру res_id
extern int 		read_elf_res_by_id (int index_listed, int res_id, int offset, void* buffer, int len);		// чтение данных ресурсов (стандартных и приложения) начиная с offset длиной len по его номеру res_id
extern int		get_fw_version();															//	возвращает версию прошивки 
extern int		get_bipos_build();															//	возвращает номер сборки проекта BipOS (нумерация сквозная)

#endif

