/**
 * @file common.h
 * @author wuzhh
 * @brief 处理公共的函数
 * @version 0.1
 * @date 2025-03-20
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#ifndef COMMON_H
#define COMMON_H

#include <gtk-3.0/gtk/gtk.h>
#include <math.h>

typedef enum {
    RECTANGLE,   // 矩形
    LINE,        // 线条
    TEXT,        // 文字
    ARROW,       // 箭头
    FREE_DRAW    // 自由绘制
} ShapeType;

typedef enum {
    WAIT_MODE,                // 等待模式
    SELECT_AREA_MODE,         // 选区模式
    FULL_SCREEN_MODE,         // 全屏模式
    DRAW_RECTANGLE_MODE,      // 画矩形模式
    DRAW_LINE_MODE,           // 画线模式
    DRAW_ARROW_MODE,          // 画箭头模式
    DRAW_FREE_MODE,           // 自由绘图模式
    DRAW_TEXT_MODE,           // 画文字模式
    SAVE_CLIPBOARD_MODE,      // 保存到剪贴板模式
    SAVE_FILE_MODE,           // 保存文件模式
    EXIT_MODE                 // 退出模式
} RunMode;

typedef struct {
    gint x_start, y_start; // 鼠标按下位置
    gint x_end, y_end;     // 鼠标当前位置
    // 颜色线宽
    gint r,g,b,a;
    gint line_width;
    // 箭头相关
    gint arrow_size;
    // 圆形相关
    double center_x, center_y;
    double circle_radius, angle1, angle2;
    // 文字相关
    gint font_size;
    char *text;
    ShapeType shape_type;
} ShapeData;

typedef struct
{
    // 背景，画板相关
    GdkPixbuf *background_buf; // 截取的屏幕图像
    gint background_width, background_height;
    cairo_surface_t *surface;
    cairo_t *cr;

    // 绘制图形相关
    ShapeData *select_area_data;
    ShapeData *current_shape_data;
    GList *draw_shape_list;

    RunMode run_mode;
    GdkCursorType cursor_type;

    gboolean is_pressing; // 是否正在绘制
    gboolean is_drawing; // 是否正在绘制
    gboolean has_select_area; // 是否有选择区

    // 存储创建的窗口
    GtkWidget *prepare_window;
    GtkWidget *capture_window;
    GtkWidget *button_window;
    GList *button_list;
    GtkWidget *input_text_window;

} MainData; // 主要数据结构，数据中心


// 创建一个默认的形状
ShapeData* create_default_shape_data();
// 拷贝当前形状
void copy_current_shape_data(ShapeData *shape_data, ShapeData *current_shape_data);
// 创建并返回当前形状的副本
ShapeData* create_and_copy_current_shape_data(ShapeData *current_shape_data);
// 限制当前所画的图形在选择区的范围内，超出则赋值为最大边界
void limit_shape_in_select_area(ShapeData *shape, ShapeData *select_area_data);
// 确保选择区的正确 x_start，y_start 顺序
void ensure_select_area_correct_order(ShapeData *shape);
// 判断点 (x, y) 是否在矩形内
gboolean is_point_in_select_area(ShapeData *shape, gint x, gint y);
// 设置光标样式的函数
void set_cursor(MainData *main_data, GdkCursorType cursor_type);
// 判断给定的坐标(x, y)是否在指定的圆内（允许偏差值 CORNER_CIRCLE_TOLERANCE）
gboolean is_point_in_corner_circle_with_tolerance(double x, double y, double circle_x, double circle_y, double radius);
// 根据给定坐标判断该点是否在四个角的圆形上（允许偏差值 tolerance）
GdkCursorType check_point_in_select_area_with_tolerance(double x, double y, ShapeData *shape_data);
// 限制移动后的选择区不超过屏幕的边缘
void limit_select_area_in_background(ShapeData *select_area_data, gint x_move, gint y_move, gint background_width, gint background_height);
// 保存到剪切板
void save_to_clipboard(MainData *data);
// 保存到本地
void save_to_file(MainData *main_data);
// 延迟退出的回调函数
gboolean delayed_quit(gpointer data);
// 定义延迟退出函数
void delayed_exit(MainData *main_data);
// 释放列表的每个元素
GList* free_draw_shape_list(GList *list);
// 移除最后一个加入的元素
GList* free_last_draw_shape(GList *list);

#define DEFAULT_SHAPE_DATA (ShapeData) {\
    0, 0, 0, 0, \
    1, 1, 1, 1, \
    2, 10, \
    0, 0, 10, 0, 2 * G_PI, \
    20, NULL, RECTANGLE}

#define SCREEN_START_X 0
#define SCREEN_START_Y 0

#define BUTTON_WINDOW_OFFSET_X 0
#define BUTTON_WINDOW_OFFSET_Y 20

#define GRAY_R 0.5  // 灰色的红色分量
#define GRAY_G 0.5  // 灰色的绿色分量
#define GRAY_B 0.5  // 灰色的蓝色分量
#define GRAY_A 0.5  // 半透明的 alpha 值，0 表示完全透明，1 表示完全不透明

// 宏定义容差值
#define CORNER_CIRCLE_TOLERANCE 2.0  // 四角圆容差值

#define DELAYED_EXIT_TIME 100 

#endif // COMMON_H
