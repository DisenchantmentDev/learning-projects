#include "x11_wrapper.h"

int
main ()
{
    xw_context *ctx = { 0 }; // zero initialize onto the stack
    XW_init_basic_window (ctx, 500, 500);
    XW_draw_basic_window (ctx);
    return 0;
}
