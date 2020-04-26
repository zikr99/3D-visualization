#ifndef __CMESH_H__
#define __CMESH_H__

#include <string.h>

class CSceneManager;

enum E_MESH_TYPE
{
     EMST_BSP = 0,
     EMST_MD2,
     EMST_UNKNOWN
};

class CMesh
{
protected:
     CSceneManager *m_scenemanager;     
     E_MESH_TYPE m_meshtype;
     char *m_filename;
          
public:
     CMesh(CSceneManager *mngr);
               
     virtual ~CMesh()
     {
           if (m_filename) delete [] m_filename;  
     }  
     
     virtual bool loadFile(char *filename)
     {
           if (m_filename) delete [] m_filename;
           m_filename = new char [strlen(filename)];
           
           strcpy(m_filename, filename);  
     }
     
     virtual const char* getMeshName() { return m_filename; }
     virtual E_MESH_TYPE getMeshType() { return m_meshtype; } 

};

#endif
