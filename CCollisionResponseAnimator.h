#ifndef __CCOLLISIONRESPONSEANIMATOR_H__
#define __CCOLLISIONRESPONSEANIMATOR_H__
#define MAX_COLLISION_NUMBER 10

#include "CAnimator.h"
#include "CVector3.h"

class CBSPSceneNode;

class CCollisionResponseAnimator: public CAnimator
{
protected:
      enum E_COLLISION_RESPONSE_MODE
      {
            ECRM_POINT = 0,
            ECRM_SPHERE,
            ECRM_ELLIPSE
      };    
          
      E_COLLISION_RESPONSE_MODE m_mode;
      CBSPSceneNode *m_bspselector;
      float m_radius;
      
      CVector3 m_ellipseradius;
      CVector3 m_gravitation;
      CVector3 m_translation;
      
      bool m_fpscamera;
      bool m_always;   
      
      CVector3 m_vl;
      bool m_hasstirrup;
      
      virtual void OnAnimateNode(unsigned int timeMs);
      
      virtual void animateNodeCollPoint(unsigned int timeMs);
      virtual void animateNodeCollMovement(unsigned int timeMs, float radius);
      virtual void animateNodeCollMovement(unsigned int timeMs, CVector3 eradius);
          
public:
      CCollisionResponseAnimator(CSceneNode *scn, CBSPSceneNode *bspselector);
      CCollisionResponseAnimator(CSceneNode *scn, CBSPSceneNode *bspselector, float radius);
      CCollisionResponseAnimator(CSceneNode *scn, CBSPSceneNode *bspselector, CVector3 ellipseradius, 
            CVector3 gravitation = CVector3(0.0f, -1.0f, 0.0f), CVector3 translation = 
            CVector3(0.0f, 0.0f, 0.0f), bool fpscamera = false, bool alwaysapplygravity = false);
      
      virtual ~CCollisionResponseAnimator() {}
       
};

#endif
