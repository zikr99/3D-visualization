#ifndef __CTIMER_H__
#define __CTIMER_H__

#include <windows.h>

class CTimer
{
private:
      LARGE_INTEGER m_startTime;
      LARGE_INTEGER m_ticksPerSecond;
  
public:
      CTimer() {}
      ~CTimer() {}

      bool Init()
      {
          if (!QueryPerformanceFrequency(&m_ticksPerSecond))
             return false;
          else
          {
             QueryPerformanceCounter(&m_startTime);
             return true;
          }
      } 

      float GetElapsedSeconds()
      {
          static LARGE_INTEGER s_lastTime = m_startTime;
          
          LARGE_INTEGER currentTime;
          QueryPerformanceCounter(&currentTime);

          float seconds = ((float)currentTime.QuadPart - (float)s_lastTime.QuadPart)/(float)m_ticksPerSecond.QuadPart;

          s_lastTime = currentTime;
          return seconds;
      }

      float GetFPS(unsigned long elapsedFrames = 1)
      {
          static LARGE_INTEGER s_lastTime = m_startTime;
          
          LARGE_INTEGER currentTime;
          QueryPerformanceCounter(&currentTime);

          float fps = (float)elapsedFrames*(float)m_ticksPerSecond.QuadPart/((float)currentTime.QuadPart - (float)s_lastTime.QuadPart);

          s_lastTime = currentTime;
          return fps;
      }

      /***************************************************************************
      It is assumed that if used, this function will be called every frame. 
      The value returned is the instantaneous fps, which will be <= targetFPS
      with no lower boundary.
      ***************************************************************************/

      float LockFPS(unsigned char targetFPS)
      {
          static LARGE_INTEGER s_lastTime = m_startTime;
          
          if (targetFPS == 0)
             targetFPS = 1;

          LARGE_INTEGER currentTime;
          float fps;

          do 
          {
             QueryPerformanceCounter(&currentTime);
             fps = (float)m_ticksPerSecond.QuadPart/((float)(currentTime.QuadPart - s_lastTime.QuadPart));
          } while (fps > (float)targetFPS);

          s_lastTime = m_startTime;
          return fps;
      }
      
};

#endif 
