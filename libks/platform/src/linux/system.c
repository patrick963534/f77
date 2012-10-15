#include <ks/sys/system.h>
#include <GLES2/gl2.h>
#include <EGL/egl.h>
#include <stdlib.h>
#include <string.h>
#include <ks/log.h>

///
//  Macros
//
#define ESUTIL_API
#define ESCALLBACK

/// esCreateWindow flag - RGB color buffer
#define ES_WINDOW_RGB           0

/// esCreateWindow flag - ALPHA color buffer
#define ES_WINDOW_ALPHA         1 

/// esCreateWindow flag - depth buffer
#define ES_WINDOW_DEPTH         2 

/// esCreateWindow flag - stencil buffer
#define ES_WINDOW_STENCIL       4

/// esCreateWindow flat - multi-sample buffer
#define ES_WINDOW_MULTISAMPLE   8

typedef struct system_t
{
    ks_extends_sys_system();
    
    EGLNativeWindowType     hwnd;
    EGLDisplay              display;
    EGLContext              context;
    EGLSurface              surface;
    GLint                   width;
    GLint                   height;

    Display*                x_display;

} system_t;

static system_t* sys = 0;

static void destruct(system_t* me)
{
    ks_unused(me);

    ks_container_remove((ks_object_t*)me);
}

static int init_window(const char* title)
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

    swa.event_mask  =  ExposureMask | PointerMotionMask | KeyPressMask;
    win = XCreateWindow(x_display, root, 0, 0, sys->width, sys->height, 0,
                        CopyFromParent, InputOutput,
                        CopyFromParent, CWEventMask, &swa);

    xattr.override_redirect = 0;
    XChangeWindowAttributes(x_display, win, CWOverrideRedirect, &xattr);

    hints.input = 1;
    hints.flags = InputHint;
    XSetWMHints(x_display, win, &hints);

    // make the window visible on the screen
    XMapWindow (x_display, win);
    XStoreName(x_display, win, title);

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
    GLuint flags = ES_WINDOW_RGB;
    EGLint attribList[] =
    {
        EGL_RED_SIZE,       5,
        EGL_GREEN_SIZE,     6,
        EGL_BLUE_SIZE,      5,
        EGL_ALPHA_SIZE,     (flags & ES_WINDOW_ALPHA) ? 8 : EGL_DONT_CARE,
        EGL_DEPTH_SIZE,     (flags & ES_WINDOW_DEPTH) ? 8 : EGL_DONT_CARE,
        EGL_STENCIL_SIZE,   (flags & ES_WINDOW_STENCIL) ? 8 : EGL_DONT_CARE,
        EGL_SAMPLE_BUFFERS, (flags & ES_WINDOW_MULTISAMPLE) ? 1 : 0,
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

static int create_window(const char* title, int w, int h)
{
    sys->width  = w;
    sys->height = h;       

    if (!init_window(title))
        return 0;

    if (!init_egl())
        return 0;

    return 1;
}

static void flush()
{
    eglSwapBuffers(sys->display, sys->surface);
}

static ks_sys_system_interface_t interface = {
    create_window,
    flush,
};

KS_API void ks_sys_system_init(ks_container_t* container)
{
    sys             = (system_t*)ks_object_new(sizeof(*sys));
    sys->destruct   = (ks_destruct_f)destruct;
    sys->klass      = &interface;

    if (container)
        ks_container_add(container, (ks_object_t*)sys);
}

KS_API ks_sys_system_t* ks_sys_system_instance()
{
    return (ks_sys_system_t*)sys;
}
