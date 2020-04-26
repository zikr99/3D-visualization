#ifndef __CTEXTURE_H__
#define __CTEXTURE_H__

#include "CBmp24GrayscaleImage.h"

class CTexture
{
private:
     GLuint m_texId;
     GLenum m_colorformat;
     int m_orwidth, m_orheight;
     int m_width, m_height;
     GLint m_minfilter, m_magfilter;
     GLint m_wraps, m_wrapt;
     
     int getScaledSize(int size);
        
public:
     CTexture();
     
     CTexture(CImage *img) 
     { 
           CTexture();
           buildTexture(img); 
     }
     
     CTexture(CImage **img, int imgnum)
     {
           CTexture();
           buildTexture(img, imgnum);
     }
     
     virtual ~CTexture()
     {
           glDeleteTextures(1, &m_texId);  
     }  
     
     virtual GLuint getId() { return m_texId; }
     virtual int getWidth() { return m_width; }
     virtual int getHeight() { return m_height; }
     virtual CVector2<int> getDimension();
     virtual int getOriginalWidth() { return m_orwidth; }
     virtual int getOriginalHeight() { return m_orheight; }
     virtual CVector2<int> getOriginalDimension();
     virtual GLenum getColorFormat() { return m_colorformat; }
     
     virtual void buildTexture(const byte *data, int width, int height, GLenum colorformat);
     virtual void buildTexture(CImage *img);
     virtual void buildTexture(CImage **img, int imgnum);
     virtual bool buildTextureBMP24(char *filename);
     virtual bool buildTextureBMP24Grayscale(char *filename);
     virtual void setParameters(GLint minFilter, GLint magFilter, GLint wrapS, GLint wrapT);
     virtual void activate();
     
};

#endif
