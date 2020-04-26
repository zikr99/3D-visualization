#ifndef __CBMP24IMAGE_H__
#define __CBMP24IMAGE_H__

#include "CImage.h"

class CBmp24Image: public CImage
{     
protected:
     virtual bool LoadImage(char *filename);
          
public: 
     CBmp24Image(char *filename = NULL);
     virtual ~CBmp24Image() {}
       
};

#endif
