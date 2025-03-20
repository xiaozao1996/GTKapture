/**
 * @file main.c
 * @author wuzhh
 * @brief 主程序
 * @version 0.1
 * @date 2025-03-20
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#include "main.h"

static GdkPixbuf* capture_screen() {
    // 获取当前屏幕的大小
    GdkWindow *root_win = gdk_get_default_root_window();
    gint width, height;
    gdk_window_get_geometry(root_win, NULL, NULL, &width, &height);
    
    // 截取屏幕图像
    return gdk_pixbuf_get_from_window(root_win, 0, 0, width, height);
}

static void init_main_data(MainData *main_data) {
    // 截取屏幕
    main_data->background_buf = capture_screen();
    main_data->background_width = gdk_pixbuf_get_width(main_data->background_buf);
    main_data->background_height = gdk_pixbuf_get_height(main_data->background_buf);
    main_data->surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, main_data->background_width, main_data->background_height);
    main_data->cr = cairo_create(main_data->surface);

    // 动态分配内存，避免局部变量的生命周期问题
    main_data->select_area_data = create_default_shape_data();
    main_data->current_shape_data = create_default_shape_data();

    main_data->run_mode = WAIT_MODE;
}

int main(int argc, char *argv[]) {
    // 初始化 GTK
    gtk_init(&argc, &argv);

    MainData main_data = {0}; 
    // 初始化主要数据
    init_main_data(&main_data);
    
    // 创建截屏窗口
    create_capture_window(&main_data);
    create_button_window(&main_data);

    // 显示所有组件
    gtk_widget_show_all(main_data.capture_window);

    // 自动点击一下选区按钮
    on_select_area_mode_click(main_data.capture_window, &main_data);

    // 进入 GTK 主循环
    gtk_main();

    // 释放屏幕截图内存
    if (main_data.background_buf) {
        g_object_unref(main_data.background_buf);
    }

    // 释放 select_area_data 内存
    if (main_data.select_area_data) {
        free(main_data.select_area_data);
    }

    return 0;
}
