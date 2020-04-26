#include "CLevel.h"
#include "CSceneManager.h"

CLevel::CLevel(CSceneManager *mngr): m_scenemanager(mngr)
{
      m_rootscenenode = new CRootSceneNode(m_scenemanager);
      CCameraSceneNode *cmr = new CCameraFPSSceneNode(m_rootscenenode, m_scenemanager,
            m_scenemanager->getCursorControl(), 500.0f, 1000.0f, false);

      m_activecamera = cmr;
}
