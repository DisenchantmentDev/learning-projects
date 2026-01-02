#ifndef x11_wrapper
#define x11_wrapper

#include <X11/Xlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    XEvent event;
    Display *display;
    Window w;
} xw_context;

#ifndef XWDEF
#define XWDEF static inline
#endif /* XWDEF */

XWDEF void XW_init_basic_window (xw_context *ctx, int x_size, int y_size);

XWDEF void XW_draw_basic_window (xw_context *ctx);

XWDEF void
XW_init_basic_window (xw_context *ctx, int x_size, int y_size)
{
    ctx->display = XOpenDisplay (NULL);
    ctx->w = XCreateSimpleWindow (
        ctx->display, DefaultRootWindow (ctx->display), 50, 50, x_size, y_size,
        1, BlackPixel (ctx->display, 0), WhitePixel (ctx->display, 0));
    XMapWindow (ctx->display, ctx->w);
    XSelectInput (ctx->display, ctx->w, ExposureMask);
}

XWDEF void
XW_draw_basic_window (xw_context *ctx)
{
    for (;;)
        {
            XNextEvent (ctx->display, &(ctx->event));
            if (ctx->event.type == Expose)
                {
                    char *message = "Hello World";
                    XDrawString (ctx->display, ctx->w,
                                 DefaultGC (ctx->display, 0), 100, 100,
                                 message, strlen (message));
                }
        }
}

#endif
