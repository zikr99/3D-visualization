#define WIN32_MEAN_AND_LEAN
#define WIN32_EXTRA_LEAN
#define DUMMYWNDCLASSNAME "CDummyDeviceWindow"

#include "CDummyDevice.h"

bool CDummyDevice::m_registered = FALSE;

LRESULT APIENTRY CDummyDeviceWndProc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
     CDummyDevice *dev = (CDummyDevice*)GetWindowLong(hwnd, GWL_USERDATA);

     if ((dev == NULL) && (umsg != WM_CREATE))
		return DefWindowProc(hwnd, umsg, wparam, lparam);

     switch (umsg)
     {
     case WM_CREATE:
          dev = (CDummyDevice*)(((LPCREATESTRUCT)lparam)->lpCreateParams);
		  SetWindowLong(hwnd, GWL_USERDATA, (LONG)dev);
          return dev->Create();
     case WM_PAINT:
          {
               PAINTSTRUCT ps;
               HDC hdc = BeginPaint(hwnd, &ps);

               HBITMAP hBitmap = NULL;
               int numbytes = dev->m_parentdevice->getWidth()*dev->m_parentdevice->getHeight()*3;

               BITMAPINFOHEADER bmih;
               bmih.biSize     = sizeof(BITMAPINFOHEADER);
               bmih.biWidth    = dev->getWidth();
               bmih.biHeight   = dev->getHeight();
               bmih.biPlanes   = 1;
               bmih.biBitCount = 24;
               bmih.biCompression  = BI_RGB;
               bmih.biSizeImage    = 0;
               bmih.biXPelsPerMeter    =   10;
               bmih.biYPelsPerMeter    =   10;
               bmih.biClrUsed    = 0;
               bmih.biClrImportant = 0;

               BITMAPINFO dbmi;
               ZeroMemory(&dbmi, sizeof(dbmi));
               dbmi.bmiHeader = bmih;
               dbmi.bmiColors->rgbBlue = 0;
               dbmi.bmiColors->rgbGreen = 0;
               dbmi.bmiColors->rgbRed = 0;
               dbmi.bmiColors->rgbReserved = 0;

               void* bits = (void*)&(dev->m_parentdevice->m_pixels[0]);

               // Create DIB
               hBitmap = CreateDIBSection(hdc, &dbmi, DIB_RGB_COLORS, &bits, NULL, 0);

               if (hBitmap == NULL)
               {
                   MessageBox(NULL, "Could not load the desired image image", "Error", MB_OK);
                   return false;
               }

               memcpy(bits, dev->m_parentdevice->m_pixels, numbytes);

               HDC hdcMem = CreateCompatibleDC(hdc);
               HGDIOBJ oldBitmap = SelectObject(hdcMem, hBitmap);

               BITMAP bitmap;
               GetObject(hBitmap, sizeof(bitmap), &bitmap);
               BitBlt(hdc, 0, 0, bitmap.bmWidth, bitmap.bmHeight, hdcMem, 0, 0, SRCCOPY);

               SelectObject(hdcMem, oldBitmap);
               DeleteDC(hdcMem);

               DeleteObject(hBitmap);

               EndPaint(hwnd, &ps);
          }
          return 0;
     /*case WM_ERASEBKGND:
          return 0;
     case WM_SETCURSOR:
          if (!dev->getCursorControl()->isVisible())
          {
			SetCursor(NULL);
			return 0;
          }
          break;
     case WM_CLOSE:
          break;*/
     case WM_DESTROY:
          SetWindowLong(hwnd, GWL_USERDATA, (LONG)NULL);
          dev->Destroy();
          return 0;
     /*case WM_ACTIVATEAPP:
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
          return 0;*/
     default:
          break;
     }

     return DefWindowProc(hwnd, umsg, wparam, lparam);
}

CDummyDevice::CDummyDevice(const char* szName, int width, int height, int bits, bool fullscreen, CMeDevice *device,
     CSceneManager *scenemanager, CEventProcessor *eventprocessor): CHolderDevice(), m_hwnd(0),
     m_width(width), m_height(height), m_bits(bits), m_fullscreen(FALSE), m_parentdevice(device),
     m_resized(FALSE), m_firstrun(TRUE)
{
     HINSTANCE hinst = (HINSTANCE)GetModuleHandle(NULL);

     if (!RegisterCDummyDeviceWindow(hinst))
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

     m_hwnd = CreateWindowEx(dwExStyle, DUMMYWNDCLASSNAME, szName, dwStyle | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
            0, 0, wwidth, wheight, NULL, NULL, hinst, (void*)this);

     if (m_hwnd == NULL)
     {
        MessageBox(NULL, "Cannot create device window.", "Error", MB_OK | MB_ICONEXCLAMATION);
        return;
     }

     ShowWindow(m_hwnd, SW_SHOW);
     //AlignWindow();
	 UpdateWindow(m_hwnd);

     m_scenemanager = scenemanager;
}

CDummyDevice::~CDummyDevice()
{
     closeDevice();

     EndFullScreen();     //
}

bool CDummyDevice::RegisterCDummyDeviceWindow(HINSTANCE hinst)
{
     if (m_registered)
        return TRUE;

     WNDCLASSEX wcx;

     wcx.cbSize                  = sizeof(WNDCLASSEX);
     wcx.style                   = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
     wcx.lpfnWndProc             = CDummyDeviceWndProc;
     wcx.cbClsExtra              = 0;
     wcx.cbWndExtra              = 0;
     wcx.hInstance               = hinst;
     wcx.hIcon                   = NULL;
     wcx.hIconSm                 = NULL;
     wcx.hCursor                 = LoadCursor(NULL, IDC_ARROW);
     wcx.hbrBackground           = (HBRUSH)(COLOR_WINDOW + 1);
     wcx.lpszMenuName            = NULL;
     wcx.lpszClassName           = DUMMYWNDCLASSNAME;

     return (m_registered = RegisterClassEx(&wcx));
}

bool CDummyDevice::BeginFullScreen(int width, int height, int bits)
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

void CDummyDevice::EndFullScreen()
{
     ChangeDisplaySettings(NULL, 0);
}

void CDummyDevice::closeDevice()
{
     if (m_hwnd != NULL)
     {
        /*list<HWND>::Iterator it = cmeDevices.begin();
        for (; it != cmeDevices.end(); ++it)
            if ((*it) == m_hwnd)
            {
			   cmeDevices.erase(it);
	  	       break;
            }*/

        DestroyWindow(m_hwnd);
     }
}

void CDummyDevice::AlignWindow()
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

bool CDummyDevice::setResizeAble(bool resize)
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

bool CDummyDevice::beginScene(bool zbuffer, bool backbuffer, float r, float g, float b, float a)
{
    m_parentdevice->beginScene(zbuffer, backbuffer, r, g, b);

	return TRUE;
}

bool CDummyDevice::endScene()
{
    glReadPixels(0, 0, m_parentdevice->getWidth(), m_parentdevice->getHeight(), GL_BGR, GL_UNSIGNED_BYTE,
        m_parentdevice->m_pixels);

    InvalidateRect(m_hwnd, NULL, false);
    UpdateWindow(m_hwnd);

    return TRUE;
}

bool CDummyDevice::Create()
{
     return !OnCreate();
}

void CDummyDevice::Destroy()
{
     closeDevice();
}

void CDummyDevice::Size(LPARAM lparam)
{
     RECT clientSize;

     GetClientRect(m_hwnd, &clientSize);
     m_width = clientSize.right - clientSize.left;
     m_height = clientSize.bottom - clientSize.top;

     OnSize();
     m_resized = TRUE;
}

void CDummyDevice::ResizeIfNecessary()
{
	if (!m_resized)
		return;

	m_resized = FALSE;
}


