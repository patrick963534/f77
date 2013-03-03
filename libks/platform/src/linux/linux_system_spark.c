#include <ks/system.h>
#include <ks/graphics.h>
#include <ks/eventq.h>
#include <ks/director.h>
#include <ks/constants.h>
#include <ks/event.h>
#include <stdlib.h>
#include <string.h>
#include <ks/log.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/Xatom.h>
#include <X11/keysym.h>

typedef struct system_t
{
    ks_extends_system();

    Display*    display;
    Window      win;

    GC          gc;
    XImage*     img;
    Pixmap      pixmap;
    char*       img_data;

} system_t;

static system_t* sys = 0;

static void destruct(system_t* me)
{
    ks_unused(me);

    ks_log("%s", "destruct system");

    ks_object_destruct((ks_object_t*)me);
}

static int create_window()
{
    Window                  root;
    XSetWindowAttributes    swa;
    XSetWindowAttributes    xattr;
    Atom                    wm_state;
    XWMHints                hints;
    XEvent                  xev;
    Window                  win;
    Display*                display;

    sys->display = display = XOpenDisplay(NULL);
    if (display == NULL)
        return 1;

    root = DefaultRootWindow(display);

    swa.event_mask =  ExposureMask | PointerMotionMask | KeyPressMask | KeyReleaseMask;

    win = XCreateWindow(display, root, 0, 0,
                        ks_director_instance()->width,
                        ks_director_instance()->height, 0,
                        CopyFromParent, InputOutput,
                        CopyFromParent, CWEventMask, &swa);

    xattr.override_redirect = 0;
    XChangeWindowAttributes(display, win, CWOverrideRedirect, &xattr);

    hints.input = 1;
    hints.flags = InputHint;
    XSetWMHints(display, win, &hints);

    // make the window visible on the screen
    XMapWindow(display, win);
    XStoreName(display, win, ks_director_instance()->title);

    // get identifiers for the provided atom name strings
    wm_state = XInternAtom (display, "_NET_WM_STATE", 0);

    memset(&xev, 0, sizeof(xev));
    xev.type                 = ClientMessage;
    xev.xclient.window       = win;
    xev.xclient.message_type = wm_state;
    xev.xclient.format       = 32;
    xev.xclient.data.l[0]    = 1;
    xev.xclient.data.l[1]    = 0;
    XSendEvent(display, DefaultRootWindow(display), 0, SubstructureNotifyMask, &xev);

    sys->win = win;

    return 1;
}

static void update_messages()
{
    XEvent xe;
    ks_event_t e;

    while (XPending(sys->display))
    {
        XNextEvent(sys->display, &xe);

        if (xe.type == ButtonPress)
        {
            ks_log("%s", "button down");
        }
        else if (xe.type == KeyPress)
        {
            e.type = ks.types.KEY_DOWN;
            e.key.code = XLookupKeysym(&xe.xkey, 0);
            ks_eventq_endqueue(&e);
            ks_log("keydown:  %d", e.key.code);
        }
        else if (xe.type == KeyRelease)
        {
            e.type = ks.types.KEY_UP;
            e.key.code = XLookupKeysym(&xe.xkey, 0);
            ks_eventq_endqueue(&e);
            ks_log("keyup:  %d", e.key.code);

            if (e.key.code >= 'a' && e.key.code <= 'z')
            {
                e.type = ks.types.KEY_CHAR;
                e.key.code = e.key.code;
                ks_eventq_endqueue(&e);
                ks_log("keychar:  %d", e.key.code);
            }
        }
    }
}

static void flush()
{
    int w = ks_director_instance()->width;
    int h = ks_director_instance()->height;
    int* ptr = (int*)sys->img_data;
    int i, j;

    //memset(sys->img_data, 255, w * h * bpp);
    for (i = 0; i < h; ++i)
    {
        for (j = 0; j < w; ++j)
        {
            ptr[i * w + j] = 0xffff00ff;
        }
    }

    XPutImage(sys->display, sys->pixmap, sys->gc, sys->img, 0, 0, 0, 0, w, h);
    XCopyArea(sys->display, sys->pixmap, sys->win, sys->gc, 0, 0, w, h, 0, 0);

    XFlush(sys->display);
    XSync(sys->display, 0);
    //sleep(3);
}

static void init_buffer_image()
{
    int w = ks_director_instance()->width;
    int h = ks_director_instance()->height;
    int screen = DefaultScreen(sys->display);
    int depth = DefaultDepth(sys->display, screen);
    int bpp = (depth + 7) / 8 + 1;

    XMapWindow(sys->display, sys->win);

    sys->gc = XCreateGC(sys->display, sys->win, 0, 0);

    XSetForeground(sys->display, sys->gc, BlackPixel(sys->display, screen));
    XSetBackground(sys->display, sys->gc, WhitePixel(sys->display, screen));

    XClearWindow(sys->display, sys->win);

    sys->img_data = malloc(w * h * bpp);

    sys->img = XCreateImage(sys->display, CopyFromParent, depth, ZPixmap, 0, sys->img_data, w, h, 8, w * bpp);
    sys->pixmap = XCreatePixmap(sys->display, sys->win, w, h, depth);
}

static ks_sys_system_interface_t interfaces = {
    update_messages,
    flush,
};

KS_API void ks_system_init(ks_object_t* container)
{
    sys             = (system_t*)ks_object_new(sizeof(*sys));
    sys->destruct   = (ks_destruct_f)destruct;
    sys->klass      = &interfaces;

    if (container)
        ks_object_add(container, (ks_object_t*)sys);

    create_window();

    init_buffer_image();

    ks_graphics_init(container);
    ks_eventq_init(container);
}

KS_API ks_system_t* ks_system_instance()
{
    return (ks_system_t*)sys;
}
