#include "CBmp24Image.h"
#include <stdio.h>

#define BITMAP_ID 0x4D42

CBmp24Image::CBmp24Image(char *filename): CImage()
{
     if (filename)
          loadFile(filename);                                 
}

bool CBmp24Image::LoadImage(char *filename)
{
     FILE *filePtr;							
     BITMAPFILEHEADER bitmapFileHeader;
     BITMAPINFOHEADER bitmapInfoHeader;		
     byte *bitmapImage;
     byte *bitmapGrayscale;
     int numread;			
     unsigned int imageIdx = 0;		
     byte tempRGB;			

     filePtr = fopen(filename, "rb");
     if (filePtr == NULL)
          return FALSE;

     fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);
     if (bitmapFileHeader.bfType != BITMAP_ID)
     {
          fclose(filePtr);
          return FALSE;
     }

     fread(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);
     fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);
  
     bitmapImage = (byte*)malloc(bitmapInfoHeader.biSizeImage);
     if (bitmapImage == NULL)
     {
          fclose(filePtr);
          return FALSE;
     }

     numread = fread(bitmapImage, 1, bitmapInfoHeader.biSizeImage, filePtr);
     if (numread < bitmapInfoHeader.biSizeImage)
     {
          free(bitmapImage);              
          fclose(filePtr);
          return FALSE;
     }

     for (imageIdx = 0; imageIdx < bitmapInfoHeader.biSizeImage; imageIdx += 3)
     {
          tempRGB = bitmapImage[imageIdx];
          bitmapImage[imageIdx] = bitmapImage[imageIdx + 2];
          bitmapImage[imageIdx + 2] = tempRGB;
     }

     fclose(filePtr);
     
     delete [] m_data;
     m_data = bitmapImage;
     m_width = bitmapInfoHeader.biWidth;
     m_height = bitmapInfoHeader.biHeight;
     m_colorformat = GL_RGB;
  
     return TRUE;
}
