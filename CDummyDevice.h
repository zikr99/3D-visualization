#ifndef __CDUMMYDEVICE_H__
#define __CDUMMYDEVICE_H__

#include "CMeDevice.h"

class CDummyDevice: public CHolderDevice
{
private:
     static bool m_registered;

     HWND m_hwnd;
     int m_width;
     int m_height;
     int m_bits;
     bool m_fullscreen;
     bool m_resized;
     CMeDevice *m_parentdevice;

     bool m_firstrun;

     friend LRESULT APIENTRY CDummyDeviceWndProc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam);
     bool RegisterCDummyDeviceWindow(HINSTANCE hinst);

     bool BeginFullScreen(int width, int height, int bits);
     void EndFullScreen();
     bool InitializeOpenGLDriver() {}
     void ReleaseOpenGLDriver() {}
     void CreateCursorControl() {}

     bool Create();
	 void Destroy();
	 void Size(LPARAM lparam);
	 void ResizeIfNecessary();

protected:
	 virtual bool OnCreate() { return true; }
	 virtual void OnSize() {}

public:
     CDummyDevice(const char* szName, int width, int height, int bits, bool fullscreen, CMeDevice *device, CSceneManager *scenemanager,
           CEventProcessor *eventprocessor = NULL);
     virtual ~CDummyDevice();
     virtual void closeDevice();

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
     virtual CMeDevice* getParentDevice() { return m_parentdevice; }

};

#endif
