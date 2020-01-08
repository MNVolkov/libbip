/////////////////////////////////////////////////////////////
//
//  FUNCTIONS LIBRARY v.1.2
//	for Amazfit Bip
//  (C) Maxim Volkov  2019 <Maxim.N.Volkov@ya.ru>
//
//	Библиотека функций для загрузчика приложений BipOS
//  
/////////////////////////////////////////////////////////////
/****
v.1.2 - 06.01.2020
+	добавлены функции прошивки:
	send_host_app_msg, send_host_app_data
	get_ptr_screen_memory,  
	show_elf_res_by_id, get_res_params, get_res_count 
	read_elf_res_by_id
	
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

#define LIBBIP_VERSION "1.2"

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
unsigned 	char	 curr_scr;					//	номер экрана
unsigned	char	 swipe_scr;					//  дополниетльный номер экрана
unsigned	char	 overlay;					//  признак оверлейного экрана
			void	*dispatch_func,				//	функция обработки жестов экрана
					*key_press,					//	функция обработки нажатия на боковую кнопку
					*scr_job_func,				//	колбэк функция таймера обновления экрана
					*scr_job_buff,				//	переменная для колбэка обновления экрана 
					*show_menu_func,			//	функция формирования экрана
					*show_menu_buff,			//	переменная для функции формирования экрана 
					*long_key_press;			//	функция обработки долгого нажатия на боковую кнопку
};

//	структура жеста при обработке нажатий на экран и боковую кнопку
 struct gesture_	{
			char	gesture;		// тип жеста
			int		touch_pos_x,	// координаты нажатия 	X
					touch_pos_y;	//						Y
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

// языки текста (локали)
#define locale_ru_RU	1049	//	русский
#define	locale_en_US	1033	//	английский
#define	locale_it_IT	1040	//	итальянский
#define	locale_es_ES	3082	//	испанский
#define	locale_fr_FR	1036	//	французский
#define	locale_de_DE	1031	//	немецкий

// язык установленный для меню мода, возвращаемый функцией get_stored_lang
#define	OPT_LANG_COUNT	6		//	количество языков
#define	OPT_LANG_AUTO	0x00	//	автовыбор по локали телефона
#define	OPT_LANG_RUS	0x01	//	русский
#define	OPT_LANG_ENG	0x02    //	английский
#define	OPT_LANG_ITA	0x03    //	итальянский
#define	OPT_LANG_SPA	0x04    //	испанский
#define	OPT_LANG_FRA	0x05    //	французский
#define	OPT_LANG_DEU	0x06    //	немецкий

// данные настроек системы ( функция get_generic_data / set_generic_data )
#define GENERIC_DATA_LOCALE		36		//	локаль телефона

//  структура меню
//	меню настроек 
#define MAX_MENU_ITEM_NAME_LEN	64
#define MAX_MENU_ENCLOSURE		15		//	максимальная вложенность меню
#define ITEMS_ON_PAGE			3
#define MAX_MENU_ITEMS_COUNT	15
#define MAX_LINES_PER_ITEM		3
//	параметры отображения пунктов меню
#define MENU_H_MARGIN	10

// статусы пунктов меню
#define MENU_ITEM_STYLE_NORMAL		0		//	обычный пункт меню	значок ">". Запуск процедуры через show_animate c параметром функции возврата
#define MENU_ITEM_STYLE_LIST		1		//	обычный пункт меню	значок ">".	Запуск процедуры с параметрами menu и index
#define MENU_ITEM_STYLE_UNCHECKED	2		//	чекбокс не отмечен	значок "0".	Запуск процедуры с параметрами menu и index
#define MENU_ITEM_STYLE_CHECKED		3		//	чекбокс отмечен		значок "v".	Запуск процедуры с параметрами menu и index
#define MENU_ITEM_STYLE_TOGGLE_ON	4		//	переключатель включен	значок "( о)".	Запуск процедуры с параметрами menu и index
#define MENU_ITEM_STYLE_TOGGLE_OFF	5		//	переключатель включен	значок "(о )".	Запуск процедуры с параметрами menu и index
#define MENU_ITEM_STYLE_LOCKED		6		//	заблокирован		значок "замок".	
#define MENU_ITEM_STYLE_DISABLED	7		//	пункт в данный момент не доступен	значок "Х"

// "Быстрое" приложение (запуск при входе в боковое меню)
#define LEFT_SIDE_APP_COUNT		4
#define LEFT_SIDE_APP_NONE		0
#define LEFT_SIDE_APP_CALC		1
#define LEFT_SIDE_APP_CALEND	2
#define LEFT_SIDE_APP_FLASH		3

 
struct menu_item_struct {
//unsigned char 	index;		//	номер пункта меню
	char		name[MAX_MENU_ITEM_NAME_LEN+1]; 		//	название пункта меню
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

struct res_struct {// sizeof=32+4
char 			res_type[5];	//	NERES, HMRES
char 			version;		//	 
char 			magic[4];		//	E8 09 1A D7 = 0xD71A09E8
char			reserved_0[4];	//	03 00 00 00
char 			reserved_1[2];	//	FF FF
unsigned int	reserved[4];	//  FF..FF
unsigned int	count;			//		
};

#define RES_MAGIC		0xD71A09E8
#define NERES_ADDRESS	0x2A0000

// параметры графических ресурсов
struct res_params_ {
  short width; 		//	ширина в рх
  short height; 	//	высота в рх 	
};

#define INDEX_MAIN_RES	0xFFFF0000

typedef struct {				//	структура запущенного процесса
	unsigned int 	process_id;	//	идентификатор процесса, присваивается загрузчиком
	int			index_listed;	//	индекс эльфа в списке загрузчика
	void* 			base;		//	указатель на выделенную под процесс память
	unsigned int	size;		//	количество выделенной под процесс памяти
	void*			ret_f;		//	точка возврата процесса
	unsigned int	ret_param0;	//	параметр функции возврата
	void(*elf_finish)(void* param0);	//	указатель на процедуру завершения эльфа, сюда надо передать 
	void(*entry_point)(void* param0);	//	указатель на процедуру запуска эльфа, точка входа. Param0 = указатель на структуру proc запущенного процесса 
	void*			param;		//	пользовательский параметр, можно хранить что угодно, например указатели вместо temp_buf_2 для фоновых процессов				
	int				argc;		//	количество параметров при запуске эльфа
	void**			argv;		//	параметры при запуске эльфа
} Elf_proc_;

// версии прошивок
#define LATIN_1_1_5_12		11512
#define LATIN_1_1_5_16		11516
#define LATIN_1_1_5_36		11536
#define LATIN_1_1_5_56		11556
#define NOT_LATIN_1_1_2_05	11205
#define UNI_LATIN			55555

// статусы функции get_app_state
#define APP_STATE_BT_CON		0x200


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


// структуры данных датчика сердца
// 1.1.5.12, 1.1.5.36
typedef struct {
int 			t_start;
short 			last_hr;
unsigned char 	field_2;
unsigned char 	field_3;
unsigned char 	field_4;
unsigned char 	heart_rate;			//	частота, ударов/мин; >200 - значение не доступно
unsigned char 	ret_code;			//	статус измерения 0-закончено, >0 измерение не закончено
unsigned char 	field_5;
} hrm_data_struct ;

// 1.1.2.05
typedef struct{
int 			t_start;
short 			last_hr;
unsigned char 	heart_rate;			//	частота, ударов/мин; >200 - значение не доступно
unsigned char 	ret_code;			//	статус измерения 0-закончено, >0 измерение не закончено
}  hrm_data_struct_legacy;


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
		
// Функции прошивки
extern void 	reg_menu(void *regmenu_, int param);		                              //	регистрация создаваемого экрана в операционной системе
extern	int		show_menu_animate(void* show_menu_function, int param, int animate);      //	отображение меню с анимацией сдвига экрана, param - параметр передаваемый функции show_menu_function
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
extern int		set_graph_callback_to_ram_1();                                            //	использование данной функции необходимо производить в соответствии с примером
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
extern 	int		set_update_period (int cmd, int period); 								  //    запуск таймера текущего экрана. колбэк таймера screen_job_func; cmd=0 остановить таймер, cmd=1 взвод таймера на количество мс равное period 
extern	int 	set_display_state_value(int state_1, int state);						  //	установка параметров экрана
extern	int 	set_close_timer(int delay_s);											  //	

extern 	int 	log_printf(int debug_level, const char *format, ...); 					  //	запись в лог устройства, debug_level=5
extern	int 	vibrate(int count, int on_ms, int off_ms);								  //	вибрация (фоновая)

extern	void* 	_pvPortMalloc (int size);                                                 //	выделение памяти прошивочная функция
extern  void	vPortFree( void *pv );                                                    //	освобождение выделенной памяти
                                                                                          //	
extern	int		get_left_side_menu_active();                                              //	возвращает переменную left_side_menu_active
                                                                                          //	
extern	int		get_current_date_time(struct datetime_* datetime);                        //	возвращает данные о текущей дате/времени
extern  int 	show_watchface();                                                         //	процедура отображения циферблата
extern  void 	show_big_digit(int color, const char * digits, int pos_x, unsigned int pos_y, int space); 	//	отображение цифр большим шрифтом
extern	void 	vTaskDelay(int delay_ms);			                                      //	приостановить текущий процесс на время мс
                                                                                          //	
extern	int		read_flash (int addr, void *data, int size);                              //	чтение из флэш памяти
extern	int		write_flash(int addr, void *data, int size);                              //	запись во флэш память (работать аккуратно)
                                                                                          //	
extern	int 	get_generic_data(int info, void* buffer);                                 //	получение данных настроек часов
extern	int		set_generic_data(int info, void *buffer);                                 //	запись данных настроек часов
                                                                                          //	
extern	int		set_backlight_state(int state);                                           //	включение/отключение подсветки дисплея
extern	int		set_backlight_percent(int percent);                                       //	установка яркости дисплея в % (из ряда значений 0, 10, 20, 40, 60, 90)
extern 	int		get_backlight_value();                                                    //	

// случайные числа
extern int 		_rand();																		  //	получение случайного числа	
extern void 	_srand(unsigned int seed);													  // 	инициализация генератора случайных чисел

// протокол общения с телефоном
extern void		send_host_app_msg (int msg);
extern void 	send_host_app_data (short a1, short channel, unsigned int size, void *data, int a2);

// проверка состояния системы
extern int 		check_app_state(long long state);											//		проверка состояния системы
 
extern int dispatch_left_side_menu(struct gesture_ * gest);								//		процедура разбора свайпов быстрого меню

extern	void	stm32_soft_reset();														//		программная перезагрузка, включение часов длительным нажатием
extern	int 	iwdg_reboot();																//		программная перезагрузка часов, часы включаются

extern 	unsigned char 	get_last_heartrate();															//		получение измеренного последнего значения с датчика сердцебиения
extern 	int		set_hrm_mode(int hrm_mode);													//	установка режима измерения пульса 0x20 - однократный
extern  void* 	get_hrm_struct();															//	получение указателя на данные датчика сердца


// Функции библиотеки
// работа с зыком
extern 	int 	get_selected_locale();	                                                  //	получение локали на основе выбранного языка меню мода
extern 	int		get_stored_lang();                                                        //	получение выбранного в меню мода языка меню
extern 	int		store_selected_lang(int lang);                                            //	запись значения языка меню мода
extern	int		get_system_locale();                                                      //	получение системной локали

// прочие функции
//	рисование прогрессбара по сереине экрана
extern 	void 	draw_progressbar(int pos_y, int width, int height, int color_bg, int color_fg, int progress_max, int progress, int border, int repaint, const char* text);
extern 	void 	dump_mem (void * address, int len);                                       //	дамп памяти в лог устройства в формате ti-txt
extern 	int 	get_res_count();                                                          //	получение количества ресурсов
extern	void* 	pvPortMalloc (int size);                                                 //	выделение памяти библиотечная функция (рекомендуется вместо _pvPortMalloc)
																						  
// меню
extern	int 	add_menu_item(struct menu_struct *	menu, char* name, void* show_func, int color, int style);		//	добавление пункта меню
extern	int 	clear_menu (struct menu_struct *	menu);								  //	очистить пользовательское меню
extern 	void 	draw_menu(struct menu_struct *	menu);                                    //	отобразить пользовательское меню
extern	int 	dispatch_menu (struct menu_struct *	menu, void *param);                   //	обработчик пользовательского меню
extern	int 	push_ret_f(struct menu_struct *	menu, void* ret_f);                       //	сохранение в стёке цепочки меню функции воврата на предыдущий пункт меню
extern	void* 	pop_ret_f(struct menu_struct *	menu);                                    //	изъятие из стека цепочки меню функции воврата на предыдущий пункт меню

// меню быстрого запуска
extern 	int 	get_stored_left_side_function();                                          //	получение значения "быстрого" запуска
extern 	int 	store_selected_left_side(int left_side);                                  //	запись значения "быстрого" запуска

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
extern int 		get_res_count(unsigned int index_listed);									// возвращает количество доступных ресурсов в источнике
extern int 		read_res_by_id (int res_id, int offset, void* buffer, int len); // чтение данных стандартного ресурса начиная с offset длиной len по его номеру res_id
extern int 		read_elf_res_by_id (unsigned int index_listed, int res_id, int offset, void* buffer, int len);		// чтение данных ресурсов (стандартных и приложения) начиная с offset длиной len по его номеру res_id
extern int		get_fw_version();															//	возврвщает версию прошивки 

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

#endif

