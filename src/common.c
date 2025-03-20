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

// 设置光标样式的函数
void set_cursor(MainData *main_data, GdkWindow *window, GdkCursorType cursor_type) {
    // 获取当前窗口的显示（Display）对象
    GdkDisplay *display = gdk_window_get_display(window);
    // 创建指定类型的光标
    GdkCursor *cursor = gdk_cursor_new_for_display(display, cursor_type);
    // 设置当前窗口的光标样式
    gdk_window_set_cursor(window, cursor);
    // 释放光标资源
    g_object_unref(cursor);
    main_data->cursor_type = cursor_type;
}