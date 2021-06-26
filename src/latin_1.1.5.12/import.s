@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@
@ Writen by Maxim Volkov Maxim.N.Volkov@ya.ru (C) 2019 - 2021
@ Imported functions for Mili_chaohu.fw.latin v 1.1.5.12
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
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

@	Функции работы с текстом
import 0x080184C8 text_out							alias_text_out
import 0x080184D8 text_out_center                   alias_text_out_center
import 0x08018C7C text_width                        alias_text_width
import 0x08018BD0 get_text_height                   alias_get_text_height
import 0x08018BC0 get_last_text_pos_x               alias_get_last_text_pos_x

@	Функции работы с графикой                       
import 0x0801878C draw_rect_or_line                 alias_draw_rect_or_line
import 0x080192D4 set_bg_color                      alias_set_bg_color
import 0x080192D8 set_fg_color                      alias_set_fg_color 
import 0x08018740 draw_horizontal_line              alias_draw_horizontal_line
import 0x080187FC draw_rect                         alias_draw_rect
import 0x080189BC draw_vertical_line                alias_draw_vertical_line
import 0x08018A68 draw_filled_rect                  alias_draw_filled_rect
import 0x08020240 show_res_by_id                    alias_show_res_by_id
import 0x08018164 fill_screen_bg                    alias_fill_screen_bg
import 0x08019418 set_graph_callback_to_ram_1       alias_set_graph_callback_to_ram_1
import 0x08019418 GUI_UC_SetEncodeUTF8       		alias_GUI_UC_SetEncodeUTF8
import 0x08018180 draw_filled_rect_bg               alias_draw_filled_rect_bg
import 0x080503B8 repaint_screen_lines              alias_repaint_screen_lines
import 0x080503A0 repaint_screen                    alias_repaint_screen
import 0x0804ECEC read_flash_by_offset              alias_read_flash_by_offset 
import 0x08018878 draw_res_by_address               alias_draw_res_by_address 

@	Функции работы с меню                           
import 0x0804EEEC reg_menu                          alias_reg_menu
import 0x08056144 show_poweroff_message             alias_show_poweroff_message  
import 0x0805D3A4 show_options_menu_2nd_screen      alias_show_options_menu_2nd_screen
import 0x0805D5C8 show_menu_options                 alias_show_menu_options
import 0x0805D650 show_menu_animate                 alias_show_menu_animate
import 0x0803C854 show_menu                         alias_show_menu
import 0x0805D21C show_options_menu_1st_screen      alias_show_options_menu_1st_screen
import 0x0805D738 animate_screen                    alias_animate_screen

@	Функции языка                                   
import 0x0805C7C8 load_font                         alias_load_font
import 0x080466AC get_res_ret_language              alias_get_res_ret_language

import 0x0802F218 _sprintf                          alias__sprintf

import 0x08011486 _strlen                           alias__strlen
import 0x08011494 _strcmp		                    alias__strcmp		
import 0x080114CA _strcpy                           alias__strcpy
import 0x080114DC _strncmp                          alias__strncmp
import 0x0801146E _strncpy                          alias__strncpy

import 0x0801141C _memclr                           alias__memclr
import 0x0801140E _memset                           alias__memset
import 0x080113CC _memcpy                           alias__memcpy
import 0x080114B0 _memcmp                           alias__memcmp

import 0x0806D6F8 get_tick_count                    alias_get_tick_count

import 0x0806928C set_update_period                 alias_set_update_period 
import 0x0805C52C set_display_state_value           alias_set_display_state_value 
import 0x08069268 set_close_timer                   alias_set_close_timer

import 0x08050D78 log_printf                        alias_log_printf
import 0x08051C7C vibrate                           alias_vibrate

import 0x080583A8 _pvPortMalloc                     alias__pvPortMalloc
import 0x0806B028 vPortFree                         alias_vPortFree

import 0x0805AF10 show_menu_touch_test              alias_show_menu_touch_test
import 0x080514AC show_main_menu_item               alias_show_main_menu_item

import 0x08032924 show_menu_alipay                  alias_show_menu_alipay 
import 0x080322F4 keypress_menu_alipay_1st_screen   alias_keypress_menu_alipay_1st_screen
import 0x080520D0 dispatch_left_side_menu           alias_dispatch_left_side_menu
import 0x080464CC get_left_side_menu_active         alias_get_left_side_menu_active 
import 0x0805BF44 set_left_side_menu_active         alias_set_left_side_menu_active 

import 0x08046760 get_current_date_time             alias_get_current_date_time
import 0x08046734 get_current_timestamp             alias_get_current_timestamp
import 0x0804844A get_system_ms             		alias_get_system_ms

import 0x0803C250 show_watchface                    alias_show_watchface 
import 0x08020110 show_big_digit                    alias_show_big_digit 
import 0x08020110 text_out_font 					alias_text_out_font 						@алиас к show_big_digit
import 0x0806B478 vTaskDelay                        alias_vTaskDelay

import 0x080390F4 show_alarm_set_screen             alias_show_alarm_set_screen  
import 0x08039294 dispatch_alarm_set                alias_dispatch_alarm_set
import 0x08066A6C store_and_enable_alarm            alias_store_and_enable_alarm

@работа с флэш памятью                              alias_ятью
import 0x08044BBC read_flash                        alias_read_flash
import 0x08044CF0 _write_flash                       alias__write_flash		@ функция удалена по соображениям безопасности

import 0x08046ECC get_generic_data                  alias_get_generic_data 
import 0x0805C5E4 set_generic_data                  alias_set_generic_data 

import 0x080465E8 get_backlight_value               alias_get_backlight_value
import 0x0805C118 set_backlight_percent             alias_set_backlight_percent
import 0x080373FC set_backlight_state               alias_set_backlight_state

import 0x080113A8 _rand                             alias__rand
import 0x080113BC _srand                            alias__srand

import 0x08052F00 send_host_app_msg                 alias_send_host_app_msg
import 0x08056B04 send_host_app_data                alias_send_host_app_data

import 0x080465CC check_app_state                   alias_check_app_state 
import 0x0801CE10 stm32_soft_reset                  alias_stm32_soft_reset
import 0x0804FCDC iwdg_reboot                       alias_iwdg_reboot

import 0x08031314 get_last_heartrate                alias_get_last_heartrate
import 0x0804C16C set_hrm_mode                      alias_set_hrm_mode  
import 0x0804C308 get_hrm_struct                    alias_get_hrm_struct

import 0x08060848 is_gps_fixed                      alias_is_gps_fixed
import 0x08039CF0 switch_gps_pressure_sensors       alias_switch_gps_pressure_sensors 
import 0x08039D34 get_navi_data                     alias_get_navi_data 

import 0x0805163C add_notification                  alias_add_notification 
import 0x0805BFF6 create_and_show_notification      alias_create_and_show_notification 

import 0x08037494 get_battery_charge                alias_get_battery_charge
import 0x0805DB70 silence_page_create_frame			alias_silence_page_create_frame			@	@	silence_page_create_frame

import 0x08039CC8 compass_get_degree				alias_compass_get_degree				@	compass_get_degree		
import 0x08039EF4 compass_need_calibration			alias_compass_need_calibration			@	compass_need_calibration
import 0x0803A624 compass_task_resume				alias_compass_task_resume				@	compass_task_resume						
import 0x0805BC8C sensors_enable					alias_sensors_enable					@	sensors_enable		


@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.section .alias
label2:

.section .text.outro
_outro_label: .string "</LibBip end>"

