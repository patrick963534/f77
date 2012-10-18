#include <ks/system.h>
#include <ks/graphics.h>
#include <ks/eventq.h>
#include <ks/director.h>
#include <ks/constants.h>
#include <ks/event.h>
#include <GLES2/gl2.h>
#include <EGL/egl.h>
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
    
    EGLNativeWindowType     hwnd;
    EGLDisplay              display;
    EGLContext              context;
    EGLSurface              surface;

    Display*                x_display;

} system_t;

static system_t* sys = 0;

static void destruct(system_t* me)
{
    ks_unused(me);

    ks_log("%s", "destruct system");

    ks_object_destruct((ks_object_t*)me);
}

static int init_window()
{
    Window                  root;
    XSetWindowAttributes    swa;
    XSetWindowAttributes    xattr;
    Atom                    wm_state;
    XWMHints                hints;
    XEvent                  xev;
    Window                  win;
    Display*                x_display;

    sys->x_display = x_display = XOpenDisplay(NULL);
    if (x_display == NULL)
        return 1;

    root = DefaultRootWindow(x_display);

    //swa.event_mask =  ExposureMask | PointerMotionMask | KeyPressMask;
    //swa.event_mask = KeyPressMask | ExposureMask | EnterWindowMask | LeaveWindowMask | PointerMotionMask | VisibilityChangeMask | ButtonPressMask | ButtonReleaseMask | StructureNotifyMask;

    swa.event_mask =  ExposureMask | PointerMotionMask | KeyPressMask | KeyReleaseMask;

    win = XCreateWindow(x_display, root, 0, 0, 
                        ks_director_instance()->width, 
                        ks_director_instance()->height, 0,
                        CopyFromParent, InputOutput,
                        CopyFromParent, CWEventMask, &swa);

    xattr.override_redirect = 0;
    XChangeWindowAttributes(x_display, win, CWOverrideRedirect, &xattr);

    hints.input = 1;
    hints.flags = InputHint;
    XSetWMHints(x_display, win, &hints);

    // make the window visible on the screen
    XMapWindow(x_display, win);
    XStoreName(x_display, win, ks_director_instance()->title);

    // get identifiers for the provided atom name strings
    wm_state = XInternAtom (x_display, "_NET_WM_STATE", 0);

    memset(&xev, 0, sizeof(xev));
    xev.type                 = ClientMessage;
    xev.xclient.window       = win;
    xev.xclient.message_type = wm_state;
    xev.xclient.format       = 32;
    xev.xclient.data.l[0]    = 1;
    xev.xclient.data.l[1]    = 0;
    XSendEvent(x_display, DefaultRootWindow(x_display), 
               0, SubstructureNotifyMask, &xev);

    sys->hwnd = (EGLNativeWindowType)win;

    return 1;
}

static int init_egl()
{
    EGLint numConfigs;
    EGLint majorVersion;
    EGLint minorVersion;
    EGLDisplay display;
    EGLContext context;
    EGLSurface surface;
    EGLConfig config;
    EGLint contextAttribs[] = {EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE, EGL_NONE };
    EGLint attribList[] =
    {
        EGL_RED_SIZE,       5,
        EGL_GREEN_SIZE,     6,
        EGL_BLUE_SIZE,      5,
        EGL_DEPTH_SIZE,     8,
        EGL_NONE
    };

    display = eglGetDisplay((EGLNativeDisplayType)sys->x_display);
    if (display == EGL_NO_DISPLAY)
        goto fail;

    if (!eglInitialize(display, &majorVersion, &minorVersion))
        goto fail;

    if (!eglGetConfigs(display, NULL, 0, &numConfigs))
        goto fail;
 
    if (!eglChooseConfig(display, attribList, &config, 1, &numConfigs))
        goto fail;
 
    surface = eglCreateWindowSurface(display, config, sys->hwnd, NULL);
    if (surface == EGL_NO_SURFACE)
        goto fail;
 
    context = eglCreateContext(display, config, EGL_NO_CONTEXT, contextAttribs);
    if (context == EGL_NO_CONTEXT)
        goto fail;
    
    if (!eglMakeCurrent(display, surface, surface, context))
        goto fail;
    
    sys->display = display;
    sys->surface = surface;
    sys->context = context;

    ks_log("display: %d", sys->display);
    ks_log("surface: %d", sys->surface);
    ks_log("context: %d", sys->context);

    return 1;

fail:
    ks_log("%s", "init egl failed...");
    return 0;
}

static int create_window()
{
    if (!init_window())
        return 0;

    if (!init_egl())
        return 0;

    return 1;
}

static void update_messages()
{
    XEvent xe;
    ks_event_t e;

    while (XPending(sys->x_display)) 
    {
        XNextEvent(sys->x_display, &xe);

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
    eglSwapBuffers(sys->display, sys->surface);
}

static ks_sys_system_interface_t interfaces = {
    update_messages,
    flush,
};

KS_API void ks_system_init(ks_container_t* container)
{
    sys             = (system_t*)ks_object_new(sizeof(*sys));
    sys->destruct   = (ks_destruct_f)destruct;
    sys->klass      = &interfaces;

    if (container)
        ks_container_add(container, (ks_object_t*)sys);

    create_window();
    
    ks_graphics_init(container);
    ks_eventq_init(container);
}

KS_API ks_system_t* ks_system_instance()
{
    return (ks_system_t*)sys;
}
