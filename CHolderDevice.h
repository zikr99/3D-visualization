#ifndef __CHOLDERDEVICE_H__
#define __CHOLDERDEVICE_H__

#include "CSceneManager.h"

class CEventProcessor;

class CHolderDevice
{
protected:
      CSceneManager *m_scenemanager;
      CCursorControl *m_cursorcontrol;
      CEventProcessor *m_eventprocessor;
      CTimer *m_timer;

      void CreateSceneManager() { m_scenemanager = new CSceneManager(this, m_cursorcontrol); }
      virtual bool ProcessUserInputWindowEvent(UINT umsg, WPARAM wparam, LPARAM lparam);

public:
      CHolderDevice();
	  virtual ~CHolderDevice();

      virtual CSceneManager* getSceneManager() { return m_scenemanager; }
      virtual CCursorControl* getCursorControl() { return m_cursorcontrol; }
      virtual CEventProcessor* getEventProcessor() { return m_eventprocessor; }
      virtual CTimer* getTimer() { return m_timer; }

      virtual bool setResizeAble(bool resize = false) {}

      virtual void setCaption(char *cap) {}
      virtual void getCaption(char *cap, int caplength) {}

      virtual int getWidth() {}
      virtual int getHeight() {}

};

#endif
