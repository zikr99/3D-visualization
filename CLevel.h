#ifndef __CLEVEL_H__
#define __CLEVEL_H__

#include "CCameraFPSSceneNode.h"

class CRootSceneNode: public CSceneNode
{
protected:
      aabbox BBox;    
      virtual const aabbox& getBoundingBox() const { return BBox; }    
              
public:
      CRootSceneNode(CSceneManager *mngr): CSceneNode(NULL, mngr) { BBox.reset(0, 0, 0); }
      virtual ~CRootSceneNode() {}
      virtual void setVisible(bool vsb) {}
      virtual void setPosition(const CVector3 &pos) {}
      virtual void setRotation(const CVector3 &rot) {}
      virtual void setScale(const CVector3 &sc) {}
      virtual void render() {} 
              
};

class CLevel
{
protected:  
      CSceneManager *m_scenemanager;
      CRootSceneNode *m_rootscenenode;
      CCameraSceneNode *m_activecamera;
      
public:
      CLevel(CSceneManager *mngr);
      
      virtual ~CLevel()
      {
            delete m_rootscenenode;  
      }
      
      virtual CSceneNode* getRootSceneNode() { return m_rootscenenode; }
      virtual void clear() { m_rootscenenode->clear(); }
      
      virtual void setActiveCamera(CCameraSceneNode *cmr) { m_activecamera = cmr; }
      virtual CCameraSceneNode* getActiveCamera() { return m_activecamera; }
      
};

#endif
