#include "x11_wrapper.h"

int
main ()
{
    xw_context *ctx = calloc (1, sizeof *ctx);
    XW_init_basic_window (ctx, 500, 500);
    XW_draw_basic_window (ctx);
    return 0;
}
