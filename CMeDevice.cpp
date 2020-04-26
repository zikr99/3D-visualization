#define WIN32_MEAN_AND_LEAN
#define WIN32_EXTRA_LEAN
#define WNDCLASSNAME "CMeDeviceWindow"

#include "CMeDevice.h"

bool CMeDevice::m_registered = FALSE;
list<HWND> CMeDevice::cmeDevices;

LRESULT APIENTRY CMeDeviceWndProc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
     CMeDevice *dev = (CMeDevice*)GetWindowLong(hwnd, GWL_USERDATA);

     if ((dev == NULL) && (umsg != WM_CREATE))
		return DefWindowProc(hwnd, umsg, wparam, lparam);

     switch (umsg)
     {
     case WM_CREATE:
          dev = (CMeDevice*)(((LPCREATESTRUCT)lparam)->lpCreateParams);
		  SetWindowLong(hwnd, GWL_USERDATA, (LONG)dev);
          return dev->Create();
     case WM_PAINT:
          {
               PAINTSTRUCT ps;
               HDC hdc = BeginPaint(hwnd, &ps);
               EndPaint(hwnd, &ps);
          }
          return 0;
     case WM_ERASEBKGND:
          return 0;
     case WM_SETCURSOR:
          if (!dev->getCursorControl()->isVisible())
          {
			SetCursor(NULL);
			return 0;
          }
          break;
     case WM_CLOSE:
          break;
     case WM_DESTROY:
          SetWindowLong(hwnd, GWL_USERDATA, (LONG)NULL);
          dev->Destroy();
          return 0;
     case WM_ACTIVATEAPP:
          if (wparam) {} else {}
          break;
     case WM_SIZE:
          if (wparam != SIZE_MINIMIZED)
				dev->Size(lparam);
		  return 0;
     case WM_SYSCOMMAND:
          if (wparam == SC_SCREENSAVE || wparam == SC_MONITORPOWER)    //prevent screensaver and power
                return 0;
          break;
     case WM_LBUTTONDOWN:
     case WM_LBUTTONUP:
     case WM_RBUTTONDOWN:
     case WM_RBUTTONUP:
     case WM_MBUTTONDOWN:
     case WM_MBUTTONUP:
     case WM_MOUSEMOVE:
     case WM_KEYDOWN:
     case WM_KEYUP:
          dev->ProcessUserInputWindowEvent(umsg, wparam, lparam);
          return 0;
     default:
          break;
     }

     return DefWindowProc(hwnd, umsg, wparam, lparam);
}

CMeDevice::CMeDevice(const char* szName, int width, int height, int bits, bool fullscreen,
     CEventProcessor *eventprocessor): CHolderDevice(), m_hwnd(0), m_hdc(0), m_hglrc(0),
     m_width(width), m_height(height), m_bits(bits), m_fullscreen(FALSE), m_initialized(FALSE),
     m_resized(FALSE), m_firstrun(TRUE)
{
     HINSTANCE hinst = (HINSTANCE)GetModuleHandle(NULL);

     if (!RegisterCMeDeviceWindow(hinst))
     {
        MessageBox(NULL, "Cannot register device window.", "Error", MB_OK | MB_ICONEXCLAMATION);
        return;
     }

     RECT clientSize;
     DWORD dwStyle, dwExStyle;
     int wwidth, wheight;

     if (fullscreen)
     {
        clientSize.left          = (long)0;
        clientSize.top           = (long)0;
        clientSize.right         = (long)width;
        clientSize.bottom        = (long)height;

        dwStyle = WS_CAPTION; //WS_POPUP;
        dwExStyle = WS_EX_APPWINDOW;

        AdjustWindowRectEx(&clientSize, dwStyle, FALSE, dwExStyle);
        wwidth = clientSize.right - clientSize.left;
        wheight = clientSize.bottom - clientSize.top;

        if (!(m_fullscreen = BeginFullScreen(wwidth, wheight, bits)))
           MessageBox(NULL, "Cannot change display setting.", "Warning", MB_OK | MB_ICONEXCLAMATION);
     }

     if (!m_fullscreen)
     {
        clientSize.left          = (long)0;
        clientSize.top           = (long)0;
        clientSize.right         = (long)width;
        clientSize.bottom        = (long)height;

        dwStyle = WS_OVERLAPPEDWINDOW;
        dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;

        AdjustWindowRectEx(&clientSize, dwStyle, FALSE, dwExStyle);
        wwidth = clientSize.right - clientSize.left;
        wheight = clientSize.bottom - clientSize.top;
     }

     m_hwnd = CreateWindowEx(dwExStyle, WNDCLASSNAME, szName, dwStyle | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
            0, 0, wwidth, wheight, NULL, NULL, hinst, (void*)this);

     if (m_hwnd == NULL)
     {
        MessageBox(NULL, "Cannot create device window.", "Error", MB_OK | MB_ICONEXCLAMATION);
        return;
     }

     ShowWindow(m_hwnd, SW_SHOW);
     //AlignWindow();
	 UpdateWindow(m_hwnd);

	 CreateCursorControl();
	 m_initialized = InitializeOpenGLDriver();
     CreateSceneManager();
     setEventProcessor(eventprocessor);
	 cmeDevices.push_back(m_hwnd);

	 SetActiveWindow(m_hwnd);
	 SetForegroundWindow(m_hwnd);

     int numbytes = getWidth()*getHeight()*3;
     m_pixels = new GLubyte [numbytes];
}

CMeDevice::~CMeDevice()
{
     delete [] m_pixels;

     closeDevice();

     EndFullScreen();     //
}

bool CMeDevice::RegisterCMeDeviceWindow(HINSTANCE hinst)
{
     if (m_registered)
        return TRUE;

     WNDCLASSEX wcx;

     wcx.cbSize                  = sizeof(WNDCLASSEX);
     wcx.style                   = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
     wcx.lpfnWndProc             = CMeDeviceWndProc;
     wcx.cbClsExtra              = 0;
     wcx.cbWndExtra              = 0;
     wcx.hInstance               = hinst;
     wcx.hIcon                   = NULL;
     wcx.hIconSm                 = NULL;
     wcx.hCursor                 = LoadCursor(NULL, IDC_ARROW);
     wcx.hbrBackground           = (HBRUSH)(COLOR_WINDOW + 1);
     wcx.lpszMenuName            = NULL;
     wcx.lpszClassName           = WNDCLASSNAME;

     return (m_registered = RegisterClassEx(&wcx));
}

bool CMeDevice::BeginFullScreen(int width, int height, int bits)
{
	DEVMODE dm;
	memset(&dm, 0, sizeof(dm));
	dm.dmSize           = sizeof(dm);
	dm.dmFields         = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL;
	dm.dmPelsWidth      = width;
	dm.dmPelsHeight     = height;
	dm.dmBitsPerPel     = bits;

	return (ChangeDisplaySettings(&dm, CDS_FULLSCREEN) == DISP_CHANGE_SUCCESSFUL);
}

void CMeDevice::EndFullScreen()
{
     ChangeDisplaySettings(NULL, 0);
}

bool CMeDevice::InitializeOpenGLDriver()
{
     HDC hdc;
     GLuint PixelFormat;
     HGLRC hrc;
     float globalambient[] = {0.8f, 0.8f, 0.8f, 1.0f};

     static	PIXELFORMATDESCRIPTOR pfd =	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW |
		PFD_SUPPORT_OPENGL |
		PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		m_bits,
		0, 0, 0, 0, 0, 0,			// color bits ignored
		0,							// no alpha buffer
		0,							// shift bit ignored
		0,							// no accumulation buffer
		0, 0, 0, 0,					// accumulation bits ignored
		16,							// 16bit z-buffer (depth buffer)
		0,                    		// no stencil buffer
		0,							// no auxiliary buffer
		PFD_MAIN_PLANE,				// main drawing layer
		0,							// reserved
		0, 0, 0						// layer masks ignored
	};

	if (!(hdc = GetDC(m_hwnd)))
	{
	   MessageBox(NULL, "Cannot create a GL device context", "Error", MB_OK | MB_ICONEXCLAMATION);
	   return FALSE;
	}

	if (!(PixelFormat = ChoosePixelFormat(hdc, &pfd)))
	{
	   MessageBox(NULL, "Cannot find a suitable pixelformat.", "Error", MB_OK | MB_ICONEXCLAMATION);
       ReleaseDC(m_hwnd, hdc);
       return FALSE;
	}

	if(!SetPixelFormat(hdc, PixelFormat, &pfd))
	{
       MessageBox(NULL, "Cannot set the pixel format.", "Error", MB_OK | MB_ICONEXCLAMATION);
       ReleaseDC(m_hwnd, hdc);
       return FALSE;
	}

    if (!(hrc = wglCreateContext(hdc)))
    {
	   MessageBox(NULL, "Cannot create a GL rendering context.", "Error", MB_OK | MB_ICONEXCLAMATION);
       ReleaseDC(m_hwnd, hdc);
       return FALSE;
	}

	if (!wglMakeCurrent(hdc, hrc))
	{
	   MessageBox(NULL, "Cannot activate GL rendering context.", "Warning", MB_OK | MB_ICONEXCLAMATION);
	   m_hdc = hdc;
       m_hglrc = hrc;
	   return TRUE;
    }

    glViewport(0, 0, m_width, m_height);
    glEnable(GL_DEPTH_TEST);
    glClearDepth(1.0f);
    glDepthFunc(GL_LEQUAL);
	glShadeModel(GL_SMOOTH);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalambient);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	if (multitextureSupported()) bindMultitextureProcs();
	if (multidrawSupported()) bindMultidrawProcs();

    m_hdc = hdc;
    m_hglrc = hrc;
    return TRUE;
}

void CMeDevice::ReleaseOpenGLDriver()
{
    if (m_hglrc)
	{
		if (!wglMakeCurrent(0, 0))
			MessageBox(NULL, "Release of dc and rc failed.", "Error",  MB_OK | MB_ICONEXCLAMATION);

		if (!wglDeleteContext(m_hglrc))
			MessageBox(NULL, "Release of rendering context failed.",  "Warning",  MB_OK | MB_ICONEXCLAMATION);

		m_hglrc = 0;
	}

	if (m_hdc)
	{
		ReleaseDC(m_hwnd, m_hdc);
		m_hdc = 0;
    }
}

void CMeDevice::closeDevice()
{
     if (m_initialized)
        ReleaseOpenGLDriver();

     m_initialized = FALSE;

     if (m_hwnd != NULL)
     {
        list<HWND>::Iterator it = cmeDevices.begin();
        for (; it != cmeDevices.end(); ++it)
            if ((*it) == m_hwnd)
            {
			   cmeDevices.erase(it);
	  	       break;
            }

        DestroyWindow(m_hwnd);
     }
}

bool CMeDevice::run()
{
     MSG msg;
     bool quit = FALSE;

     while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
     {
           TranslateMessage(&msg);
           DispatchMessage(&msg);

           if (msg.message == WM_QUIT)
              quit = TRUE;

           if (CMeDevice::cmeDevices.getSize() < 1)
              quit = TRUE;
     }

     if (!quit)
           ResizeIfNecessary();

     if (m_firstrun)
     {
           m_timer->Init();
           m_firstrun = FALSE;
     }

     return !quit;
}

void CMeDevice::AlignWindow()
{
    RECT rect;
    GetWindowRect(m_hwnd, &rect);

    POINT pt;
    pt.x = 0;
    pt.y = 0;

    ClientToScreen(m_hwnd, &pt);

    int posx = rect.left - pt.x;
    int posy = rect.top - pt.y;

    SetWindowPos(m_hwnd, HWND_TOP, posx, posy, 0, 0, SWP_NOSIZE);
}

bool CMeDevice::setResizeAble(bool resize)
{
	if (m_fullscreen)
		return (resize == FALSE);

	LONG style = WS_POPUP;

	if (!resize)
		style = WS_SYSMENU | WS_BORDER | WS_CAPTION | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
	else
		style = WS_THICKFRAME | WS_SYSMENU | WS_CAPTION | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_MAXIMIZEBOX;

	if (!SetWindowLong(m_hwnd, GWL_STYLE, style))
	{
		MessageBox(NULL, "Could not change window style.", "Error", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
    }

    RECT clientSize;
	clientSize.top = 0;
	clientSize.left = 0;
	clientSize.right = m_width;
	clientSize.bottom = m_height;

	AdjustWindowRect(&clientSize, style, FALSE);
    int wwidth = clientSize.right - clientSize.left;
	int wheight = clientSize.bottom - clientSize.top;

    SetWindowPos(m_hwnd, HWND_TOP, 0, 0, wwidth, wheight,
		SWP_FRAMECHANGED | SWP_NOMOVE | SWP_SHOWWINDOW);

	return TRUE;
}

bool CMeDevice::beginScene(bool zbuffer, bool backbuffer, float r, float g, float b, float a)
{
    if (!m_initialized)
        return FALSE;

    if (!wglMakeCurrent(m_hdc, m_hglrc))
        return FALSE;

	GLbitfield mask = 0;

	if (backbuffer)
	{
		glClearColor(r, g, b, a);
		mask |= GL_COLOR_BUFFER_BIT;
	}

	if (zbuffer)
	{
		glDepthMask(GL_TRUE);
		mask |= GL_DEPTH_BUFFER_BIT;
	}

	glClear(mask);
	return TRUE;
}

bool CMeDevice::endScene()
{
    if (!m_initialized)
        return FALSE;

    return SwapBuffers(m_hdc) == TRUE;
}

bool CMeDevice::Create()
{
     return !OnCreate();
}

void CMeDevice::Destroy()
{
     closeDevice();
}

void CMeDevice::Size(LPARAM lparam)
{
     RECT clientSize;

     GetClientRect(m_hwnd, &clientSize);
     m_width = clientSize.right - clientSize.left;
     m_height = clientSize.bottom - clientSize.top;

     OnSize();
     m_resized = TRUE;
}

void CMeDevice::ResizeIfNecessary()
{
	if (!m_resized)
		return;

	if (m_hglrc)
	{
		//glViewport(0, 0, m_width, m_height);

        //glMatrixMode(GL_PROJECTION);
		//glLoadIdentity();

		//float nearClip = 1.0f / 100.0f;
		//float farClip  = 256.0f;//127.0f;
        //gluPerspective(54.0f, (float)width/(float)height, 0.1f, 200.0f);
        //camera must be updated

		//glMatrixMode(GL_MODELVIEW);
		//glLoadIdentity();
	}

	m_resized = FALSE;
}


