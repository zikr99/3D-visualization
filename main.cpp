#include "CSimpleDemoApplication.h"

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine,
      int iCmdShow)
{ 
      CSimpleDemoApplication simpledemoapplication;
    
      simpledemoapplication.initialize();
      simpledemoapplication.run();
      simpledemoapplication.finalize();
    
      return 0;
}
