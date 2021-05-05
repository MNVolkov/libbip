@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@
@ Dev by Maxim Volkov Maxim.N.Volkov@ya.ru (C) 2021	
@ Imported functions aliases for BipOS
@
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@
@адреса библиотечных функций uni_latin  
@		     func_name								alias

.set alias_text_out							,	0xFACE0000 
.set alias_text_out_center					,	0xFACE0002 
.set alias_text_width                    	,	0xFACE0004 
.set alias_get_text_height               	,	0xFACE0006 
.set alias_get_last_text_pos_x             	,	0xFACE0008 
.set alias_set_bg_color                  	,	0xFACE000A 
.set alias_set_fg_color                  	,	0xFACE000C 
.set alias_draw_horizontal_line          	,	0xFACE000E 
.set alias_draw_rect                     	,	0xFACE0010 
.set alias_draw_vertical_line            	,	0xFACE0012 
.set alias_draw_filled_rect              	,	0xFACE0014 
.set alias_show_res_by_id                	,	0xFACE0016 
.set alias_fill_screen_bg                	,	0xFACE0018 
.set alias_set_graph_callback_to_ram_1   	,	0xFACE001A 
.set alias_set_encoding_utf8                ,	0xFACE001A	@алиас к alias_set_graph_callback_to_ram_1
.set alias_draw_filled_rect_bg           	,	0xFACE001C 
.set alias_draw_rect_or_line             	,	0xFACE001E 
.set alias_repaint_screen_lines          	,	0xFACE0020 
.set alias_repaint_screen                	,	0xFACE0022 
.set alias_reg_menu                      	,	0xFACE0024 
.set alias_show_poweroff_message         	,	0xFACE0026 
.set alias_show_options_menu_2nd_screen  	,	0xFACE0028 
.set alias_show_menu_options             	,	0xFACE002A 
.set alias_show_menu_animate             	,	0xFACE002C 
.set alias_show_menu                     	,	0xFACE002E 
.set alias_show_options_menu_1st_screen  	,	0xFACE0030 
.set alias_load_font                     	,	0xFACE0032 
.set alias_get_res_ret_language          	,	0xFACE0034 
.set alias__sprintf                      	,	0xFACE0036 
.set alias__strlen                       	,	0xFACE0038 
.set alias__strcmp		                	,	0xFACE003A 
.set alias__strcpy                       	,	0xFACE003C 
.set alias__strncmp                      	,	0xFACE003E 
.set alias__strncpy                      	,	0xFACE0040 
.set alias__memclr                       	,	0xFACE0042 
.set alias__memset                       	,	0xFACE0044 
.set alias__memcpy                       	,	0xFACE0046 
.set alias__memcmp                       	,	0xFACE0048 
.set alias_get_tick_count                	,	0xFACE004A 
.set alias_set_update_period             	,	0xFACE004C 
.set alias_set_display_state_value       	,	0xFACE004E 
.set alias_set_close_timer               	,	0xFACE0050 
.set alias_log_printf                    	,	0xFACE0052 
.set alias_vibrate                       	,	0xFACE0054 
.set alias__pvPortMalloc                  	,	0xFACE0056 
.set alias_vPortFree                     	,	0xFACE0058 
.set alias_show_menu_touch_test          	,	0xFACE005A 
.set alias_show_main_menu_item           	,	0xFACE005C 
.set alias_show_menu_alipay              	,	0xFACE005E 
.set alias_keypress_menu_alipay_1st_screen	,	0xFACE0060 
.set alias_dispatch_left_side_menu       	,	0xFACE0062 
.set alias_get_left_side_menu_active     	,	0xFACE0064 
.set alias_get_current_date_time         	,	0xFACE0066 
.set alias_show_watchface                	,	0xFACE0068 
.set alias_show_big_digit                	,	0xFACE006A 
.set alias_text_out_font	                ,	0xFACE006A 
.set alias_vTaskDelay                    	,	0xFACE006C 
.set alias_show_alarm_set_screen         	,	0xFACE006E 
.set alias_dispatch_alarm_set            	,	0xFACE0070 
.set alias_store_and_enable_alarm        	,	0xFACE0072 
.set alias_read_flash                    	,	0xFACE0074 
.set alias__write_flash                   	,	0xFACE0076 
.set alias_get_generic_data              	,	0xFACE0078 
.set alias_set_generic_data              	,	0xFACE007A 
.set alias_get_backlight_value           	,	0xFACE007C 
.set alias_set_backlight_percent         	,	0xFACE007E 
.set alias_set_backlight_state           	,	0xFACE0080 
.set alias__rand 					        ,  	0xFACE0082 
.set alias__srand           				,	0xFACE0084 
.set alias_send_host_app_msg				,	0xFACE0086
.set alias_send_host_app_data				,	0xFACE0088
.set alias_check_app_state					,	0xFACE008A

.set alias_stm32_soft_reset					,	0xFACE008C
.set alias_iwdg_reboot						,	0xFACE008E 
.set alias_get_last_heartrate				,	0xFACE0090
.set alias_set_hrm_mode						,	0xFACE0092
.set alias_get_hrm_struct					,	0xFACE0094
.set alias_get_res_string_by_id				,	0xFACE0096
.set alias_set_left_side_menu_active		,	0xFACE0098
.set alias_is_gps_fixed						,	0xFACE009A
.set alias_switch_gps_pressure_sensors		,	0xFACE009C
.set alias_get_navi_data 					,	0xFACE009E
.set alias_add_notification 				,	0xFACE00A0
.set alias_create_and_show_notification		,	0xFACE00A2
.set alias_get_current_timestamp			,	0xFACE00A4
.set alias_get_battery_charge				,	0xFACE00A6
.set alias_draw_res_by_address				,	0xFACE00A8
.set alias_read_flash_by_offset				,	0xFACE00AA
.set alias_animate_screen					,	0xFACE00AC

.set alias_text_out_center_wrap				,	0xFACE00AE
.set alias__atoi        					,	0xFACE00B0
.set alias_GUI_SetPenSize                   ,	0xFACE00B2
.set alias__strstr                          ,	0xFACE00B4
.set alias__strcat                          ,	0xFACE00B6
.set alias_motor_set_complex                ,	0xFACE00B8
.set alias_dispatch_menu_alipay_1st_screen  ,	0xFACE00BA
.set alias_get_res_flash_addr               ,	0xFACE00BC
.set alias_alarm_set_on_click               ,	0xFACE00BE

.set alias_show_more_detail_page            ,	0xFACE00C0
.set alias_get_display_state                ,	0xFACE00C2
.set alias_set_update_2_period              ,	0xFACE00C4
.set alias_f_log_flush              		,	0xFACE00C6
.set alias_silence_page_create_frame   		,	0xFACE00C8
.set alias_set_backlight		           	,	0xFACE00CA 

