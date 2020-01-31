/////////////////////////////////////////////////////////////
//
//  FUNCTIONS LIBRARY v.1.2
//	for Amazfit Bip
//  (C) Maxim Volkov  2019 <Maxim.N.Volkov@ya.ru>
//
//	Function library for the BipOS application loader
//
/////////////////////////////////////////////////////////////
/****
v.1.2 - 06.01.2020
+	New features:
	send_host_app_msg, send_host_app_data
	get_ptr_screen_memory,
	show_elf_res_by_id, get_res_params, get_res_count
	read_elf_res_by_id

*	Fixes:
	pvPortMalloc - Was causing the watch to reboot when unable to allocate memory, now returns NULL
	read_res_by_id - Implementation bug fixed that prevented stock resources from being read

*

v.1.1 - 12.10.2019
+	New firmware features:
	+	_rand, _srand (Thanks @Rainberd)
+	New library functions:
	+	ElfReadSettings,
	+	ElfWriteSettings,
	+	ElfGetSettingsSize,
	+	show_elf_res_by_id
*	Changes: Elf_proc_ (Added parameter index_listed)

v.1.0
-	First version of the library

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

// screen structure
 struct regmenu_ {
unsigned 	char	 curr_scr;					//	screen number
unsigned	char	 swipe_scr;					//  extra screen number
unsigned	char	 overlay;					//  sign of overlay screen
			void	*dispatch_func,				//	screen gesture processing function
					*key_press,					//	function for processing clicking on the side button
					*scr_job_func,				//	callback screen update timer function
					*scr_job_buff,				//	variable for the screen update callback
					*show_menu_func,			//	screen forming function
					*show_menu_buff,			//	variable for the screen-forming function
					*long_key_press;			//	function for processing a long press on the side button
};

//	gesture structure when processing clicks on the screen and side button
 struct gesture_	{
			char	gesture;		// gesture type
			int		touch_pos_x,	// coordinates of pressing	X
					touch_pos_y;	//							Y
};

// gesture type
#define GESTURE_CLICK			1 // simple click on the screen, click coordinates in variables touch_pos_x touch_pos_y
#define GESTURE_SWIPE_UP		2 // swipe from bottom to top
#define GESTURE_SWIPE_DOWN		3 // swipe from top to bottom
#define GESTURE_SWIPE_LEFT		4 // swipe from right to left
#define GESTURE_SWIPE_RIGHT		5 // swipe from left to right

// time date structure
struct datetime_ {
unsigned short year;
unsigned char	month,
				day,
				hour,
				min,
				sec,
				weekday;
unsigned char	h24;
};

// animation view for show_menu_animate function
#define ANIMATE_LEFT		0	// animation transition screen from right to left
#define ANIMATE_RIGHT		1	// animation transition screen from left to right
#define ANIMATE_UP			2	// animation of transition of the screen from bottom to top
#define ANIMATE_DOWN		3	// animation of the transition of the screen from top to bottom

// colors
#define	COLOR_BLACK		0x000000		//	black
#define	COLOR_RED		0x0000FF		//	red
#define	COLOR_GREEN		0x00FF00		//	green
#define	COLOR_BLUE		0xFF0000		//	blue
#define	COLOR_YELLOW	0x00FFFF		//	yellow
#define	COLOR_AQUA		0xFFFF00		//	aquamarine
#define	COLOR_PURPLE	0xFF00FF		//	lilac
#define	COLOR_WHITE		0xFFFFFF		//	white
#define	COLOR_MASK		0xFFFFFF		//	color mask

// colors in short format
#define COLOR_SH_BLACK		0b0000		//	black
#define COLOR_SH_RED		0b0001		//	red
#define COLOR_SH_GREEN		0b0010		//	green
#define COLOR_SH_BLUE		0b0100		//	blue
#define COLOR_SH_YELLOW		0b0011		//	yellow
#define COLOR_SH_AQUA		0b0110		//	aquamarine
#define COLOR_SH_PURPLE		0b0101		//	lilac
#define COLOR_SH_WHITE		0b0111		//	white
#define COLOR_SH_MASK		0b1111		//	color mask

// text languages ​​(locales)
#define locale_ru_RU	1049	//	Russian
#define locale_en_US	1033	//	English
#define locale_it_IT	1040	//	Italian
#define locale_es_ES	3082	//	Spanish
#define locale_fr_FR	1036	//	French
#define locale_de_DE	1031	//	German

// language set for the mod menu returned by get_stored_lang function
#define OPT_LANG_COUNT	6		//	number of languages
#define OPT_LANG_AUTO	0x00	//	auto select by phone locale
#define OPT_LANG_RUS	0x01	//	Russian
#define OPT_LANG_ENG	0x02    //	English
#define OPT_LANG_ITA	0x03    //	Italian
#define OPT_LANG_SPA	0x04    //	Spanish
#define OPT_LANG_FRA	0x05    //	French
#define OPT_LANG_DEU	0x06    //	German

// system settings data (function get_generic_data / set_generic_data)
#define GENERIC_DATA_LOCALE		36		//	phone locale

//  menu structure
//	settings menu
#define MAX_MENU_ITEM_NAME_LEN	64
#define MAX_MENU_ENCLOSURE		15		//	maximum menu nesting
#define ITEMS_ON_PAGE			3
#define MAX_MENU_ITEMS_COUNT	15
#define MAX_LINES_PER_ITEM		3
//	menu item display options
#define MENU_H_MARGIN	10

// menu item statuses
#define MENU_ITEM_STYLE_NORMAL		0		//	regular menu item			">". Procedure start through show_animate with return function parameter
#define MENU_ITEM_STYLE_LIST		1		//	regular menu item			">". Starting a procedure with menu and index parameters
#define MENU_ITEM_STYLE_UNCHECKED	2		//	checkbox not marked with	"0". Starting a procedure with menu and index parameters
#define MENU_ITEM_STYLE_CHECKED		3		//	checkbox marked with		"v". Starting a procedure with menu and index parameters
#define MENU_ITEM_STYLE_TOGGLE_ON	4		//	switch 						"( o)". Starting a procedure with menu and index parameters
#define MENU_ITEM_STYLE_TOGGLE_OFF	5		//	switch						"(o )". Starting a procedure with menu and index parameters
#define MENU_ITEM_STYLE_LOCKED		6		//	the lock icon is locked.
#define MENU_ITEM_STYLE_DISABLED	7		//	the item is currently not available the "X" icon

// "Quick" application (launch when entering the side menu)
#define LEFT_SIDE_APP_COUNT		4
#define LEFT_SIDE_APP_NONE		0
#define LEFT_SIDE_APP_CALC		1
#define LEFT_SIDE_APP_CALEND	2
#define LEFT_SIDE_APP_FLASH		3


struct menu_item_struct {
//unsigned char 	index;		//	menu item number
	char		name[MAX_MENU_ITEM_NAME_LEN+1];		//	menu item name
	void*		show_func;		//	start function
	int 		item_color;		//	menu item color
	int			item_style;		//	menu bar status
	int 		item_param0;	//	arbitrary parameter of the menu item
//unsigned char	premium;		//	premium version item
};

struct menu_struct {
struct 	menu_item_struct	items[MAX_MENU_ITEMS_COUNT];	//	menu structure
unsigned char 				page;							//	number of the currently displayed page
unsigned char				item_count;						//	number of menu items
		void*				show_func;						//	address of the display function of the current screen
		void*				ret_f[MAX_MENU_ENCLOSURE];		//	pointer to return to the previous screen
		int 				menu_level;						//	current nesting level
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

// parameters of graphic resources
struct res_params_ {
  short width; 		//	width in px
  short height; 	//	height in px
};

#define INDEX_MAIN_RES	0xFFFF0000

typedef struct {						//	structure of the running process
	unsigned int 	process_id;			//	process identifier assigned by the loader
	int				index_listed;		//	elf index in the bootloader list
	void* 			base;				//	pointer to the memory allocated for the process
	unsigned int	size;				//	amount of memory allocated for the process
	void*			ret_f;				//	process return point
	unsigned int	ret_param0;			//	return function parameter
	void(*elf_finish)(void* param0);	//	pointer to the elf completion procedure, here you need to pass
	void(*entry_point)(void* param0);	//	pointer to the elf launch procedure, entry point. Param0 = pointer to the proc structure of the running process
	void*			param;				//	user parameter, you can store anything, for example pointers instead of temp_buf_2 for background processes
	int				argc;				//	number of parameters when running the elf
	void**			argv;				//	parameters when starting the elf
} Elf_proc_;

// firmware versions
#define LATIN_1_1_5_12		11512
#define LATIN_1_1_5_16		11516
#define LATIN_1_1_5_36		11536
#define LATIN_1_1_5_56		11556
#define NOT_LATIN_1_1_2_05	11205
#define UNI_LATIN			55555

// statuses of get_app_state function
#define APP_STATE_BT_CON		0x200


// data structure containing alarm information
struct alarm_struct {
unsigned char index; 		//  bit_7 - on / off; bit_0..4 - alarm number
unsigned char hours;        //  clock
unsigned char minutes;      //  minutes
unsigned char repeat;       //  bit_0 - bit_7 - days of the week from Monday to Friday
                            //  value 128 - once; 127 - every day;
							//	31 - on workers; 0 - not created; 60 - on weekends
};

// data structure containing information about the number of the selected page and data from 10 alarms
struct alarm_10 {
unsigned char 	page; 		//	current page for setting alarms
unsigned char 	field_0;	//	unknown structure field, possibly not used
struct 			alarm_struct alarms[10];	//	array of 10 alarms
};


// heart sensor data structures
// 1.1.5.12, 1.1.5.36
typedef struct {
int 			t_start;
short 			last_hr;
unsigned char 	field_2;
unsigned char 	field_3;
unsigned char 	field_4;
unsigned char 	heart_rate;			//	frequency, beats / min; >200 - value not available
unsigned char 	ret_code;			//	measurement status is 0-finished, >0 measurement is not finished
unsigned char 	field_5;
} hrm_data_struct;

// 1.1.2.05
typedef struct{
int 			t_start;
short 			last_hr;
unsigned char 	heart_rate;			//	frequency, beats / min; >200 - value not available
unsigned char 	ret_code;			//	measurement status is 0-finished, >0 measurement is not finished
}  hrm_data_struct_legacy;


// Global variables
unsigned char get_var_current_screen();                                 //	active screen number
unsigned char set_var_current_screen(unsigned char val); 			    //
unsigned char get_var_swipe_screen_active_number();                     //	active screen subnumber
unsigned char set_var_swipe_screen_active_number(unsigned char val);    //
unsigned char get_var_menu_overlay();		                            //	active overlay screen (notification, incoming call, etc.)
unsigned char set_var_menu_overlay(unsigned char val);			        //
		void* get_ptr_menu_dispatch_func();                             //	pointer to the touchscreen handler and the click of a button
		void* set_ptr_menu_dispatch_func(void* val);                    //
		void* get_ptr_key_press_func();                                 //	pointer to the button click handler
		void* set_ptr_key_press_func(void* val);                        //
		void* get_ptr_screen_job_func();                                //	pointer to the screen timer handler
		void* set_ptr_screen_job_func(void* val);                       //
		void* get_ptr_temp_buf_1();                                     //	pointer to the screen data (lost when the overlay screen appears)
		void* get_ptr_show_menu_func();                                 //	pointer to the function of creating the current screen
		void* set_ptr_show_menu_func(void* val);                        //
		void* get_ptr_temp_buf_2();                                     //	pointer to the screen data (not lost when the overlay screen appears)
		void* get_ptr_long_key_press_func();                            //	pointer to the handler for a long press on the button (not valid on all screens)
		void* set_ptr_long_key_press_func(void* val);                   //
unsigned char get_var_left_side_menu_active();                          //	value of the variable 1-call through the left menu; 0-through the main menu
		  int get_var_free_ram();                                       //	amount of free memory
		void* get_ptr_screen_memory();                                  //	pointer to screen memory
		void* get_ptr_free_list();										//	pointer to the tree of free memory blocks

// Firmware functions
extern void 	reg_menu(void *regmenu_, int param);                                      //	register the created screen in the operating system
extern	int		show_menu_animate(void* show_menu_function, int param, int animate);      //	display a menu with screen shift animation, param - parameter passed to the show_menu_function function
extern 	int 	show_menu (void* show_menu_function, int param);                          //	display the menu without animation, param - parameter passed to the show_menu_function function
extern  int		text_width(const char *text);                                             //	returns the width of the text that will be displayed in the current font
extern void 	text_out(const char *text, int pos_x, int pos_y);                         //	text output by coordinates, pos_x, pos_y coordinates of the upper left corner of the inscription
extern void 	text_out_center(const char *text, int pos_center_x, int pos_y);           //	text output by coordinates, pos_x, pos_y coordinates of the middle of the top edge of the inscription
extern	int		get_text_height();                                                        //	returns the height of the text, the current font
extern 	int		get_last_text_pos_x();													  //	returns the X coordinate immediately after the last displayed text
                                                                                          //
extern void 	set_bg_color(long color);                                                 //	choice of background color for subsequent graphic operations
extern void 	set_fg_color (long color);                                                //	color selection for subsequent graphic operations
extern void		fill_screen_bg();                                                         //	fill the screen with the background color
extern 	int		set_graph_callback_to_ram_1();                                            //	use of this function must be done in accordance with an example
extern void 	repaint_screen_lines(int from, int to);                                   //	update the lines on the screen (copying from video memory to the display)
extern void 	repaint_screen();                                                         //	refresh the whole screen ((copying from video memory to display))
extern void 	draw_horizontal_line(int pos_y, int from_x, int to_x);                    //	draw a horizontal line
extern void 	draw_rect(int from_x, int from_y, int to_x, int to_y);                    //	draw a rectangle
extern void 	draw_vertical_line(int pos_x, int from_y, int to_y);                      //	draw a vertical line
extern void 	draw_filled_rect(int from_x, int from_y, int to_x, int to_y);             //	draw the filled rectangle
extern void 	draw_filled_rect_bg(int from_x, int from_y, int to_x, int to_y);          //	draw a rectangle filled with the color of the background
extern void 	draw_rect_or_line(int from_x, int from_y, int to_x, int to_y);            //	draw a rectangle ???
                                                                                          //
extern	void	load_font ();                                                             //	font download
extern	 int	get_res_ret_language();                                                   //	get the language of the clock interface
extern	void	show_res_by_id(int res_ID, int pos_x, int pos_y);                         //	display a graphic resource by its number
                                                                                          //
extern	 int	_sprintf (char *buf, const char *format, ...);                            //	sprintf function built into the firmware
                                                                                          //
extern	int		_strlen (const char *str);                                                //	strlen function built into the firmware
extern	int		_strcmp (const char * string1, const char * string2);                     //	strcmp function built into the firmware
extern	int		_strncmp (const char * string1, const char * string2, int num);           //	strncmp function built into the firmware
extern	int		_strcpy (char * destptr, const char * srcptr);                            //	strcpy function built into the firmware
extern	int		_strncpy (char * destptr, const char * srcptr, int num);                  //	strncpy function built into the firmware
                                                                                          //
extern	int		_memclr (void *buf, int len);                                             //	memclr function built into the firmware
extern	int		_memset (void *buf, int len, int val);                                    //	built-in memset function
extern	int		_memcpy (void *dest, const void *srcptr, int num);                        //	built-in memcpy function
extern	int		_memcmp	(const void* p1, const void* p2, int size);                       //	built-in memcmp function

extern	int		get_tick_count();														  //	returns the number of ticks of the system timer from the moment of reboot (approximately 510 ticks per second)
extern	int		set_update_period(int cmd, int period); 								  //    start the timer of the current screen. timer callback screen_job_func; cmd = 0 stop the timer, cmd = 1 cocking the timer for the number of ms equal to period
extern	int		set_display_state_value(int state_1, int state);						  //	set screen parameters
extern	int		set_close_timer(int delay_s);											  //

extern 	int 	log_printf(int debug_level, const char *format, ...); 					  //	write to the device log, debug_level = 5
extern	int 	vibrate(int count, int on_ms, int off_ms);								  //	vibration (background)

extern	void* 	_pvPortMalloc (int size);                                                 //	memory allocation firmware function
extern  void	vPortFree( void *pv );                                                    //	release the allocated memory
                                                                                          //
extern	int		get_left_side_menu_active();                                              //	returns the variable left_side_menu_active
                                                                                          //
extern	int		get_current_date_time(struct datetime_ * datetime);                       //	returns data about the current date / time
extern  int 	show_watchface();                                                         //	dial display procedure
extern  void 	show_big_digit(int color, const char* digits, int pos_x, unsigned int pos_y, int space); 	//	display numbers in large print
extern	void 	vTaskDelay(int delay_ms);			                                      //	pause the current process for the duration of ms
                                                                                          //
extern	int		read_flash (int addr, void *data, int size);                              //	read from flash memory
extern	int		write_flash(int addr, void *data, int size);                              //	write to flash memory (work carefully)
                                                                                          //
extern 	int 	get_generic_data(int info, void* buffer);                                 //	get the clock settings data
extern 	int		set_generic_data(int info, void *buffer);                                 //	record clock settings data
                                                                                          //
extern	int		set_backlight_state(int state);                                           //	enable / disable display backlight
extern	int		set_backlight_percent(int percent);                                       //	set the brightness of the display in% (from a number of values ​​0, 10, 20, 40, 60, 90)
extern 	int		get_backlight_value();                                                    //

// random numbers
extern int		_rand();																	//	get a random number
extern void 	_srand(unsigned int seed);													//	initialization of the random number generator

// telephone communication protocol
extern void		send_host_app_msg (int msg);
extern void 	send_host_app_data (short a1, short channel, unsigned int size, void *data, int a2);

// check system status
extern int 		check_app_state(long long state);											//	check system status

extern int 		dispatch_left_side_menu(struct gesture_ * gest);							//	quick menu swipe parsing procedure

extern	void	stm32_soft_reset();															//	soft reset, turn on the clock with a long press
extern	int 	iwdg_reboot();																//	software reset of the clock, the clock turns on

extern 	unsigned char 	get_last_heartrate();												//	receive the measured last value from the heart rate sensor
extern 	int		set_hrm_mode(int hrm_mode);													//	setting the pulse measurement mode 0x20 - single
extern 	void* 	get_hrm_struct();															//	get a pointer to the heart sensor data


// library functions
// work with the language
extern 	int 	get_selected_locale();	                                                  //	get the locale based on the selected language of the mod menu
extern 	int		get_stored_lang();                                                        //	get the menu language selected in the menu
extern 	int		store_selected_lang(int lang);                                            //	write the language value of the mod menu
extern	int		get_system_locale();                                                      //	get the system locale

// other functions
// draw progress bar in the middle of the screen
extern 	void 	draw_progressbar(int pos_y, int width, int height, int color_bg, int color_fg, int progress_max, int progress, int border, int repaint, const char* text);
extern 	void 	dump_mem (void * address, int len);                                       //	memory dump to the device log in ti-txt format
extern 	int 	get_res_count();                                                          //	get the amount of resources
extern	void* 	pvPortMalloc (int size);                                                  //	memory allocation library function (recommended instead of _pvPortMalloc)

// menu
extern	int 	add_menu_item(struct menu_struct *	menu, char* name, void* show_func, int color, int style);		//	add menu item
extern	int 	clear_menu (struct menu_struct *	menu);								  //	clear user menu
extern 	void 	draw_menu(struct menu_struct *	menu);                                    //	display user menu
extern	int 	dispatch_menu (struct menu_struct *	menu, void *param);                   //	custom menu handler
extern	int 	push_ret_f(struct menu_struct *	menu, void* ret_f);                       //	save in the stack of the menu chain the function of returning to the previous menu item
extern	void* 	pop_ret_f(struct menu_struct *	menu);                                    //	remove from the stack of the menu chain the function of returning to the previous menu item

// quick launch menu
extern 	int 	get_stored_left_side_function();                                          //	get the value of the "quick" start
extern 	int 	store_selected_left_side(int left_side);                                  //	write the value of the "quick" start

// bootloader functions
extern int 		load_elf_by_index(int index_listed, void* ret_f, unsigned int ret_param0, int argc, void** argv);      //	download and launch the application by its index
extern int 		load_elf_by_name(char* name, void* ret_f, unsigned int ret_param0, int argc, void** argv);             //	download and launch the application by its name
extern void* 	init_elf_loader(int force_scan);                                                                       //	initialize the application loader. force_scan - force scanning resources in search of elves
extern Elf_proc_* get_proc_by_addr(void* addr);												//	returns a pointer to the structure of the running process by calculating the address of the character
extern void 	elf_finish(void* addr);				                                                                   //	application termination function, as a parameter it is necessary to pass the address of the main application screen display function
extern int 		ElfReadSettings (int index_listed, void* buffer, int offset, int len);		//	read the settings section of a specific elf
extern int 		ElfWriteSettings (int index_listed, void* buffer, int offset, int len); 	//	record the settings section of a specific elf
extern int 		ElfGetSettingsSize (int index_listed);                                  	//	returns the size of the settings section .elf.settings stored in resources
extern int 		show_elf_res_by_id(int index_listed, int res_id, int pos_x, int pos_y); 	//	displays the graphic resource of a specific elf contained in the .elf.resources section on the screen
																							//	resource numbering starts at 0 for each elf
extern int 		get_res_params(int index_listed, int res_id, struct res_params_* res_params);//	returns a res_params_ structure with the size of the graphic resource
extern int 		get_res_count(unsigned int index_listed);									// returns the number of available resources in the source
extern int 		read_res_by_id (int res_id, int offset, void* buffer, int len); 			// read standard resource data starting with offset len ​​length by its res_id number
extern int 		read_elf_res_by_id (unsigned int index_listed, int res_id, int offset, void* buffer, int len); // read resource data (standard and application) starting with offset len ​​length by its res_id number
extern int		get_fw_version();															//	returns firmware version

// resources
#define BIG_COLON		573
#define ICON_RECYCLE	293
#define ICON_ALARM		227
#define ICON_CALEND		228
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
