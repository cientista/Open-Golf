#include "golf/scripting.h"

#include <stdio.h>

#include "s7/s7.h"
#include "golf/file.h"

static char sc_error[1024];
static s7_scheme *sc;

/*
static s7_pointer sc_error_handler(s7_scheme *sc, s7_pointer args) {
    snprintf(sc_error, 1024, "error: %s", s7_string(s7_car(args)));
    printf("error %s\n", s7_string(s7_car(args)));
    return s7_f(sc);
}
*/

void golf_scripting_init(void) {
    sc = s7_init();
    /*
    s7_define_function(sc, "error-handler", sc_error_handler, 1, 0, false, "our error handler");
    s7_eval_c_string(sc, "(set! (hook-functions *error-hook*)   \n\
        (list (lambda (hook)                                    \n\
               (error-handler                                   \n\
                (apply format #f (hook 'data)))                 \n\
               (set! (hook 'result) 'our-error))))");
               */

    s7_gc_on(sc, false);

    char *data;
    int data_len;
    if (golf_file_load_data("data/scripts/editor.scm", &data, &data_len)) {
        s7_pointer port = s7_open_input_string(sc, data);
        s7_pointer code = s7_read(sc, port);
        while (!s7_is_eq(s7_eof_object(sc), code)) {
            s7_pointer result = s7_eval(sc, code, s7_nil(sc));
            code = s7_read(sc, port);
        }
        s7_close_input_port(sc, port);
        golf_free(data);
    } 

    s7_pointer fib_fn = s7_name_to_value(sc, "fib");
    if (s7_is_procedure(fib_fn)) {
        printf("%s\n", s7_object_to_c_string(sc, s7_closure_args(sc, fib_fn)));
    }

    s7_gc_on(sc, true);
}