#include <ks/sys/system.h>
#include <GLES2/gl2.h>
#include <EGL/egl.h>
#include <stdlib.h>
#include <string.h>
#include <ks/log.h>
#include <windows.h>
#include "ks/sys/graphics.h"
#include "ks/sys/eventq.h"
#include "ks/constants.h"
#include "ks/director.h"

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

    ks_object_destruct((ks_object_t*)me);
}

LRESULT WINAPI ESWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) 
{
    LRESULT  lRet = 1; 
    ks_event_t e;

    switch (uMsg) 
    { 
        case WM_DESTROY:
            PostQuitMessage(0);             
            break; 

        case WM_KEYDOWN:
            e.type = ks.types.KEY_DOWN;
            e.key.code = (int)wParam;
            ks_sys_eventq_instance()->klass->endqueue(&e);
            break;

        case WM_KEYUP:
            e.type = ks.types.KEY_UP;
            e.key.code = (int)wParam;
            ks_sys_eventq_instance()->klass->endqueue(&e);
            break;

        case WM_CHAR:
            e.type = ks.types.KEY_CHAR;
            e.key.character = (int)wParam;
            ks_sys_eventq_instance()->klass->endqueue(&e);
            break;

        default: 
            lRet = DefWindowProc (hWnd, uMsg, wParam, lParam); 
            break; 
    } 

    return lRet; 
}

static int init_window()
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
    windowRect.right = ks_director_instance()->width;
    windowRect.bottom = ks_director_instance()->height;

    AdjustWindowRect ( &windowRect, wStyle, FALSE );

    sys->hwnd = CreateWindow(
        "opengles2.0",
        ks_director_instance()->title,
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

    EGLint attribList[] = {
        EGL_RED_SIZE,       5,
        EGL_GREEN_SIZE,     6,
        EGL_BLUE_SIZE,      5,
        EGL_DEPTH_SIZE,     8,
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
    MSG sMessage;

    if(PeekMessage(&sMessage, NULL, 0, 0, PM_REMOVE)) 
    {
        TranslateMessage(&sMessage);
        DispatchMessage(&sMessage);
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

KS_API void ks_sys_system_init(ks_container_t* container)
{
    sys             = (system_t*)ks_object_new(sizeof(*sys));
    sys->destruct   = (ks_destruct_f)destruct;
    sys->klass      = &interfaces;

    if (container)
        ks_container_add(container, (ks_object_t*)sys);

    create_window();

    ks_sys_graphics_init(container);
    ks_sys_eventq_init(container);
}

KS_API ks_sys_system_t* ks_sys_system_instance()
{
    return (ks_sys_system_t*)sys;
}
