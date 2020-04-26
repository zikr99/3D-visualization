#ifndef __CBOUNCEANIMATOR_H__
#define __CBOUNCEANIMATOR_H__

#include <time.h>
#include <stdlib.h>
#include "CCollisionResponseAnimator.h"

class CSceneNode;
class CEnemy;

class CBounceAnimator: public CCollisionResponseAnimator
{
protected:
      unsigned int m_tfall;
      
      virtual void animateNodeCollMovement(unsigned int timeMs, CVector3 eradius);     
              
public:
      CBounceAnimator(CEnemy *scn, CBSPSceneNode *bspselector, CVector3 ellipseradius, 
            CVector3 gravitation = CVector3(0.0f, -1.0f, 0.0f)):
            CCollisionResponseAnimator((CSceneNode*)scn, bspselector, ellipseradius, 
            gravitation, CVector3(0.0f, 0.0f, 0.0f), true, true), m_tfall(0)       
      {
            srand(time(NULL));
      }
      
      virtual ~CBounceAnimator() {}
       
};

#endif
