#pragma once

#include <common.h>

static const int SCREEN_WIDTH = 640;   // 160 * 4 scale
static const int SCREEN_HEIGHT = 576;  // 144 * 4 scale

void ui_init();
void delay(u32 ms);
void ui_handle_events();

void ui_update();