extern "C"
{
#include "my_basic.h"
}

#include <TTGO.h>
#include "hardware/display.h"
#include "lvgl/lvgl.h"

static lv_obj_t *my_basic_output_label;
static lv_obj_t *my_basic_output_page;
lv_style_t *my_basic_page_main_style;
LV_FONT_DECLARE(Ubuntu_16px);

static int lvglprint(const char *format, ...) {
    char *buf = (char *)malloc(128); // Massimo 128 bytes per linea ? Nessun controllo qui !
    int result = 0;
    va_list ap;
    va_start(ap, format);
    result = vsnprintf(buf, 128, format, ap);
    lv_label_ins_text(my_basic_output_label, LV_LABEL_POS_LAST, buf);
    va_end(ap);
    free(buf);
    return result;
}

static int lvglclear(struct mb_interpreter_t* s, void** l) {
    int result = MB_FUNC_OK;

    mb_assert(s && l);
    mb_check(mb_attempt_func_begin(s, l));
    mb_check(mb_attempt_func_end(s, l));

    lv_label_set_text(my_basic_output_label, "");

    return result;
}

static void lv_obj_unref(struct mb_interpreter_t* s, void* d) {
    lv_obj_t* p = (lv_obj_t*)d;

    mb_assert(s);

    if (p != my_basic_output_page) free(p);
}

static void dont_unref(struct mb_interpreter_t* s, void* d) {
}

static void* lv_obj_clone(struct mb_interpreter_t* s, void* d) {
    lv_obj_t* p = (lv_obj_t*)d;
    lv_obj_t* q = (lv_obj_t*)malloc(sizeof(lv_obj_t));

    mb_assert(s);

    *q = *p;

    return q;
}

lv_coord_t xor_coords(lv_area_t c) {
    return c.x1 ^ c.x2 ^ c.y1 ^ c.y2;
}

static unsigned int lv_obj_hash(struct mb_interpreter_t* s, void* d) {
    lv_obj_t* p = (lv_obj_t*)d;

    mb_assert(s);

    return xor_coords(p->coords);
}

static int lv_obj_cmp(struct mb_interpreter_t* s, void* l, void* r) {
    lv_obj_t* p = (lv_obj_t*)l;
    lv_obj_t* q = (lv_obj_t*)r;
    int tmp = 0;

    mb_assert(s);

    tmp = xor_coords(p->coords) - xor_coords(q->coords);
    return tmp;
}

static int lv_obj_fmt(struct mb_interpreter_t* s, void* d, char* b, unsigned z) {
    int result = 0;
    lv_obj_t* p = (lv_obj_t*)d;

    mb_assert(s);

    result = snprintf(b, z, "%x", (unsigned int)p) + 1;

    return result;
}

static int _lv_btn_create(struct mb_interpreter_t* s, void** l) {
    int result = MB_FUNC_OK;

    mb_assert(s && l);
    mb_check(mb_attempt_open_bracket(s, l));

    {
        lv_obj_t* p;
        int64_t x1, y1, x2, y2;
        mb_value_t arg;
        mb_make_nil(arg);
        mb_check(mb_pop_value(s, l, &arg));
        mb_check(mb_pop_int(s, l, &x1));
        mb_check(mb_pop_int(s, l, &y1));
        mb_check(mb_pop_int(s, l, &x2));
        mb_check(mb_pop_int(s, l, &y2));

        mb_check(mb_get_ref_value(s, l, arg, (void**)&p));

        lv_style_t new_lv_label_main_style;
        lv_style_init(&new_lv_label_main_style);
        lv_style_copy(&new_lv_label_main_style, my_basic_page_main_style);
        lv_style_set_text_font(&new_lv_label_main_style, LV_STATE_DEFAULT, &Ubuntu_16px);
        lv_style_set_text_color(&new_lv_label_main_style, LV_STATE_DEFAULT, LV_COLOR_BLUE);
        lv_style_set_bg_color(&new_lv_label_main_style, LV_OBJ_PART_MAIN, LV_COLOR_YELLOW);
        lv_task_handler();
        lv_obj_t* _p = lv_btn_create(p, NULL);
        lv_task_handler();
        lv_obj_add_style(_p, LV_OBJ_PART_MAIN, &new_lv_label_main_style);
        lv_obj_set_pos(_p, x1, y1);
        lv_obj_set_size(_p, x2, y2);

        mb_value_t ret;
        mb_make_ref_value(s, _p, &ret, lv_obj_unref, lv_obj_clone, lv_obj_hash, lv_obj_cmp, lv_obj_fmt);
        mb_check(mb_push_value(s, l, ret));
        mb_check(mb_unref_value(s, l, arg));
    }

    mb_check(mb_attempt_close_bracket(s, l));
    lv_task_handler();
    return result;
}



static int _lv_label_create(struct mb_interpreter_t* s, void** l) {
    int result = MB_FUNC_OK;

    mb_assert(s && l);
    mb_check(mb_attempt_open_bracket(s, l));

    {
        lv_obj_t* p;
        int64_t  x1, y1, x2, y2;
        mb_value_t arg;
        mb_make_nil(arg);
        mb_check(mb_pop_value(s, l, &arg));
        mb_check(mb_pop_int(s, l, &x1));
        mb_check(mb_pop_int(s, l, &y1));
        mb_check(mb_pop_int(s, l, &x2));
        mb_check(mb_pop_int(s, l, &y2));

        mb_check(mb_get_ref_value(s, l, arg, (void**)&p));

        lv_style_t new_lv_label_main_style;
        lv_style_init(&new_lv_label_main_style);
        lv_style_copy(&new_lv_label_main_style, my_basic_page_main_style);
        lv_style_set_text_font(&new_lv_label_main_style, LV_STATE_DEFAULT, &Ubuntu_16px);
        lv_style_set_text_color(&new_lv_label_main_style, LV_STATE_DEFAULT, LV_COLOR_BLUE);
        lv_style_set_bg_color(&new_lv_label_main_style, LV_OBJ_PART_MAIN, LV_COLOR_YELLOW);
        lv_task_handler();
        lv_obj_t* _p = lv_label_create(p, NULL);
        lv_task_handler();
        lv_obj_add_style(_p, LV_OBJ_PART_MAIN, &new_lv_label_main_style);
        lv_obj_set_pos(_p, x1, y1);
        lv_obj_set_size(_p, x2, y2);

        mb_value_t ret;
        mb_make_ref_value(s, _p, &ret, lv_obj_unref, lv_obj_clone, lv_obj_hash, lv_obj_cmp, lv_obj_fmt);
        mb_check(mb_push_value(s, l, ret));
        mb_check(mb_unref_value(s, l, arg));
    }

    mb_check(mb_attempt_close_bracket(s, l));
    lv_task_handler();
    return result;
}

static int _lv_label_set_text(struct mb_interpreter_t* s, void** l) {
    int result = MB_FUNC_OK;

    mb_assert(s && l);
    mb_check(mb_attempt_open_bracket(s, l));

    {
        lv_obj_t* p;
        char* str = 0;
        mb_value_t arg;
        mb_make_nil(arg);
        mb_check(mb_pop_value(s, l, &arg));
        mb_check(mb_get_ref_value(s, l, arg, (void**)&p));
        mb_check(mb_pop_string(s, l, &str));

        lv_label_set_text(p, str);

        mb_check(mb_unref_value(s, l, arg));
    }

    mb_check(mb_attempt_close_bracket(s, l));
    lv_task_handler();
    return result;
}

static int _get_main_lv_obj(struct mb_interpreter_t* s, void** l) {
    int result = MB_FUNC_OK;

    mb_assert(s && l);
    mb_check(mb_attempt_open_bracket(s, l));
    mb_check(mb_attempt_close_bracket(s, l));

    {
        mb_value_t ret;
        lv_obj_t* _p = (lv_obj_t*)malloc(sizeof(lv_obj_t));
        memcpy(_p, my_basic_output_page, sizeof(lv_obj_t));
        mb_make_ref_value(s, _p, &ret, dont_unref, lv_obj_clone, lv_obj_hash, lv_obj_cmp, lv_obj_fmt);
        mb_check(mb_push_value(s, l, ret));
    }

    return result;
}


void enableLVGLprint(struct mb_interpreter_t* bas, lv_obj_t *l) {
  my_basic_output_label=l;
  mb_set_printer(bas, lvglprint);
  mb_register_func(bas, "CLS", lvglclear);
}

void enableLVGL(struct mb_interpreter_t* bas, lv_obj_t *p, lv_style_t *s) {
    my_basic_output_page = p;
    my_basic_page_main_style = s;
    //mb_begin_module(s, "LVGL");
    mb_register_func(bas, "GetMainLvObj", _get_main_lv_obj);
    mb_register_func(bas, "LvButtonCreate", _lv_btn_create);
    mb_register_func(bas, "LvLabelCreate", _lv_label_create);
    mb_register_func(bas, "LvLabelSetText", _lv_label_set_text);
    //mb_end_module(s);
}