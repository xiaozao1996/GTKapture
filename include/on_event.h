/**
 * @file on_event.h
 * @author wuzhh
 * @brief 处理事件相关的函数
 * @version 0.1
 * @date 2025-03-20
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#ifndef ON_EVENT_H
#define ON_EVENT_H


#include "common.h"
#include "draw.h"

void on_mouse_press(GtkWidget *widget, GdkEventButton *event, gpointer data);
void on_mouse_release(GtkWidget *widget, GdkEventButton *event, gpointer data);
void on_motion_notify(GtkWidget *widget, GdkEventMotion *event, gpointer data);
void on_draw(GtkWidget *widget, cairo_t *cr, gpointer data);

// 各个按钮的回调函数声明

void on_select_area_mode_click(GtkWidget *widget, gpointer data);
void on_full_screen_mode_click(GtkWidget *widget, gpointer data);
void on_draw_rectangle_mode_click(GtkWidget *widget, gpointer data);
void on_draw_line_mode_click(GtkWidget *widget, gpointer data);
void on_draw_arrow_mode_click(GtkWidget *widget, gpointer data);
void on_draw_free_mode_click(GtkWidget *widget, gpointer data);
void on_draw_text_mode_click(GtkWidget *widget, gpointer data);
void on_save_clipboard_click(GtkWidget *widget, gpointer data);
void on_save_file_click(GtkWidget *widget, gpointer data);
void on_exit_click(GtkWidget *widget, gpointer data);

// ============================================== 因为此处与 window.h 互相调用，所以需要 window.h 的声明
void move_button_window(MainData *main_data, gint x, gint y);
void create_button_window(MainData *main_data);
void create_capture_window(MainData *main_data);

#endif // ON_EVENT_H