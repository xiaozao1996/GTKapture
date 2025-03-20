/**
 * @file on_event.c
 * @author wuzhh
 * @brief 处理事件相关的函数
 * @version 0.1
 * @date 2025-03-20
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "on_event.h"

// 鼠标按下事件
void on_mouse_press(GtkWidget *widget, GdkEventButton *event, gpointer data) {
    MainData *main_data = (MainData *)data;
    ShapeData *current_shape_data = main_data->current_shape_data;

    // 标记当前坐标
    current_shape_data->x_start = (gint)event->x;
    current_shape_data->y_start = (gint)event->y;
    main_data->is_pressing = TRUE; // 标记按压

    // 左键按压
    if (event->button == GDK_BUTTON_PRIMARY) {
        switch (main_data->run_mode) {
            case WAIT_MODE:
                // 执行等待模式的代码
                break;
                
            case SELECT_AREA_MODE:
                // 执行选区模式的代码
                break;
        
            case FULL_SCREEN_MODE:
                // 执行全屏模式的代码
                break;
        
            case DRAW_RECTANGLE_MODE:
                main_data->current_shape_data->shape_type = RECTANGLE;
                break;
        
            case DRAW_LINE_MODE:
                main_data->current_shape_data->shape_type = LINE;
                break;
        
            case DRAW_ARROW_MODE:
                main_data->current_shape_data->shape_type = ARROW;
                break;
        
            case DRAW_FREE_MODE:
                main_data->current_shape_data->shape_type = FREE_DRAW;
                break;
        
            case DRAW_TEXT_MODE:
                main_data->current_shape_data->shape_type = TEXT;
                break;
        
            default:
                // 如果 run_mode 的值不在预期的范围内，可以处理未知情况
                break;
        }        
        
    } else if (event->button == GDK_BUTTON_SECONDARY) { // 右键
        gtk_main_quit();  // 退出程序
    }
}

// 鼠标移动事件
void on_motion_notify(GtkWidget *widget, GdkEventMotion *event, gpointer data) {
    MainData *main_data = (MainData *)data;
    ShapeData *current_shape_data = main_data->current_shape_data;
    ShapeData *select_area_data = main_data->select_area_data;

    // 标记当前坐标
    current_shape_data->x_end = (gint)event->x;
    current_shape_data->y_end = (gint)event->y;

    // 更新鼠标拖动的位置
    if (main_data->is_pressing) {
        main_data->is_drawing = TRUE;
        switch (main_data->run_mode) {
            case WAIT_MODE:
                // 执行等待模式的代码
                break;
                
            case SELECT_AREA_MODE:
                copy_current_shape_data(select_area_data, current_shape_data);
                ensure_select_area_correct_order(select_area_data);
                draw_select_area_surface(main_data);
                move_button_window(main_data);
                break;
        
            case FULL_SCREEN_MODE:
                // 执行全屏模式的代码
                break;
        
            case DRAW_RECTANGLE_MODE:
            case DRAW_LINE_MODE:
            case DRAW_ARROW_MODE:
            case DRAW_TEXT_MODE:
                limit_shape_in_select_area(current_shape_data, select_area_data);
                draw_rectangle_line_arrow_text_surface(main_data);
                move_button_window(main_data);
                break;
        
            case DRAW_FREE_MODE:
                // 执行自由绘图模式的代码
                break;
        
            default:
                // 如果 run_mode 的值不在预期的范围内，可以处理未知情况
                break;
        }
    }
    else{ // 没有点击的情况下
        if (main_data->run_mode == SELECT_AREA_MODE) {
            if (is_point_in_select_area(main_data->select_area_data, event->x, event->y)) {
                // 选择区域内，改变鼠标为十字箭头
                g_print("选择区域内，改变鼠标为十字箭头\n");
                set_cursor(main_data, event->window, GDK_CROSSHAIR);
            }else {
                // 选择区域外，改变鼠标为十字
                g_print("选择区域外，改变鼠标为十字\n");
                set_cursor(main_data, event->window, GDK_CROSS);
            }
        }
    }
    
    gtk_widget_queue_draw(widget); // 触发重绘
}

void on_mouse_release(GtkWidget *widget, GdkEventButton *event, gpointer data) {
    MainData *main_data = (MainData *)data;
    ShapeData *select_area_data = main_data->select_area_data;

    // 绘制结束
    if (event->button == GDK_BUTTON_PRIMARY) { // 左键
        switch (main_data->run_mode) {
            case SELECT_AREA_MODE:
                if(main_data->is_drawing){
                    move_button_window(main_data);
                }
                break;
        
            case FULL_SCREEN_MODE:
                // 执行全屏模式的代码
                break;
        
            case DRAW_RECTANGLE_MODE:
            case DRAW_LINE_MODE:
            case DRAW_ARROW_MODE:
            case DRAW_TEXT_MODE:
                ShapeData *new_shape = create_and_copy_current_shape_data(main_data->current_shape_data);
                main_data->draw_shape_list = g_list_append(main_data->draw_shape_list, new_shape);
                break;
        
            case DRAW_FREE_MODE:
                // 执行自由绘图模式的代码
                break;
        
            default:
                // 如果 run_mode 的值不在预期的范围内，可以处理未知情况
                break;
        }
    }
    main_data->is_drawing = FALSE;
    main_data->is_pressing = FALSE;
}


// 各个按钮的回调函数
void on_select_area_mode_click(GtkWidget *widget, gpointer data) {
    MainData *main_data = (MainData *)data;

    if(main_data->run_mode != SELECT_AREA_MODE){
        draw_background(main_data->cr, main_data->background_buf);
        draw_full_gray_surface(main_data->cr, main_data->background_width, main_data->background_height);
        main_data->run_mode = SELECT_AREA_MODE;

    }
    
}

void on_full_screen_mode_click(GtkWidget *widget, gpointer data) {
    MainData *main_data = (MainData *)data;
    const gchar *button_label = gtk_button_get_label(GTK_BUTTON(widget));
    g_print("按钮 %s 被点击了\n", button_label);
    main_data->run_mode = FULL_SCREEN_MODE;
}

void on_draw_rectangle_mode_click(GtkWidget *widget, gpointer data) {
    MainData *main_data = (MainData *)data;
    const gchar *button_label = gtk_button_get_label(GTK_BUTTON(widget));
    g_print("按钮 %s 被点击了\n", button_label);
    main_data->run_mode = DRAW_RECTANGLE_MODE;
}

void on_draw_line_mode_click(GtkWidget *widget, gpointer data) {
    MainData *main_data = (MainData *)data;
    main_data->run_mode = DRAW_LINE_MODE;

    const gchar *button_label = gtk_button_get_label(GTK_BUTTON(widget));
    g_print("按钮 %s 被点击了\n", button_label);
    
}

void on_draw_arrow_mode_click(GtkWidget *widget, gpointer data) {
    MainData *main_data = (MainData *)data;
    main_data->run_mode = DRAW_ARROW_MODE;

    const gchar *button_label = gtk_button_get_label(GTK_BUTTON(widget));
    g_print("按钮 %s 被点击了\n", button_label);
}

void on_draw_free_mode_click(GtkWidget *widget, gpointer data) {
    MainData *main_data = (MainData *)data;
    main_data->run_mode = DRAW_FREE_MODE;

    const gchar *button_label = gtk_button_get_label(GTK_BUTTON(widget));
    g_print("按钮 %s 被点击了\n", button_label);
}

void on_draw_text_mode_click(GtkWidget *widget, gpointer data) {
    const gchar *button_label = gtk_button_get_label(GTK_BUTTON(widget));
    g_print("按钮 %s 被点击了\n", button_label);
}

void on_save_clipboard_click(GtkWidget *widget, gpointer data) {
    const gchar *button_label = gtk_button_get_label(GTK_BUTTON(widget));
    g_print("按钮 %s 被点击了\n", button_label);
}

void on_save_file_click(GtkWidget *widget, gpointer data) {
    const gchar *button_label = gtk_button_get_label(GTK_BUTTON(widget));
    g_print("按钮 %s 被点击了\n", button_label);
}

void on_exit_click(GtkWidget *widget, gpointer data) {
    gtk_main_quit();
}

void on_draw(GtkWidget *widget, cairo_t *cr, gpointer data) {
    MainData *main_data = (MainData *)data;
    ShapeData *select_area_data = main_data->select_area_data;

    // 检查 main_data->surface 是否有效
    if (main_data->surface != NULL) {
        // 将 main_data->surface 复制到传入的 cr 上下文
        cairo_set_source_surface(cr, main_data->surface, SCREEN_START_X, SCREEN_START_Y);  // 设置源 surface
        cairo_paint(cr);  // 将 surface 绘制到传入的上下文 cr 上
    } else {
        g_print("Error: No surface to draw\n");
    }
    
}

