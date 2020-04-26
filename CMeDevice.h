#ifndef __CMEDEVICE_H__
#define __CMEDEVICE_H__

#include "CHolderDevice.h"

class CMeDevice: public CHolderDevice
{
private:
     static bool m_registered;

     HWND m_hwnd;
     HDC m_hdc;
     HGLRC m_hglrc;
     int m_width;
     int m_height;
     int m_bits;
     bool m_fullscreen;
     bool m_initialized;
     bool m_resized;

     bool m_firstrun;

     friend LRESULT APIENTRY CMeDeviceWndProc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam);
     bool RegisterCMeDeviceWindow(HINSTANCE hinst);

     bool BeginFullScreen(int width, int height, int bits);
     void EndFullScreen();
     bool InitializeOpenGLDriver();
     void ReleaseOpenGLDriver();
     void CreateCursorControl() { m_cursorcontrol = new CCursorControl(m_hwnd, m_fullscreen); }

     bool Create();
	 void Destroy();
	 void Size(LPARAM lparam);
	 void ResizeIfNecessary();

protected:
	 virtual bool OnCreate() { return true; }
	 virtual void OnSize() {}

public:
     static list<HWND> cmeDevices;
     GLubyte *m_pixels;

     CMeDevice(const char* szName, int width, int height, int bits, bool fullscreen,
           CEventProcessor *eventprocessor = NULL);
     virtual ~CMeDevice();
     virtual void closeDevice();

     virtual bool run();
     virtual void setEventProcessor(CEventProcessor *eventprocessor) { m_eventprocessor = eventprocessor; }

     virtual bool beginScene(bool zbuffer = true, bool backbuffer = true, float r = 0.0f,
           float g = 0.0f, float b = 0.0f, float a = 1.0f);
     virtual bool endScene();

     virtual void AlignWindow();
     bool setResizeAble(bool resize = false);
     virtual void setCaption(char *cap) { SetWindowText(m_hwnd, cap); }
     virtual void getCaption(char *cap, int caplength) { GetWindowText(m_hwnd, cap, caplength); }

     virtual int getWidth() { return m_width; }
     virtual int getHeight() { return m_height; }

};

#endif
