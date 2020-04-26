#ifndef __CBMP24GRAYSCALEIMAGE_H__
#define __CBMP24GRAYSCALEIMAGE_H__

#include "CBmp24Image.h"
#include <math.h>

class CBmp24GrayscaleImage: public CBmp24Image
{
public:
     CBmp24GrayscaleImage(char *filename = NULL)
     {
           m_colorformat = GL_LUMINANCE;
           m_width = 0;
           m_height = 0;
           m_isloaded = FALSE;
           m_data = new byte [1];
           
           if (filename)
                  loadFile(filename);                          
     }
     
     virtual ~CBmp24GrayscaleImage() {}
     
     virtual bool LoadImage(char *filename)
     {  
           int gsize;
           byte *bitmapGrayscale;
           byte r, g, b;
           
           m_colorformat = GL_LUMINANCE;  
           
           if (CBmp24Image::LoadImage(filename))
           {
                  gsize = m_width*m_height;
                  bitmapGrayscale = (byte*)malloc(gsize);
                  if (bitmapGrayscale == NULL)
                        return FALSE;
                        
                  for (int i = 0; i < gsize; i++)
                  {
                        r = m_data[i*3 + 0];
                        g = m_data[i*3 + 1];
                        b = m_data[i*3 + 2];
                        bitmapGrayscale[i] = (byte)sqrt((r*r + g*g + b*b)/3);
                  }             
                  
                  delete [] m_data;
                  m_data = bitmapGrayscale; 
                  m_colorformat = GL_LUMINANCE;
                  return TRUE;                     
           }
           
           return FALSE;
     }
      
};

#endif
