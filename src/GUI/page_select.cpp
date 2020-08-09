#include <lvgl.h>

#include "GUI/display.h"
#include "GUI/nav.h"
#include "GUI/page_select.h"
#include "GUI/profiles.h"

lv_obj_t *page;
lv_obj_t *btn_mtrx;



lv_obj_t* page_select_init(lv_obj_t * tabs)
{
    page = lv_tabview_add_tab(tabs, "Select");
    lv_page_set_scrollbar_mode(page, LV_SCROLLBAR_MODE_OFF);

    
    static const uint8_t btnm_id_map[10] = {PLA, PETG, ABS, 0, TPU, PC, PVA, 0, DESSICANT, KEEP_DRY};
    static const char* btnm_map[11] = {"A", "B", "C", "\n", "D", "E", "F", "\n", "G", "H", ""};

    for (int i = 0; i < (8 + 2); i++)
    {
        if (btnm_id_map[i] != 0)
        {
            Serial.print("button: ");
            Serial.print(i);
            Serial.print(" : ");
            Serial.println(get_profile(btnm_id_map[i])->name);
            btnm_map[i] = get_profile(btnm_id_map[i])->name;
        }
    }

    btn_mtrx = lv_btnmatrix_create(page, NULL);
    lv_btnmatrix_set_map(btn_mtrx, btnm_map);
    lv_obj_align(btn_mtrx, NULL, LV_ALIGN_IN_TOP_LEFT, 5, 4);
    lv_obj_set_size(btn_mtrx, dispWidth-10, 93);

    static lv_style_t style_btn;
    lv_style_init(&style_btn);
    lv_style_set_radius(&style_btn, LV_STATE_DEFAULT, 10);

    lv_style_set_border_width(&style_btn, LV_STATE_DEFAULT, 2);

    lv_style_set_pad_inner(&style_btn, LV_STATE_DEFAULT, 5);
    lv_style_set_pad_top(&style_btn, LV_STATE_DEFAULT, 0);
    lv_style_set_pad_bottom(&style_btn, LV_STATE_DEFAULT, 0);
    lv_style_set_pad_left(&style_btn, LV_STATE_DEFAULT, 0);
    lv_style_set_pad_right(&style_btn, LV_STATE_DEFAULT, 0);
    lv_style_set_radius(&style_btn, LV_STATE_DEFAULT, 10);

    // lv_obj_reset_style_list(btn_mtrx, LV_BTNMATRIX_PART_BTN);         /*Remove the styles coming from the theme*/
    lv_obj_add_style(btn_mtrx, LV_BTN_PART_MAIN, &style_btn);
    lv_obj_add_style(btn_mtrx, LV_BTNMATRIX_PART_BTN, &style_btn);
    lv_obj_set_style_local_border_opa(btn_mtrx, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, 0);
    lv_obj_set_style_local_bg_opa(btn_mtrx, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, 0);

    lv_obj_set_event_cb(btn_mtrx, buttons_cb);

    return page;
}


void setTab_select(lv_group_t * group)
{
    lv_group_remove_all_objs(group);
    lv_group_add_obj(group, btn_mtrx);
    lv_group_focus_obj(btn_mtrx);
    lv_group_set_editing(group, true);
}

void buttons_cb(lv_obj_t * obj, lv_event_t event)
{

    Serial.print("buttons: ");
    Serial.println(event);

    

    switch (event) {

    case LV_EVENT_SHORT_CLICKED:
        Serial.print(lv_btnmatrix_get_active_btn_text(btn_mtrx));
        Serial.println(" Clicked\n");

        set_tab(TAB_SETUP);
        break;

    case LV_EVENT_FOCUSED:
        /*if (lv_group_get_editing(group) == false)
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
        }*/
        break;
    }
}
