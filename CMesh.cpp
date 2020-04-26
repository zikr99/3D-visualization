#include "CMesh.h"
#include "CSceneManager.h"

CMesh::CMesh(CSceneManager *mngr): m_scenemanager(mngr), m_meshtype(EMST_UNKNOWN), 
      m_filename(NULL)
{
      m_scenemanager->addMesh(this);
}
