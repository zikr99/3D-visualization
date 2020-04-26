#ifndef __CIMAGE_H__
#define __CIMAGE_H__

#include <windows.h>
#include <gl\glu.h>
#include "CVector2.h"

class CImage
{
protected:
     byte *m_data;
     GLenum m_colorformat;
     int m_width, m_height;
     bool m_isloaded;
     
     virtual bool LoadImage(char *filename) = 0;
          
public:
     CImage(): m_colorformat(GL_RGB), m_width(0), m_height(0), m_isloaded(FALSE)
     {
          m_data = new byte [1];   
     }
     
     virtual ~CImage()
     {
          if (m_data) delete [] m_data;
     }
     
     virtual void loadFile(char *filename) { m_isloaded = LoadImage(filename); }
     
     virtual GLenum getColorFormat() { return m_colorformat; } 
     virtual int getWidth() { return m_width; }
     virtual int getHeight() { return m_height; }
     
     virtual CVector2<int> getDimension()
     {
          CVector2<int> dim;
          dim.x = m_width;
          dim.y = m_height; 
          
          return dim;  
     }
     
     virtual const byte* getImageData() { return m_data; }
     virtual bool isLoaded() { return m_isloaded; } 
      
};

#endif
