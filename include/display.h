#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#include <lvgl.h>
#include <TFT_eSPI.h>

#define dispWidth 240
#define dispHeight 135
#define DEFAULT_SCREEN_BRIGHTNESS 100

void guiTask(void *pvParameter);
void display_init(void);
void setScreenBrightness(uint8_t brightness);

void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p);
bool read_encoder(lv_indev_drv_t * indev, lv_indev_data_t * data);

#if LV_USE_LOG != 0
/* Serial debugging */
void my_print(lv_log_level_t level, const char * file, uint32_t line, const char * func, const char * dsc);
#endif

#endif // __DISPLAY_H__