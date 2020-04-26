#ifndef __CTESTSCENENODE_H__
#define __CTESTSCENENODE_H__

#include "CSceneNode.h"

class CTestSceneNode: public CSceneNode
{
protected:
      aabbox BBox;        
      virtual const aabbox& getBoundingBox() const { return BBox; }
      
      virtual void OnPrepareRender();
      virtual void OnRender();
              
public:
      CTestSceneNode(CSceneNode *parent, CSceneManager *mngr): CSceneNode(parent, mngr) 
      { 
            BBox.reset(0, 0, 0);
            BBox.addInternalPoint(0.5f, 0.5f, 0.5f);
            BBox.addInternalPoint(-0.5f, -0.5f, -0.5f);
      }
      
      virtual ~CTestSceneNode() {}
       
};

#endif
