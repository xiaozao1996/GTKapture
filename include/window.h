/**
 * @file window.h
 * @author wuzhh
 * @brief 处理窗口相关的函数
 * @version 0.1
 * @date 2025-03-20
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#ifndef WINDOW_H
#define WINDOW_H


#include "common.h"
#include "on_event.h"

// 移动按钮窗口，加入偏移量
void move_button_window(MainData *main_data, gint x, gint y);
void create_button_window(MainData *main_data);
void create_capture_window(MainData *main_data);

#endif // WINDOW_H