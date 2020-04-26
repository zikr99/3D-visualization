#ifndef __CCURSORCONTROL_H__
#define __CCURSORCONTROL_H__

#include <windows.h>
#include "CVector2.h"

class CCursorControl
{
private:
      HWND m_hwnd;
      int m_borderx, m_bordery, m_borderybottom;
      bool m_isvisible;
      int m_cursorposx, m_cursorposy;
      int m_width, m_height;

      void UpdateInternalCursorPosition()
      {
           POINT p;
           RECT rect;

           GetCursorPos(&p);
           GetWindowRect(m_hwnd, &rect);

           m_cursorposx = p.x - rect.left - m_borderx;
           m_cursorposy = p.y - rect.top - m_bordery;

           m_width = rect.right - rect.left - m_borderx - m_borderx;
           m_height = rect.bottom - rect.top - m_bordery - m_borderybottom;
      }

public:
      CCursorControl(HWND hwnd, bool fullscreen): m_hwnd(hwnd), m_borderx(0),
           m_bordery(0), m_borderybottom(0), m_isvisible(TRUE)
      {
           if (!fullscreen)
           {
              RECT wrect, crect;
              GetWindowRect(m_hwnd, &wrect);
              GetClientRect(m_hwnd, &crect);

              m_borderx = (wrect.right - wrect.left - crect.right + crect.left)/2;
              m_borderybottom = m_borderx;
              m_bordery = wrect.bottom - wrect.top - crect.bottom + crect.top - m_borderybottom;
           }
      }

      virtual void setPosition(int x, int y)
      {
           RECT rect;
           if (GetWindowRect(m_hwnd, &rect))
              SetCursorPos(x + rect.left + m_borderx, y + rect.top + m_bordery);
      }

      virtual void setPosition(float x, float y)
      {
           RECT rect;
           int width, height;

           if (GetWindowRect(m_hwnd, &rect))
           {
              width = rect.right - rect.left - m_borderx - m_borderx;
              height = rect.bottom - rect.top - m_bordery - m_borderybottom;

              SetCursorPos((int)(x*width) + rect.left + m_borderx, (int)(y*height) + rect.top + m_bordery);
           }
      }

      virtual int getPositionX()
      {
           UpdateInternalCursorPosition();
           return m_cursorposx;
      }

      virtual int getPositionY()
      {
           UpdateInternalCursorPosition();
           return m_cursorposy;
      }

      virtual void getPosition(CVector2<int> &vec)
      {
           UpdateInternalCursorPosition();
           vec.x = m_cursorposx;
           vec.y = m_cursorposy;
      }

      virtual void getRelativePosition(CVector2<float> &vec)
      {
           UpdateInternalCursorPosition();
           vec.x = (float)m_cursorposx/m_width;
           vec.y = (float)m_cursorposy/m_height;
      }

      virtual void setVisible(bool visible) { m_isvisible = visible; }
      virtual bool isVisible() { return m_isvisible; }

};

#endif
