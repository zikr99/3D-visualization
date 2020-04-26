#include "CTexture.h"

CTexture::CTexture(): m_colorformat(GL_RGBA), m_orwidth(128), m_orheight(128), m_width(128), 
     m_height(128) 
{
     glGenTextures(1, &m_texId);
     setParameters(GL_NEAREST_MIPMAP_LINEAR, GL_NEAREST_MIPMAP_LINEAR, GL_REPEAT, GL_REPEAT);
     
     byte rgba[128][128][4];
     
     for (int i = 0; i < 128; i++)
          for (int j = 0; j < 128; j++)
               for (int k = 0; k < 4; k++)
                    rgba[i][j][k] = 255;
     
     buildTexture((byte*)rgba, 128, 128, GL_RGBA);
}

int CTexture::getScaledSize(int size)
{   
     int fsize = 1;  
       
     while (fsize < size)
          fsize *= 2;
  
     return fsize;
}

CVector2<int> CTexture::getDimension()
{
     CVector2<int> dim;
     dim.x = m_width;
     dim.y = m_height;
     
     return dim;
}

CVector2<int> CTexture::getOriginalDimension()
{
     CVector2<int> dim;
     dim.x = m_orwidth;
     dim.y = m_orheight;
     
     return dim;
}

void CTexture::buildTexture(const byte *data, int width, int height, GLenum colorformat)
{
     int mxs, ss;
     int numbyte;
     byte *scaledImageData;
     
     glBindTexture(GL_TEXTURE_2D, m_texId);
     GLenum t_colorformat = colorformat;
     
     mxs = width;
     if (mxs < height) mxs = height;
     ss = getScaledSize(mxs);
     
     if (t_colorformat == GL_RGB) numbyte = 3;
     else if (t_colorformat == GL_RGBA) numbyte = 4;
     else if (t_colorformat == GL_LUMINANCE) numbyte = 1;
     
     if ((ss != width) || (ss != height))
     { 
            scaledImageData = (byte*)malloc(ss*ss*numbyte);
            if (scaledImageData == NULL)
                 return;
            
            gluScaleImage(t_colorformat, width, height, GL_UNSIGNED_BYTE, 
                 data, ss, ss, GL_UNSIGNED_BYTE, scaledImageData);    
                 
            glTexImage2D(GL_TEXTURE_2D, 0, numbyte, ss, ss, 0, t_colorformat, 
                 GL_UNSIGNED_BYTE, scaledImageData);     
            gluBuild2DMipmaps(GL_TEXTURE_2D, t_colorformat, ss, ss, 
                 t_colorformat, GL_UNSIGNED_BYTE, scaledImageData);
            
            free(scaledImageData);
     }
     else
     {
            glTexImage2D(GL_TEXTURE_2D, 0, numbyte, ss, ss, 0, t_colorformat, 
                 GL_UNSIGNED_BYTE, data);
            gluBuild2DMipmaps(GL_TEXTURE_2D, t_colorformat, ss, ss, 
                 t_colorformat, GL_UNSIGNED_BYTE, data);
     }
     
     m_colorformat = t_colorformat;
     m_orwidth = width;
     m_orheight = height;
     m_width = ss;
     m_height = ss;   
}

void CTexture::buildTexture(CImage *img)
{
     buildTexture(img->getImageData(), img->getWidth(), img->getHeight(), img->getColorFormat());
}

void CTexture::buildTexture(CImage **img, int imgnum)
{
     int cw, ch, cs, ms;
     
     glBindTexture(GL_TEXTURE_2D, m_texId);
     GLenum t_colorformat = img[0]->getColorFormat();
     
     cw = img[0]->getWidth();
     ch = img[0]->getHeight();
     cs = (int)pow(2, imgnum - 1);
           
     if ((cw == cs) && (ch == cs))
     {
           ms = (int)pow(2, imgnum);
             
           for (int i = 0; i < imgnum; i++)
           {
                 ms /= 2;
                 glTexImage2D(GL_TEXTURE_2D, i, t_colorformat, ms, ms, 0, 
                       t_colorformat, GL_UNSIGNED_BYTE, img[i]->getImageData());
           }
           
           if ((m_minfilter == GL_NEAREST) || (m_minfilter == GL_LINEAR))
           {   
                 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
                 m_minfilter = GL_NEAREST_MIPMAP_NEAREST;
           }
           
           m_colorformat = t_colorformat;
           m_orwidth = cs;
           m_orheight = cs;
           m_width = cs;
           m_height = cs;   
     }
     else
     {
           buildTexture(img[0]);
     }
}

bool CTexture::buildTextureBMP24(char *filename)
{
     CImage *img;
     char *extStr;
     
     extStr = strchr(filename, '.');
     extStr++;
     
     if ((strcmp(extStr, "BMP") == 0) || (strcmp(extStr, "bmp") == 0))
     {            
           img = new CBmp24Image(filename);
           
           if (img->isLoaded()) 
           {
                 int wp = img->getWidth();
                 int hp = img->getHeight();
                                
                 int nump = wp*hp;
                 byte *data = new byte [nump*4];
                 const byte *imdata = img->getImageData();
                 
                 for (int i = 0; i < nump; i++)
                 {
                       data[i*4] = imdata[i*3];
                       data[i*4 + 1] = imdata[i*3 + 1];
                       data[i*4 + 2] = imdata[i*3 + 2];
                       data[i*4 + 3] = 255;
                 }
                 
                 buildTexture(data, wp, hp, GL_RGBA);
                 
                 delete [] data;
                 delete img;
                 return TRUE;
           }
           else
           {
                 delete img;
                 return FALSE;
           }                   
     } 
     
     return FALSE;
}

bool CTexture::buildTextureBMP24Grayscale(char *filename)
{
     CImage *img;
     char *extStr;
     
     extStr = strchr(filename, '.');
     extStr++;

     if ((strcmp(extStr, "BMP") == 0) || (strcmp(extStr, "bmp") == 0))
     {
           img = new CBmp24GrayscaleImage(filename);
           
           if (img->isLoaded()) 
           {
                 buildTexture(img);
                 delete img;
                 return TRUE;
           }
           else
           {
                 delete img;
                 return FALSE;
           }              
     }
     
     return FALSE;
}

void CTexture::setParameters(GLint minFilter, GLint magFilter, GLint wrapS, GLint wrapT)
{
     glBindTexture(GL_TEXTURE_2D, m_texId);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);
     
     m_minfilter = minFilter;
     m_magfilter = magFilter;
     m_wraps = wrapS;
     m_wrapt = wrapT;
}

void CTexture::activate()
{          
     glBindTexture(GL_TEXTURE_2D, m_texId);
}
