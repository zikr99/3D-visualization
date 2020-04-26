#ifndef __CBULLETRESPONSEANIMATOR_H__
#define __CBULLETRESPONSEANIMATOR_H__

#include "CAnimator.h"
#include "aabbox.h"
#include "CVector3.h"
#include "CBmp24Image.h"

class CSceneNode;
class CBSPSceneNode;
class CExplosionSceneNode;

class CBulletResponseAnimator: public CAnimator
{
protected:    
      CExplosionSceneNode *m_explosion;
      CSceneNode *m_owner;
      CSceneNode *m_root;
      CVector3 m_radius; 
      
      CVector3 m_alastposition;
      CVector3 m_amove;
      
      aabbox m_box;
      float m_bx0, m_bx1, m_by0, m_by1, m_bz0, m_bz1;
      
      float m_outputfraction;
      CSceneNode *m_hitnode;
      
      virtual void OnAnimateNode(unsigned int timeMs);
      
      void CheckNodes(CSceneNode *rnode);
      void CheckBB(CSceneNode *rnode);
      void CheckBSP(CBSPSceneNode *rnode);
      bool IsRectIntersect(float r1maxa, float r1maxb, float r1mina, float r1minb,
           float r2maxa, float r2maxb, float r2mina, float r2minb);
          
public:
      CBulletResponseAnimator(CSceneNode *scn, CSceneNode *owner, CSceneNode *wroot, CVector3 eradius);
      virtual ~CBulletResponseAnimator();
       
};

#endif
