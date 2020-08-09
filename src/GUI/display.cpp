
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "esp_system.h"
#include "driver/gpio.h"
#include "arduino.h"

/* Littlevgl specific */
#include <lvgl.h>
#include <TFT_eSPI.h>

#include "GUI/input.h"
#include "GUI/nav.h"
#include "GUI/display.h"

TFT_eSPI tft = TFT_eSPI(); /* TFT instance */
static lv_disp_buf_t disp_buf;
static lv_color_t buf[LV_HOR_RES_MAX * 10];


void display_init(void)
{
    lv_init();

    lv_disp_buf_init(&disp_buf, buf, NULL, LV_HOR_RES_MAX * 10);

    lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = dispWidth;
    disp_drv.ver_res = dispHeight;
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.buffer = &disp_buf;
    lv_disp_drv_register(&disp_drv);

    #if LV_USE_LOG != 0
    lv_log_register_print_cb(my_print); /* register print function for debugging */
    #endif

    tft.begin(); /* TFT init */
    tft.setRotation(1); /* Landscape orientation */

    ledcSetup(0, 4000, 8); // setup PWM for Backlight of Screen
    ledcAttachPin(4, 0);
    setScreenBrightness(DEFAULT_SCREEN_BRIGHTNESS);

    nav_init();
}


void setScreenBrightness(uint8_t brightness)
{
    ledcWrite(0, brightness);
}

#if LV_USE_LOG != 0
/* Serial debugging */
void my_print(lv_log_level_t level, const char * file, uint32_t line, const char * func, const char * dsc)
{

    Serial.printf("%s@%din%s->%s\r\n", file, line, func, dsc);
    Serial.flush();
}
#endif


/* Display flushing */
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);

    tft.startWrite();
    tft.setAddrWindow(area->x1, area->y1, w, h);
    tft.pushColors(&color_p->full, w * h, true);
    tft.endWrite();

    lv_disp_flush_ready(disp);
}


/* Reading input device (simulated encoder here) */





