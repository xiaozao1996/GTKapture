/**
 * @file common.c
 * @author wuzhh
 * @brief 处理公共的函数
 * @version 0.1
 * @date 2025-03-20
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#include "common.h"

// 创建一个默认的形状
ShapeData* create_default_shape_data(){
    // 动态分配内存给 ShapeData
    ShapeData *shape_data = malloc(sizeof(ShapeData));
    if (shape_data != NULL) {
        // 将 DEFAULT_SHAPE_DATA 的值复制到 shape_data 中
        *shape_data = DEFAULT_SHAPE_DATA;
    } else {
        // 如果指针为空，可以选择打印错误信息或者执行其他操作
        // 这里仅打印一个简单的错误信息
        g_printerr("Error: NULL pointer encountered in create_default_shape_data.\n");
    }

    return shape_data;
}

// 拷贝当前形状
void copy_current_shape_data(ShapeData *shape_data, ShapeData *current_shape_data) {
    // 检查指针是否为 NULL
    if (shape_data != NULL && current_shape_data != NULL) {
        *shape_data = *current_shape_data;  // 拷贝数据
    } else {
        // 如果指针为空，可以选择打印错误信息或者执行其他操作
        // 这里仅打印一个简单的错误信息
        g_printerr("Error: NULL pointer encountered in copy_current_shape_data.\n");
    }
}

// 创建并返回当前形状的副本
ShapeData* create_and_copy_current_shape_data(ShapeData *current_shape_data) {
    ShapeData* shape = create_default_shape_data();
    copy_current_shape_data(shape, current_shape_data);
    return shape;
}

// 确保选择区的正确 x_start，y_start 顺序
void ensure_select_area_correct_order(ShapeData *shape) {
    // 如果 x_start 大于 x_end，则交换它们
    if (shape->x_start > shape->x_end) {
        gint temp = shape->x_start;
        shape->x_start = shape->x_end;
        shape->x_end = temp;
    }

    // 如果 y_start 大于 y_end，则交换它们
    if (shape->y_start > shape->y_end) {
        gint temp = shape->y_start;
        shape->y_start = shape->y_end;
        shape->y_end = temp;
    }
}

// 限制当前所画的图形在选择区的范围内，超出则赋值为最大边界
void limit_shape_in_select_area(ShapeData *shape, ShapeData *select_area_data) {
    // 确保 shape 的 x_start 在 select_area_data 的 x_start 和 x_end 之间
    if (shape->x_start < select_area_data->x_start) {
        shape->x_start = select_area_data->x_start;  // 限制到 select_area_data 的左边界
    } else if (shape->x_start > select_area_data->x_end) {
        shape->x_start = select_area_data->x_end;  // 限制到 select_area_data 的右边界
    }

    // 确保 shape 的 x_end 在 select_area_data 的 x_start 和 x_end 之间
    if (shape->x_end < select_area_data->x_start) {
        shape->x_end = select_area_data->x_start;  // 限制到 select_area_data 的左边界
    } else if (shape->x_end > select_area_data->x_end) {
        shape->x_end = select_area_data->x_end;  // 限制到 select_area_data 的右边界
    }

    // 确保 shape 的 y_start 在 select_area_data 的 y_start 和 y_end 之间
    if (shape->y_start < select_area_data->y_start) {
        shape->y_start = select_area_data->y_start;  // 限制到 select_area_data 的上边界
    } else if (shape->y_start > select_area_data->y_end) {
        shape->y_start = select_area_data->y_end;  // 限制到 select_area_data 的下边界
    }

    // 确保 shape 的 y_end 在 select_area_data 的 y_start 和 y_end 之间
    if (shape->y_end < select_area_data->y_start) {
        shape->y_end = select_area_data->y_start;  // 限制到 select_area_data 的上边界
    } else if (shape->y_end > select_area_data->y_end) {
        shape->y_end = select_area_data->y_end;  // 限制到 select_area_data 的下边界
    }
}

// 判断点 (x, y) 是否在矩形内
gboolean is_point_in_select_area(ShapeData *shape, gint x, gint y) {
    // 判断点 (x, y) 是否在矩形的范围内
    if (x >= shape->x_start && x <= shape->x_end && y >= shape->y_start && y <= shape->y_end) {
        return TRUE;  // 点在矩形内
    }
    return FALSE;  // 点不在矩形内
}

// 设置光标的形状（十字形，十字箭头形等）
void set_cursor(MainData *draw_data, GdkCursorType cursor_type) {
    // 获取显示设备
    GdkDisplay *display = gtk_widget_get_display(draw_data->capture_window);  
    // 创建指定类型的光标
    GdkCursor *cursor = gdk_cursor_new_for_display(display, cursor_type);  
    // 设置窗口的光标
    gdk_window_set_cursor(gtk_widget_get_window(draw_data->capture_window), cursor);
    // 更新当前光标类型
    draw_data->cursor_type = cursor_type;
    // 释放光标资源
    g_object_unref(cursor);
}

// 判断给定的坐标(x, y)是否在指定的圆内（允许偏差值 CORNER_CIRCLE_TOLERANCE）
gboolean is_point_in_corner_circle_with_tolerance(double x, double y, double circle_x, double circle_y, double radius) {
    double distance = sqrt((x - circle_x) * (x - circle_x) + (y - circle_y) * (y - circle_y));
    return distance <= (radius + CORNER_CIRCLE_TOLERANCE);  // 使用宏定义的容差值
}

// 根据给定坐标判断该点是否在四个角的圆形上（允许偏差值 tolerance）
GdkCursorType check_point_in_select_area_with_tolerance(double x, double y, ShapeData *shape_data) {
    // 计算四个圆心的坐标
    double circle_x[4] = {shape_data->x_start, shape_data->x_end, shape_data->x_end, shape_data->x_start};
    double circle_y[4] = {shape_data->y_start, shape_data->y_start, shape_data->y_end, shape_data->y_end};

    // 对应的角落位置名称
    GdkCursorType positions[4] = {GDK_TOP_LEFT_CORNER, GDK_TOP_RIGHT_CORNER, GDK_BOTTOM_RIGHT_CORNER, GDK_BOTTOM_LEFT_CORNER};

    // 检查每个角落的圆是否包含给定的点
    for (int i = 0; i < 4; i++) {
        if (is_point_in_corner_circle_with_tolerance(x, y, circle_x[i], circle_y[i], shape_data->circle_radius)) {
            return positions[i];  // 返回相应的角落字符串
        }
    }

    return GDK_ARROW;  // 如果点不在任何圆内，返回 GDK_ARROW
}

void limit_select_area_in_background(ShapeData *select_area_data, gint x_move, gint y_move, gint background_width, gint background_height) {
    // 计算移动后的选择区域的起始和结束坐标
    gint new_x_start = select_area_data->x_start + x_move;
    gint new_y_start = select_area_data->y_start + y_move;
    gint new_x_end = select_area_data->x_end + x_move;
    gint new_y_end = select_area_data->y_end + y_move;

    // 判断选择区域是否超出屏幕的边界
    if (new_x_start < SCREEN_START_X) { // 超出左边界
        x_move = SCREEN_START_X - select_area_data->x_start;  // 保持在左边界
    }
    if (new_y_start < SCREEN_START_Y) { // 超出上边界
        y_move = SCREEN_START_Y - select_area_data->y_start;  // 保持在上边界
    }
    if (new_x_end > background_width) { // 超出右边界
        x_move = background_width - select_area_data->x_end;  // 保持在右边界
    }
    if (new_y_end > background_height) { // 超出下边界
        y_move = background_height - select_area_data->y_end;  // 保持在下边界
    }

    // 更新选择区域的位置
    select_area_data->x_start += x_move;
    select_area_data->y_start += y_move;
    select_area_data->x_end += x_move;
    select_area_data->y_end += y_move;
}

// 保存到剪切板
void save_to_clipboard(MainData *main_data) {
    // 获取选择区域的坐标
    ShapeData *select_data = main_data->select_area_data;
    gint x_start = select_data->x_start;
    gint y_start = select_data->y_start;
    gint x_end = select_data->x_end;
    gint y_end = select_data->y_end;

    // 计算选择区域的宽度和高度
    gint width = x_end - x_start;
    gint height = y_end - y_start;

    // 确保选择区域的尺寸有效
    if (width <= 0 || height <= 0) {
        g_print("Invalid selection area\n");
        return;
    }

    // 创建一个新的 Pixbuf 来存储选择区域的图像
    GdkPixbuf *pixbuf = gdk_pixbuf_new(GDK_COLORSPACE_RGB, TRUE, 8, width, height);

    // 获取 Cairo surface 和相关数据
    cairo_surface_t *surface = main_data->surface;
    cairo_t *cr = main_data->cr;

    // 确保 Cairo surface 已经刷新
    cairo_surface_flush(surface);

    // 从 Cairo surface 中提取图像数据到 Pixbuf 中
    // 这里的函数会从 cairo surface 中截取 (x_start, y_start) 到 (x_end, y_end) 区域的内容
    pixbuf = gdk_pixbuf_get_from_surface(surface, x_start, y_start, width, height);

    // 获取剪切板
    GtkClipboard *clipboard = gtk_clipboard_get(GDK_SELECTION_CLIPBOARD);

    // 将 Pixbuf 设置到剪切板
    gtk_clipboard_set_image(clipboard, pixbuf);

    // 释放 Pixbuf 资源
    g_object_unref(pixbuf);

    // 可以在这里做更多的处理，比如释放其他资源等
}

// 保存到本地
void save_to_file(MainData *main_data) {
    // 获取当前时间戳
    time_t t;
    struct tm *tm_info;
    char filename[64];
    time(&t);
    tm_info = localtime(&t);
    // 格式化时间戳为文件名
    strftime(filename, sizeof(filename), "screenshot_%Y%m%d_%H%M%S.png", tm_info);
    // 获取选择区域的坐标
    ShapeData *select_data = main_data->select_area_data;
    gint x_start = select_data->x_start;
    gint y_start = select_data->y_start;
    gint x_end = select_data->x_end;
    gint y_end = select_data->y_end;
    // 计算选择区域的宽度和高度
    gint width = x_end - x_start;
    gint height = y_end - y_start;

    // 获取 cairo_surface_t，这里假设 cr 已经有目标 surface
    cairo_surface_t *surface = cairo_get_target(main_data->cr);

    // 将 cairo_surface_t 转换为 GdkPixbuf
    GdkPixbuf *pixbuf = gdk_pixbuf_get_from_surface(surface, x_start, y_start, width, height);

    if (pixbuf == NULL) {
        g_print("Failed to create GdkPixbuf from cairo surface\n");
        return;
    }

    // 创建文件保存对话框
    GtkWidget *dialog = gtk_file_chooser_dialog_new(
        "Save Screenshot", NULL,
        GTK_FILE_CHOOSER_ACTION_SAVE,
        "_Cancel", GTK_RESPONSE_CANCEL,
        "_Save", GTK_RESPONSE_ACCEPT,
        NULL
    );

    // 设置默认文件名
    gtk_file_chooser_set_current_name(GTK_FILE_CHOOSER(dialog), filename);

    // 显示对话框并等待用户选择
    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
        // 获取用户选择的文件路径
        char *filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));

        // 将图像保存到指定路径
        GError *error = NULL;
        if (!gdk_pixbuf_save(pixbuf, filename, "png", &error, NULL)) {
            g_print("Error saving file: %s\n", error->message);
            g_error_free(error);
        } else {
            g_print("Screenshot saved to: %s\n", filename);
        }

        // 释放文件名
        g_free(filename);
    }

    // 关闭对话框
    gtk_widget_destroy(dialog);

    // 释放 GdkPixbuf
    g_object_unref(pixbuf);
}

// 延迟退出的回调函数
gboolean delayed_quit(gpointer data) {
    gtk_main_quit(); // 调用退出主循环
    return FALSE; // 只执行一次
}

// 定义延迟退出函数
void delayed_exit(MainData *main_data) {
    gtk_widget_hide(main_data->button_window);
    gtk_widget_hide(main_data->button_window);
    g_timeout_add(DELAYED_EXIT_TIME, delayed_quit, NULL); // 设置定时器，延迟调用 delayed_quit 函数
}

// 释放列表的每个元素
GList* free_draw_shape_list(GList *list) {
    // 遍历 GList 列表，释放每个元素
    for (GList *l = list; l != NULL; l = l->next) {
        // 假设每个元素都是动态分配的内存，需要释放
        // 如果元素是指针类型，则可以在这里释放每个元素
        g_free(l->data);
    }
    return NULL;
}

// 移除最后一个加入的元素
GList* free_last_draw_shape(GList *list) {
    GList *result = list;
    GList *last_node = g_list_last(list);  // 获取链表最后一个节点
    if (last_node != NULL) {
        result = g_list_remove_link(list, last_node);
        g_list_free_1(last_node);  // 释放链表节点
    }
    return result;
}
