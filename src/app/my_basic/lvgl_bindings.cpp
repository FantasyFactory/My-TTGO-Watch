#define LILYGO_WATCH_2020_V1
#ifdef LILYGO_WATCH_2020_V1
#include <TTGO.h>
#include "hardware/display.h"
#define wait(millis) vTaskDelay(millis)
#else
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#define log_i printf
#define wait(millis) SDL_Delay(millis)
#endif
#include "my_basic.h"
#include "lvgl/lvgl.h"
#include "lvgl_bindings.h"

static lv_obj_t* my_basic_output_label;
static lv_obj_t* my_basic_main_lv_obj;
lv_style_t* my_basic_main_lv_style;
LV_FONT_DECLARE(Ubuntu_16px);
static char* lv_msgbox_opt = NULL;
#define MaxLvEvtHandlers 32
#define LogDebug
//#define SimpleUsertype

struct LvEventHandler {
    lv_obj_t* obj;
    mb_value_t routine;
    struct mb_interpreter_t* s;
    void** l;
} LvEventHandlers[MaxLvEvtHandlers];
int_t LvEvtHandlersCount = 0;


static int lvglprint(const char* format, ...) {
    char* buf = (char*)malloc(128); // Massimo 128 bytes per linea ? Nessun controllo qui !
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

    if (p != my_basic_main_lv_obj) {
#ifdef LogDebug
        log_i("{Destroy lv_obj_t * 0x%llx}\n", (long long)p);
#endif
        lv_obj_clean(p);
    }
}

static void* lv_obj_clone(struct mb_interpreter_t* s, void* d) {
    lv_obj_t* p = (lv_obj_t*)d;
    lv_obj_t* q = (lv_obj_t*)malloc(sizeof(lv_obj_t));

    mb_assert(s);
    if (q) memcpy(q, p, sizeof(lv_obj_t));

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

    result = snprintf(b, z, "[0x%llx]", (long long)p) + 1;

    return result;
}

static int _lv_btn_create(struct mb_interpreter_t* s, void** l) {
    int result = MB_FUNC_OK;

    mb_assert(s && l);
    mb_check(mb_attempt_open_bracket(s, l));

    {
        lv_obj_t* p;

#ifdef SimpleUsertype
        mb_check(mb_pop_usertype(s, l, (void**)&p));
#else
        mb_value_t arg;
        mb_make_nil(arg);
        mb_check(mb_pop_value(s, l, &arg));
        mb_check(mb_get_ref_value(s, l, arg, (void**)&p));
#endif

        int_t x1, y1, x2, y2;
        mb_check(mb_pop_int(s, l, &x1));
        mb_check(mb_pop_int(s, l, &y1));
        mb_check(mb_pop_int(s, l, &x2));
        mb_check(mb_pop_int(s, l, &y2));

        lv_obj_t* _p = lv_btn_create(p, NULL);
        lv_obj_set_pos(_p, x1, y1);
        lv_obj_set_size(_p, x2, y2);
        lv_task_handler();

#ifdef SimpleUsertype
        mb_value_t ret;
        mb_make_nil(ret);
        ret.type = MB_DT_USERTYPE;
        ret.value.usertype = _p;
        mb_check(mb_push_usertype(s, l, &ret));
#else
        mb_value_t ret;
        mb_check(mb_make_ref_value(s, _p, &ret, lv_obj_unref, lv_obj_clone, lv_obj_hash, lv_obj_cmp, lv_obj_fmt));
        mb_check(mb_push_value(s, l, ret));
        mb_check(mb_unref_value(s, l, arg));
#endif
#ifdef LogDebug
        log_i("{lv_btn_create(0x%llx)=0x%llx}\n", (long long)p, (long long)_p);
#endif
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

#ifdef SimpleUsertype
        mb_check(mb_pop_usertype(s, l, (void**)&p));
#else
        mb_value_t arg;
        mb_make_nil(arg);
        mb_check(mb_pop_value(s, l, &arg));
        mb_check(mb_get_ref_value(s, l, arg, (void**)&p));
#endif

        int_t x1, y1, x2, y2;
        mb_check(mb_pop_int(s, l, &x1));
        mb_check(mb_pop_int(s, l, &y1));
        mb_check(mb_pop_int(s, l, &x2));
        mb_check(mb_pop_int(s, l, &y2));

        lv_obj_t* _p = lv_label_create(p, NULL);
        lv_obj_set_pos(_p, x1, y1);
        lv_obj_set_size(_p, x2, y2);
        lv_task_handler();

#ifdef SimpleUsertype
        mb_value_t ret;
        mb_make_nil(ret);
        ret.type = MB_DT_USERTYPE;
        ret.value.usertype = _p;
        mb_check(mb_push_usertype(s, l, &ret));
#else
        mb_value_t ret;
        mb_check(mb_make_ref_value(s, _p, &ret, lv_obj_unref, lv_obj_clone, lv_obj_hash, lv_obj_cmp, lv_obj_fmt));
        mb_check(mb_push_value(s, l, ret));
        mb_check(mb_unref_value(s, l, arg));
#endif
#ifdef LogDebug
        log_i("{lv_label_create(0x%llx)=0x%llx}\n", (long long)p, (long long)_p);
#endif
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

#ifdef SimpleUsertype
        mb_check(mb_pop_usertype(s, l, (void**)&p));
#else
        mb_value_t arg;
        mb_make_nil(arg);
        mb_check(mb_pop_value(s, l, &arg));
        mb_check(mb_get_ref_value(s, l, arg, (void**)&p));
#endif

        char* str = 0;
        mb_check(mb_pop_string(s, l, &str));

        lv_label_set_text(p, str);
        lv_task_handler();

#ifndef SimpleUsertype
        mb_check(mb_unref_value(s, l, arg));
#endif 
        //log_i("{_lv_label_set_text(0x%llx)=%s}\n", (long long)p, str);
    }

    mb_check(mb_attempt_close_bracket(s, l));
    lv_task_handler();
    return result;
}

static void _lv_msgbox_event_handler(lv_obj_t* obj, lv_event_t event)
{
    if (event == LV_EVENT_VALUE_CHANGED) {
        lv_msgbox_opt = (char*)lv_msgbox_get_active_btn_text(obj);
        //printf("Button: %s\n", lv_msgbox_opt);
    }
}


static int _lv_msgbox(struct mb_interpreter_t* s, void** l) {
    int result = MB_FUNC_OK;

    mb_assert(s && l);
    mb_check(mb_attempt_open_bracket(s, l));

    {
        char* text;
        mb_check(mb_pop_string(s, l, &text));
        char* str[4];
        mb_check(mb_pop_string(s, l, &str[0]));
        mb_check(mb_pop_string(s, l, &str[1]));
        mb_check(mb_pop_string(s, l, &str[2]));
        mb_check(mb_pop_string(s, l, &str[3]));

        lv_obj_t* mbox1 = lv_msgbox_create(lv_scr_act(), NULL);
        lv_msgbox_set_text(mbox1, text);
        lv_msgbox_add_btns(mbox1, (const char**)str);
        //lv_obj_set_width(mbox1, 200);
        lv_obj_set_event_cb(mbox1, _lv_msgbox_event_handler);
        lv_obj_align(mbox1, NULL, LV_ALIGN_CENTER, 0, 0); /*Align to the corner*/

        lv_msgbox_opt = NULL;
        while (lv_msgbox_opt == NULL) {
            lv_task_handler();
            wait(20);       /*Just to let the system breathe */
        }

        if (lv_msgbox_opt) {
            mb_check(mb_push_string(s, l, lv_msgbox_opt));
#ifdef LogDebug
            log_i("{_lv_msgbox()=%s}\n", lv_msgbox_opt);
#endif
        }
        else {
            mb_check(mb_push_string(s, l, ""));
#ifdef LogDebug
            log_i("{_lv_msgbox()=???}\n");
#endif
        }

        lv_obj_del(mbox1);

    }

    mb_check(mb_attempt_close_bracket(s, l));
    lv_task_handler();
    return result;
}


static int _get_main_lv_obj(struct mb_interpreter_t* s, void** l) {
    int result = MB_FUNC_OK;

    mb_assert(s && l);
    mb_check(mb_attempt_open_bracket(s, l));

    log_i("{_get_main_lv_obj(0x%lx)=0x%lx}\n", s, my_basic_main_lv_obj);

    {
        mb_value_t ret;
        mb_make_nil(ret);
#ifdef SimpleUsertype
        ret.type = MB_DT_USERTYPE;
        ret.value.usertype = my_basic_main_lv_obj;
#else
        mb_make_ref_value(s, my_basic_main_lv_obj, &ret, lv_obj_unref, lv_obj_clone, lv_obj_hash, lv_obj_cmp, lv_obj_fmt);
#endif 
        mb_check(mb_push_value(s, l, ret));
        log_i("{_get_main_lv_obj(0x%lx)=0x%lx}\n", s, ret.value.usertype);
#ifdef LogDebug
        
#endif
    }

    mb_check(mb_attempt_close_bracket(s, l));
    lv_task_handler();
    return result;
}



static void _LvEventHandler(lv_obj_t* obj, lv_event_t event) {

    switch (event) {
    case(LV_EVENT_CLICKED):
    {
        for (int i = 0; i < LvEvtHandlersCount; i++) {
            if (LvEventHandlers[i].obj == obj) {
#ifdef LogDebug
                log_i("{Handle event %d for object 0x%lx at 0x%lx}\n", event, LvEventHandlers[i].obj, LvEventHandlers[i].routine.value.routine);
#endif
#ifdef objasparameter
                mb_value_t args[2];
                mb_make_nil(args[0]);

#ifdef SimpleUsertype
                args[0].type = MB_DT_USERTYPE;
                args[0].value.usertype = LvEventHandlers[i].obj;
#else
                lv_obj_t* _p = (lv_obj_t*)lv_obj_clone(LvEventHandlers[i].s, LvEventHandlers[i].obj);
                mb_make_ref_value(LvEventHandlers[i].s, _p, &args[0], lv_obj_unref, lv_obj_clone, lv_obj_hash, lv_obj_cmp, lv_obj_fmt);
#endif
                mb_make_nil(args[1]);
                args[1].type = MB_DT_INT;
                args[1].value.integer = event;

                mb_value_t ret;
                mb_make_nil(ret);
                mb_eval_routine(LvEventHandlers[i].s, LvEventHandlers[i].l, LvEventHandlers[i].routine, args, 2, &ret);
                printf("Returned %d.\n", ret.value.integer);
#else
                mb_value_t args[1];
                mb_make_nil(args[0]);

                args[0].type = MB_DT_INT;
                args[0].value.integer = event;

                mb_value_t ret;
                mb_make_nil(ret);
                mb_eval_routine(LvEventHandlers[i].s, LvEventHandlers[i].l, LvEventHandlers[i].routine, &args[0], 1, &ret);
#endif
            }
        }

        break;
    }
    }
}

static int _SetLvEventHandler(struct mb_interpreter_t* s, void** l) {
    int result = MB_FUNC_OK;

    mb_assert(s && l);
    mb_check(mb_attempt_open_bracket(s, l));

    {
        lv_obj_t* p;

#ifdef SimpleUsertype
        mb_check(mb_pop_usertype(s, l, (void**)&p));
#else
        mb_value_t arg;
        mb_make_nil(arg);
        mb_check(mb_pop_value(s, l, &arg));
        mb_check(mb_get_ref_value(s, l, arg, (void**)&p));
#endif

        char* str = 0;
        mb_check(mb_pop_string(s, l, &str));

#ifdef LogDebug
        log_i("{Set handler %d for object 0x%lx as %s}=", LvEvtHandlersCount, p, str);
#endif  

        for (int i = 0; str[i] != '\0'; i++) {
            if (str[i] >= 'a' && str[i] <= 'z') {
                str[i] = str[i] - 32;
            }
        }

        mb_value_t routine;
        mb_make_nil(routine);
        mb_check(mb_get_routine(s, l, str, &routine));   /* Get the routine from name*/

#ifdef LogDebug
        log_i("0x%lx\n", routine);
#endif 
        LvEventHandlers[LvEvtHandlersCount].obj = p;
        LvEventHandlers[LvEvtHandlersCount].routine = routine;
        LvEventHandlers[LvEvtHandlersCount].s = s;
        LvEventHandlers[LvEvtHandlersCount].l = l;
        lv_obj_set_event_cb(p, _LvEventHandler);

#ifdef LogDebug
        log_i("{Set handler %d for object 0x%lx as %s}\n", LvEvtHandlersCount, p, str);
#endif
        LvEvtHandlersCount++;

    }

    mb_check(mb_attempt_close_bracket(s, l));
    lv_task_handler();
    return result;
}


void enableLVGLprint(struct mb_interpreter_t* bas, lv_obj_t* l) {
    my_basic_output_label = l;
    mb_set_printer(bas, lvglprint);
    mb_register_func(bas, "CLS", lvglclear);
}
void SetLvPtr(lv_obj_t* p, lv_style_t* s) {
    my_basic_main_lv_obj = p;
    my_basic_main_lv_style = s;
#ifdef LogDebug
        log_i("{SetLvObj(0x%lx, 0x%lx)}\n", p, s);
#endif
}

void enableLVGL(struct mb_interpreter_t* bas, lv_obj_t* p, lv_style_t* s) {
#ifdef LogDebug
        log_i("{EnableLVGL(0x%lx, 0x%lx, 0x%lx)}\n", bas, p, s);
#endif
    //mb_begin_module(s, "LVGL");
    mb_register_func(bas, "GetMainLvObj", _get_main_lv_obj);
    mb_register_func(bas, "LvButtonCreate", _lv_btn_create);
    mb_register_func(bas, "LvLabelCreate", _lv_label_create);
    mb_register_func(bas, "LvLabelSetText", _lv_label_set_text);
    mb_register_func(bas, "SetLvEventHandler", _SetLvEventHandler);
    mb_register_func(bas, "LvMsgbox", _lv_msgbox);
    SetLvPtr(p,s);
    //mb_end_module(s);
}
