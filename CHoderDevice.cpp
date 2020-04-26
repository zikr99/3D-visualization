#include "CHolderDevice.h"
#include "CEventProcessor.h"

CHolderDevice::CHolderDevice(): m_scenemanager(NULL), m_cursorcontrol(NULL),
     m_eventprocessor(NULL)
{
     m_timer = new CTimer();
}

CHolderDevice::~CHolderDevice()
{
     if (m_scenemanager)
        delete m_scenemanager;
     
     if (m_cursorcontrol)
        delete m_cursorcontrol;
     
     if (m_eventprocessor)
        delete m_eventprocessor;
     
     delete m_timer;
}

bool CHolderDevice::ProcessUserInputWindowEvent(UINT umsg, WPARAM wparam, LPARAM lparam)
{
     if (m_eventprocessor)
          if (m_eventprocessor->ProcessEvent(this, umsg, wparam, lparam))
                return TRUE;  
     
     if (m_scenemanager)
          return m_scenemanager->ProcessEvent(umsg, wparam, lparam);  
     
     return FALSE;  
}
