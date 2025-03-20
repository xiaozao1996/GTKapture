/**
 * @file draw.h
 * @author wuzhh
 * @brief 处理绘图相关的函数
 * @version 0.1
 * @date 2025-03-20
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#ifndef DRAW_H
#define DRAW_H


#include "common.h"

// 绘制背景
void draw_background(cairo_t *cr, GdkPixbuf *background_buf);
// 绘制全屏灰色的图层
void draw_full_gray_surface(cairo_t *cr, gint background_width, gint background_height);
// 绘制选择框图层
void draw_select_area_surface(MainData *main_data);
// 绘制方框，直线，箭头，文本的图层
void draw_rectangle_line_arrow_text_surface(MainData *main_data);

#endif // DRAW_H