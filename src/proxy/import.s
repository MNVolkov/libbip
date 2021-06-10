@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@
@ Writen by Maxim Volkov Maxim.N.Volkov@ya.ru (C) 2019 - 2021	
@ Imported functions for ELFLoader
@ for libbip library
@
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.file "import.s"
.include "alias.s"

.code   16
.section .text.intro
_intro_label: .string "<LibBip>"

.text
.code   16
.global _start
_start:  
		push    {r0-r1}
        ldr     r0, =main
        str     r0,[sp,#4]
        pop     {r0,PC}

.macro import address functionname
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
        
    __\functionname: .word \address
.endm

@@@@@@@@@@@   Функции прошивки @@@@@@@@@@@@@@
@	Функции работы с текстом
import alias_text_out						text_out
import alias_text_out_center				text_out_center
import alias_text_width						text_width
import alias_get_text_height				get_text_height
import alias_get_last_text_pos_x			get_last_text_pos_x

@	Функции работы с графикой
import alias_set_bg_color					set_bg_color
import alias_set_fg_color					set_fg_color 
import alias_draw_horizontal_line			draw_horizontal_line
import alias_draw_rect						draw_rect
import alias_draw_vertical_line				draw_vertical_line
import alias_draw_filled_rect				draw_filled_rect
import alias_show_res_by_id					show_res_by_id
import alias_fill_screen_bg					fill_screen_bg
import alias_set_graph_callback_to_ram_1	set_graph_callback_to_ram_1
import alias_draw_filled_rect_bg			draw_filled_rect_bg
import alias_draw_rect_or_line				draw_rect_or_line
import alias_repaint_screen_lines			repaint_screen_lines
import alias_repaint_screen					repaint_screen

@	Функции работы с меню
import alias_reg_menu						reg_menu
import alias_show_poweroff_message			show_poweroff_message  
import alias_show_options_menu_2nd_screen	show_options_menu_2nd_screen
import alias_show_menu_options				show_menu_options
import alias_show_menu_animate				show_menu_animate
import alias_show_menu						show_menu
import alias_show_options_menu_1st_screen	show_options_menu_1st_screen

@	Функции языка
import alias_load_font						load_font
import alias_get_res_ret_language			get_res_ret_language

import alias__sprintf						_sprintf

import alias__strlen						_strlen
import alias__strcmp						_strcmp		
import alias__strcpy						_strcpy
import alias__strncmp						_strncmp
import alias__strncpy						_strncpy

import alias__memclr						_memclr
import alias__memset						_memset
import alias__memcpy						_memcpy
import alias__memcmp						_memcmp

import alias_get_tick_count					get_tick_count

import alias_set_update_period				set_update_period 
import alias_set_display_state_value		set_display_state_value 
import alias_set_close_timer				set_close_timer

import alias_log_printf						log_printf
import alias_vibrate						vibrate

import alias__pvPortMalloc					_pvPortMalloc
import alias_vPortFree						vPortFree

import alias_show_menu_touch_test			show_menu_touch_test
import alias_show_main_menu_item			show_main_menu_item

import alias_show_menu_alipay				show_menu_alipay 
import alias_keypress_menu_alipay_1st_screen	keypress_menu_alipay_1st_screen
import alias_dispatch_left_side_menu		dispatch_left_side_menu
import alias_get_left_side_menu_active		get_left_side_menu_active 

import alias_get_current_date_time			get_current_date_time

import alias_show_watchface					show_watchface 
import alias_show_big_digit					show_big_digit 
import alias_text_out_font					text_out_font 	@алиас к show_big_digit
import alias_vTaskDelay						vTaskDelay

import alias_show_alarm_set_screen			show_alarm_set_screen  
import alias_dispatch_alarm_set				dispatch_alarm_set
import alias_store_and_enable_alarm			store_and_enable_alarm

@работа с флэш памятью
import alias_read_flash						read_flash
@import alias_write_flash					write_flash			@ функция удалена по соображениям безопасности

import alias_get_generic_data				get_generic_data 
import alias_set_generic_data				set_generic_data 

import alias_get_backlight_value			get_backlight_value
import alias_set_backlight_percent			set_backlight_percent
import alias_set_backlight_state			set_backlight_state
import alias_set_backlight					set_backlight

import alias__rand							_rand
import alias__srand							_srand

import alias_send_host_app_msg				send_host_app_msg
import alias_send_host_app_data				send_host_app_data

import alias_check_app_state				check_app_state
import alias_stm32_soft_reset				stm32_soft_reset
import alias_iwdg_reboot					iwdg_reboot

import alias_get_last_heartrate				get_last_heartrate
import alias_set_hrm_mode					set_hrm_mode
import alias_get_hrm_struct					get_hrm_struct

import alias_get_res_string_by_id			get_res_string_by_id
import alias_set_left_side_menu_active		set_left_side_menu_active

import alias_is_gps_fixed					is_gps_fixed
import alias_switch_gps_pressure_sensors	switch_gps_pressure_sensors
import alias_get_navi_data					get_navi_data

import alias_add_notification				add_notification
import alias_create_and_show_notification	create_and_show_notification
import alias_get_current_timestamp			get_current_timestamp
import alias_f_log_flush					f_log_flush
import alias_get_battery_charge				get_battery_charge
import alias_silence_page_create_frame		silence_page_create_frame

import alias_compass_get_degree				compass_get_degree				@	compass_get_degree		
import alias_compass_need_calibration		compass_need_calibration		@	compass_need_calibration
import alias_compass_task_resume			compass_task_resume				@	compass_task_resume						
import alias_sensors_enable					sensors_enable					@	sensors_enable		



@@@@@@@@@     Функции библиотеки    @@@@@@@@@@@@
@@ common.c
import 0xFADE0002 get_selected_locale
import 0xFADE0004 get_stored_lang
import 0xFADE0006 store_selected_lang
import 0xFADE0008 get_system_locale
import 0xFADE000A draw_progressbar
import 0xFADE000C dump_mem
import 0xFADE000E get_res_count
import 0xFADE0010 read_res_by_id
import 0xFADE0012 add_menu_item
import 0xFADE0014 clear_menu
import 0xFADE0016 draw_menu
import 0xFADE0018 dispatch_menu
import 0xFADE001A push_ret_f
import 0xFADE001C pop_ret_f
import 0xFADE0042 pvPortMalloc
import 0xFADE0044 get_fw_version
import 0xFADE004A write_flash
import 0xFADE004C get_bipos_build

@@ options.c
@ не нужны в пользовательских приложениях
@import 0xFADE001E get_stored_left_side_function
@import 0xFADE0020 store_selected_left_side


@@ loader.c
import 0xFADE0030 load_elf_by_index
import 0xFADE0032 load_elf_by_name
import 0xFADE0034 init_elf_loader
import 0xFADE0036 elf_finish
import 0xFADE0038 ElfReadSettings
import 0xFADE003A ElfWriteSettings
import 0xFADE003C ElfGetSettingsSize
import 0xFADE003E show_elf_res_by_id
import 0xFADE0040 get_res_params
import 0xFADE0046 get_proc_by_addr
import 0xFADE0048 read_elf_res_by_id


@@@@@ Глобальные переменные @@@@@@
import 0xBEAD0010 get_var_current_screen
import 0xBEAD0011 set_var_current_screen 				
import 0xBEAD0020 get_var_swipe_screen_active_number	
import 0xBEAD0021 set_var_swipe_screen_active_number	
import 0xBEAD0030 get_var_menu_overlay					
import 0xBEAD0031 set_var_menu_overlay					
import 0xBEAD0040 get_ptr_menu_dispatch_func         	
import 0xBEAD0041 set_ptr_menu_dispatch_func         	
import 0xBEAD0050 get_ptr_key_press_func             	
import 0xBEAD0051 set_ptr_key_press_func             	
import 0xBEAD0060 get_ptr_screen_job_func            	
import 0xBEAD0061 set_ptr_screen_job_func            	
import 0xBEAD0070 get_ptr_temp_buf_1                 	
import 0xBEAD0080 get_ptr_show_menu_func             	
import 0xBEAD0081 set_ptr_show_menu_func             	
import 0xBEAD0090 get_ptr_temp_buf_2                 	
import 0xBEAD00A0 get_ptr_long_key_press_func        	
import 0xBEAD00A1 set_ptr_long_key_press_func        	
import 0xBEAD00B0 get_var_left_side_menu_active		
import 0xBEAD00C0 get_var_free_ram						
import 0xBEAD00D0 get_ptr_screen_memory        	
import 0xBEAD00E0 get_ptr_free_list

.section .text.outro
_outro_label: .string "</LibBip end>"

