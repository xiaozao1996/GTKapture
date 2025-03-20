/**
 * @file draw.c
 * @author wuzhh
 * @brief 处理绘图相关的函数
 * @version 0.1
 * @date 2025-03-20
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#include "draw.h"


// 绘制矩形
void draw_rectangle(cairo_t *cr, ShapeData *shape_data){
    cairo_set_source_rgba(cr, shape_data->r, shape_data->g, shape_data->b, shape_data->a);
    cairo_set_line_width(cr, shape_data->line_width);
    cairo_rectangle(cr, shape_data->x_start, shape_data->y_start, shape_data->x_end - shape_data->x_start, shape_data->y_end - shape_data->y_start);
    cairo_stroke(cr);
}

// 绘制直线
void draw_line(cairo_t *cr, ShapeData *shape_data){
    cairo_set_source_rgba(cr, shape_data->r, shape_data->g, shape_data->b, shape_data->a);
    cairo_set_line_width(cr, shape_data->line_width);
    cairo_move_to(cr, shape_data->x_start, shape_data->y_start);
    cairo_line_to(cr, shape_data->x_end, shape_data->y_end);
    cairo_stroke(cr);
}

// 绘制箭头
void draw_arrow(cairo_t *cr, ShapeData *shape_data) {
    cairo_set_source_rgba(cr, shape_data->r, shape_data->g, shape_data->b, shape_data->a);
    cairo_set_line_width(cr, shape_data->line_width);

    // 绘制线段
    cairo_move_to(cr, shape_data->x_start, shape_data->y_start);
    cairo_line_to(cr, shape_data->x_end, shape_data->y_end);
    cairo_stroke(cr);

    // 绘制箭头头部
    double angle = atan2(shape_data->y_end - shape_data->y_start, shape_data->x_end - shape_data->x_start);
    double arrow_size = shape_data->arrow_size;  // 箭头大小

    // 箭头的两边
    cairo_move_to(cr, shape_data->x_end, shape_data->y_end);
    cairo_line_to(cr, shape_data->x_end - arrow_size * cos(angle - G_PI / 6), shape_data->y_end - arrow_size * sin(angle - G_PI / 6));
    cairo_move_to(cr, shape_data->x_end, shape_data->y_end);
    cairo_line_to(cr, shape_data->x_end - arrow_size * cos(angle + G_PI / 6), shape_data->y_end - arrow_size * sin(angle + G_PI / 6));
    cairo_stroke(cr);
}

// 绘制实心圆
void draw_filled_circle(cairo_t *cr, ShapeData *shape_data) {
    // 设置圆形的颜色和透明度
    cairo_set_source_rgba(cr, shape_data->r, shape_data->g, shape_data->b, shape_data->a);
    // 绘制实心圆
    cairo_arc(cr, shape_data->center_x, shape_data->center_y, shape_data->circle_radius, shape_data->angle1, shape_data->angle2);
    cairo_fill(cr);  // 使用填充绘制圆形
}

// 绘制文本
void draw_text(cairo_t *cr, ShapeData *shape_data) {
    cairo_set_source_rgba(cr, shape_data->r, shape_data->g, shape_data->b, shape_data->a);
    cairo_set_font_size(cr, shape_data->font_size);
    cairo_move_to(cr, shape_data->x_start, shape_data->y_start);  // 设置文本起始位置
    cairo_show_text(cr, shape_data->text);  // 绘制文本
    cairo_stroke(cr);
}

// 绘制每个图形
void draw_shape(gpointer data, gpointer user_data) {
    ShapeData *shape = (ShapeData *)data;
    cairo_t *cr = (cairo_t *)user_data;
    switch (shape->shape_type) {
        case RECTANGLE:
            draw_rectangle(cr, shape);
            break;
        case LINE:
            draw_line(cr, shape);
            break;
        case TEXT:
            draw_text(cr, shape);
            break;
        case ARROW:
            draw_arrow(cr, shape);
            break;
        default:
            printf("Unknown shape.\n");
            break;
    }
}

// 绘制背景
void draw_background(cairo_t *cr, GdkPixbuf *background_buf){
    // 将背景图像绘制到 cairo 上下文
    gdk_cairo_set_source_pixbuf(cr, background_buf, SCREEN_START_X, SCREEN_START_Y);  // 使用背景图像绘制到 cr 上
    cairo_paint(cr);  // 绘制背景
}

// 绘制选择框四周的4个灰色矩形
void draw_gray_surface(cairo_t *cr, ShapeData *shape_data, gint background_width, gint background_height) {
    gint x_start = shape_data->x_start;
    gint y_start = shape_data->y_start;
    gint x_end = shape_data->x_end;
    gint y_end = shape_data->y_end;

    cairo_set_source_rgba(cr, GRAY_R, GRAY_G, GRAY_B, GRAY_A);  // 确保透明度正确
    cairo_set_line_width(cr, shape_data->line_width);

    // 计算矩形的宽度和高度
    gint width = x_end - x_start;
    gint height = y_end - y_start;

    // 绘制四个灰色矩形覆盖四个边
    // 上部矩形
    cairo_rectangle(cr, 0, 0, background_width, y_start);  // 从 (0,0) 到矩形上方
    cairo_fill(cr);

    // 下部矩形
    cairo_rectangle(cr, 0, y_end, background_width, background_height - y_end);  // 从矩形底部到底部
    cairo_fill(cr);

    // 左部矩形
    cairo_rectangle(cr, 0, y_start, x_start, height);  // 从屏幕左边到矩形左边
    cairo_fill(cr);

    // 右部矩形
    cairo_rectangle(cr, x_end, y_start, background_width - x_end, height);  // 从矩形右边到屏幕右边
    cairo_fill(cr);
}

// 绘制全屏灰色的图层
void draw_full_gray_surface(cairo_t *cr, gint background_width, gint background_height) {
    cairo_set_source_rgba(cr, GRAY_R, GRAY_G, GRAY_B, GRAY_A);  // 确保透明度正确
    cairo_rectangle(cr, SCREEN_START_X, SCREEN_START_Y, background_width, background_height);
    cairo_fill(cr);
}

// 绘制选择框四个角实心圆的图层
void draw_select_area_circle_surface(cairo_t *cr, ShapeData *shape_data) {
    // 计算左上角的圆心
    shape_data->center_x = shape_data->x_start;
    shape_data->center_y = shape_data->y_start;
    draw_filled_circle(cr, shape_data);

    // 计算右上角的圆心
    shape_data->center_x = shape_data->x_end;
    shape_data->center_y = shape_data->y_start;
    draw_filled_circle(cr, shape_data);

    // 计算右下角的圆心
    shape_data->center_x = shape_data->x_end;
    shape_data->center_y = shape_data->y_end;
    draw_filled_circle(cr, shape_data);

    // 计算左下角的圆心
    shape_data->center_x = shape_data->x_start;
    shape_data->center_y = shape_data->y_end;
    draw_filled_circle(cr, shape_data);
}

// 绘制选择框图层
void draw_select_area_surface(MainData *main_data){
    // 绘制背景
    draw_background(main_data->cr, main_data->background_buf);
    // 绘制四周灰色方框
    draw_gray_surface(main_data->cr, main_data->select_area_data, main_data->background_width, main_data->background_height);
    // 绘制四角实心圆形
    draw_select_area_circle_surface(main_data->cr, main_data->select_area_data);
    // 绘制选择框
    draw_rectangle(main_data->cr, main_data->select_area_data);
}

// 绘制方框，直线，箭头，文本的图层
void draw_rectangle_line_arrow_text_surface(MainData *main_data){
    // 绘制选择框图层
    draw_select_area_surface(main_data);
    // 绘制方框，直线，箭头，文本的图层
    // 使用 g_list_foreach 遍历 GList
    g_list_foreach(main_data->draw_shape_list, draw_shape, main_data->cr);
    // 画出当前正在绘制的图形
    draw_shape(main_data->current_shape_data, main_data->cr);
}

