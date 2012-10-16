#include <ks/sys/system.h>
#include <GLES2/gl2.h>
#include <EGL/egl.h>
#include <stdlib.h>
#include <string.h>
#include <ks/log.h>
#include <windows.h>

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

} system_t;

static system_t* sys = 0;

static void destruct(system_t* me)
{
    ks_unused(me);

    ks_container_remove((ks_object_t*)me);
}

LRESULT WINAPI ESWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) 
{
    LRESULT  lRet = 1; 

    switch (uMsg) 
    { 
    case WM_CREATE:
        break;

    case WM_PAINT:
        {
            //ESContext *esContext = (ESContext*)(LONG_PTR) GetWindowLongPtr ( hWnd, GWL_USERDATA );

            //if ( esContext && esContext->drawFunc )
            //    esContext->drawFunc ( esContext );

            //ValidateRect( esContext->hWnd, NULL );
        }
        break;

    case WM_DESTROY:
        PostQuitMessage(0);             
        break; 

    case WM_CHAR:
        {
            //POINT      point;
            //ESContext *esContext = (ESContext*)(LONG_PTR) GetWindowLongPtr ( hWnd, GWL_USERDATA );

            //GetCursorPos( &point );

            //if ( esContext && esContext->keyFunc )
            //    esContext->keyFunc ( esContext, (unsigned char) wParam, 
            //    (int) point.x, (int) point.y );
        }
        break;

    default: 
        lRet = DefWindowProc (hWnd, uMsg, wParam, lParam); 
        break; 
    } 

    return lRet; 
}

static int init_window(const char* title)
{
    WNDCLASS wndclass = {0}; 
    DWORD    wStyle   = 0;
    RECT     windowRect;
    HINSTANCE hInstance = GetModuleHandle(NULL);

    wndclass.style         = CS_OWNDC;
    wndclass.lpfnWndProc   = (WNDPROC)ESWindowProc; 
    wndclass.hInstance     = hInstance; 
    wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH); 
    wndclass.lpszClassName = "opengles2.0"; 

    if (!RegisterClass (&wndclass) ) 
        return FALSE; 

    wStyle = WS_VISIBLE | WS_POPUP | WS_BORDER | WS_SYSMENU | WS_CAPTION;

    // Adjust the window rectangle so that the client area has
    // the correct number of pixels
    windowRect.left = 0;
    windowRect.top = 0;
    windowRect.right = sys->width;
    windowRect.bottom = sys->height;

    AdjustWindowRect ( &windowRect, wStyle, FALSE );

    sys->hwnd = CreateWindow(
        "opengles2.0",
        title,
        wStyle,
        0,
        0,
        windowRect.right - windowRect.left,
        windowRect.bottom - windowRect.top,
        NULL,
        NULL,
        hInstance,
        NULL);

    // Set the ESContext* to the GWL_USERDATA so that it is available to the 
    // ESWindowProc
    //SetWindowLongPtr (sys->hwnd, GWL_USERDATA, (LONG) (LONG_PTR)sys);

    if ( sys->hwnd == NULL )
        return GL_FALSE;

    ShowWindow(sys->hwnd, TRUE);

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
        EGL_DEPTH_SIZE,     8,
        EGL_ALPHA_SIZE,     (flags & ES_WINDOW_ALPHA) ? 8 : EGL_DONT_CARE,
        EGL_DEPTH_SIZE,     (flags & ES_WINDOW_DEPTH) ? 8 : EGL_DONT_CARE,
        EGL_STENCIL_SIZE,   (flags & ES_WINDOW_STENCIL) ? 8 : EGL_DONT_CARE,
        EGL_SAMPLE_BUFFERS, (flags & ES_WINDOW_MULTISAMPLE) ? 1 : 0,
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
        EGL_NONE
    };

    display = eglGetDisplay(GetDC(sys->hwnd));
    if (display == EGL_NO_DISPLAY)
        goto fail;

    if (!eglInitialize(display, &majorVersion, &minorVersion))
        goto fail;

    if (!eglGetConfigs(display, NULL, 0, &numConfigs))
        goto fail;
 
    if (!eglChooseConfig(display, attribList, &config, 1, &numConfigs))
        goto fail;

    if (numConfigs <= 0)
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

static int get_width()
{
    return sys->width;
}

static int get_height()
{
    return sys->height;
}

static ks_sys_system_interface_t interface = {
    create_window,
    get_width,
    get_height,
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
