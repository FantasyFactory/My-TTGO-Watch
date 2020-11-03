extern "C"
{
#include "my_basic.h"
}

#include <TTGO.h>
#include <stdio.h>

static int _file_open(struct mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;
	FILE* fp = 0;
	char* fn = 0;
	char* fm = 0;

	mb_assert(s && l);
	mb_check(mb_attempt_open_bracket(s, l));
	mb_check(mb_pop_string(s, l, &fn));
	mb_check(mb_pop_string(s, l, &fm));
	mb_check(mb_attempt_close_bracket(s, l));

	if(!fn) {
		result = MB_FUNC_ERR;
		goto _exit;
	}

	fp = fopen(fn, fm);
	if(!fp) {
		result = MB_FUNC_ERR;
		goto _exit;
	}

_exit:
	mb_check(mb_push_usertype(s, l, (void*)fp));

	return result;
}

static int _file_close(struct mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;
	FILE* fp = 0;
	void* up = 0;

	mb_assert(s && l);
	mb_check(mb_attempt_open_bracket(s, l));
	mb_check(mb_pop_usertype(s, l, &up));
	mb_check(mb_attempt_close_bracket(s, l));

	if(!up)
		return MB_FUNC_ERR;

	fp = (FILE*)up;
	fclose(fp);

	return result;
}

static int _file_peek(struct mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;
	FILE* fp = 0;
	void* up = 0;
	long ft = -1; /* Push -1 for error */

	mb_assert(s && l);
	mb_check(mb_attempt_open_bracket(s, l));
	mb_check(mb_pop_usertype(s, l, &up));
	mb_check(mb_attempt_close_bracket(s, l));

	if(!up) {
		result = MB_FUNC_ERR;
		goto _exit;
	}

	fp = (FILE*)up;
	ft = ftell(fp);

_exit:
	mb_check(mb_push_int(s, l, ft));

	return result;
}

static int _file_poke(struct mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;
	FILE* fp = 0;
	void* up = 0;
	int_fast64_t fo = 0;

	mb_assert(s && l);
	mb_check(mb_attempt_open_bracket(s, l));
	mb_check(mb_pop_usertype(s, l, &up));
	mb_check(mb_pop_int(s, l, &fo));
	mb_check(mb_attempt_close_bracket(s, l));

	if(!up)
		return MB_FUNC_ERR;

	fp = (FILE*)up;
	fseek(fp, fo, SEEK_SET);

	return result;
}

static int _file_read(struct mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;
	FILE* fp = 0;
	void* up = 0;
	int64_t ln = 0;

	mb_assert(s && l);
	mb_check(mb_attempt_open_bracket(s, l));
	mb_check(mb_pop_usertype(s, l, &up));

	if(mb_has_arg(s, l)) {
		mb_check(mb_pop_int(s, l, &ln));
	}

	mb_check(mb_attempt_close_bracket(s, l));

	if(!up) {
		result = MB_FUNC_ERR;
		goto _exit;
	}

	fp = (FILE*)up;
	if(ln) {
		char* buf = (char*)malloc(ln + 1);
		fgets(buf, ln + 1, fp);
		buf[ln] = '\0';
		mb_check(mb_push_string(s, l, mb_memdup(buf, ln + 1)));
		free(buf);
	} else {
		int ret = fgetc(fp);
		mb_check(mb_push_int(s, l, ret));
	}

_exit:

	return result;
}

static int _file_read_line(struct mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;
	FILE* fp = 0;
	void* up = 0;

	mb_assert(s && l);
	mb_check(mb_attempt_open_bracket(s, l));
	mb_check(mb_pop_usertype(s, l, &up));
	mb_check(mb_attempt_close_bracket(s, l));

	if(!up) {
		result = MB_FUNC_ERR;
		goto _exit;
	}

	fp = (FILE*)up;
	{
		char* buf = 0;
		size_t size = 0;
		while(!feof(fp)) {
			buf = (char*)realloc(buf, ++size);
			buf[size - 1] = (char)fgetc(fp);
			if(buf[size - 1] == '\n' || buf[size - 1] == '\r') {
				long l = ftell(fp);
				if(fgetc(fp) != '\r')
					fseek(fp, l, SEEK_SET);
				break;
			}
		}
		if(buf) {
			buf[size - 1] = '\0';
			mb_check(mb_push_string(s, l, mb_memdup(buf, size)));
		} else {
			mb_check(mb_push_string(s, l, mb_memdup("", 1)));
		}
		if(buf)
			free(buf);
	}

_exit:
	return result;
}

static int _file_write(struct mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;
	FILE* fp = 0;
	void* up = 0;
	mb_value_t val;

	mb_assert(s && l);
	mb_check(mb_attempt_open_bracket(s, l));
	mb_check(mb_pop_usertype(s, l, &up));

	if(!up) {
		result = MB_FUNC_ERR;
		goto _exit;
	}

	fp = (FILE*)up;

	while(mb_has_arg(s, l)) { /* Support variadic */
		mb_check(mb_pop_value(s, l, &val));
		switch(val.type) {
		case MB_DT_INT:
			fputc(val.value.integer, fp);

			break;
		case MB_DT_REAL:
			fputc((int_t)val.value.float_point, fp);

			break;
		case MB_DT_STRING:
			fputs(val.value.string, fp);

			break;
		default:
			result = MB_FUNC_ERR;

			break;
		}
	}

_exit:
	mb_check(mb_attempt_close_bracket(s, l));

	return result;
}

static int _file_write_line(struct mb_interpreter_t* s, void** l) {
	int result = MB_FUNC_OK;
	FILE* fp = 0;
	void* up = 0;
	mb_value_t val;

	mb_assert(s && l);
	mb_check(mb_attempt_open_bracket(s, l));
	mb_check(mb_pop_usertype(s, l, &up));

	if(!up) {
		result = MB_FUNC_ERR;
		goto _exit;
	}

	fp = (FILE*)up;

	while(mb_has_arg(s, l)) { /* Support variadic */
		mb_check(mb_pop_value(s, l, &val));
		switch(val.type) {
		case MB_DT_INT:
			fputc(val.value.integer, fp);

			break;
		case MB_DT_REAL:
			fputc((int_t)val.value.float_point, fp);

			break;
		case MB_DT_STRING:
			fputs(val.value.string, fp);

			break;
		default:
			result = MB_FUNC_ERR;

			break;
		}
	}

	fputc('\n', fp);
	fputc('\r', fp);

_exit:
	mb_check(mb_attempt_close_bracket(s, l));

	return result;
}

void enableFileModule(struct mb_interpreter_t* bas)
{
    mb_register_func(bas, "FOPEN", _file_open);
    mb_register_func(bas, "FCLOSE", _file_close);
    mb_register_func(bas, "FPEEK", _file_peek);
    mb_register_func(bas, "FPOKE", _file_poke);
    mb_register_func(bas, "FREAD", _file_read);
    mb_register_func(bas, "FREAD_LINE", _file_read_line);
    mb_register_func(bas, "FWRITE", _file_write);
    mb_register_func(bas, "FWRITE_LINE", _file_write_line);
}