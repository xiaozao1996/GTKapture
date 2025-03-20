
/**
 * @file window.c
 * @author wuzhh
 * @brief 处理窗口相关的函数
 * @version 0.1
 * @date 2025-03-20
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#include "window.h"

// 移动按钮窗口
void move_button_window(MainData *main_data){
    ShapeData *select_area_data = main_data->select_area_data;
    gint x_end = select_area_data->x_end;
    gint y_end = select_area_data->y_end;

    GtkWidget *window = main_data->button_window;

    // 获取窗口的宽度
    gint window_width = gtk_widget_get_allocated_width(window);
    gint window_height = gtk_widget_get_allocated_height(window);

    // 计算新位置，使窗口的最右边与 x_end 对齐
    gint new_x = x_end - window_width;
    gint new_y = y_end;

    // 移动窗口
    gtk_window_move(GTK_WINDOW(window), new_x, new_y);
    // 显示所有组件
    gtk_widget_show_all(window);
    // 确保窗口显示在最前面
    gtk_window_present(GTK_WINDOW(window));
}


void create_button_window(MainData *main_data){
    GtkWidget *window, *hbox;
    GtkWidget *select_area_mode_button, *full_screen_mode_button, *draw_rectangle_mode_button;
    GtkWidget *draw_line_mode_button, *draw_arrow_mode_button, *draw_free_mode_button, *draw_text_mode_button;
    GtkWidget *save_clipboard_button, *save_file_button, *exit_button;

    // 创建一个没有标题的窗口，设置大小为 100x30
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_decorated(GTK_WINDOW(window), FALSE);  // 隐藏标题栏
    gtk_window_set_default_size(GTK_WINDOW(window), 100, 30);

    // 创建一个水平布局容器
    hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5); // 5 是按钮之间的间距

    // 创建按钮
    select_area_mode_button = gtk_button_new_with_label("选区");
    full_screen_mode_button = gtk_button_new_with_label("全屏");
    draw_rectangle_mode_button = gtk_button_new_with_label("方框");
    draw_line_mode_button = gtk_button_new_with_label("画线");
    draw_arrow_mode_button = gtk_button_new_with_label("箭头");
    draw_free_mode_button = gtk_button_new_with_label("自由");
    draw_text_mode_button = gtk_button_new_with_label("文字");
    save_clipboard_button = gtk_button_new_with_label("剪贴");
    save_file_button = gtk_button_new_with_label("文件");
    exit_button = gtk_button_new_with_label("退出");

    // 连接按钮点击的回调函数
    g_signal_connect(select_area_mode_button, "clicked", G_CALLBACK(on_select_area_mode_click), main_data);
    g_signal_connect(full_screen_mode_button, "clicked", G_CALLBACK(on_full_screen_mode_click), main_data);
    g_signal_connect(draw_rectangle_mode_button, "clicked", G_CALLBACK(on_draw_rectangle_mode_click), main_data);
    g_signal_connect(draw_line_mode_button, "clicked", G_CALLBACK(on_draw_line_mode_click), main_data);
    g_signal_connect(draw_arrow_mode_button, "clicked", G_CALLBACK(on_draw_arrow_mode_click), main_data);
    g_signal_connect(draw_free_mode_button, "clicked", G_CALLBACK(on_draw_free_mode_click), main_data);
    g_signal_connect(draw_text_mode_button, "clicked", G_CALLBACK(on_draw_text_mode_click), main_data);
    g_signal_connect(save_clipboard_button, "clicked", G_CALLBACK(on_save_clipboard_click), main_data);
    g_signal_connect(save_file_button, "clicked", G_CALLBACK(on_save_file_click), main_data);
    g_signal_connect(exit_button, "clicked", G_CALLBACK(on_exit_click), main_data);

    // 将按钮添加到容器中
    gtk_box_pack_start(GTK_BOX(hbox), select_area_mode_button, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(hbox), full_screen_mode_button, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(hbox), draw_rectangle_mode_button, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(hbox), draw_line_mode_button, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(hbox), draw_arrow_mode_button, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(hbox), draw_free_mode_button, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(hbox), draw_text_mode_button, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(hbox), save_clipboard_button, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(hbox), save_file_button, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(hbox), exit_button, TRUE, TRUE, 0);

    // 将按钮添加到 main->button_list 中
    main_data->button_list = g_list_append(main_data->button_list, select_area_mode_button);
    main_data->button_list = g_list_append(main_data->button_list, full_screen_mode_button);
    main_data->button_list = g_list_append(main_data->button_list, draw_rectangle_mode_button);
    main_data->button_list = g_list_append(main_data->button_list, draw_line_mode_button);
    main_data->button_list = g_list_append(main_data->button_list, draw_arrow_mode_button);
    main_data->button_list = g_list_append(main_data->button_list, draw_free_mode_button);
    main_data->button_list = g_list_append(main_data->button_list, draw_text_mode_button);
    main_data->button_list = g_list_append(main_data->button_list, save_clipboard_button);
    main_data->button_list = g_list_append(main_data->button_list, save_file_button);
    main_data->button_list = g_list_append(main_data->button_list, exit_button);

    // 将容器添加到窗口中
    gtk_container_add(GTK_CONTAINER(window), hbox);

    // 连接窗口关闭信号
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // 保存窗口指针
    main_data->button_window = window;
}

void create_capture_window(MainData *main_data){
    GtkWidget *window, *drawing_area;

    // 创建全屏窗口
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), NULL);
    gtk_window_fullscreen(GTK_WINDOW(window)); // 设置为全屏

    // 创建绘图区域
    drawing_area = gtk_drawing_area_new();
    gtk_container_add(GTK_CONTAINER(window), drawing_area);

    // 连接事件
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(drawing_area, "button-press-event", G_CALLBACK(on_mouse_press), main_data);
    g_signal_connect(drawing_area, "button-release-event", G_CALLBACK(on_mouse_release), main_data);
    g_signal_connect(drawing_area, "motion-notify-event", G_CALLBACK(on_motion_notify), main_data);
    g_signal_connect(drawing_area, "draw", G_CALLBACK(on_draw), main_data);

    // 启用事件监听
    gtk_widget_set_events(drawing_area, GDK_BUTTON_PRESS_MASK | GDK_BUTTON_RELEASE_MASK | GDK_POINTER_MOTION_MASK);

    main_data->capture_window = window;
}
