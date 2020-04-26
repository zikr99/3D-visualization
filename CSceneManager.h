#ifndef __CSCENEMANAGER_H__
#define __CSCENEMANAGER_H__

#include "CTimer.h"
#include "CLevel.h"
#include "CCollisionManager.h"

class CHolderDevice;

enum E_SCENE_NODE_RENDER_PASS
{
	ESNRP_CAMERA,
	ESNRP_SOLID,
	ESNRP_TRANSPARENT,
	ESNRP_COUNT
};

class CSceneManager
{
protected:
      CHolderDevice *m_dev;
      CCursorControl *m_cursorcontrol;

      list<CLevel*> m_levels;
      CLevel *m_activelevel;

      CCollisionManager *m_collisionmanager;

      E_SCENE_NODE_RENDER_PASS CurrentRenderPass;
      array<CSceneNode*> CameraList;
      array<CSceneNode*> SolidNodeList;
      array<CSceneNode*> TransparentNodeList;

      list<CTexture*> m_textures;
      list<CMesh*> m_meshes;

public:
      CSceneManager(CHolderDevice *dev, CCursorControl *cursorcontrol): m_dev(dev),
            m_cursorcontrol(cursorcontrol), CurrentRenderPass(ESNRP_COUNT)
      {
            CLevel *lv = new CLevel(this);
            m_levels.push_back(lv);
            m_activelevel = lv;

            m_collisionmanager = new CCollisionManager(this);
      }

      virtual ~CSceneManager()
      {
            list<CLevel*>::Iterator itl = m_levels.begin();
            for (; itl != m_levels.end(); ++itl)
               delete *itl;

            delete m_collisionmanager;

            list<CTexture*>::Iterator itt = m_textures.begin();
            for (; itt != m_textures.end(); ++itt)
               delete *itt;

            list<CMesh*>::Iterator itm = m_meshes.begin();
            for (; itm != m_meshes.end(); ++itm)
               delete *itm;
      }

      virtual CCursorControl* getCursorControl() { return m_cursorcontrol; }
      virtual CLevel* getActiveLevel() { return m_activelevel; }
      virtual CCollisionManager *getCollisionManager() { return m_collisionmanager; }

      virtual void registerNodeForRendering(CSceneNode *node, E_SCENE_NODE_RENDER_PASS pass);
      virtual E_SCENE_NODE_RENDER_PASS getSceneNodeRenderPass() { return CurrentRenderPass; }
      virtual void drawAll(CCameraSceneNode* camera = NULL);

      virtual bool ProcessEvent(UINT umsg, WPARAM wparam, LPARAM lparam);

      virtual void addTexture(CTexture *tex) { m_textures.push_back(tex); }
      virtual void addMesh(CMesh *msh) { m_meshes.push_back(msh); }
      CHolderDevice* getDevice() { return m_dev; }
};

#endif
