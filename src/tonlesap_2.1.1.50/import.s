@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@
@ Writen by Maxim Volkov Maxim.N.Volkov@ya.ru (C) 2019 - 2021
@ Imported functions for Bip S (tonlesap) 2.1.1.50
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
import 0x00016018						text_out								alias_text_out  
import 0x00016018						text_out_center							alias_text_out_center
import 0x000162B0						text_out_center_wrap					alias_text_out_center_wrap
import 0x00016498						text_width								alias_text_width
import 0x00014F00						get_text_height							alias_get_text_height
import 0x 								get_last_text_pos_x						alias_get_last_text_pos_x
import 0x000388E8						get_res_string_by_id					alias_get_res_string_by_id
			
@	Функции работы с графикой			
import 0x000159E8						set_bg_color							alias_set_bg_color
import 0x000159EC						set_fg_color							alias_set_fg_color
import 0x00015ADC						draw_filled_rect_bg						alias_draw_filled_rect_bg
import 0x00017438						draw_filled_rect						alias_draw_filled_rect 
import 0x								draw_rect								alias_draw_rect
import 0x								draw_horizontal_line					alias_draw_horizontal_line 
import 0x								draw_vertical_line						alias_draw_vertical_line
			
import 0x								draw_rect_or_line						alias_draw_rect_or_line
			
import 0x00019374						set_encoding_utf8						alias_set_encoding_utf8 		@set_graph_callback_to_ram_1
import 0x00015B50						fill_screen_bg							alias_fill_screen_bg			@GUI_Clear  
import 0x000381E4						show_res_by_id							alias_show_res_by_id 
import 0x00018C7C						draw_res_by_address						alias_draw_res_by_address 
import 0x000381C4						read_flash_by_offset					alias_read_flash_by_offset
			
import 0x00036C98						reg_menu								alias_reg_menu
import 0x0003636A						show_menu_animate						alias_show_menu_animate 
import 0x00036334						show_menu								alias_show_menu
import 0x								animate_screen							alias_animate_screen
			
import 0x000AE92A						_memclr									alias__memclr 
import 0x								_memset									alias__memset
import 0x000AE8E2						_memcpy									alias__memcpy
import 0x								_memcmp									alias__memcmp 
import 0x								_strlen									alias__strlen
			
import 0x								_strcmp									alias__strcmp		
import 0x								_strcpy									alias__strcpy
import 0x								_strncmp								alias__strncmp
import 0x								_strncpy								alias__strncpy
			
import 0x								_sprintf								alias__sprintf 
import 0x00037808						set_display_state_value					alias_set_display_state_value
import 0x000380A8						get_display_state						alias_get_display_state
import 0x0003630C						set_close_timer							alias_set_close_timer
			
import 0x00070A28						_pvPortMalloc							alias__pvPortMalloc
import 0x00070B34						vPortFree								alias_vPortFree
import 0x								vTaskDelay								alias_vTaskDelay 
import 0x00045BE0						load_font								alias_load_font
			
import 0x000238D4						log_printf								alias_log_printf
import 0x0006FA2C						get_tick_count							alias_get_tick_count
import 0x000362A4						set_update_period						alias_set_update_period
import 0x000362D8						set_update_2_period						alias_set_update_2_period
import 0x								repaint_screen_lines					alias_repaint_screen_lines
import 0x0003910C						repaint_screen							alias_repaint_screen
			
import 0x0004132C						show_watchface							alias_show_watchface  
import 0x0000A8F0						vibrate									alias_vibrate

import 0x								dispatch_menu_alipay_1st_screen			alias_dispatch_menu_alipay_1st_screen
import 0x								show_menu_alipay						alias_show_menu_alipay
import 0x								keypress_menu_alipay_1st_screen			alias_keypress_menu_alipay_1st_screen
import 0x								dispatch_left_side_menu					alias_dispatch_left_side_menu
import 0x								get_left_side_menu_active				alias_get_left_side_menu_active
import 0x								set_left_side_menu_active				alias_set_left_side_menu_active 
		
import 0x								get_res_flash_addr						alias_get_res_flash_addr
import 0x								alarm_set_on_click						alias_alarm_set_on_click 
import 0x								show_alarm_set_screen					alias_show_alarm_set_screen 
import 0x								show_big_digit							alias_show_big_digit
import 0x								text_out_font							alias_text_out_font 				@алиас к show_big_digit

import 0x0000FE98						get_current_date_time					alias_get_current_date_time
import 0x0000FEEC						get_current_timestamp					alias_get_current_timestamp

import 0x								show_poweroff_message					alias_show_poweroff_message
import 0x								show_options_menu_2nd_screen			alias_show_options_menu_2nd_screen
import 0x								show_menu_options						alias_show_menu_options
import 0x								show_options_menu_1st_screen			alias_show_options_menu_1st_screen
		
import 0x00048698						show_more_detail_page					alias_show_more_detail_page
		
import 0x								show_menu_touch_test					alias_show_menu_touch_test
import 0x								get_res_ret_language					alias_get_res_ret_language 
		
import 0x								dispatch_alarm_set						alias_dispatch_alarm_set  
import 0x								store_and_enable_alarm					alias_store_and_enable_alarm 
		
@работа с флэш памятью		
@import 0x								_write_flash							alias__write_flash		@ функция удалена по соображениям безопасности (проверить как работает на tonlesap)
@import 0x								read_flash								alias_read_flash
import 0x000788C8 						ad_nvms_read							alias_ad_nvms_read
import 0x000788E2 						ad_nvms_write							alias_ad_nvms_write



import 0x0002EA28						get_generic_data						alias_get_generic_data 
import 0x								set_generic_data						alias_set_generic_data
					
import 0x								get_backlight_value						alias_get_backlight_value
import 0x								set_backlight_percent					alias_set_backlight_percent
import 0x								set_backlight_state						alias_set_backlight_state
					
import 0x								_rand									alias__rand
import 0x								_srand									alias__srand
			
import 0x								send_host_app_msg						alias_send_host_app_msg 
import 0x								send_host_app_data						alias_send_host_app_data
					
import 0x								check_app_state							alias_check_app_state 
import 0x								stm32_soft_reset						alias_stm32_soft_reset
import 0x								iwdg_reboot								alias_iwdg_reboot
					
import 0x								get_last_heartrate						alias_get_last_heartrate
import 0x								set_hrm_mode							alias_set_hrm_mode
import 0x								get_hrm_struct							alias_get_hrm_struct
		
import 0x								is_gps_fixed							alias_is_gps_fixed
import 0x								switch_gps_pressure_sensors				alias_switch_gps_pressure_sensors
import 0x								get_navi_data							alias_get_navi_data
			
import 0x								add_notification						alias_add_notification
import 0x								create_and_show_notification			alias_create_and_show_notification
			
import 0x								get_battery_charge						alias_get_battery_charge

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.section .alias
label2:

.section .text.outro
_outro_label: .string "</LibBip end>"

