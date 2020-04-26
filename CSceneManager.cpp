#include "CSceneManager.h"
#include "CHolderDevice.h"

void CSceneManager::registerNodeForRendering(CSceneNode *node, E_SCENE_NODE_RENDER_PASS pass)
{
     switch (pass)
     {
     case ESNRP_CAMERA:
          CameraList.push_back(node);
          break;
     case ESNRP_SOLID:
          SolidNodeList.push_back(node);
          break;
     case ESNRP_TRANSPARENT:
          TransparentNodeList.push_back(node);
          break;
     }
}

void CSceneManager::drawAll(CCameraSceneNode* camera)
{
     float timesec = m_dev->getTimer()->GetElapsedSeconds();
     unsigned int timeMs = (unsigned int)(timesec*1000);

     getActiveLevel()->getRootSceneNode()->update(timeMs);
     getActiveLevel()->getRootSceneNode()->preparerender();

     CurrentRenderPass = ESNRP_CAMERA;

     for (int i = 0; i < CameraList.size(); ++i)
          CameraList[i]->render();

     CameraList.clear();

     CurrentRenderPass = ESNRP_SOLID;

     for (int i = 0; i < SolidNodeList.size(); ++i)
          SolidNodeList[i]->render();

     SolidNodeList.clear();

     CurrentRenderPass = ESNRP_TRANSPARENT;

     glDepthMask(GL_FALSE);
     for (int i = 0; i < TransparentNodeList.size(); ++i) TransparentNodeList[i]->render();
     glDepthMask(GL_TRUE);

     TransparentNodeList.clear();

     CurrentRenderPass = ESNRP_COUNT;
}

bool CSceneManager::ProcessEvent(UINT umsg, WPARAM wparam, LPARAM lparam)
{
     if (m_activelevel && m_activelevel->getActiveCamera())
          return m_activelevel->getActiveCamera()->ProcessEvent(umsg, wparam, lparam);

     return FALSE;
}
