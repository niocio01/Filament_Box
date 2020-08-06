#include <lvgl.h>

#include "GUI/display.h"
#include "GUI/nav.h"
#include "GUI/page_select.h"

lv_obj_t *page;
lv_obj_t *btn_mtrx;
lv_obj_t *label;

lv_style_t lv_style_btn_rel;


lv_obj_t* page_setup_init(lv_obj_t * tabs)
{
    page = lv_tabview_add_tab(tabs, "Select");
    lv_page_set_scrollbar_mode(page, LV_SCROLLBAR_MODE_OFF);

    static const char* btnm_map[] ={ "PLA", "ABS", "PETG", "\n", "TPU-R", "PC", "+", "" };
    btn_mtrx = lv_btnmatrix_create(page, NULL);
    lv_btnmatrix_set_map(btn_mtrx, btnm_map);
    lv_obj_align(btn_mtrx, NULL, LV_ALIGN_IN_TOP_LEFT, 5, 4);
    lv_obj_set_size(btn_mtrx, dispWidth-10, 75);

    label = lv_label_create(page, NULL);
    lv_obj_align(label, btn_mtrx, LV_ALIGN_OUT_BOTTOM_LEFT, 0, 2);
    lv_label_set_text(label, "No programm running  -  Select a preset or create a new one  -");
    lv_label_set_long_mode(label, LV_LABEL_LONG_SROLL_CIRC);
    lv_label_set_anim_speed(label, 20);
    lv_obj_set_width(label, dispWidth - 10);
    

    static lv_style_t style_btn;
    lv_style_init(&style_btn);
    lv_style_copy(&style_btn, &lv_style_btn_rel);    
    lv_style_set_radius(&style_btn, LV_STATE_DEFAULT, 10);

    lv_style_set_border_width(&style_btn, LV_STATE_DEFAULT, 3);

    lv_style_set_pad_inner(&style_btn, LV_STATE_DEFAULT, 5);
    lv_style_set_pad_top(&style_btn, LV_STATE_DEFAULT, 7);
    lv_style_set_pad_bottom(&style_btn, LV_STATE_DEFAULT, 7);
    lv_style_set_pad_left(&style_btn, LV_STATE_DEFAULT, 7);
    lv_style_set_pad_right(&style_btn, LV_STATE_DEFAULT, 7);
    lv_style_set_radius(&style_btn, LV_STATE_DEFAULT, 10);

    // lv_obj_reset_style_list(btn_mtrx, LV_BTNMATRIX_PART_BTN);         /*Remove the styles coming from the theme*/
    lv_obj_add_style(btn_mtrx, LV_BTN_PART_MAIN, &style_btn);
    lv_obj_add_style(btn_mtrx, LV_BTNMATRIX_PART_BTN, &style_btn);

    return page;
}

/*
void page_cb(lv_obj_t * obj, lv_event_t event)
{
    Serial.print("buttons: ");
    Serial.println(event);
}
*/

/*
void buttons_cb(lv_obj_t * obj, lv_event_t event)
{

    Serial.print("buttons: ");
    Serial.println(event);

    switch (event) {

    case LV_EVENT_SHORT_CLICKED:
        Serial.print(lv_btnmatrix_get_active_btn_text(buttons));
        Serial.println(" Clicked\n");

        lv_tabview_set_tab_act(tabs, 1, LV_ANIM_ON);
        break;

    case LV_EVENT_FOCUSED:
        if (lv_group_get_editing(group1) == false)
        {
            if (enterbutton)
            {
                enterbutton = false;
            }
            else
            {
                lv_group_focus_obj(tabs);
                lv_group_set_editing(group1, true);
            }
        }
        break;
    }
}
*/