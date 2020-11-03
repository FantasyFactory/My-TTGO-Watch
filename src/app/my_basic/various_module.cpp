extern "C"
{
#include "my_basic.h"
}

#include "Arduino.h"


static int _swap(struct mb_interpreter_t* s, void** l) {
    int result = MB_FUNC_OK;
    void* v0 = 0;
    void* v1 = 0;
    mb_value_t val0;
    mb_value_t val1;

    mb_assert(s && l);

    mb_check(mb_attempt_open_bracket(s, l));

    mb_check(mb_get_var(s, l, &v0, true));
    mb_check(mb_get_var(s, l, &v1, true));

    mb_check(mb_attempt_close_bracket(s, l));

    mb_get_var_value(s, v0, &val0);
    mb_get_var_value(s, v1, &val1);
    mb_set_var_value(s, v0, val1);
    mb_set_var_value(s, v1, val0);

    return result;
}

void enableVariousModule(struct mb_interpreter_t* bas)
{
    mb_register_func(bas, "SWAP", _swap);
}
