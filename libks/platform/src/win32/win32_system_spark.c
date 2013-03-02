#include <ks/system.h>
#include <ks/graphics.h>
#include <ks/eventq.h>
#include <ks/constants.h>
#include <ks/director.h>
#include <ks/log.h>

#include <windows.h>
#include <stdlib.h>
#include <string.h>

typedef struct system_t
{
    ks_extends_system();

    HWND    hwnd;

} system_t;

static system_t* sys = 0;

static void destruct(system_t* me)
{
    ks_object_destruct((ks_object_t*)me);
}

LRESULT WINAPI WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) 
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
            ks_eventq_endqueue(&e);
            break;

        case WM_KEYUP:
            e.type = ks.types.KEY_UP;
            e.key.code = (int)wParam;
            ks_eventq_endqueue(&e);
            break;

        case WM_CHAR:
            e.type = ks.types.KEY_CHAR;
            e.key.character = (int)wParam;
            ks_eventq_endqueue(&e);
            break;

        default: 
            lRet = DefWindowProc (hWnd, uMsg, wParam, lParam); 
            break; 
    } 

    return lRet; 
}

static int create_window()
{
    WNDCLASS wndclass = {0}; 
    DWORD    wStyle   = 0;
    RECT     windowRect;
    HINSTANCE hInstance = GetModuleHandle(NULL);

    wndclass.style         = CS_OWNDC;
    wndclass.lpfnWndProc   = (WNDPROC)WindowProc; 
    wndclass.hInstance     = hInstance; 
    wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH); 
    wndclass.lpszClassName = "spark"; 

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

    sys->hwnd = CreateWindow( "spark", ks_director_instance()->title,
        wStyle, 0, 0,
        windowRect.right - windowRect.left,
        windowRect.bottom - windowRect.top,
        NULL, NULL, hInstance, NULL);

    if (sys->hwnd == NULL)
        return FALSE;

    ShowWindow(sys->hwnd, TRUE);

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

    ks_graphics_init(container);
    ks_eventq_init(container);
}

KS_API ks_system_t* ks_system_instance()
{
    return (ks_system_t*)sys;
}
