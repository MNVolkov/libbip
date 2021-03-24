@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@
@ Writen by Maxim Volkov Maxim.N.Volkov@ya.ru (C) 2019 - 2021
@ Imported functions for Mili_chaohu.fw.latin v 1.1.6.48
@
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.file "import.s"

.include "alias.s"

.code   16
.section .text.intro
_intro_label: .string "<LibBip>"

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@ макрос import 
@ создает в секции text последовательность перехода на указанный адрес без
@ изменения других регистров
@ кроме того создает в секциях .alias и .addr указатели на алиас и реальный адрес функции
@ для поледующей релокации загрузчиком

.macro import address functionname alias_addr
	.section .text.\functionname
	.align  2
    .thumb_func
    .type   \functionname,function
    .global \functionname
    \functionname:
        push    {r0-r1}
        ldr     r0,__\functionname
        str     r0,[sp,#4]
        pop     {r0,PC}

	.global  __\functionname   
    __\functionname: .word \address+1
	
	.section .alias @.\functionname
	.global  _alias_\functionname   
	_alias_\functionname: .word \alias_addr
	
	.section .addr	@.\functionname
	.global  _addr_\functionname   
	_addr_\functionname: .word \address+1
	
	
.endm
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.text
.code   16
.global _start
_start: @b.w main
		push    {r0-r1}
        ldr     r0, =main
        str     r0,[sp,#4]
        pop     {r0,PC}
		

@	Заголовок файла релокации
.align 2
.section .header
.ascii "LIBBIP"					@ сигнатура файла релокации

.align 3, 0xFF
.word 	( label2-label1 )		@ длина секции алиасов в байтах
.word 	FW_VERSION				@ версия прошивки для которой предназначен файл релокации
.short 	0						@ минорная версия файла (обычно 0. Может быть 1,2,3 и т.д. при исправлениях внутри одной базовой версии)
.short 	( label2-label1 )>>2 	@ базовая версия файла (равна количеству прошивочных функций в библиотеке)
.align 	4, 0xFF					@ выравнивание, для обеспечения длины заголовка 32 байт 

@ конец закгловка файла релокации

.section .alias
label1:
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@		адрес в прощивке	имя функции				алиас									@имя в прошивке huami	@
@	Функции работы с текстом		
import 0x080184F4 text_out  						alias_text_out  						@	GUI_DispStringAt
import 0x08018504 text_out_center					alias_text_out_center					@	GUI_DispStringHCenterAt
import 0x08018474 text_out_center_wrap	 			alias_text_out_center_wrap				@	GUI_DispStringInRectWrap 
import 0x08018A6A text_width						alias_text_width						@	GUI_GetStringDistX
import 0x080189D8 get_text_height	 				alias_get_text_height	 				@	GUI_GetFontDistY
import 0x080189C8 get_last_text_pos_x				alias_get_last_text_pos_x				@	GUI_GetDispPosX
import 0x0804D108 get_res_string_by_id				alias_get_res_string_by_id				@	---
			
@	Функции работы с графикой		
import 0x08019060 set_bg_color						alias_set_bg_color						@	GUI_SetBkColor 
import 0x08019064 set_fg_color						alias_set_fg_color						@	GUI_SetColor
import 0x080181E4 draw_filled_rect_bg				alias_draw_filled_rect_bg				@	GUI_ClearRect
import 0x08018958 draw_filled_rect 					alias_draw_filled_rect 					@	GUI_FillRect
import 0x08018778 draw_rect							alias_draw_rect							@	GUI_DrawRect
import 0x08018758 draw_horizontal_line 				alias_draw_horizontal_line 				@	GUI_DrawHLine
import 0x08018900 draw_vertical_line				alias_draw_vertical_line				@	GUI_DrawVLine

import 0x08018770 draw_rect_or_line					alias_draw_rect_or_line					@	GUI_DrawLine
import 0x080190D8 GUI_SetPenSize					alias_GUI_SetPenSize					@	GUI_SetPenSize

import 0x08019190 set_graph_callback_to_ram_1		alias_set_graph_callback_to_ram_1		@	GUI_UC_SetEncodeUTF8
import 0x080181C8 fill_screen_bg					alias_fill_screen_bg					@	GUI_Clear
import 0x0801FF8C show_res_by_id 					alias_show_res_by_id 					@	@	HM_DrawBitmap
import 0x080187AC draw_res_by_address 				alias_draw_res_by_address 				@	GUI_DrawStreamedBitmapExAuto
import 0x0804BC30 read_flash_by_offset				alias_read_flash_by_offset				@	@	image_getdata_func

import 0x0804BE30 reg_menu							alias_reg_menu							@	@	init_activity_page_data
import 0x0805A754 show_menu_animate 				alias_show_menu_animate 				@	@	shift_frame
import 0x0803B2F0 show_menu							alias_show_menu							@	@	display_frame 
import 0x0805A82C animate_screen					alias_animate_screen					@	@	shift_frame_handle

import 0x0801141C _memclr 							alias__memclr 							@	_aeabi_memclr
import 0x08011420 _memset							alias__memset							@	memset
import 0x080113CC _memcpy							alias__memcpy							@	__aeabi_memcpy
import 0x080114C4 _memcmp 							alias__memcmp 							@	memcmp
import 0x0801149A _strlen							alias__strlen							@	strlen

import 0x080114A8 _strcmp							alias__strcmp							@	strcmp	
import 0x080114DE _strcpy							alias__strcpy							@	strcpy
import 0x080114F0 _strncmp							alias__strncmp							@	strncmp
import 0x0801146E _strncpy							alias__strncpy							@	strncpy
import 0x0801144A _strstr							alias__strstr							@	strstr
import 0x08011432 _strcat							alias__strcat							@	strcat

import 0x0802DA70 _sprintf							alias__sprintf							@   _sprintf 
import 0x08059600 set_display_state_value			alias_set_display_state_value			@	@	set_dis_state
import 0x08066004 set_close_timer					alias_set_close_timer					@	@	ui_set_close_timer

import 0x080553E0 _pvPortMalloc						alias__pvPortMalloc						@	pvPortMalloc
import 0x08067DC8 vPortFree							alias_vPortFree							@	vPortFree
import 0x08068218 vTaskDelay						alias_vTaskDelay						@	vTaskDelay 
import 0x080598B0 load_font							alias_load_font							@	@	set_hmzk_font

import 0x0804DFDC log_printf						alias_log_printf						@	@	log_raw
import 0x080406A0 f_log_flush						alias_f_log_flush						@	f_log_flush
import 0x0806A3C4 get_tick_count					alias_get_tick_count					@	xTaskGetTickCount
import 0x08066028 set_update_period					alias_set_update_period					@	ui_set_update_timer
import 0x0804D61C repaint_screen_lines				alias_repaint_screen_lines				@	@	lcd_redraw_at
import 0x0804D604 repaint_screen					alias_repaint_screen					@	@	lcd_redraw

import 0x0803B070 show_watchface  					alias_show_watchface  					@	@	dial_page_init
import 0x0804ED44 vibrate							alias_vibrate							@	@	motor_set_single
import 0x0804EC78 motor_set_complex  				alias_motor_set_complex  				@	@	motor_set_complex

import 0x08030A0C dispatch_menu_alipay_1st_screen	alias_dispatch_menu_alipay_1st_screen	@	@	alipay_auth_touch_handle
import 0x08030FC8 show_menu_alipay					alias_show_menu_alipay					@	@	alipay_main_page_create_frame
import 0x080309CC keypress_menu_alipay_1st_screen	alias_keypress_menu_alipay_1st_screen	@	@	alipay_auth_page_key_event
import 0x0804F1C8 dispatch_left_side_menu			alias_dispatch_left_side_menu			@	@	negative_page_touch_event
import 0x08043354 get_left_side_menu_active			alias_get_left_side_menu_active			@	@	get_active_source
import 0x08059018 set_left_side_menu_active 		alias_set_left_side_menu_active 		@	@	set_active_source

import 0x0804D190 get_res_flash_addr				alias_get_res_flash_addr				@	@	---
import 0x08037A20 show_alarm_set_screen 			alias_show_alarm_set_screen 			@	@	clock_switch_page_create_frame
import 0x0801FE5C show_big_digit					alias_show_big_digit					@	@	HM_DispStringAt
import 0x0801FE5C text_out_font 					alias_text_out_font 					@алиас к show_big_digit @ HM_DispStringAt

import 0x080435E8 get_current_date_time				alias_get_current_date_time				@	@	get_current_time
import 0x080435BC get_current_timestamp				alias_get_current_timestamp				@	@	get_current_rtc

import 0x080530A4 show_poweroff_message				alias_show_poweroff_message				@	@	poweroff_page_create_frame
import 0x0805A48C show_options_menu_2nd_screen		alias_show_options_menu_2nd_screen		@	@	setting_page2_create_frame
import 0x0805A64C show_menu_options					alias_show_menu_options					@	@	setting_page_create_frame
import 0x0805A340 show_options_menu_1st_screen		alias_show_options_menu_1st_screen		@	@	setting_page1_create_frame
import 0x0804E650 show_main_menu_item				alias_show_main_menu_item				@	@	main_page_touch_event
import 0x08057FE4 show_menu_touch_test				alias_show_menu_touch_test				@	@	selftest_touch_bl_page_create_frame
import 0x08043534 get_res_ret_language 				alias_get_res_ret_language 				@	@	get_cur_language

import 0x08037C30 dispatch_alarm_set  				alias_dispatch_alarm_set  				@	@	clock_switch_page_touch_handle
import 0x08037B60 alarm_set_on_click 				alias_alarm_set_on_click 				@	@	clock_switch_page_press_handle
import 0x08063660 store_and_enable_alarm			alias_store_and_enable_alarm			@	@	store_and_enable_alarm 

@работа с флэш памятью                              
import 0x08041B78 write_flash						alias_write_flash						@	@	flash_update_part_sector
import 0x08041A44 read_flash						alias_read_flash						@	@	flash_read

import 0x08043D8C get_generic_data 					alias_get_generic_data 					@	@	get_generic_data
import 0x080596B8 set_generic_data 					alias_set_generic_data 					@	@	set_generic_data

import 0x08043470 get_backlight_value				alias_get_backlight_value				@	@	get_backlight_lvl	
import 0x080591EC set_backlight_percent				alias_set_backlight_percent				@	@	set_backlight_percent
import 0x08035BB8 set_backlight_state				alias_set_backlight_state				@	@	backlight_up
import 0x080591D0 set_backlight						alias_set_backlight						@ 		set_backlight

import 0x080113A8	_rand							alias__rand								@	rand
import 0x080113BC	_srand							alias__srand							@	srand
import 0x08011774	_atoi							alias__atoi								@	atoi

import 0x0804FE90 send_host_app_msg 				alias_send_host_app_msg 				@	@	notify_slre_data
import 0x08053A7C send_host_app_data				alias_send_host_app_data				@	@	prf_send_notify

import 0x08043454 check_app_state 					alias_check_app_state 					@	@	get_app_state
import 0x0801CB5C stm32_soft_reset					alias_stm32_soft_reset					@	@	HAL_NVIC_SystemReset
import 0x0804CE34 iwdg_reboot  						alias_iwdg_reboot  						@	@	iwdg_reboot

import 0x0802FC80 get_last_heartrate				alias_get_last_heartrate				@	@	alg_get_min_heartrate
import 0x080490A8 set_hrm_mode						alias_set_hrm_mode						@	@	hrm_enable_mode
import 0x08049244 get_hrm_struct					alias_get_hrm_struct					@	@	hrm_get_result

import 0x0805D97C is_gps_fixed						alias_is_gps_fixed						@	@	sport_get_gps_state
import 0x08038628 switch_gps_pressure_sensors		alias_switch_gps_pressure_sensors		@	@	compass_location_enable
import 0x0803866C get_navi_data						alias_get_navi_data						@	@	compass_location_get

import 0x0804E7E8 add_notification					alias_add_notification					@	@	malloc_notification_and_push
import 0x080590CA create_and_show_notification		alias_create_and_show_notification		@	@	set_ancs_and_notify

import 0x08043580 get_battery_charge				alias_get_battery_charge				@	@	get_current_capacity
import 0x0805AC1C silence_page_create_frame			alias_silence_page_create_frame			@	@	silence_page_create_frame




@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.section .alias
label2:

.section .text.outro
_outro_label: .string "</LibBip end>"

