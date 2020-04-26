#include "DriverExtension.h"
#include <windows.h>
#include <string.h>

PFNGLMULTITEXCOORD2FARBPROC glMultiTexCoord2fARB = NULL;
PFNGLACTIVETEXTUREARBPROC glActiveTextureARB = NULL;
PFNGLCLIENTACTIVETEXTUREARBPROC glClientActiveTextureARB = NULL;
PFNGLMULTIDRAWELEMENTSEXTPROC glMultiDrawElementsEXT = NULL;

bool inStr(char *searchStr, char *allStr)
{
      int idx = 0; 
      char *str = allStr;
      char *endOfStr = str + strlen(str); 

      while (str < endOfStr)
      {
            idx = strcspn(str, " ");
            if ((strlen(searchStr) == idx) && (strncmp(searchStr, str, idx) == 0))
                  return true;

            str += (idx + 1);
      }
      
      return false;
}

bool multitextureSupported()
{
      char *extensionStr = (char*)glGetString(GL_EXTENSIONS);
      return inStr("GL_ARB_multitexture", extensionStr);
}

void bindMultitextureProcs()
{
      glMultiTexCoord2fARB = (PFNGLMULTITEXCOORD2FARBPROC)wglGetProcAddress("glMultiTexCoord2fARB");
      glActiveTextureARB = (PFNGLACTIVETEXTUREARBPROC)wglGetProcAddress("glActiveTextureARB");
      glClientActiveTextureARB = (PFNGLCLIENTACTIVETEXTUREARBPROC)wglGetProcAddress("glClientActiveTextureARB");
}

bool multidrawSupported()
{
      char *extensionStr = (char*)glGetString(GL_EXTENSIONS);
      return inStr("GL_EXT_multi_draw_arrays", extensionStr);
}

void bindMultidrawProcs()
{
      glMultiDrawElementsEXT = (PFNGLMULTIDRAWELEMENTSEXTPROC)wglGetProcAddress("glMultiDrawElementsEXT");
}
