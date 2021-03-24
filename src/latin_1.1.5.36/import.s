@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@
@ Writen by Maxim Volkov Maxim.N.Volkov@ya.ru (C) 2019 - 2021
@ Imported functions for Mili_chaohu.fw.latin v 1.1.5.36
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
import 0x080183E4 text_out  								alias_text_out
import 0x080183F4 text_out_center							alias_text_out_center
@import 0x08018478 text_out_center_wrap          			alias_text_out_center_wrap
import 0x0801895A text_width                    			alias_text_width
import 0x080188C8 get_text_height               			alias_get_text_height
import 0x080188B8 get_last_text_pos_x           			alias_get_last_text_pos_x
import 0x0804D82C get_res_string_by_id          			alias_get_res_string_by_id
			
@	Функции работы с графикой			
import 0x08018F50 set_bg_color								alias_set_bg_color
import 0x08018F54 set_fg_color                  			alias_set_fg_color
import 0x080180D4 draw_filled_rect_bg           			alias_draw_filled_rect_bg
import 0x08018848 draw_filled_rect              			alias_draw_filled_rect 
import 0x08018668 draw_rect                     			alias_draw_rect
import 0x08018648 draw_horizontal_line          			alias_draw_horizontal_line 
import 0x080187F0 draw_vertical_line            			alias_draw_vertical_line
																
import 0x08018660 draw_rect_or_line             			alias_draw_rect_or_line

import 0x08019234 set_graph_callback_to_ram_1				alias_set_graph_callback_to_ram_1
import 0x080180B8 fill_screen_bg                    		alias_fill_screen_bg
import 0x080205EC show_res_by_id                    		alias_show_res_by_id 
import 0x0801869C draw_res_by_address               		alias_draw_res_by_address 
import 0x0804C588 read_flash_by_offset              		alias_read_flash_by_offset
															
import 0x0804C788 reg_menu                          		alias_reg_menu
import 0x0805AD00 show_menu_animate                 		alias_show_menu_animate 
import 0x0803BCCC show_menu                         		alias_show_menu
import 0x0805ADE8 animate_screen                    		alias_animate_screen
															
import 0x0801141C _memclr                           		alias__memclr 
import 0x0801140E _memset                           		alias__memset
import 0x080113CC _memcpy                           		alias__memcpy
import 0x080114C4 _memcmp                           		alias__memcmp 
import 0x0801149A _strlen                           		alias__strlen
															
import 0x080114A8 _strcmp		                    		alias__strcmp		
import 0x080114DE _strcpy                           		alias__strcpy
import 0x080114F0 _strncmp                          		alias__strncmp
import 0x0801146E _strncpy                          		alias__strncpy
															
import 0x0802E7C0 _sprintf                          		alias__sprintf 
import 0x08059C00 set_display_state_value           		alias_set_display_state_value
import 0x08065FD8 set_close_timer                   		alias_set_close_timer
															
import 0x080559FC _pvPortMalloc                     		alias__pvPortMalloc
import 0x08067D9C vPortFree                         		alias_vPortFree
import 0x080681EC vTaskDelay                        		alias_vTaskDelay 
import 0x08059E9C load_font                         		alias_load_font
															
															
import 0x0804E6E0 log_printf                        		alias_log_printf
import 0x0806A390 get_tick_count                    		alias_get_tick_count
import 0x08065FFC set_update_period                 		alias_set_update_period
import 0x0804DD20 repaint_screen_lines              		alias_repaint_screen_lines
import 0x0804DD08 repaint_screen                    		alias_repaint_screen
															
import 0x0803B6FC show_watchface                    		alias_show_watchface  
import 0x0804F430 vibrate                           		alias_vibrate
															
@import 0x080316A0 dispatch_menu_alipay_1st_screen   		alias_dispatch_menu_alipay_1st_screen
import 0x08031C5C show_menu_alipay                  		alias_show_menu_alipay
import 0x08031660 keypress_menu_alipay_1st_screen   		alias_keypress_menu_alipay_1st_screen
import 0x0804F8A0 dispatch_left_side_menu           		alias_dispatch_left_side_menu
import 0x08043CE0 get_left_side_menu_active         		alias_get_left_side_menu_active
import 0x08059618 set_left_side_menu_active         		alias_set_left_side_menu_active 
															
@import 0x0804935C get_res_flash_addr                		alias_get_res_flash_addr
@import 0x080385EC alarm_set_on_click                		alias_alarm_set_on_click 
import 0x080384AC show_alarm_set_screen             		alias_show_alarm_set_screen 
import 0x080204BC show_big_digit                    		alias_show_big_digit
															
@алиас к show_big_digit                             		alias_digit
import 0x080204BC text_out_font 	                		alias_text_out_font 	
															
import 0x08043F74 get_current_date_time             		alias_get_current_date_time
import 0x08043F48 get_current_timestamp             		alias_get_current_timestamp
															
import 0x080537B8 show_poweroff_message             		alias_show_poweroff_message
import 0x0805AA98 show_options_menu_2nd_screen      		alias_show_options_menu_2nd_screen
import 0x0805AC24 show_menu_options                 		alias_show_menu_options
import 0x080392A8 show_options_menu_1st_screen      		alias_show_options_menu_1st_screen
import 0x0804ED40 show_main_menu_item               		alias_show_main_menu_item
import 0x080585E4 show_menu_touch_test              		alias_show_menu_touch_test
import 0x08043EC0 get_res_ret_language              		alias_get_res_ret_language 
															
import 0x0803869C dispatch_alarm_set                		alias_dispatch_alarm_set  
import 0x08063800 store_and_enable_alarm            		alias_store_and_enable_alarm 
															
@работа с флэш памятью                              		
import 0x08042504 write_flash                       		alias_write_flash
import 0x080423D0 read_flash                        		alias_read_flash
															
import 0x08044700 get_generic_data                  		alias_get_generic_data 
import 0x08059CB8 set_generic_data                  		alias_set_generic_data
															
import 0x08043DFC get_backlight_value               		alias_get_backlight_value
import 0x080597EC set_backlight_percent             		alias_set_backlight_percent
import 0x080366EC set_backlight_state               		alias_set_backlight_state
															
import 0x080113A8 _rand                             		alias__rand
import 0x080113BC _srand                            		alias__srand
															
import 0x08050560 send_host_app_msg                 		alias_send_host_app_msg 
import 0x0805413C send_host_app_data                		alias_send_host_app_data
															
import 0x08043DE0 check_app_state                   		alias_check_app_state 
import 0x0801D1BC stm32_soft_reset                  		alias_stm32_soft_reset
import 0x0804D558 iwdg_reboot                       		alias_iwdg_reboot
															
import 0x08030908 get_last_heartrate                		alias_get_last_heartrate
import 0x08049A00 set_hrm_mode                      		alias_set_hrm_mode
import 0x08049B9C get_hrm_struct                    		alias_get_hrm_struct
															
import 0x0805DEE4 is_gps_fixed                      		alias_is_gps_fixed
import 0x08039094 switch_gps_pressure_sensors       		alias_switch_gps_pressure_sensors
import 0x080390D8 get_navi_data                     		alias_get_navi_data
															
import 0x0804EED8 add_notification                  		alias_add_notification
import 0x080596CA create_and_show_notification      		alias_create_and_show_notification
															
import 0x08036784 get_battery_charge						alias_get_battery_charge
import 0x0805B1F8 silence_page_create_frame					alias_silence_page_create_frame			@	@	silence_page_create_frame

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.section .alias
label2:

.section .text.outro
_outro_label: .string "</LibBip end>"

